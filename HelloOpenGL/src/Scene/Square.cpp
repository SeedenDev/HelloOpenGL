#include "Square.h"

Square::Square(glm::vec3 position, glm::vec4 color)
    : m_Color(color)
{
    m_Transform.position = position;

    m_Vao.Bind();
    m_Vbo.Bind();
    m_Ibo.Bind();

    VertexLayout attributes;
    attributes.AddAttr<float>(2);
    attributes.AddAttr<float>(3);
    attributes.AddAttr<float>(2);
    m_Vao.ApplyLayout(m_Vbo, attributes);

    m_Vao.Unbind();
    m_Vbo.Unbind();
    m_Ibo.Unbind();

    UpdateModelMatrix();
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