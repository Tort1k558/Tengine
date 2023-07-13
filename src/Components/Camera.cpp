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
	Mat4 viewMatrix(1.0f);
	std::shared_ptr<Transform> transform = getParent()->getComponent<Transform>();
	viewMatrix *= GetRotationMatrix(transform->getRotation());
	return GetLookAtMatrix(transform->getPosition(), viewMatrix*Vec4( 0.0f,0.0f,1.0f ,1.0f), {0.0f,1.0f,0.0f});
}

void Camera::setCameraType(ProjectionType type)
{
	switch (type)
	{
	case ProjectionType::Perspective:
		m_projection = GetPerspectiveMatrix(45.0f, 16.0f / 9.0f, 0.1f, 100000.0f);
		break;
	case ProjectionType::Orthographical:
		m_projection = GetOrthographicMatrix(-1.0f, 1.0f, -1.0f, 1.0f,0.1f,100000.0f);
		break;
	}
}