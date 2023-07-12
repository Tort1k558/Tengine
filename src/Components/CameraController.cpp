#include "CameraController.h"

CameraController::CameraController()
{
	m_camera = Camera::Create(CameraType::Perspective);
}

CameraController::CameraController(CameraType type)
{
	m_camera = Camera::Create(type);
}

void CameraController::setCameraType(CameraType type)
{
	m_camera = Camera::Create(type);
}
