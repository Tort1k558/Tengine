#pragma once

#include<memory>

#include"Core/Math.h"

enum class CameraType
{
	Perspective,
	Orthographical
};

class Camera
{
public:
	virtual ~Camera() = default;
	Mat4 getProjection() { return m_projection; }
	
	static std::shared_ptr<Camera> Create(CameraType type);
protected:
	Mat4 m_projection;
};