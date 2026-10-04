#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace Input
{
	inline bool isKeyDown(const int keyCode)
	{
		GLFWwindow *windowPtr = ApplicationWindow::Get().GetWindowPointer();
		return glfwGetKey(windowPtr, keyCode) == GLFW_PRESS && ApplicationWindow::Get().IsFocused();
	}

	inline bool IsMouseButtonPressed(const int button)
	{
		GLFWwindow* windowPtr = ApplicationWindow::Get().GetWindowPointer();
		return glfwGetMouseButton(windowPtr, button) == GLFW_PRESS && ApplicationWindow::Get().IsFocused();
	}

	inline glm::vec2 GetMousePos()
	{
		GLFWwindow* windowPtr = ApplicationWindow::Get().GetWindowPointer();
		double mouseX, mouseY;
		glfwGetCursorPos(windowPtr, &mouseX, &mouseY);
		return { mouseX, mouseY };
	}
}