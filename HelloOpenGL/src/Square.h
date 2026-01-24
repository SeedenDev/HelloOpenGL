#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Geometry.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"

const unsigned int g_SquareIndices[] = {
    0, 1, 2,
    2, 3, 0
};

class Square
{
private:

    VertexArray m_Vao;
    Transform3D m_Transform;

    glm::mat4 m_ModelMatrix;

public:
    Square(glm::vec3 position, glm::vec3 color = glm::vec3(1.0f));
    ~Square();

    const glm::vec3& GetPosition() const { return m_Transform.position; }
    const glm::vec3& GetEulerRotation() const { return m_Transform.rotation; }
    const glm::vec3& GetScale() const { return m_Transform.scale; }

    void SetPosition(glm::vec3 newPos)
    {
        m_Transform.position = newPos;
        ComputeModelMatrix();
    }
    void Translate(glm::vec3 translation)
    {
        m_Transform.position += translation;
        ComputeModelMatrix();
    }
    void SetEulerRotation(glm::vec3 newRotation)
    {
        m_Transform.rotation = newRotation;
        ComputeModelMatrix();
    }
    void RotateEuler(glm::vec3 rotation)
    {
        m_Transform.rotation += rotation;
        ComputeModelMatrix();
    }
    void SetScale(glm::vec3 newScale)
    {
        m_Transform.scale = newScale;
        ComputeModelMatrix();
    }

    void Draw() const;

    inline const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }

private:
    
    void ComputeModelMatrix()
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, m_Transform.position);
        //TODO: Rotations are euler angles for now so gimble lock + not sure about the tri-rotation
        model = glm::rotate(model, glm::radians(m_Transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_Transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, m_Transform.scale);
        m_ModelMatrix = model;
    }
};