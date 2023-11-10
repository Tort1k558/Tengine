#pragma once

#include<string>

#include<ECS/Object.h>

namespace TengineEditor
{
	using namespace Tengine;


	class WindowObjects
	{
	public:
		WindowObjects() = delete;

		static void Render();
		static void SetSelectedObject(std::shared_ptr<Object> object);
		static std::shared_ptr<Object> GetSelectedObject();
	private:
		static std::shared_ptr<Object> m_selectedObject;
	};

	
}