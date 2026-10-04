#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "GlobalUtil.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"

const float g_QuadVertices[] = {
    // pos      // color          // texture
    -0.5, -0.5, 1.0f, 1.0f, 1.0f, 0.0, 0.0, // 0 (bottom-left)
     0.5, -0.5, 1.0f, 1.0f, 1.0f, 1.0, 0.0, // 1 (bottom-right)
     0.5,  0.5, 1.0f, 1.0f, 1.0f, 1.0, 1.0, // 2 (top-right)
    -0.5,  0.5, 1.0f, 1.0f, 1.0f, 0.0, 1.0  // 3 (top-left)
};

const unsigned int g_QuadIndices[] = {
    0, 1, 2,
    2, 3, 0
};

class Quad
{
private:
    VertexArray m_Vao;
    VertexBuffer m_Vbo = VertexBuffer(g_QuadVertices, sizeof(g_QuadVertices));
    IndexBuffer m_Ibo = IndexBuffer(g_QuadIndices, sizeof(g_QuadIndices));
    Geometry::Transform3D m_Transform;
    glm::vec4 m_Color;

    glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

public:
    explicit Quad(const glm::vec3& position, const glm::vec4& color = glm::vec4(1.0f));
    ~Quad();

    const glm::vec3& GetPosition() const { return m_Transform.position; }
    const glm::vec3& GetEulerRotation() const { return m_Transform.rotation; }
    const glm::vec3& GetScale() const { return m_Transform.scale; }
    const glm::vec4& GetColor() const { return m_Color; }

    void SetPosition(const glm::vec3& newPos)
    {
        m_Transform.position = newPos;
        UpdateModelMatrix();
    }
    void Translate(const glm::vec3& translation)
    {
        m_Transform.position += translation;
        UpdateModelMatrix();
    }
    void SetEulerRotation(const glm::vec3& newRotation)
    {
        m_Transform.rotation = newRotation;
        UpdateModelMatrix();
    }
    void RotateEuler(const glm::vec3& rotation)
    {
        m_Transform.rotation += rotation;
        UpdateModelMatrix();
    }
    void SetScale(const glm::vec3& newScale)
    {
        m_Transform.scale = newScale;
        UpdateModelMatrix();
    }

    void Draw() const;

    const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }

private:

    void UpdateModelMatrix()
    {
        m_ModelMatrix = MathUtil::ComputeModelMatrix(m_Transform);
    }
};