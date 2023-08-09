#pragma once

namespace Tengine
{
	class CodeGenerator
	{
	public:
		static void UpdateScripts();
	private:
		static void GeneratePremake();
		static void BuildDll();
		static void GenerateInitFiles();
	};
}