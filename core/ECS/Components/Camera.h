#pragma once

#include"ECS/Component.h"
#include"Core/Math.h"

enum class ProjectionType
{
	Perspective = 0,
	Orthographical
};
class Camera;

class Projection
{
public:
	virtual ~Projection() = default;
	virtual void updateProjection() = 0;

	Mat4 getProjectionMatrix() const;
	void setZNear(float zNear);
	void setZFar(float zFar);

	float getZNear() const;
	float getZFar() const;
protected:
	Mat4 m_projection;
	float m_zNear = 0.01f;
	float m_zFar = 10000.0f;

	friend class Camera;
};

class PerspectiveProjection : public Projection
{
public:
	PerspectiveProjection();
	PerspectiveProjection(float fov, float aspectRatio, float zNear, float zFar);
	void setAspectRatio(float aspect);
	void setFov(float fov);
	void updateProjection() final;
	float getFov() const;
	float getAspectRatio() const;
private:
	float m_aspectRatio = 16.0f / 9.0f;
	float m_fov = 60.0f;

	friend class Camera;
};

class OrthographicalProjection : public Projection
{
public:
	OrthographicalProjection();
	OrthographicalProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	void setLeft(float left);
	void setRight(float right);
	void setBottom(float bottom);
	void setTop(float top);
	void setBorders(float left, float right, float bottom, float top, float zNear, float zFar);

	float getLeft();
	float getRight();
	float getBottom();
	float getTop();
	void updateProjection() final;
private:
	float m_left = -5.0f;
	float m_right = 5.0f;
	float m_bottom = -5.0f;
	float m_top = 5.0f;

	friend class Camera;
};


enum class RotationOrder
{
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
};
class Camera: public Component
{
public:
	Camera();
	Camera(ProjectionType type);

	void setCameraType(ProjectionType type);
	void setRotationOrder(RotationOrder order);

	Mat4 getViewMatrix();
	Vec3 getDirection() const;
	Vec3 getUp();
	std::shared_ptr<PerspectiveProjection> getPerspectiveProjection() const;
	std::shared_ptr<OrthographicalProjection> getOrthographicalProjection() const;
	std::shared_ptr<Projection> getProjection() const;
	RotationOrder getRotationOrder() const;
	ProjectionType getProjectionType() const;

	DisplayInfo getDisplayInfo() final;
private:
	Mat4 getRotationMatrix(Vec3 rotation) const;

	ProjectionType m_projectionType;
	std::shared_ptr<Projection> m_projection;
	RotationOrder m_rotationOrder = RotationOrder::YXZ;
	Vec3 m_direction = { 0.0f,0.0f, -1.0f };
	Vec3 m_up = { 0.0f,1.0f,0.0f };
};
