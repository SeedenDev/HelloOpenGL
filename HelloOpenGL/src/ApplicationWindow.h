#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class ApplicationWindow
{
private:
	GLFWwindow* m_Window = nullptr;
	int m_Width, m_Height;
	float m_AspectRatio;
	bool m_Vsync = 1; // = VSYNC ON by default (monitor refresh rate = fps) - Better to be on (otherwise my gpu explodes lmao)

public:
	ApplicationWindow(const std::string& title, int width, int height);
	~ApplicationWindow();

	void Update();

	void ToggleVsync()
	{
		m_Vsync = !m_Vsync;
		glfwSwapInterval(m_Vsync);
	}

	inline bool IsVsync() const { return m_Vsync; }

	void SetTitle(const std::string& title) const
	{
		glfwSetWindowTitle(m_Window, title.c_str());
	}

	void Resize(int width, int height)
	{
		glViewport(0, 0, width, height);
		m_Width = width;
		m_Height = height;
		m_AspectRatio = (float)width / (float)height;
	}

	inline int ShouldClose() const { return glfwWindowShouldClose(m_Window); }

	inline GLFWwindow* GetWindowPointer() const { return m_Window; }

	inline float GetAspectRatio() const { return m_AspectRatio; }

private:

	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MousePosCallback(GLFWwindow* window, double mouseX, double mouseY);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};