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
	return GetLookAtMatrix(transform->getPosition(), getDirection(), m_up);
}

Vec3 Camera::getDirection()
{
	std::shared_ptr<Transform> transform = getParent()->getComponent<Transform>();
	Mat4 viewMatrix = GetRotationMatrix(transform->getRotation());
	return viewMatrix* Vec4(m_direction,1.0);
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

void Camera::setCameraType(ProjectionType type)
{
	m_projectionType = type;
	updateProjection();
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

void Camera::setUp(Vec3 up)
{
	m_up = up;
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
