#include"ECS/Component.h"


namespace Tengine
{
	class TENGINE LightIntensable
	{
	public:
		virtual ~LightIntensable() = default;
		float getIntensity();
		void setIntensity(float intensity);
	protected:
		float m_intensity = 1.0f;
	};

	class TENGINE LightRangable
	{
	public:
		virtual ~LightRangable() = default;
		float getRange();
		void setRange(float range);
	protected:
		float m_range = 5.0f;
	};

	class TENGINE DirectionLight : public Component, public LightIntensable
	{
	public:
		DirectionLight();
		ComponentInfo getInfo() override;
	};
	class TENGINE PointLight : public Component, public LightIntensable, public LightRangable
	{
	public:
		PointLight();
		ComponentInfo getInfo() override;
	};
	class TENGINE SpotLight : public Component, public LightIntensable, public LightRangable
	{
	public:
		SpotLight();
		float getInnerConeAngle();
		void setInnerConeAngle(float angle);
		float getOuterConeAngle();
		void setOuterConeAngle(float angle);
		ComponentInfo getInfo() override;

	private:
		float m_innerConeAngle = 12.5f;
		float m_outerConeAngle = 16.5f;
	};
}