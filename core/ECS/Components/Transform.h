#pragma once

#include"ECS/Component.h"
#include"Core/Math.h"
class Transform : public Component
{
public:
	Transform() = default;
	Transform(Vec3 position, Vec3 rotation, Vec3 scale);

	void setPosition(Vec3 position);
	void setPositionX(float position);
	void setPositionY(float position);
	void setPositionZ(float position);
	void setRotation(Vec3 angles);
	void setRotationX(float angle);
	void setRotationY(float angle);
	void setRotationZ(float angle);
	void setScale(Vec3 scale);
	void setScale(float scale);
	void setScaleX(float scale);
	void setScaleY(float scale);
	void setScaleZ(float scale);

	Vec3 getPosition() { return m_position; }
	Vec3 getRotation() { return m_rotation; }
	Vec3 getScale() { return m_scale; }
	Mat4 getMatrix();
private:
	Vec3 m_position = Vec3(0.0f);
	Vec3 m_rotation = Vec3(0.0f);
	Vec3 m_scale = Vec3(1.0f, 1.0f, 1.0f);
};