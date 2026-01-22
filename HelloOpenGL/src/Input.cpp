#include "Input.h"

GLFWwindow* Input::s_WindowPtr = nullptr;

bool Input::IsKeyPressed(const int keyCode)
{
	return glfwGetKey(s_WindowPtr, keyCode) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int button)
{
	return glfwGetMouseButton(s_WindowPtr, button) == GLFW_PRESS;
}

glm::vec2 Input::GetMousePos()
{
	double mouseX, mouseY;
	glfwGetCursorPos(s_WindowPtr, &mouseX, &mouseY);
	return { mouseX, mouseY };
}

void Input::SetWindowPointer(GLFWwindow* windowPtr)
{
	s_WindowPtr = windowPtr;
}