#include "Framebuffer.h"

//TODO: make MSAA work

Framebuffer::Framebuffer(int width, int height)
    : m_Width(width), m_Height(height)
{
    m_Vao.Bind();
    m_Vbo.Bind();
    m_Ibo.Bind();

    VertexLayout attributes;
    attributes.AddAttr<float>(2);
    attributes.AddAttr<float>(2);
    m_Vao.ApplyLayout(m_Vbo, attributes);

    m_Vao.Unbind();
    m_Vbo.Unbind();
    m_Ibo.Unbind();

    // GL framebuffer setup
    glGenFramebuffers(1, &m_Fb);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Fb);

    // Create a texture attachement for the color 
    glGenTextures(1, &m_ColorTexture);
    glActiveTexture(GL_TEXTURE30);
    glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // RGB or RGBA maybe RGBA8 doesn't work
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

    //Note: for depth buffer and stencil buffer it is also possible to set a texture, and even ONE of 32bits for both (24bits depth buffer + 8bits stencil). 
    // Attachment = GL_DEPTH/STENCIL_ATTACHEMENT ; Component (for glTexImage instead of RGBA8): GL_DEPTH_COMPONENT/GL_STENCIL_INDEX
    // Or for 2 in 1: glTexImage = GL_DEPTH24_STENCIL8 + GL_DEPTH_STENCIL + GL_UNSIGNED_INT_24_8 
    //                glFbTexture = GL_DEPTH_STENCIL_ATTACHMENT
    // Texture: if we want read/write. Renderbuffer if we don't need to read the samples = good for depth/stencil

    // For the depth/stencil then a write-only render buffer object
    glGenRenderbuffers(1, &m_Rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "You fucked up." << std::endl;
        throw std::exception::exception("Framebuffer is invalid. Aborting program.");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
    glDeleteTextures(1, &m_ColorTexture);
    glDeleteRenderbuffers(1, &m_Rbo);
    glDeleteFramebuffers(1, &m_Fb);
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Fb);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//Note: don't forget to bind the framebuffer where we want to draw beforehand calling this
void Framebuffer::Draw(Shader& shader)
{
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.Bind();
    shader.SetUniform1i("u_Texture", 30);
    m_Vao.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    m_Vao.Unbind();
    shader.Unbind();
    glEnable(GL_DEPTH_TEST);
}

void Framebuffer::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;

    glActiveTexture(GL_TEXTURE30);
    glBindTexture(GL_TEXTURE_2D, m_ColorTexture); // be sure the slot hasn't' been changed
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    Bind();
    glViewport(0, 0, width, height);
    Unbind();
}