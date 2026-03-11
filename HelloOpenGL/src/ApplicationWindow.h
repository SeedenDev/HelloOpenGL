#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class ApplicationWindow
{
private:
	static ApplicationWindow* s_Instance;

	GLFWwindow* m_Window = nullptr;
	int m_Width, m_Height;
	float m_AspectRatio;
	bool m_Vsync = 1; // = VSYNC ON by default (monitor refresh rate = fps) - Better to be on (otherwise my gpu explodes lmao)
	bool m_Paused = 0;
	bool m_Focused = 1;

	double m_LastTimePausePressed; // Temp fix for "pausing" (access cursor) bc it's hell

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

	inline bool IsPaused() const { return m_Paused; }

	inline bool IsFocused() const { return m_Focused; }

	inline int GetWidth() const { return m_Width; }

	inline int GetHeight() const { return m_Height; }

private:

	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MousePosCallback(GLFWwindow* window, double mouseX, double mouseY);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void WindowFocusCallback(GLFWwindow* window, int focused);
};