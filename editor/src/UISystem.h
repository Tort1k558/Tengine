#pragma once

#include<memory>

#include<Core/Window.h>
#include<Scene/Object.h>
#include<Scene/System.h>
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
		void setSceneCamera(std::shared_ptr<Object> object);
		std::shared_ptr<Object> getSceneCamera();
		static std::shared_ptr<UISystem> GetInstance();
	private:
		void renderMenuBar();
		void renderWindowInfo();

		std::shared_ptr<Window> m_window;
		std::shared_ptr<Object> m_sceneCamera;
		std::shared_ptr<FrameBuffer> m_sceneFramebuffer;

		static std::shared_ptr<UISystem> m_instance;
	};
}