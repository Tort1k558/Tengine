#pragma once

#include<ECS/Object.h>
#include<functional>
#include<string>
#include<unordered_map>

namespace TengineEditor
{
	using namespace Tengine;


	class WindowMonitor
	{
	public:
		WindowMonitor() = delete;

		static void Render();
		static void SetMonitoringObject(std::shared_ptr<Object> object);
		static void SetPathMonitoringFile(std::filesystem::path pathToFile);
		static void AddFormatHandler(std::string_view format,std::function<void(std::filesystem::path pathToFile)> func);
		static void ShowField(std::shared_ptr<FieldInfo> element);
	private:
		enum class MonitoringType
		{
			None = 0,
			Object,
			File
		};
		static MonitoringType m_monitoringType;
		static std::unordered_map<std::string, std::function<void(std::filesystem::path)>> m_formatHandlers;
		static UUID m_idMonitoringObject;
		static std::filesystem::path m_pathToMonitoringFile;

	};

	
}