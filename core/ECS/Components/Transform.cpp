#include"Transform.h"

Transform::Transform()
{
}

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

DisplayInfo Transform::getDisplayInfo()
{
	DisplayInfo displayInfo;
	displayInfo.setComponentName("Transform");
	std::shared_ptr<DisplayInfoElementSlider> positionSlider = std::make_shared<DisplayInfoElementSlider>();
	positionSlider->minValue = -10.0f;
	positionSlider->maxValue = 10.0f;
	positionSlider->name = "Position";
	positionSlider->data = &m_position;
	positionSlider->type = DisplayTypeElement::Slider3;
	displayInfo.addElement(positionSlider);
	std::shared_ptr<DisplayInfoElementSlider> rotationSlider = std::make_shared<DisplayInfoElementSlider>();
	rotationSlider->minValue = -360.0f;
	rotationSlider->maxValue = 360.0f;
	rotationSlider->name = "Rotation";
	rotationSlider->data = &m_rotation;
	rotationSlider->type = DisplayTypeElement::Slider3;
	displayInfo.addElement(rotationSlider);
	std::shared_ptr<DisplayInfoElementSlider> scaleSlider = std::make_shared<DisplayInfoElementSlider>();
	scaleSlider->minValue = 0.0f;
	scaleSlider->maxValue = 10.0f;
	scaleSlider->name = "Scale";
	scaleSlider->data = &m_scale;
	scaleSlider->type = DisplayTypeElement::Slider3;
	displayInfo.addElement(scaleSlider);
	return displayInfo;
}

bool Transform::hasDisplayInfo()
{
	return true;
}