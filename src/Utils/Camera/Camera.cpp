#include "Camera.h"

#include"Utils/Camera/CameraOrthographical.h"
#include"Utils/Camera/CameraPerspective.h"
std::shared_ptr<Camera> Camera::Create(CameraType type)
{
	switch (type)
	{
	case CameraType::Perspective:
		return std::make_shared<CameraPerspective>();
	case CameraType::Orthographical:
		return std::make_shared<CameraOrthographical>();
	}
}
