#pragma once

#include"Utils/Camera/Camera.h"
#include"ECS/Component.h"

class CameraController : public Component
{
public:
	CameraController();
	CameraController(CameraType type);
	Mat4 getProjectionMatrix() { return m_camera->getProjection(); }
	void setCameraType(CameraType type);
private:
	std::shared_ptr<Camera> m_camera;
};