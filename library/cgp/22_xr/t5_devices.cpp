#include "t5_devices.hpp"


namespace cgp
{

constexpr std::chrono::milliseconds operator""_ms(unsigned long long ms) {
	return std::chrono::milliseconds(ms);
}

template <typename T> T get(const tiltfive::Result<T>& result, const std::string& error_prefix) {
	if (!result) {
		std::cerr << error_prefix << result.error().message() << std::endl;
		std::exit(EXIT_FAILURE);
	}
	return *result;
}

void get(const tiltfive::Result<void>& result, const std::string& error_prefix) {
	if (!result) {
		std::cerr << error_prefix << result.error().message() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

template <typename T>
T waitForService(Client& client, const std::function<tiltfive::Result<T>(Client& client)>& func, const std::string& error_prefix) {
	for (bool waitingForService = false; ; waitingForService = true) {
		auto result = func(client);
		if (result || result.error() != tiltfive::Error::kNoService) 
			return get(result, error_prefix);

		std::cout << (waitingForService ? "." : "Waiting for service...") << std::flush;
		std::this_thread::sleep_for(100_ms);
	}
}

/****************************************************************************
After the window and GL context are created try to get a pair of T5 glasses 
and set up assets for rendering.
*****************************************************************************/
bool t5_devices::initialize()
{
	auto client_result = tiltfive::obtainClient("com.tiltfive.test", "0.1.0", nullptr);
	if (!client_result)
	{
		std::cerr << "Failed to create client: " << client_result.error().message() << std::endl;
		return false;
	}
	client = *client_result;
	std::cout << "Obtained client : " << client << std::endl;
	std::function<tiltfive::Result<std::string>(Client& client)> func = [](Client& c) { return c->getServiceVersion(); };
	serviceVersion = waitForService<std::string>(client, func, "Error while getting service version: ");
	std::cout << "Service version : " << serviceVersion << std::endl;

	auto glassesIds_result = client->listGlasses();
	if (!glassesIds_result)
	{
		std::cerr << "Error while listing glasses: " << glassesIds_result.error().message() << std::endl;
		return false;
	}
	glassesIds = *glassesIds_result;
	for (auto& glassesId : glassesIds)
	{
		std::cout << "Found glasses: " << glassesId << std::endl;
		if (!connect(glassesId))
			return false;
	}

	for(T5_GameboardType type : { kT5_GameboardType_LE, kT5_GameboardType_XE, kT5_GameboardType_XE_Raised})
	{
		auto result = client->getGameboardSize(type);
		if (!result) {
			std::cerr << "Error getting gameboard size : " << result.error().message() << std::endl;
			return false;
		}
		float x0 = -result->viewableExtentNegativeX;
		float x1 =  result->viewableExtentPositiveX;
		float y0 = -result->viewableExtentNegativeY;
		float y1 =  result->viewableExtentPositiveY;
		float z0 =  0;
		// float z1 =  result->viewableExtentPositiveZ;
		mesh quad = mesh_primitive_quadrangle({x0,y0,z0}, {x1,y0,z0}, {x1,y1,z0}, {x0,y1,z0});
		gameboard_drawable[type - kT5_GameboardType_None].initialize_data_on_gpu(quad);
	}

	mesh glasses_mesh = mesh_primitive_frame({}, 0.1);
	glasses_drawable.initialize_data_on_gpu(glasses_mesh);

	return true;
}

/****************************************************************************
Connect one pair of T5 glasses by ID.

There might be delays in getting connected and the main gui shouldn't be hung 
up waiting. So in a real application parts of this should probably be an done
in a non blocking way as part of the render loop. 
*****************************************************************************/
bool t5_devices::connect(const std::string& glassesId)
{
	Player player(glassesId, client);
	if (!player.initialize())
		return false;

	players.push_back(player);
	return true;
}

bool t5_devices::Player::initialize()
{	
    auto glasses_result = tiltfive::obtainGlasses(id, client);
	if (!glasses_result)
	{
		std::cerr << "Error creating glasses " << id << " : " << glasses_result.error().message() << std::endl;
		return false;
	}
	std::cout << "Created glasses: " << id << std::endl;
	glasses = *glasses_result;

	// Get the friendly name for the glasses
	std::string friendlyName = id;
	// This is the name that's user set in the Tilt Five™ control panel.
	auto friendlyName_res = glasses->getFriendlyName();
	if (friendlyName_res) {
		friendlyName = *friendlyName_res;
		std::cout << "Obtained friendly name : " << *friendlyName_res << std::endl;
	} else if (friendlyName_res.error() == tiltfive::Error::kSettingUnknown) {
		std::cout << "Couldn't get friendly name : Service reports it's not set" << std::endl;
	} else {
		std::cerr << "Error obtaining friendly name : " << friendlyName_res.error().message() << std::endl;
		return false;
	}
	
	{
		// Wait for exclusive glasses connection
		auto connectionHelper = glasses->createConnectionHelper(friendlyName);
		auto connectionResult = connectionHelper->awaitConnection(std::chrono::milliseconds(10000));
		if (connectionResult) {
			std::cout << "Glasses connected for exclusive use" << std::endl;
		} else {
			std::cerr << "Error connecting glasses for exclusive use : " << connectionResult.error().message()
					<< std::endl;
			return false;
		}
	}

	T5_GraphicsContextGL settings;
	settings.textureMode = kT5_GraphicsApi_GL_TextureMode_Pair;
	auto result = glasses->initGraphicsContext(kT5_GraphicsApi_GL, &settings);
	if (!result) {
		std::cerr << "Error initializing OpenGL context : " << result.error().message() << std::endl;
		return false;
	}
	
	float ipd = *(glasses->getIpd());
	leftEyePose.translation = {-ipd / 2.0f,0,0 };
	rightEyePose.translation = {ipd / 2.0f,0,0 };

	leftFramebuffer.initialize(width, height);
	rightFramebuffer.initialize(width, height);

	/*
	T5_CameraStreamConfig config = T5_CameraStreamConfig();
	config.cameraIndex = 0;
	config.enabled = true;
	auto result = glasses->configureCameraStream(config);
	if (!result) {
		std::cerr << "Error configureCameraStream : " << result.error().message() << result.error() << std::endl;
	} else {
		std::cout << "Camera Stream configured !" << std::endl;
	}
	
	int bufferSize = T5_MIN_CAM_IMAGE_BUFFER_WIDTH * T5_MIN_CAM_IMAGE_BUFFER_HEIGHT;
	for (int i = 0; i<10; ++i)
	{
		images[i] = T5_CamImage();
		images[i].bufferSize = bufferSize;
		images[i].pixelData = new uint8_t[bufferSize];
		images[i].cameraIndex = 0;
		result = glasses->submitEmptyCamImageBuffer(images+i);
		if (!result) {
			std::cerr << "Error submitEmptyCamImageBuffer : " << result.error().message() << result.error() << std::endl;
		} else {
			std::cout << "EmptyCamImageBuffer Submitted !" << std::endl;
		}
	}
	*/

	gameboard_type = kT5_GameboardType_None;
		
	return true;
}

t5_devices::Player::Player(const std::string& id, const Client& client) :
	id(id), client(client)
{
}

/****************************************************************************
Read the pose from the glasses and set the application headPose transform.
It will also dump the pose to standard out when 'P' is pressed.
*****************************************************************************/
void t5_devices::Player::update()
{
	bool wasPoseValid = isPoseValid;
	auto pose = glasses->getLatestGlassesPose(kT5_GlassesPoseUsage_GlassesPresentation);
	if (!pose) {
		isPoseValid = false;
		if(!wasPoseValid) return;
		if (pose.error() == tiltfive::Error::kTryAgain) {
			std::cout << "Pose unavailable - Is gameboard visible?" << std::endl;
		} else {
			std::cerr << "Pose unavailable - " << pose.error().message() << std::endl;
		}
	} else {
		isPoseValid = true;
		if(!wasPoseValid) std::cout << *pose << std::endl;
		headPose.translation = { pose->posGLS_GBD.x, pose->posGLS_GBD.y, pose->posGLS_GBD.z};
		headPose.rotation.data = { pose->rotToGLS_GBD.x, pose->rotToGLS_GBD.y, pose->rotToGLS_GBD.z, -pose->rotToGLS_GBD.w};
		gameboard_type = pose->gameboardType;
		//get_camera_frame();
	}
}

/****************************************************************************
Send the rendered textures and the pose they were rendered as to the glasses 
for projection. 
*****************************************************************************/

void t5_devices::Player::send_frame()
{
	if (!isPoseValid) return;
	
	T5_FrameInfo frameInfo;

	frameInfo.vci.startY_VCI = -tan(fov * 0.5f * Pi/180);
	frameInfo.vci.startX_VCI = frameInfo.vci.startY_VCI * width / (float) height;
	frameInfo.vci.width_VCI = -2.0f * frameInfo.vci.startX_VCI;
	frameInfo.vci.height_VCI = -2.0f * frameInfo.vci.startY_VCI;

	frameInfo.texWidth_PIX = width;
	frameInfo.texHeight_PIX = height;

	frameInfo.leftTexHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(leftFramebuffer.texture.id));
	frameInfo.rightTexHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(rightFramebuffer.texture.id));

	quaternion& q = headPose.rotation.data;
	frameInfo.rotToLVC_GBD = frameInfo.rotToRVC_GBD = { -q.w, q.x, q.y, q.z };

	vec3 u = headPose * leftEyePose.translation;
	frameInfo.posLVC_GBD = { u.x, u.y, u.z };

	vec3 v = headPose * rightEyePose.translation;
	frameInfo.posRVC_GBD = { v.x, v.y, v.z };

	frameInfo.isUpsideDown = false;
	frameInfo.isSrgb = false;

	auto result = glasses->sendFrame(&frameInfo);
	if (!result) {
		std::cerr << "Error while sending frame: " << result.error().message() << std::endl;
	}
}

void t5_devices::Player::get_camera_frame()
{
	auto result = glasses->getFilledCamImageBuffer();
	if (!result) {
		std::cerr << "getFilledCamImageBuffer: " << result.error().message() << std::endl;
		return;
	}

	T5_CamImage& image = *result;
	std::cout << image.imageWidth << "x" << image.imageHeight  << std::endl;
	//glasses->submitEmptyCamImageBuffer();
	//glasses->cancelCamImageBuffer()

	auto submit_result = glasses->submitEmptyCamImageBuffer(&image);
}


}