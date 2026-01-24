#include "Cube.h"

Cube::Cube(glm::vec3 position, glm::vec3 color)
{
    float cubeVertices[] = {
        // pos               // color                   // texture  // normal
        // Back face
         0.5f, -0.5f, -0.5f, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // 0 right bottom far
        -0.5f, -0.5f, -0.5f, color.r, color.g, color.b, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // 1 left bottom far
        -0.5f,  0.5f, -0.5f, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // 2 left top far
         0.5f,  0.5f, -0.5f, color.r, color.g, color.b, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // 3 right top far

        // Front face
        -0.5f, -0.5f,  0.5f, color.r, color.g, color.b, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 4 left bottom near
         0.5f, -0.5f,  0.5f, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 5 right bottom near
         0.5f,  0.5f,  0.5f, color.r, color.g, color.b, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 6 right top near
        -0.5f,  0.5f,  0.5f, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 7 left top near

        // Left face
        -0.5f, -0.5f, -0.5f, color.r, color.g, color.b, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // 8 left bottom far DUP1
        -0.5f, -0.5f,  0.5f, color.r, color.g, color.b, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 9 left bottom near DUP4
        -0.5f,  0.5f,  0.5f, color.r, color.g, color.b, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 10 left top near DUP7
        -0.5f,  0.5f, -0.5f, color.r, color.g, color.b, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // 11 left top far DUP2

        // Right face
         0.5f, -0.5f,  0.5f, color.r, color.g, color.b, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 12 right bottom near DP5
         0.5f, -0.5f, -0.5f, color.r, color.g, color.b, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 13 right bottom far DUP0
         0.5f,  0.5f, -0.5f, color.r, color.g, color.b, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 14 right top far DUP3
         0.5f,  0.5f,  0.5f, color.r, color.g, color.b, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 15 right top near DUP6

        // Bottom face
        -0.5f, -0.5f, -0.5f, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // 16 left bottom far DUP1
         0.5f, -0.5f, -0.5f, color.r, color.g, color.b, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // 17 right bottom far DUP0
         0.5f, -0.5f,  0.5f, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // 18 right bottom near DUP5
        -0.5f, -0.5f,  0.5f, color.r, color.g, color.b, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // 19 left bottom near DUP4

        // Top face
        -0.5f,  0.5f,  0.5f, color.r, color.g, color.b, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 20 left top near DUP7
         0.5f,  0.5f,  0.5f, color.r, color.g, color.b, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 21 right top near DUP6
         0.5f,  0.5f, -0.5f, color.r, color.g, color.b, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 22 right top far DUP3
        -0.5f,  0.5f, -0.5f, color.r, color.g, color.b, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f  // 23 left top far DUP2
    };

    m_Transform.position = position;
    
    VertexBuffer vbo(cubeVertices, sizeof(cubeVertices));
    IndexBuffer ibo(g_CubeIndices, sizeof(g_CubeIndices));

    VertexLayout attributes;
    attributes.AddAttr<float>(3);
    attributes.AddAttr<float>(3);
    attributes.AddAttr<float>(2);
    attributes.AddAttr<float>(3);
    m_Vao.ApplyLayout(vbo, attributes);

    m_Vao.Unbind();
    vbo.Unbind();
    ibo.Unbind();

    ComputeModelMatrix();
}

Cube::~Cube()
{
}

void Cube::Draw() const
{
    m_Vao.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(g_CubeIndices), GL_UNSIGNED_INT, 0);
    m_Vao.Unbind();
}