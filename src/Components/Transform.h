#pragma once

#include"ECS/Component.h"
#include"Core/Math.h"
class Transform : public Component
{
public:
	Transform();
	Transform(Vec3 position, Vec3 rotation, Vec3 scale);

	Transform& setPosition(Vec3 position);
	Transform& setRotation(Vec3 angles);
	Transform& setScale(Vec3 scale);

	Vec3 getPosition() { return m_position; }
	Vec3 getRotation() { return m_rotation; }
	Vec3 getScale() { return m_scale; }
	Mat4 getMatrix();
private:
	Vec3 m_position;
	Vec3 m_rotation;
	Vec3 m_scale;
};