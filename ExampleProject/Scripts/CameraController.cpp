#include "CameraController.h"

#include"Core/Logger.h"
#include"Components/Transform.h"
#include"Components/Camera.h"
#include"Core/Timer.h"
#include"Core/Input.h"

namespace Tengine
{
	void CameraController::start()
	{

	}

	void CameraController::update()
	{
		std::shared_ptr<Transform> transform = getParent()->getComponent<Transform>();
		std::shared_ptr<Camera> camera = getParent()->getComponent<Camera>();
		if (Input::IsKeyPressed(KeyCode::W))
		{
			transform->setPosition(transform->getPosition() + camera->getDirection() * Vec3(static_cast<float>(speed * Timer::GetDeltaTime())));
		}
		if (Input::IsKeyPressed(KeyCode::S))
		{
			transform->setPosition(transform->getPosition() + camera->getDirection() * Vec3(static_cast<float>(-speed * Timer::GetDeltaTime())));
		}
		if (Input::IsKeyPressed(KeyCode::A))
		{
			transform->setPosition(transform->getPosition() + Normalize(Cross(camera->getDirection(), camera->getUp()))
				* Vec3(static_cast<float>(-speed * Timer::GetDeltaTime())));
		}
		if (Input::IsKeyPressed(KeyCode::D))
		{
			transform->setPosition(transform->getPosition() + Normalize(Cross(camera->getDirection(), camera->getUp()))
				* Vec3(static_cast<float>(speed * Timer::GetDeltaTime())));
		}
		if (Input::IsKeyPressed(KeyCode::LEFT_SHIFT))
		{
			transform->setPosition(transform->getPosition() + Vec3(0.0f, static_cast<float>(-speed * Timer::GetDeltaTime()), 0.0f));
		}
		if (Input::IsKeyPressed(KeyCode::SPACE))
		{
			transform->setPosition(transform->getPosition() + Vec3(0.0f, static_cast<float>(speed * Timer::GetDeltaTime()), 0.0f));
		}
		if (Input::IsMouseButtonPressed(MouseButton::BUTTON_2))
		{
			static Vec2 deltaMouse(0.0f, 0.0f);
			if (deltaMouse.x != 0)
			{
				transform->setRotationY(transform->getRotation().y + deltaMouse.x * cameraSensitivity);
			}
			if (deltaMouse.y != 0)
			{
				if (transform->getRotation().x + deltaMouse.y * cameraSensitivity < 89.0f && transform->getRotation().x + deltaMouse.y * cameraSensitivity > -89.0f)
				{
					transform->setRotationX(transform->getRotation().x + deltaMouse.y * cameraSensitivity);
				}
			}
			deltaMouse = Input::GetPrevMousePosition() - Input::GetMousePosition();
		}
	}
}
