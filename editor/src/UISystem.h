#pragma once

#include<memory>

#include"Core/Window.h"
#include"ECS/System.h"
#include<Utils/EditorElements/ComponentInfo.h>

namespace TengineEditor
{
	using namespace Tengine;

	class UISystem : public System
	{
	public:
		void init() final;
		void update() final;
		void destroy() final;
		void setWindow(std::shared_ptr<Window> window);

		static std::shared_ptr<UISystem> GetInstance();
	private:
		void displayElement(std::shared_ptr<FieldInfo> element);
		void renderMenuBar();
		void renderWindowObjects();
		void renderWindowComponents();
		void renderWindowInfo();
		std::shared_ptr<Window> m_window;
		std::string m_nameOfSelectedObject;
		static std::shared_ptr<UISystem> m_instance;
	};
}