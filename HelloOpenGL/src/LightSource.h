#pragma once

#include "Cube.h"

class LightSource : public Cube
{
public:
	// For now, white light only
	LightSource(glm::vec3 pos)
		: Cube(pos, glm::vec3(1.0f))
	{

	}
};