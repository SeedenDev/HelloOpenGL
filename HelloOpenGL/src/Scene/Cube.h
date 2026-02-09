#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "GlobalUtil.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexLayout.h"

const float g_CubeVertices[] = {
    // pos               // color          // texture  // normal
    // Back face
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // 0 right bottom far
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // 1 left bottom far
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // 2 left top far
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // 3 right top far

     // Front face
     -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 4 left bottom near
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 5 right bottom near
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 6 right top near
     -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 7 left top near

     // Left face
     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // 8 left bottom far DUP1
     -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 9 left bottom near DUP4
     -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 10 left top near DUP7
     -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // 11 left top far DUP2

     // Right face
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 12 right bottom near DP5
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 13 right bottom far DUP0
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 14 right top far DUP3
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 15 right top near DUP6

      // Bottom face
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // 16 left bottom far DUP1
       0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // 17 right bottom far DUP0
       0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // 18 right bottom near DUP5
      -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // 19 left bottom near DUP4

      // Top face
      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 20 left top near DUP7
       0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 21 right top near DUP6
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 22 right top far DUP3
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f  // 23 left top far DUP2
};

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
    //TODO: future batch rendering, no vao here. A BatchRenderer class, and then cube.GetVerticesData()+IndicesData() (to create the big buffers) 
    // and perhaps GetTransform() => the model matrix will be computed in the renderer (no UpdateModelMatrix() that just queries a util func and update a member bruh)
    VertexArray m_Vao;
    VertexBuffer m_Vbo = VertexBuffer(g_CubeVertices, sizeof(g_CubeVertices));
    IndexBuffer m_Ibo = IndexBuffer(g_CubeIndices, sizeof(g_CubeIndices));
    Geometry::Transform3D m_Transform;
    glm::vec4 m_Color;

    glm::mat4 m_ModelMatrix;

public:
    Cube(glm::vec3 position, glm::vec4 color = glm::vec4(1.0f));
    ~Cube();

    inline const glm::vec3& GetPosition() const { return m_Transform.position; }
    inline const glm::vec3& GetEulerRotation() const { return m_Transform.rotation; }
    inline const glm::vec3& GetScale() const { return m_Transform.scale; }
    inline const glm::vec4& GetColor() const { return m_Color; }

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