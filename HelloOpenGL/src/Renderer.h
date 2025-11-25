#pragma once

#include <string>

#include "ApplicationWindow.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

class Renderer
{
private:
	//TODO: unique_ptr? shared_ptr from Application.cpp?
	VertexArray m_VA;
	VertexBuffer* m_VB = nullptr;
	IndexBuffer* m_IB = nullptr;
	Shader* m_Shader= nullptr;

	int m_FrameCount;
	double m_LastTime;
	double m_DeltaTime;
	double m_FPS;

public:
	Renderer();
	~Renderer();

	//NOTE: meh.. Renderer renders the RenderContext containing those maybe?
	void SetVertices(float* vertices, unsigned int size);
	void SetIndices(unsigned int* indices, unsigned int count);
	void SetVertexLayout(VertexLayout& vLayout);
	void LoadShader(const std::string& vertexShaderPath, const std::string& fragShaderPath);

	double GetDeltaTime() const;
	double GetFPS() const;

private:
	void UpdateFPS();
	void UpdateDeltaTime();
	void Tick(ApplicationWindow& appWindow);
};