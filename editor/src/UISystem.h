#pragma once

#include<memory>

#include<Core/Window.h>
#include<ECS/Object.h>
#include<ECS/System.h>
#include<Renderer/FrameBuffer.h>

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
		void renderMenuBar();
		void renderWindowObjects();
		void renderWindowComponents();
		void renderWindowInfo();
		void renderFileBrowser();

		std::shared_ptr<Window> m_window;
		std::string m_nameOfSelectedObject;
		std::shared_ptr<Object> m_sceneCamera;
		std::shared_ptr<FrameBuffer> m_sceneFramebuffer;

		static std::shared_ptr<UISystem> m_instance;
	};
}