#pragma once

#include <GLFW/glfw3.h>

namespace Input
{
	inline bool isKeyDown(const int keyCode)
	{
		const GLFWwindow* const windowPtr = ApplicationWindow::Get().GetWindowPointer();
		return glfwGetKey((GLFWwindow*)windowPtr, keyCode) == GLFW_PRESS && ApplicationWindow::Get().IsFocused();
	}

	inline bool IsMouseButtonPressed(const int button)
	{
		const GLFWwindow* const windowPtr = ApplicationWindow::Get().GetWindowPointer();
		return glfwGetMouseButton((GLFWwindow*)windowPtr, button) == GLFW_PRESS && ApplicationWindow::Get().IsFocused();
	}

	inline std::pair<double, double> GetMousePos()
	{
		const GLFWwindow* const windowPtr = ApplicationWindow::Get().GetWindowPointer();
		double mouseX, mouseY;
		glfwGetCursorPos((GLFWwindow*)windowPtr, &mouseX, &mouseY);
		return { mouseX, mouseY };
	}
}