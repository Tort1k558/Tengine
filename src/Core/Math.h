#pragma once

#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/euler_angles.hpp>
using Mat2 = glm::mat2;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using UVec2 = glm::uvec2;
using UVec3 = glm::uvec3;
using UVec4 = glm::uvec4;

//The angle of rotation is indicated in degrees!

inline Mat4 RotateMatrix(Mat4& mat, Vec3 axis, float angle)
{
	return glm::rotate(mat, glm::radians(angle), axis);
}

inline Mat4 TranslateMatrix(Mat4& matrix, Vec3 position)
{
	return glm::translate(matrix, position);
}

inline Mat4 GetEulerMatrixXYZ(Vec3 angles)
{
	return glm::yawPitchRoll(glm::radians(angles.x), glm::radians(angles.y), glm::radians(angles.z));
}
inline Mat4 GetEulerMatrixXZY(Vec3 angles)
{
	return glm::yawPitchRoll(glm::radians(angles.x), glm::radians(angles.z), glm::radians(angles.y));
}
inline Mat4 GetEulerMatrixYXZ(Vec3 angles)
{
	return glm::yawPitchRoll(glm::radians(angles.y), glm::radians(angles.x), glm::radians(angles.z));
}
inline Mat4 GetEulerMatrixYZX(Vec3 angles)
{
	return glm::yawPitchRoll(glm::radians(angles.y), glm::radians(angles.z), glm::radians(angles.x));
}
inline Mat4 GetEulerMatrixZXY(Vec3 angles)
{
	return glm::yawPitchRoll(glm::radians(angles.z), glm::radians(angles.x), glm::radians(angles.y));
}
inline Mat4 GetEulerMatrixZYX(Vec3 angles)
{
	return glm::yawPitchRoll(glm::radians(angles.z), glm::radians(angles.y), glm::radians(angles.x));
}

inline Mat4 GetScaleMatrix(Vec3 scale)
{
	return Mat4(scale.x,	0,				 0,				0,
				0,			scale.y,		 0,				0,
				0,			0,				 scale.z,		0,
				0,			0,				 0,				1);
}

inline Mat4 GetPerspectiveMatrix(float fov, float aspect, float zNear, float zFar)
{
	return glm::perspective(glm::radians(fov), aspect, zNear, zFar);
}

inline Mat4 GetOrthographicMatrix(float left, float right, float bottom, float top)
{
	return glm::ortho(left, right, bottom, top);
}

inline Mat4 GetOrthographicMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
{
	return glm::ortho(left, right, bottom, top, zNear, zFar);
}

inline Mat4 GetLookAtMatrix(Vec3 position, Vec3 direction, Vec3 up)
{
	return glm::lookAt(position, position + direction, up);
}

inline Vec3 Cross(Vec3 vec1, Vec3 vec2)
{
	return glm::cross(vec1, vec2);
}

inline Vec3 Normalize(Vec3 vec)
{
	return glm::normalize(vec);
}

inline Vec2 Normalize(Vec2 vec)
{
	return glm::normalize(vec);
}
inline Vec4 Normalize(Vec4 vec)
{
	return glm::normalize(vec);
}