#include "Light.h"

namespace Tengine
{
	float LightIntensable::getIntensity()
	{
		return m_intensity;
	}
	void LightIntensable::setIntensity(float intensity)
	{
		m_intensity = intensity;
	}

	float LightRangable::getRange()
	{
		return m_range;
	}

	void LightRangable::setRange(float range)
	{
		m_range = range;
	}
	DirectionLight::DirectionLight()
	{
	}


	ComponentInfo DirectionLight::getInfo()
	{
		ComponentInfo componentInfo;
		componentInfo.setComponentName("DirectionLight");

		std::shared_ptr<FieldFloat> fieldIntensity = std::make_shared<FieldFloat>();
		fieldIntensity->name = "Intensity";
		fieldIntensity->minValue = 0.000001f;
		fieldIntensity->maxValue = 100.0f;
		fieldIntensity->data = &m_intensity;
		componentInfo.addElement(fieldIntensity);
		
		return componentInfo;
	}

	PointLight::PointLight()
	{
	}
	
	ComponentInfo PointLight::getInfo()
	{
		ComponentInfo componentInfo;
		componentInfo.setComponentName("PointLight");

		std::shared_ptr<FieldFloat> fieldIntensity = std::make_shared<FieldFloat>();
		fieldIntensity->name = "Intensity";
		fieldIntensity->minValue = 0.000001f;
		fieldIntensity->maxValue = 100.0f;
		fieldIntensity->data = &m_intensity;
		componentInfo.addElement(fieldIntensity);

		std::shared_ptr<FieldFloat> fieldRange = std::make_shared<FieldFloat>();
		fieldRange->name = "Range";
		fieldRange->minValue = 0.000001f;
		fieldRange->maxValue = 10000.0f;
		fieldRange->data = &m_range;
		componentInfo.addElement(fieldRange);

		return componentInfo;
	}
	
	SpotLight::SpotLight()
	{
	}

	float SpotLight::getInnerConeAngle()
	{
		return m_innerConeAngle;
	}

	void SpotLight::setInnerConeAngle(float angle)
	{
		m_innerConeAngle = angle;
	}

	float SpotLight::getOuterConeAngle()
	{
		return m_outerConeAngle;
	}

	void SpotLight::setOuterConeAngle(float angle)
	{
		m_outerConeAngle = angle;
	}


	ComponentInfo SpotLight::getInfo()
	{
		ComponentInfo componentInfo;
		componentInfo.setComponentName("SpotLight");

		std::shared_ptr<FieldFloat> fieldIntensity = std::make_shared<FieldFloat>();
		fieldIntensity->name = "Intensity";
		fieldIntensity->minValue = 0.000001f;
		fieldIntensity->maxValue = 100.0f;
		fieldIntensity->data = &m_intensity;
		componentInfo.addElement(fieldIntensity);

		std::shared_ptr<FieldFloat> fieldRange = std::make_shared<FieldFloat>();
		fieldRange->name = "Range";
		fieldRange->minValue = 0.000001f;
		fieldRange->maxValue = 10000.0f;
		fieldRange->data = &m_range;
		componentInfo.addElement(fieldRange);

		std::shared_ptr<FieldFloat> fieldInnerConeAngle = std::make_shared<FieldFloat>();
		fieldInnerConeAngle->name = "InnerConeAngle";
		fieldInnerConeAngle->minValue = 0.000001f;
		fieldInnerConeAngle->maxValue = 360.0f;
		fieldInnerConeAngle->data = &m_innerConeAngle;
		componentInfo.addElement(fieldInnerConeAngle);

		std::shared_ptr<FieldFloat> fieldOuterConeAngle = std::make_shared<FieldFloat>();
		fieldOuterConeAngle->name = "OuterConeAngle";
		fieldOuterConeAngle->minValue = 0.000001f;
		fieldOuterConeAngle->maxValue = 360.0f;
		fieldOuterConeAngle->data = &m_outerConeAngle;
		componentInfo.addElement(fieldOuterConeAngle);

		return componentInfo;
	}
}
