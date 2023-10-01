#pragma once

namespace TengineEditor
{
	class Builder
	{
	public:
		Builder() = delete;
		void static Build();
	private:
		void static GenerateInitFiles();
		void static GenerateCMake();
		void static BuildSolution();
		void static CollectFiles();
	};
}