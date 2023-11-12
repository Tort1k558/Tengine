#include"Transform.h"

#include"Core/Math.h"

namespace Tengine
{
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

	Vec3 Transform::getPosition() const
	{
		return m_position;
	}

	Vec3 Transform::getRotation() const
	{
		return m_rotation;
	}

	Vec3 Transform::getScale() const
	{
		return m_scale;
	}

	Mat4 Transform::getMatrix() const
	{
		Mat4 translateMatrix = Mat4(1.0f);
		translateMatrix = Math::TranslateMatrix(translateMatrix, m_position);
		Mat4 rotationMatrix(1.0f);
		rotationMatrix = Math::RotateMatrix(rotationMatrix, Vec3(1.0f, 0.0f, 0.0f), m_rotation.x);
		rotationMatrix = Math::RotateMatrix(rotationMatrix, Vec3(0.0f, 1.0f, 0.0f), m_rotation.y);
		rotationMatrix = Math::RotateMatrix(rotationMatrix, Vec3(0.0f, 0.0f, 1.0f), m_rotation.z);
		Mat4 scaleMatrix = Math::GetScaleMatrix(m_scale);
		return translateMatrix * rotationMatrix * scaleMatrix;
	}

	Vec3 Transform::getForwardVector() const
	{
		Mat4 rotationMatrix(1.0f);
		rotationMatrix = Math::RotateMatrix(rotationMatrix, Vec3(1.0f, 0.0f, 0.0f), m_rotation.x);
		rotationMatrix = Math::RotateMatrix(rotationMatrix, Vec3(0.0f, 1.0f, 0.0f), m_rotation.y);
		rotationMatrix = Math::RotateMatrix(rotationMatrix, Vec3(0.0f, 0.0f, 1.0f), m_rotation.z);

		return -Vec3(rotationMatrix[2]);
	}

	ComponentInfo Transform::getInfo()
	{
		ComponentInfo displayInfo;
		displayInfo.setComponentName("Transform");

		std::shared_ptr<FieldVec3> positionSlider = std::make_shared<FieldVec3>();
		positionSlider->minValue = -10.0f;
		positionSlider->maxValue = 10.0f;
		positionSlider->name = "Position";
		positionSlider->data = &m_position;
		displayInfo.addElement(positionSlider);

		std::shared_ptr<FieldVec3> rotationSlider = std::make_shared<FieldVec3>();
		rotationSlider->minValue = -360.0f;
		rotationSlider->maxValue = 360.0f;
		rotationSlider->name = "Rotation";
		rotationSlider->data = &m_rotation;
		displayInfo.addElement(rotationSlider);

		std::shared_ptr<FieldVec3> scaleSlider = std::make_shared<FieldVec3>();
		scaleSlider->minValue = 0.0f;
		scaleSlider->maxValue = 10.0f;
		scaleSlider->name = "Scale";
		scaleSlider->data = &m_scale;
		displayInfo.addElement(scaleSlider);

		return displayInfo;
	}
}