#version 300 es // OpenGL ES 3 - Compatible WebGL
precision mediump float;

layout(location=0) out vec4 FragColor;
uniform vec3 color; // Unifor color of the object

void main()
{
	FragColor = vec4(color, 1.0);
}
