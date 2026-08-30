#pragma once

#include <imgui/imgui.h>
#include <glm/mat4x4.hpp>

class ImGuiDebugInterface
{
public:
	virtual void ImGuiDebugDraw() = 0;
};