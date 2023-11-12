#include "Camera.h"

#include"Components/Transform.h"
#include"Scene/SceneManager.h"

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
		return Math::GetLookAtMatrix(transform->getPosition(), direction, up);
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
		ComponentInfo componentInfo;
		componentInfo.setComponentName("Camera");
		std::shared_ptr<FieldEnum> projectionType = std::make_shared<FieldEnum>();
		projectionType->name = "Projection";
		projectionType->elements = { "Perspective","Orthographical" };
		projectionType->currentElement = reinterpret_cast<int*>(&m_projectionType);
		projectionType->callback = [this](int element)
		{
			this->setCameraType(m_projectionType);
		};
		componentInfo.addElement(projectionType);

		std::shared_ptr<FieldFloat> projectionSettingsZNear = std::make_shared<FieldFloat>();
		projectionSettingsZNear->data = &m_projection->m_zNear;
		projectionSettingsZNear->minValue = 0.01f;
		projectionSettingsZNear->maxValue = 10000.0f;
		projectionSettingsZNear->name = "zNear";
		projectionSettingsZNear->callback = [this]() {m_projection->updateProjection(); };
		componentInfo.addElement(projectionSettingsZNear);
		std::shared_ptr<FieldFloat> projectionSettingsZFar = std::make_shared<FieldFloat>();
		projectionSettingsZFar->data = &m_projection->m_zFar;
		projectionSettingsZFar->minValue = 0.01f;
		projectionSettingsZFar->maxValue = 10000.0f;
		projectionSettingsZFar->name = "zFar";
		projectionSettingsZFar->callback = [this]() {m_projection->updateProjection(); };
		componentInfo.addElement(projectionSettingsZFar);

		switch (m_projectionType)
		{
		case ProjectionType::Perspective:
		{
			std::shared_ptr<PerspectiveProjection> perspective = std::dynamic_pointer_cast<PerspectiveProjection>(getProjection());
			std::shared_ptr<FieldFloat> projectionSettingsAspect = std::make_shared<FieldFloat>();
			projectionSettingsAspect->data = &perspective->m_aspectRatio;
			projectionSettingsAspect->minValue = 0.0f;
			projectionSettingsAspect->maxValue = 3.0f;
			projectionSettingsAspect->name = "Aspect";
			projectionSettingsAspect->callback = [this]() {m_projection->updateProjection(); };
			componentInfo.addElement(projectionSettingsAspect);
			std::shared_ptr<FieldFloat> projectionSettingsFov = std::make_shared<FieldFloat>();
			projectionSettingsFov->data = &perspective->m_fov;
			projectionSettingsFov->minValue = 0.0f;
			projectionSettingsFov->maxValue = 360.0f;
			projectionSettingsFov->name = "Fov";
			projectionSettingsFov->callback = [this]() {m_projection->updateProjection(); };
			componentInfo.addElement(projectionSettingsFov);
			break;
		}
		case ProjectionType::Orthographical:
		{
			std::shared_ptr<OrthographicalProjection> projection = std::dynamic_pointer_cast<OrthographicalProjection>(getProjection());
			std::shared_ptr<FieldFloat> projectionSettingsLeft = std::make_shared<FieldFloat>();
			projectionSettingsLeft->data = &projection->m_left;
			projectionSettingsLeft->minValue = -0.01f;
			projectionSettingsLeft->maxValue = -10.0f;
			projectionSettingsLeft->name = "Left";
			projectionSettingsLeft->callback = [this]() {m_projection->updateProjection(); };
			componentInfo.addElement(projectionSettingsLeft);
			std::shared_ptr<FieldFloat> projectionSettingsRight = std::make_shared<FieldFloat>();
			projectionSettingsRight->data = &projection->m_right;
			projectionSettingsRight->minValue = 0.01f;
			projectionSettingsRight->maxValue = 10.0f;
			projectionSettingsRight->name = "Right";
			projectionSettingsRight->callback = [this]() {m_projection->updateProjection(); };
			componentInfo.addElement(projectionSettingsRight);
			std::shared_ptr<FieldFloat> projectionSettingsBottom = std::make_shared<FieldFloat>();
			projectionSettingsBottom->data = &projection->m_bottom;
			projectionSettingsBottom->minValue = -0.01f;
			projectionSettingsBottom->maxValue = -10.0f;
			projectionSettingsBottom->name = "Bottom";
			projectionSettingsBottom->callback = [this]() {m_projection->updateProjection(); };
			componentInfo.addElement(projectionSettingsBottom);
			std::shared_ptr<FieldFloat> projectionSettingsTop = std::make_shared<FieldFloat>();
			projectionSettingsTop->data = &projection->m_top;
			projectionSettingsTop->minValue = 0.01f;
			projectionSettingsTop->maxValue = 10.0f;
			projectionSettingsTop->name = "Top";
			projectionSettingsTop->callback = [this]() {m_projection->updateProjection(); };
			componentInfo.addElement(projectionSettingsTop);
			break;
		}
		default:
			break;
		}
		std::shared_ptr<FieldBool> enableLighting = std::make_shared<FieldBool>();
		enableLighting->data = &m_isLighting;
		enableLighting->name = "Lighting";
		componentInfo.addElement(enableLighting);
		return componentInfo;
	}

	bool Camera::isLighting() const
	{
		return m_isLighting;
	}

	void Camera::setLighting(bool value)
	{
		m_isLighting = value;
	}

	std::shared_ptr<Projection> Camera::getProjection() const
	{
		return m_projection;
	}

	RotationOrder Camera::getRotationOrder() const
	{
		return m_rotationOrder;
	}

	Mat4 Camera::getRotationMatrix(Vec3 rotation) const
	{
		switch (m_rotationOrder)
		{
		case RotationOrder::XYZ:
			return Math::GetEulerMatrixXYZ(rotation);
		case RotationOrder::XZY:
			return Math::GetEulerMatrixXZY(rotation);
		case RotationOrder::YXZ:
			return Math::GetEulerMatrixYXZ(rotation);
		case RotationOrder::YZX:
			return Math::GetEulerMatrixYZX(rotation);
		case RotationOrder::ZXY:
			return Math::GetEulerMatrixZXY(rotation);
		case RotationOrder::ZYX:
			return Math::GetEulerMatrixZYX(rotation);
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
		m_projection = Math::GetPerspectiveMatrix(m_fov, m_aspectRatio, m_zNear, m_zFar);
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
		m_projection = Math::GetOrthographicMatrix(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
	}
}