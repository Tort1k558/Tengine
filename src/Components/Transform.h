#pragma once

#include"ECS/Component.h"
#include"Core/Math.h"
class Transform : public Component
{
public:
	Transform() = default;
	Transform(Vec3 position, Vec3 rotation, Vec3 scale);

	Transform& setPosition(Vec3 position);
	Transform& setPositionX(float position);
	Transform& setPositionY(float position);
	Transform& setPositionZ(float position);
	Transform& setRotation(Vec3 angles);
	Transform& setRotationX(float angle);
	Transform& setRotationY(float angle);
	Transform& setRotationZ(float angle);
	Transform& setScale(Vec3 scale);
	Transform& setScaleX(float scale);
	Transform& setScaleY(float scale);
	Transform& setScaleZ(float scale);

	Vec3 getPosition() { return m_position; }
	Vec3 getRotation() { return m_rotation; }
	Vec3 getScale() { return m_scale; }
	Mat4 getMatrix();
private:
	Vec3 m_position = Vec3(0.0f);
	Vec3 m_rotation = Vec3(0.0f);
	Vec3 m_scale = Vec3(1.0f, 1.0f, 1.0f);
};