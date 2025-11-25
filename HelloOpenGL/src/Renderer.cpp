#include "Renderer.h"

#include <GLFW/glfw3.h>

//WIP: Renderer class

Renderer::Renderer()
	: m_VA(VertexArray()), m_FrameCount(0), m_LastTime(glfwGetTime()), m_DeltaTime(0), m_FPS(0)
{

}

Renderer::~Renderer()
{

}

void Renderer::SetVertices(float* vertices, unsigned int size)
{

}

void Renderer::SetIndices(unsigned int* indices, unsigned int count)
{

}

void Renderer::SetVertexLayout(VertexLayout& vLayout)
{

}

void Renderer::LoadShader(const std::string& vertexShaderPath, const std::string& fragShaderPath)
{

}

double Renderer::GetDeltaTime() const
{
	return m_DeltaTime;
}

double Renderer::GetFPS() const
{
	return m_FPS;
}

// private

void Renderer::UpdateFPS()
{
	/*double renderMs = 1000.0 / m_FrameCount;
	m_FPS = m_FrameCount / m_DeltaTime;

	std::string windowTitle =
		"Hello OpenGL (FPS: " + std::to_string(m_FPS) + " - " + std::to_string(renderMs) + "ms) "
		"FrameCount: " + std::to_string(m_FrameCount) + " - DeltaTime:" + std::to_string(m_DeltaTime);

	appWindow.SetTitle(windowTitle);
	m_FrameCount = 0;
	m_LastTime = currentTime;*/
}

void Renderer::UpdateDeltaTime()
{
	/*double currentTime = glfwGetTime();
	m_DeltaTime = currentTime - m_LastTime;
	m_FrameCount++;*/
}

void Renderer::Tick(ApplicationWindow& appWindow)
{
	double currentTime = glfwGetTime();
	m_DeltaTime = currentTime - m_LastTime;
	m_FrameCount++;
	if (m_DeltaTime >= 1.0)
	{
		double renderMs = 1000.0 / m_FrameCount;
		m_FPS = m_FrameCount / m_DeltaTime;

		std::string windowTitle =
			"Hello OpenGL (FPS: " + std::to_string(m_FPS) + " - " + std::to_string(renderMs) + "ms) "
			"FrameCount: " + std::to_string(m_FrameCount) + " - DeltaTime:" + std::to_string(m_DeltaTime);

		appWindow.SetTitle(windowTitle);
		m_FrameCount = 0;
		m_LastTime = currentTime;
	}
}