#pragma once

#include<memory>

#include"ECS/System.h"
namespace Tengine
{
	class ControllerSystem : public System
	{
	public:
		void init() final;
		void update() final;
		void destroy() final;

		static std::shared_ptr<ControllerSystem> GetInstance();
	private:
		static std::shared_ptr<ControllerSystem> m_instance;
	};
}