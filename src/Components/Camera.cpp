#include "Camera.h"

#include"Components/Transform.h"

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

void Camera::updateProjection()
{
	switch (m_projectionType)
	{
	case ProjectionType::Perspective:
		m_projection = std::make_shared<PerspectiveProjection>();
		break;
	case ProjectionType::Orthographical:
		m_projection = std::make_shared<OrthographicalProjection>();
		break;
	}
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
}

void Camera::setCameraType(ProjectionType type)
{
	m_projectionType = type;
	updateProjection();
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
