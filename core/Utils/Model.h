#pragma once



#include "Components/Mesh.h"

namespace Tengine
{
	class TENGINE Model : public Resource
	{
	public:
		Model() = default;
		std::shared_ptr<Mesh> getMesh();
		void setMesh(std::shared_ptr<Mesh> mesh);
	private:
		std::shared_ptr<Mesh> m_mesh;
	};
}
