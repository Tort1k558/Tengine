#include "Model.h"

namespace Tengine
{
	std::shared_ptr<Mesh> Model::getMesh()
	{
		return m_mesh;
	}
	void Model::setMesh(std::shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
	}
}
