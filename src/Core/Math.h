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

inline Mat4 RotateMatrix(Mat4& mat, Vec3 axis, float angle)
{
	return glm::rotate(mat, angle, axis);
}

inline Mat4 TranslateMatrix(Mat4& matrix, Vec3 position)
{
	return glm::translate(matrix, position);
}

inline Mat4 GetRotationMatrix(Vec3 angles)
{
	return glm::yawPitchRoll(angles.y, angles.x, angles.z);
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
	return glm::perspective(fov, aspect, zNear, zFar);
}

inline Mat4 GetOrthographicMatrix(float left, float right, float bottom, float top)
{
	return glm::ortho(left, right, bottom, top);
}

inline Mat4 GetOrthographicMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
{
	return glm::ortho(left, right, bottom, top, zNear, zFar);
}