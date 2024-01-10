#pragma once


#include "cgp/09_geometric_transformation/quaternion/quaternion.hpp"
#include "cgp/05_vec/vec.hpp"
#include "cgp/06_mat/mat.hpp"



namespace cgp
{


	/** Structure handling a rotation.
	* Provides an interface compatible with different representation of rotation (matrix, quaternion, axis/angle)
	* The internal storage of the rotation is a unit quaternion, but its use may not be aware of it.
	* The structure can be manipulated and "interact" like if it was a matrix: rotation*vec3, rotation*mat3, etc.
	*/
	struct rotation_transform
	{
		quaternion data;

		// Empty constructor: identity, q = [(0,0,0), 1]
		rotation_transform();
		rotation_transform(quaternion const& q);

		// Construct rotation from a quaternion representation
		static rotation_transform from_quaternion(quaternion const& q);
		// Construct rotation from an (axis,angle) representation
		static rotation_transform from_axis_angle(vec3 const& axis, float angle);
		// Construct rotation from its matrix representation
		static rotation_transform from_matrix(mat3 const& M);

		// Compute a rotation R such that R e_source = e_target
		//  Conditions: ||e_source||=||e_target||=1
		static rotation_transform from_vector_transform(vec3 const& e_source, vec3 const& e_target);

		// Rotation R such that R e1 = e1_target and R e2 = e2_target
		//  Conditions: ||e1||=||e1_target||=1
		//              ||e2||=||e2_target||=1
		//              dot(e1,e2) = 0
		//              dot(e1_target,e2_target) = 0
		static rotation_transform from_frame_transform(vec3 const& e1, vec3 const& e2, vec3 const& e1_target, vec3 const& e2_target);


		mat3 matrix() const;
		quaternion const& get_quaternion() const;
		void to_axis_angle(vec3& axis, float& angle) const;

		vec3 matrix_row_x() const;
		vec3 matrix_row_y() const;
		vec3 matrix_row_z() const;
		vec3 matrix_col_x() const;
		vec3 matrix_col_y() const;
		vec3 matrix_col_z() const;

		// Provide exhaustive conversion between rotation representation
		static mat3 convert_axis_angle_to_matrix(vec3 const& axis, float angle);
		static void convert_matrix_to_axis_angle(mat3 const& m, vec3& axis, float& angle);

		static mat3 convert_quaternion_to_matrix(quaternion const& q);
		static quaternion convert_matrix_to_quaternion(mat3 const& m);
		
		static quaternion convert_axis_angle_to_quaternion(vec3 const& axis, float angle);
		static void convert_quaternion_to_axis_angle(quaternion const& q, vec3& axis, float& angle);


		// Linear interpolation of rotation
		static rotation_transform lerp(rotation_transform const& r1, rotation_transform const& r2, float const alpha);
		// Spherical Linear interpolation of rotation
		//static rotation slerp(rotation const& r1, rotation const& r2, float const alpha);

	};

	// Inverse of the rotation
	rotation_transform inverse(rotation_transform const& r);

	// Composition between rotations r = r1 o r2
	rotation_transform operator*(rotation_transform const& r1, rotation_transform const& r2);
	// Composition r1 = r1 o r2
	rotation_transform& operator*=(rotation_transform& r1, rotation_transform const& r2);

	// Apply rotation to vector
	vec3 operator*(rotation_transform const& r, vec3 const& p);
	// Apply rotation to homogeneous 4D vector 
	vec4 operator*(rotation_transform const& r, vec4 const& p);
	// Multiply rotation matrix to mat3
	mat3 operator*(rotation_transform const& r, mat3 const& M);
	// Multiply rotation matrix to mat4
	mat4 operator*(rotation_transform const& r, mat4 const& M);


	std::string type_str(rotation_transform const& );
	std::string str(rotation_transform const& r);
	std::ostream& operator<<(std::ostream& s, rotation_transform const& r);

	// quick function to generate a rotation from axis angle
	rotation_transform rotation_axis_angle(vec3 const& axis, float angle);
}
