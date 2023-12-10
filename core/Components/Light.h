#include"Scene/Component.h"


namespace Tengine
{
	class TENGINE Light
	{
	public:
		virtual ~Light() = default;
		float getIntensity();
		void setIntensity(float intensity);
		Vec3 getColor();
		void setColor(Vec3 color);
	protected:
		float m_intensity = 1.0f;
		Vec3 m_color = { 1.0f,1.0f,1.0f };
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

	class TENGINE DirectionLight : public Component, public Light
	{
	public:
		DirectionLight();
		ComponentInfo getInfo() override;
	};
	class TENGINE PointLight : public Component, public Light, public LightRangable
	{
	public:
		PointLight();
		ComponentInfo getInfo() override;
	};
	class TENGINE SpotLight : public Component, public Light, public LightRangable
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