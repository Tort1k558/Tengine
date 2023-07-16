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

void Camera::setCameraType(ProjectionType type)
{
	switch (type)
	{
	case ProjectionType::Perspective:
		m_projection = GetPerspectiveMatrix(m_fov, m_aspect, 0.1f, 1000.0f);
		break;
	case ProjectionType::Orthographical:
		m_projection = GetOrthographicMatrix(-1.0f, 1.0f, -1.0f, 1.0f,0.1f,1000.0f);
		break;
	}
}

void Camera::setAspectRatio(float aspect)
{
	m_aspect = aspect;
}

void Camera::setFov(float fov)
{
	m_fov = fov;
}
