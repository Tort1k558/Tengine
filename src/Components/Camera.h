#pragma once

#include"ECS/Component.h"
#include"Core/Math.h"

enum class ProjectionType
{
	Perspective,
	Orthographical
};

class Camera: public Component
{
public:
	Camera();
	Camera(ProjectionType type);
	Mat4 getProjectionMatrix() { return m_projection; }
	Mat4 getViewMatrix();
	void setCameraType(ProjectionType type);
private:
	Mat4 m_projection;
};