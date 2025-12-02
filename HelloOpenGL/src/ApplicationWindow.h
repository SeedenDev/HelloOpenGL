#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class ApplicationWindow
{
private:
	GLFWwindow* m_Window = nullptr; //TODO: maybe unique_ptr ?
	int m_Width, m_Height;
	float m_DefaultRatio; //TODO: implement framebuffer screen ratio to avoid distording render after resizing
	bool m_Vsync = 1; // = VSYNC ON by default (monitor refresh rate = fps) - Better to be on (otherwise my gpu explodes lmao)

	//TODO: Move to Camera.cpp
	double m_LastMouseX, m_LastMouseY;

public:
	ApplicationWindow(const std::string& title, int width, int height);
	~ApplicationWindow();

	void HandleKeyInput();

	//NOTE: is it bad to have these getters/setters inside the header?
	void ToggleVsync()
	{
		m_Vsync = !m_Vsync;
		glfwSwapInterval(m_Vsync);
	}

	inline bool IsVsync() { return m_Vsync; }

	void SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_Window, title.c_str());
	}

	inline int ShouldClose() const { return glfwWindowShouldClose(m_Window); }

	//NOTE: temp because Renderer not done and I don't think it is a good thing that we can access this ptr
	inline GLFWwindow* GetWindowPointerTemp() const { return m_Window; }

private:

	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MousePosCallback(GLFWwindow* window, double mouseX, double mouseY);
};