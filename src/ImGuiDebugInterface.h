#pragma once

// This include allows child classes to have imgui included without them needing to individually have this line
#include <imgui/imgui.h>

class ImGuiDebugInterface
{
public:
	virtual void ImGuiDebugDraw() = 0;
	// TLDR: polymorphic class needs to override the default non-virtual destructor because it is UB to have a polymorphic class with it as calling a derived-class destructor
	// as its base class would cal the base class (non-virtual) destructor instead of the derived class destructor. Making it virtual = it'll use the one from the derived class.
	// Source: https://stackoverflow.com/questions/22881085/destructors-in-non-polymorphic-base-classes (same for the HasPosition/HasDirection "interfaces")
	virtual ~ImGuiDebugInterface() = default;
};