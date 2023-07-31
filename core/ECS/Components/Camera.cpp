#include "Camera.h"

#include"ECS/Components/Transform.h"

Camera::Camera()
{
	setCameraType(ProjectionType::Perspective);
}

Camera::Camera(ProjectionType type)
{
	setCameraType(type);
}

Mat4 Camera::getViewMatrix()
{
	std::shared_ptr<Transform> transform = getParent()->getComponent<Transform>();
	Vec3 rotation = transform->getRotation();
	Mat4 rotateMatrix = getRotationMatrix(rotation);
	Vec3 up = rotateMatrix * Vec4(m_up,1.0f);
	Vec3 direction = rotateMatrix * Vec4(m_direction, 1.0f);
	return GetLookAtMatrix(transform->getPosition(), direction, up);
}

Vec3 Camera::getDirection()
{
	std::shared_ptr<Transform> transform = getParent()->getComponent<Transform>();
	Vec3 rotation = transform->getRotation();
	Mat4 rotateMatrix = getRotationMatrix(rotation);
	return rotateMatrix * Vec4(m_direction,1.0);
}

Vec3 Camera::getUp()
{
	std::shared_ptr<Transform> transform = getParent()->getComponent<Transform>();
	Vec3 rotation = transform->getRotation();
	Mat4 rotateMatrix = getRotationMatrix(rotation);
	return rotateMatrix * Vec4(m_up,1.0f);
}

std::shared_ptr<PerspectiveProjection> Camera::getPerspectiveProjection()
{
	if (m_projectionType == ProjectionType::Perspective)
	{
		return std::dynamic_pointer_cast<PerspectiveProjection>(m_projection);
	}
	return nullptr;
}

std::shared_ptr<OrthographicalProjection> Camera::getOrthographicalProjection()
{
	if (m_projectionType == ProjectionType::Orthographical)
	{
		return std::dynamic_pointer_cast<OrthographicalProjection>(m_projection);
	}
	return nullptr;
}

std::shared_ptr<Projection> Camera::getProjection()
{
	return m_projection;
}

DisplayInfo Camera::getDisplayInfo()
{
	DisplayInfo displayInfo;
	displayInfo.setComponentName("Camera");
	std::shared_ptr<DisplayInfoElementCombo> projectionType = std::make_shared<DisplayInfoElementCombo>();
	projectionType->name = "Projection";
	projectionType->type = DisplayTypeElement::Combo;
	projectionType->elements = { "Perspective","Orthographical" };
	projectionType->currentElement = reinterpret_cast<int*>(&m_projectionType);
	projectionType->callback = [this](int element)
	{
		this->setCameraType(m_projectionType);
	};
	displayInfo.addElement(projectionType);
	
	std::shared_ptr<DisplayInfoElementSlider> projectionSettingsZNear = std::make_shared<DisplayInfoElementSlider>();
	projectionSettingsZNear->data = &m_projection->m_zNear;
	projectionSettingsZNear->minValue = 0.01f;
	projectionSettingsZNear->maxValue = 10000.0f;
	projectionSettingsZNear->name = "zNear";
	projectionSettingsZNear->type = DisplayTypeElement::Slider;
	projectionSettingsZNear->callback = [this]() {m_projection->updateProjection(); };
	displayInfo.addElement(projectionSettingsZNear);
	std::shared_ptr<DisplayInfoElementSlider> projectionSettingsZFar = std::make_shared<DisplayInfoElementSlider>();
	projectionSettingsZFar->data = &m_projection->m_zFar;
	projectionSettingsZFar->minValue = 0.01f;
	projectionSettingsZFar->maxValue = 10000.0f;
	projectionSettingsZFar->name = "zFar";
	projectionSettingsZFar->type = DisplayTypeElement::Slider;
	projectionSettingsZFar->callback = [this]() {m_projection->updateProjection(); };
	displayInfo.addElement(projectionSettingsZFar);

	if (m_projectionType == ProjectionType::Perspective)
	{
		std::shared_ptr<PerspectiveProjection> projection = std::dynamic_pointer_cast<PerspectiveProjection>(m_projection);
		std::shared_ptr<DisplayInfoElementSlider> projectionSettingsAspect = std::make_shared<DisplayInfoElementSlider>();
		projectionSettingsAspect->data = &projection->m_aspect;
		projectionSettingsAspect->minValue = 0.0f;
		projectionSettingsAspect->maxValue = 3.0f;
		projectionSettingsAspect->name = "Aspect";
		projectionSettingsAspect->type = DisplayTypeElement::Slider;
		projectionSettingsAspect->callback = [this]() {m_projection->updateProjection(); };
		displayInfo.addElement(projectionSettingsAspect);
		std::shared_ptr<DisplayInfoElementSlider> projectionSettingsFov = std::make_shared<DisplayInfoElementSlider>();
		projectionSettingsFov->data = &projection->m_fov;
		projectionSettingsFov->minValue = 0.0f;
		projectionSettingsFov->maxValue = 360.0f;
		projectionSettingsFov->name = "Fov";
		projectionSettingsFov->type = DisplayTypeElement::Slider;
		projectionSettingsFov->callback = [this]() {m_projection->updateProjection(); };
		displayInfo.addElement(projectionSettingsFov);

	}
	return displayInfo;
}

bool Camera::hasDisplayInfo()
{
	return true;
}

Mat4 Camera::getRotationMatrix(Vec3 rotation)
{
	switch (m_rotationOrder)
	{
	case RotationOrder::XYZ:
		return GetEulerMatrixXYZ(rotation);
	case RotationOrder::XZY:
		return GetEulerMatrixXZY(rotation);
	case RotationOrder::YXZ:
		return GetEulerMatrixYXZ(rotation);
	case RotationOrder::YZX:
		return GetEulerMatrixYZX(rotation);
	case RotationOrder::ZXY:
		return GetEulerMatrixZXY(rotation);
	case RotationOrder::ZYX:
		return GetEulerMatrixZYX(rotation);
	}
	return Mat4(1.0f);
}

void Camera::setCameraType(ProjectionType type)
{
	m_projectionType = type;
	switch (type)
	{
	case ProjectionType::Perspective:
		m_projection = std::make_shared<PerspectiveProjection>();
		break;
	case ProjectionType::Orthographical:
		m_projection = std::make_shared<OrthographicalProjection>();
		break;
	default:
		break;
	}
}

void Camera::setRotationOrder(RotationOrder order)
{
	m_rotationOrder = order;
}

void PerspectiveProjection::setAspectRatio(float aspect)
{
	m_aspect = aspect;
	updateProjection();
}

void PerspectiveProjection::setFov(float fov)
{
	m_fov = fov;
	updateProjection();
}

void PerspectiveProjection::updateProjection()
{
	m_projection = GetPerspectiveMatrix(m_fov, m_aspect, m_zNear, m_zFar);
}

void Projection::setZNear(float zNear)
{
	m_zNear = zNear;
	updateProjection();
}

void Projection::setZFar(float zFar)
{
	m_zFar = zFar;
	updateProjection();
}

PerspectiveProjection::PerspectiveProjection()
{
	updateProjection();
}

PerspectiveProjection::PerspectiveProjection(float fov, float aspect, float zNear, float zFar) :
	m_fov(fov),m_aspect(aspect)
{
	m_zNear = zNear;
	m_zFar = zFar;
	updateProjection();
}

OrthographicalProjection::OrthographicalProjection()
{
	updateProjection();
}

OrthographicalProjection::OrthographicalProjection(float left, float right, float bottom, float top, float zNear, float zFar) :
	m_left(left),m_right(right),m_bottom(bottom),m_top(top)
{
	m_zNear = zNear;
	m_zFar = zFar;
	updateProjection();
}

void OrthographicalProjection::setLeft(float left)
{
	m_left = left;
	updateProjection();
}

void OrthographicalProjection::setRight(float right)
{
	m_right = right;
	updateProjection();
}

void OrthographicalProjection::setBottom(float bottom)
{
	m_bottom = bottom;
	updateProjection();
}

void OrthographicalProjection::setTop(float top)
{
	m_top = top;
	updateProjection();
}

void OrthographicalProjection::setBorders(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_left = left;
	m_right = right;
	m_bottom = bottom;
	m_top = top;
	m_zNear = zNear;
	m_zFar = zFar;
	updateProjection();
}

void OrthographicalProjection::updateProjection()
{
	m_projection = GetOrthographicMatrix(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
}
