#pragma once

#include<string>

#include"Scene/System.h"
#include"Scene/SceneManager.h"
#include"Scene/Object.h"
#include"Core/Core.h"
#include"Components/Script.h"

namespace Tengine
{
	class TENGINE ScriptSystem : public System
	{
	public:
		virtual void init() = 0;
		virtual void update() = 0;
		virtual void destroy() = 0;

		virtual std::shared_ptr<Component> addScript(std::shared_ptr<Object> object, std::string_view nameScript) = 0;
		virtual std::vector<std::string> getScriptNames() = 0;

		static std::shared_ptr<ScriptSystem> GetInstance();
	protected:
		static void SetInstance(std::shared_ptr<ScriptSystem> system);
		static std::shared_ptr<ScriptSystem> m_instance;
	};
}