#include"Transform.h"

Transform::Transform(Vec3 position, Vec3 rotation, Vec3 scale) :
	m_position(position), m_rotation(rotation), m_scale(scale)
{

}

void Transform::setPosition(Vec3 position)
{
	m_position = position;
}

void Transform::setPositionX(float position)
{
	m_position.x = position;
}

void Transform::setPositionY(float position)
{
	m_position.y = position;
}

void Transform::setPositionZ(float position)
{
	m_position.z = position;
}

void Transform::setRotation(Vec3 angles)
{
	m_rotation = angles;
}

void Transform::setRotationX(float angle)
{
	m_rotation.x = angle;
}

void Transform::setRotationY(float angle)
{
	m_rotation.y = angle;
}

void Transform::setRotationZ(float angle)
{
	m_rotation.z = angle;
}

void Transform::setScale(Vec3 scale)
{
	m_scale = scale;
}

void Transform::setScale(float scale)
{
	m_scale = Vec3(scale, scale, scale);
}

void Transform::setScaleX(float scale)
{
	m_scale.x = scale;
}

void Transform::setScaleY(float scale)
{
	m_scale.y = scale;
}

void Transform::setScaleZ(float scale)
{
	m_scale.z = scale;
	
}

Mat4 Transform::getMatrix()
{
	Mat4 translateMatrix = TranslateMatrix(Mat4(1.0f), m_position);
	Mat4 rotateMatrix(1.0f);
	rotateMatrix = RotateMatrix(rotateMatrix, Vec3(1.0f, 0.0f, 0.0f), m_rotation.x);
	rotateMatrix = RotateMatrix(rotateMatrix, Vec3(0.0f, 1.0f, 0.0f), m_rotation.y);
	rotateMatrix = RotateMatrix(rotateMatrix, Vec3(0.0f, 0.0f, 1.0f), m_rotation.z);
	Mat4 scaleMatrix = GetScaleMatrix(m_scale);
	return translateMatrix * rotateMatrix * scaleMatrix;
}
