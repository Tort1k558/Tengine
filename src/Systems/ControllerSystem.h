#pragma once

#include<memory>

#include"ECS/System.h"

class ControllerSystem : public System
{
public:
	void init() final;
	void update() final;
	void destroy() final;
};