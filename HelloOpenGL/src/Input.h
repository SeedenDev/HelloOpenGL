#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class Input
{
private:
	//TODO: just restart a project from scratch with a better architecture to avoid this and allow a singleton without "gl.h included before glew.h" error lmao
	static GLFWwindow* s_WindowPtr;

public:

	static bool IsKeyPressed(const int keyCode);

	static bool IsMouseButtonPressed(int button);

	static glm::vec2 GetMousePos();

	static void SetWindowPointer(GLFWwindow* windowPtr);
};