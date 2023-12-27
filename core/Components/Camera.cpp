#include "Camera.h"

#include"Components/Transform.h"
#include"Scene/SceneManager.h"

namespace Tengine
{

	Camera::Camera()
	{
		setResolution(m_resolution);
		setCameraType(ProjectionType::Perspective);
	}
	Camera::Camera(UVec2 resolution)
	{
		setResolution(resolution);
		setCameraType(ProjectionType::Perspective);
	}

	Camera::Camera(UVec2 resolution, ProjectionType type)
	{
		setResolution(resolution);
		setCameraType(type);
	}

	ProjectionType Camera::getProjectionType() const
	{
		return m_projectionType;
	}

	std::shared_ptr<CubeMapTexture> Camera::getSkybox()
	{
		return m_skybox;
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

	void Camera::setAntiAliasingType(AntiAliasingType type)
	{
		m_antiAliasingType = type;
		recreateFramebuffer();
	}
	AntiAliasingType Camera::getAntiAliasingType() const
	{
		return m_antiAliasingType;
	}

	void Camera::setGamma(float gamma)
	{
		m_gamma = gamma;
	}
	float Camera::getGamma() const
	{
		return m_gamma;
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

		std::shared_ptr<FieldFile> skybox = std::make_shared<FieldFile>();
		skybox->name = "CubeMapTexture";
		skybox->path = m_skybox ? m_skybox->getPath() : "";
		skybox->callback = [this](const std::string& path) 
			{
				if (std::filesystem::exists(path))
				{
					this->setSkybox(AssetManager::LoadCubeMapTexture(path));
				}
			};
		componentInfo.addElement(skybox);

		std::shared_ptr<FieldEnum> antiAliasingType = std::make_shared<FieldEnum>();
		antiAliasingType->name = "AntiAliasing";
		antiAliasingType->elements = { "None","MSAA2","MSAA4","MSAA8" };
		antiAliasingType->currentElement = reinterpret_cast<int*>(&m_antiAliasingType);
		antiAliasingType->callback = [this](int element)
			{
				this->setAntiAliasingType(static_cast<AntiAliasingType>(element));
			};
		componentInfo.addElement(antiAliasingType);

		std::shared_ptr<FieldVec2> resolution = std::make_shared<FieldVec2>();
		resolution->name = "Resolution";
		resolution->minValue = 1.0f;
		resolution->data = &m_resolution;
		resolution->callback = [this]()
			{
				this->setResolution(m_resolution);
			};
		componentInfo.addElement(resolution);
		
		std::shared_ptr<FieldFloat> gamma = std::make_shared<FieldFloat>();
		gamma->name = "Gamma";
		gamma->minValue = 0.01f;
		gamma->data = &m_gamma;
		gamma->callback = [this]()
			{
				this->setGamma(m_gamma);
			};
		componentInfo.addElement(gamma);

		return componentInfo;
	}

	bool Camera::isLighting() const
	{
		return m_isLighting;
	}

	std::shared_ptr<FrameBuffer> Camera::getFramebuffer() const
	{
		return m_framebuffer;
	}

	Vec2 Camera::getResolution()
	{
		return m_resolution;
	}

	void Camera::setResolution(Vec2 resolution)
	{
		m_resolution = resolution;
		recreateFramebuffer();
	}

	void Camera::setLighting(bool value)
	{
		m_isLighting = value;
	}

	void Camera::setSkybox(std::shared_ptr<CubeMapTexture> skybox)
	{
		m_skybox = skybox;
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

	void Camera::recreateFramebuffer()
	{
		m_framebuffer = FrameBuffer::Create();
		switch (m_antiAliasingType)
		{
		case Tengine::AntiAliasingType::None:
			m_framebuffer->attachTexture(Texture::Create(nullptr, m_resolution, TextureType::RGBA8, TextureFilter::None), FrameBufferAttachment::Color);
			m_framebuffer->attachTexture(Texture::Create(nullptr, m_resolution, TextureType::DEPTH32F, TextureFilter::None), FrameBufferAttachment::Depth);
			break;
		case Tengine::AntiAliasingType::MSAA2:
			m_framebuffer->attachTexture(MultisampleTexture::Create(m_resolution, TextureType::RGBA8, 2), FrameBufferAttachment::Color);
			m_framebuffer->attachTexture(MultisampleTexture::Create(m_resolution, TextureType::DEPTH32F, 2), FrameBufferAttachment::Depth);
			break;
		case Tengine::AntiAliasingType::MSAA4:
			m_framebuffer->attachTexture(MultisampleTexture::Create(m_resolution, TextureType::RGBA8, 4), FrameBufferAttachment::Color);
			m_framebuffer->attachTexture(MultisampleTexture::Create(m_resolution, TextureType::DEPTH32F, 4), FrameBufferAttachment::Depth);
			break;
		case Tengine::AntiAliasingType::MSAA8:
			m_framebuffer->attachTexture(MultisampleTexture::Create(m_resolution, TextureType::RGBA8, 8), FrameBufferAttachment::Color);
			m_framebuffer->attachTexture(MultisampleTexture::Create(m_resolution, TextureType::DEPTH32F, 8), FrameBufferAttachment::Depth);
			break;
		default:
			break;
		}
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