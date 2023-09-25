#include "Camera.h"

#include"Components/Transform.h"

namespace Tengine
{

	Camera::Camera()
	{
		setCameraType(ProjectionType::Perspective);
	}

	Camera::Camera(ProjectionType type)
	{

		setCameraType(type);
	}

	ProjectionType Camera::getProjectionType() const
	{
		return m_projectionType;
	}

	Mat4 Camera::getViewMatrix()
	{
		std::shared_ptr<Transform> transform = getParent()->getComponent<Transform>();
		Vec3 rotation = transform->getRotation();
		Mat4 rotateMatrix = getRotationMatrix(rotation);
		Vec3 up = rotateMatrix * Vec4(m_up, 1.0f);
		Vec3 direction = rotateMatrix * Vec4(m_direction, 1.0f);
		return GetLookAtMatrix(transform->getPosition(), direction, up);
	}

	Vec3 Camera::getDirection() const
	{
		std::shared_ptr<Transform> transform = getParent()->getComponent<Transform>();
		Vec3 rotation = transform->getRotation();
		Mat4 rotateMatrix = getRotationMatrix(rotation);
		return rotateMatrix * Vec4(m_direction, 1.0);
	}

	Vec3 Camera::getUp()
	{
		std::shared_ptr<Transform> transform = getParent()->getComponent<Transform>();
		Vec3 rotation = transform->getRotation();
		Mat4 rotateMatrix = getRotationMatrix(rotation);
		return rotateMatrix * Vec4(m_up, 1.0f);
	}
	ComponentInfo Camera::getInfo()
	{
		ComponentInfo displayInfo;
		displayInfo.setComponentName("Camera");
		std::shared_ptr<DisplayInfoElementCombo> projectionType = std::make_shared<DisplayInfoElementCombo>();
		projectionType->name = "Projection";
		projectionType->type = DisplayTypeElement::Combo;
		projectionType->elements = { "Perspective","Orthographical" };
		projectionType->currentElement = reinterpret_cast<int*>(&m_projectionType);
		projectionType->callback = [this](int element)
		{
			this->setCameraType(m_projectionType);
		};
		displayInfo.addElement(projectionType);

		std::shared_ptr<DisplayInfoElementSlider> projectionSettingsZNear = std::make_shared<DisplayInfoElementSlider>();
		projectionSettingsZNear->data = &m_projection->m_zNear;
		projectionSettingsZNear->minValue = 0.01f;
		projectionSettingsZNear->maxValue = 10000.0f;
		projectionSettingsZNear->name = "zNear";
		projectionSettingsZNear->type = DisplayTypeElement::Slider;
		projectionSettingsZNear->callback = [this]() {m_projection->updateProjection(); };
		displayInfo.addElement(projectionSettingsZNear);
		std::shared_ptr<DisplayInfoElementSlider> projectionSettingsZFar = std::make_shared<DisplayInfoElementSlider>();
		projectionSettingsZFar->data = &m_projection->m_zFar;
		projectionSettingsZFar->minValue = 0.01f;
		projectionSettingsZFar->maxValue = 10000.0f;
		projectionSettingsZFar->name = "zFar";
		projectionSettingsZFar->type = DisplayTypeElement::Slider;
		projectionSettingsZFar->callback = [this]() {m_projection->updateProjection(); };
		displayInfo.addElement(projectionSettingsZFar);

		switch (m_projectionType)
		{
		case ProjectionType::Perspective:
		{
			std::shared_ptr<PerspectiveProjection> perspective = getPerspectiveProjection();
			std::shared_ptr<DisplayInfoElementSlider> projectionSettingsAspect = std::make_shared<DisplayInfoElementSlider>();
			projectionSettingsAspect->data = &perspective->m_aspectRatio;
			projectionSettingsAspect->minValue = 0.0f;
			projectionSettingsAspect->maxValue = 3.0f;
			projectionSettingsAspect->name = "Aspect";
			projectionSettingsAspect->type = DisplayTypeElement::Slider;
			projectionSettingsAspect->callback = [this]() {m_projection->updateProjection(); };
			displayInfo.addElement(projectionSettingsAspect);
			std::shared_ptr<DisplayInfoElementSlider> projectionSettingsFov = std::make_shared<DisplayInfoElementSlider>();
			projectionSettingsFov->data = &perspective->m_fov;
			projectionSettingsFov->minValue = 0.0f;
			projectionSettingsFov->maxValue = 360.0f;
			projectionSettingsFov->name = "Fov";
			projectionSettingsFov->type = DisplayTypeElement::Slider;
			projectionSettingsFov->callback = [this]() {m_projection->updateProjection(); };
			displayInfo.addElement(projectionSettingsFov);
			break;
		}
		case ProjectionType::Orthographical:
		{
			std::shared_ptr<OrthographicalProjection> projection = getOrthographicalProjection();
			std::shared_ptr<DisplayInfoElementSlider> projectionSettingsLeft = std::make_shared<DisplayInfoElementSlider>();
			projectionSettingsLeft->data = &projection->m_left;
			projectionSettingsLeft->minValue = -0.01f;
			projectionSettingsLeft->maxValue = -10.0f;
			projectionSettingsLeft->name = "Left";
			projectionSettingsLeft->type = DisplayTypeElement::Slider;
			projectionSettingsLeft->callback = [this]() {m_projection->updateProjection(); };
			displayInfo.addElement(projectionSettingsLeft);
			std::shared_ptr<DisplayInfoElementSlider> projectionSettingsRight = std::make_shared<DisplayInfoElementSlider>();
			projectionSettingsRight->data = &projection->m_right;
			projectionSettingsRight->minValue = 0.01f;
			projectionSettingsRight->maxValue = 10.0f;
			projectionSettingsRight->name = "Right";
			projectionSettingsRight->type = DisplayTypeElement::Slider;
			projectionSettingsRight->callback = [this]() {m_projection->updateProjection(); };
			displayInfo.addElement(projectionSettingsRight);
			std::shared_ptr<DisplayInfoElementSlider> projectionSettingsBottom = std::make_shared<DisplayInfoElementSlider>();
			projectionSettingsBottom->data = &projection->m_bottom;
			projectionSettingsBottom->minValue = -0.01f;
			projectionSettingsBottom->maxValue = -10.0f;
			projectionSettingsBottom->name = "Bottom";
			projectionSettingsBottom->type = DisplayTypeElement::Slider;
			projectionSettingsBottom->callback = [this]() {m_projection->updateProjection(); };
			displayInfo.addElement(projectionSettingsBottom);
			std::shared_ptr<DisplayInfoElementSlider> projectionSettingsTop = std::make_shared<DisplayInfoElementSlider>();
			projectionSettingsTop->data = &projection->m_top;
			projectionSettingsTop->minValue = 0.01f;
			projectionSettingsTop->maxValue = 10.0f;
			projectionSettingsTop->name = "Top";
			projectionSettingsTop->type = DisplayTypeElement::Slider;
			projectionSettingsTop->callback = [this]() {m_projection->updateProjection(); };
			displayInfo.addElement(projectionSettingsTop);
			break;
		}
		default:
			break;
		}
		return displayInfo;
	}
	std::shared_ptr<PerspectiveProjection> Camera::getPerspectiveProjection() const
	{
		if (m_projectionType == ProjectionType::Perspective)
		{
			return std::dynamic_pointer_cast<PerspectiveProjection>(m_projection);
		}
		return nullptr;
	}

	std::shared_ptr<OrthographicalProjection> Camera::getOrthographicalProjection() const
	{
		if (m_projectionType == ProjectionType::Orthographical)
		{
			return std::dynamic_pointer_cast<OrthographicalProjection>(m_projection);
		}
		return nullptr;
	}

	std::shared_ptr<Projection> Camera::getProjection() const
	{
		return m_projection;
	}

	RotationOrder Camera::getRotationOrder() const
	{
		return m_rotationOrder;
	}

	void Camera::serialize(nlohmann::json& data)
	{
		// Serialize
		data["camera"]["rotationOrder"] = getRotationOrder();
		data["camera"]["projectionType"] = getProjectionType();
		switch (getProjectionType())
		{
		case ProjectionType::Perspective:
		{
			std::shared_ptr<PerspectiveProjection> perspective = getPerspectiveProjection();
			data["camera"]["perspective"]["zNear"] = perspective->getZNear();
			data["camera"]["perspective"]["zFar"] = perspective->getZFar();
			data["camera"]["perspective"]["fov"] = perspective->getFov();
			data["camera"]["perspective"]["aspectRatio"] = perspective->getAspectRatio();
			break;
		}
		case ProjectionType::Orthographical:
		{
			std::shared_ptr<OrthographicalProjection> orthographical = getOrthographicalProjection();
			data["camera"]["orthographical"]["zNear"] = orthographical->getZNear();
			data["camera"]["orthographical"]["zFar"] = orthographical->getZFar();
			data["camera"]["orthographical"]["left"] = orthographical->getLeft();
			data["camera"]["orthographical"]["right"] = orthographical->getRight();
			data["camera"]["orthographical"]["bottom"] = orthographical->getBottom();
			data["camera"]["orthographical"]["top"] = orthographical->getTop();
			break;
		}
		default:
			break;
		}
	}

	Mat4 Camera::getRotationMatrix(Vec3 rotation) const
	{
		switch (m_rotationOrder)
		{
		case RotationOrder::XYZ:
			return GetEulerMatrixXYZ(rotation);
		case RotationOrder::XZY:
			return GetEulerMatrixXZY(rotation);
		case RotationOrder::YXZ:
			return GetEulerMatrixYXZ(rotation);
		case RotationOrder::YZX:
			return GetEulerMatrixYZX(rotation);
		case RotationOrder::ZXY:
			return GetEulerMatrixZXY(rotation);
		case RotationOrder::ZYX:
			return GetEulerMatrixZYX(rotation);
		}
		return Mat4(1.0f);
	}

	void Camera::setCameraType(ProjectionType type)
	{

		m_projectionType = type;
		switch (type)
		{
		case ProjectionType::Perspective:
			m_projection = std::make_shared<PerspectiveProjection>();
			break;
		case ProjectionType::Orthographical:
			m_projection = std::make_shared<OrthographicalProjection>();
			break;
		default:
			break;
		}
	}

	void Camera::setRotationOrder(RotationOrder order)
	{
		m_rotationOrder = order;
	}

	void PerspectiveProjection::setAspectRatio(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
		updateProjection();
	}

	void PerspectiveProjection::setFov(float fov)
	{
		m_fov = fov;
		updateProjection();
	}

	void PerspectiveProjection::updateProjection()
	{
		m_projection = GetPerspectiveMatrix(m_fov, m_aspectRatio, m_zNear, m_zFar);
	}

	float PerspectiveProjection::getFov() const
	{
		return m_fov;
	}

	float PerspectiveProjection::getAspectRatio() const
	{
		return m_aspectRatio;
	}

	Mat4 Projection::getProjectionMatrix() const
	{
		return m_projection;
	}

	void Projection::setZNear(float zNear)
	{
		m_zNear = zNear;
		updateProjection();
	}

	void Projection::setZFar(float zFar)
	{
		m_zFar = zFar;
		updateProjection();
	}

	float Projection::getZNear() const
	{
		return m_zNear;
	}

	float Projection::getZFar() const
	{
		return m_zFar;
	}

	PerspectiveProjection::PerspectiveProjection()
	{
		updateProjection();
	}

	PerspectiveProjection::PerspectiveProjection(float fov, float aspectRatio, float zNear, float zFar) :
		m_fov(fov), m_aspectRatio(aspectRatio)
	{
		m_zNear = zNear;
		m_zFar = zFar;
		updateProjection();
	}

	OrthographicalProjection::OrthographicalProjection()
	{
		updateProjection();
	}

	OrthographicalProjection::OrthographicalProjection(float left, float right, float bottom, float top, float zNear, float zFar) :
		m_left(left), m_right(right), m_bottom(bottom), m_top(top)
	{
		m_zNear = zNear;
		m_zFar = zFar;
		updateProjection();
	}

	void OrthographicalProjection::setLeft(float left)
	{
		m_left = left;
		updateProjection();
	}

	void OrthographicalProjection::setRight(float right)
	{
		m_right = right;
		updateProjection();
	}

	void OrthographicalProjection::setBottom(float bottom)
	{
		m_bottom = bottom;
		updateProjection();
	}

	void OrthographicalProjection::setTop(float top)
	{
		m_top = top;
		updateProjection();
	}

	void OrthographicalProjection::setBorders(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
		m_zNear = zNear;
		m_zFar = zFar;
		updateProjection();
	}

	float OrthographicalProjection::getLeft()
	{
		return m_left;
	}

	float OrthographicalProjection::getRight()
	{
		return m_right;
	}

	float OrthographicalProjection::getBottom()
	{
		return m_bottom;
	}

	float OrthographicalProjection::getTop()
	{
		return m_top;
	}

	void OrthographicalProjection::updateProjection()
	{
		m_projection = GetOrthographicMatrix(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
	}
}