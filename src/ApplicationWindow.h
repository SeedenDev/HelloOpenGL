#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ApplicationWindow
{
private:
	static ApplicationWindow* s_Instance;

	GLFWwindow* m_Window;
	int m_Width, m_Height;
	float m_AspectRatio;
	bool m_Vsync = 1; // = VSYNC ON by default (monitor refresh rate = fps) - Better to be on (otherwise my gpu explodes lmao)
	bool m_Paused = 0;
	bool m_Focused = 1;

	double m_LastTimePausePressed = 0; // Temp fix for "pausing" (access cursor) bc it's hell

public:
	ApplicationWindow(const std::string& title, int width, int height);
	~ApplicationWindow();

	static ApplicationWindow& Get() { return *s_Instance; }

	void Update();

	void ToggleVsync()
	{
		m_Vsync = !m_Vsync;
		glfwSwapInterval(m_Vsync);
	}

	bool IsVsync() const { return m_Vsync; }

	void SetTitle(const std::string& title) const
	{
		glfwSetWindowTitle(m_Window, title.c_str());
	}

	void Resize(const int width, const int height)
	{
		glViewport(0, 0, width, height);
		m_Width = width;
		m_Height = height;
		m_AspectRatio = (float)width / (float)height;
	}

	int ShouldClose() const { return glfwWindowShouldClose(m_Window); }

	GLFWwindow* GetWindowPointer() const { return m_Window; }

	float GetAspectRatio() const { return m_AspectRatio; }

	bool IsPaused() const { return m_Paused; }

	bool IsFocused() const { return m_Focused; }

	int GetWidth() const { return m_Width; }

	int GetHeight() const { return m_Height; }

private:

	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MousePosCallback(GLFWwindow* window, double mouseX, double mouseY);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void WindowFocusCallback(GLFWwindow* window, int focused);
};