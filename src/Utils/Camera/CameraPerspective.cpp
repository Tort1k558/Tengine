#include "CameraPerspective.h"

CameraPerspective::CameraPerspective()
{
	m_projection = GetPerspectiveMatrix(45.0f, 16.0f / 9.0f, 0.1f, 100000.0f);
}
