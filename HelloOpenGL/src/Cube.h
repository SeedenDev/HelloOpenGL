#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "GlobalUtil.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"

const unsigned int g_CubeIndices[] = {
    0, 1, 2,       2, 3, 0,    // Back 
    4, 5, 6,       6, 7, 4,    // Front
    8, 9, 10,      10, 11, 8,  // Left
    12, 13, 14,    14, 15, 12, // Right
    16, 17, 18,    18, 19, 16, // Bottom
    20, 21, 22,    22, 23, 20, // Top
};

class Cube
{
private:

    VertexArray m_Vao;
    Geometry::Transform3D m_Transform;

    glm::mat4 m_ModelMatrix;

public:
    Cube(glm::vec3 position, glm::vec3 color = glm::vec3(1.0f));
    ~Cube();

    const glm::vec3& GetPosition() const { return m_Transform.position; }
    const glm::vec3& GetEulerRotation() const { return m_Transform.rotation; }
    const glm::vec3& GetScale() const { return m_Transform.scale; }

    void SetPosition(glm::vec3 newPos)
    {
        m_Transform.position = newPos;
        UpdateModelMatrix();
    }
    void Translate(glm::vec3 translation)
    {
        m_Transform.position += translation;
        UpdateModelMatrix();
    }
    void SetEulerRotation(glm::vec3 newRotation)
    {
        m_Transform.rotation = newRotation;
        UpdateModelMatrix();
    }
    void RotateEuler(glm::vec3 rotation)
    {
        m_Transform.rotation += rotation;
        UpdateModelMatrix();
    }
    void SetScale(glm::vec3 newScale)
    {
        m_Transform.scale = newScale;
        UpdateModelMatrix();
    }

    void Draw() const;

    inline const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }

private:

    void UpdateModelMatrix()
    {
        m_ModelMatrix = MathUtil::ComputeModelMatrix(m_Transform);
    }
};