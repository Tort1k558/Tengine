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
		fieldIntensity->minValue = 0.000001f;
		fieldIntensity->maxValue = 100.0f;
		fieldIntensity->data = &m_intensity;
		fieldIntensity->name = "Intensity";
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
		fieldIntensity->minValue = 0.000001f;
		fieldIntensity->maxValue = 100.0f;
		fieldIntensity->data = &m_intensity;
		fieldIntensity->name = "Intensity";
		componentInfo.addElement(fieldIntensity);

		std::shared_ptr<FieldFloat> fieldRange = std::make_shared<FieldFloat>();
		fieldRange->minValue = 0.000001f;
		fieldRange->maxValue = 10000.0f;
		fieldRange->data = &m_range;
		fieldRange->name = "Range";
		componentInfo.addElement(fieldRange);

		return componentInfo;
	}
	
	SpotLight::SpotLight()
	{
	}

	ComponentInfo SpotLight::getInfo()
	{
		ComponentInfo componentInfo;
		componentInfo.setComponentName("SpotLight");

		std::shared_ptr<FieldFloat> fieldIntensity = std::make_shared<FieldFloat>();
		fieldIntensity->minValue = 0.000001f;
		fieldIntensity->maxValue = 100.0f;
		fieldIntensity->data = &m_intensity;
		fieldIntensity->name = "Intensity";
		componentInfo.addElement(fieldIntensity);

		std::shared_ptr<FieldFloat> fieldRange = std::make_shared<FieldFloat>();
		fieldRange->minValue = 0.000001f;
		fieldRange->maxValue = 10000.0f;
		fieldRange->data = &m_range;
		fieldRange->name = "Range";
		componentInfo.addElement(fieldRange);

		return componentInfo;
	}
}
