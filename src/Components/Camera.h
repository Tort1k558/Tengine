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

	void setCameraType(ProjectionType type);
	void setAspectRatio(float aspect);
	void setFov(float fov);
	void setUp(Vec3 up);

	Mat4 getProjectionMatrix() { return m_projection; }
	Mat4 getViewMatrix();
	Vec3 getDirection();
	Vec3 getUp() { return m_up; }
private:
	Mat4 m_projection;
	float m_aspect = 16.0f / 9.0f;
	float m_fov = 60.0f;
	Vec3 m_direction = { 0.0f,0.0f, -1.0f };
	Vec3 m_up = { 0.0f,1.0f,0.0f };
};