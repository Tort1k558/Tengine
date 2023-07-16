#include"Transform.h"

Transform::Transform() :
	m_position(Vec3(0.0f)), m_rotation(Vec3(0.0f)), m_scale(Vec3(1.0f,1.0f,1.0f))
{
}

Transform::Transform(Vec3 position, Vec3 rotation, Vec3 scale) :
	m_position(position), m_rotation(rotation), m_scale(scale)
{

}

Transform& Transform::setPosition(Vec3 position)
{
	m_position = position;
	return *this;
}

Transform& Transform::setPositionX(float position)
{
	m_position.x = position;
	return *this;
}

Transform& Transform::setPositionY(float position)
{
	m_position.y = position;
	return *this;
}

Transform& Transform::setPositionZ(float position)
{
	m_position.z = position;
	return *this;
}

Transform& Transform::setRotation(Vec3 angles)
{
	m_rotation = angles;
	return *this;
}

Transform& Transform::setRotationX(float angle)
{
	m_rotation.x = angle;
	return *this;
}

Transform& Transform::setRotationY(float angle)
{
	m_rotation.y = angle;
	return *this;
}

Transform& Transform::setRotationZ(float angle)
{
	m_rotation.z = angle;
	return *this;
}

Transform& Transform::setScale(Vec3 scale)
{
	m_scale = scale;
	return *this;
}

Transform& Transform::setScaleX(float scale)
{
	m_scale.x = scale;
	return *this;
}

Transform& Transform::setScaleY(float scale)
{
	m_scale.y = scale;
	return *this;
}

Transform& Transform::setScaleZ(float scale)
{
	m_scale.z = scale;
	return *this;
}

Mat4 Transform::getMatrix()
{
	Mat4 translateMatrix = TranslateMatrix(Mat4(1.0f), m_position);
	Mat4 rotateMatrix = GetRotationMatrix(m_rotation);
	Mat4 scaleMatrix = GetScaleMatrix(m_scale);
	return translateMatrix * rotateMatrix * scaleMatrix;
}
