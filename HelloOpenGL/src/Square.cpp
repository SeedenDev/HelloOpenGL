#include "Square.h"

Square::Square(glm::vec3 position, glm::vec3 color)
{
    float squareVertices[] = {
        // pos      // color                   // texture
        -0.5, -0.5, color.r, color.g, color.b, 0.0, 0.0, // 0 (bottom-left)
         0.5, -0.5, color.r, color.g, color.b, 1.0, 0.0, // 1 (bottom-right)
         0.5,  0.5, color.r, color.g, color.b, 1.0, 1.0, // 2 (top-right)
        -0.5,  0.5, color.r, color.g, color.b, 0.0, 1.0  // 3 (top-left)
    };

    m_Transform.position = position;
    
    VertexBuffer vbo(squareVertices, sizeof(squareVertices));
    IndexBuffer ibo(g_SquareIndices, sizeof(g_SquareIndices));

    VertexLayout attributes;
    attributes.AddAttr<float>(2);
    attributes.AddAttr<float>(3);
    attributes.AddAttr<float>(2);
    m_Vao.ApplyLayout(vbo, attributes);

    m_Vao.Unbind();
    vbo.Unbind();
    ibo.Unbind();

    ComputeModelMatrix();
}

Square::~Square()
{
}

void Square::Draw() const
{
    m_Vao.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    m_Vao.Unbind();
}