#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "ApplicationWindow.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

int main(void)
{
    ApplicationWindow appWindow("Hello OpenGL", 720, 480);

    std::cout << glGetString(GL_VERSION) << std::endl;

    //TODO: everything below in a Renderer class

    /* VBO(vertices), IBO(indices) & VAO(attributes + VBO/IBO) + Shader setup */
    float vertices[] = {
       // pos      // color
       -0.5, -0.5, 0.0, 0.0, 1.0, // 0 (bottom-left)
        0.5, -0.5, 0.0, 1.0, 0.0, // 1 (bottom-right)
        0.5,  0.5, 1.0, 0.0, 0.0, // 2 (top-right)
       -0.5,  0.5, 0.0, 0.0, 1.0  // 3 (top-left)
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray vao;
    VertexBuffer vbo(vertices, sizeof(vertices));
    IndexBuffer ibo(indices, sizeof(indices));

    VertexLayout vLayout;
    vLayout.AddAttr<float>(2);
    vLayout.AddAttr<float>(3);
    vao.ApplyLayout(vbo, vLayout);

    vao.Unbind();
    vbo.Unbind();
    ibo.Unbind();

	Shader shaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    double lastTime = glfwGetTime();
    // Graphics settings for fps: [SET/UNLIMITED/VSYNC]
    double fpsLimit = 1.0 / 60.0;
    bool unlimitedFPS = 1;
    //appWindow.ToggleVsync();

    while (!appWindow.ShouldClose())
    {
        /* DeltaTime into [Vsync/Set/Unlimited]-FPS based render */
        bool vSync = appWindow.IsVsync();
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        if (vSync || (!vSync && (!unlimitedFPS && deltaTime >= fpsLimit) || (unlimitedFPS)))
        {
            /* Fps counter */
            double fps = 1 / deltaTime;
            double renderMs = 1000.0 / fps;

            std::string windowTitle =
                "Hello OpenGL (FPS: " + std::to_string(fps) + " - " + std::to_string(renderMs) + "ms) DeltaTime:" + std::to_string(deltaTime);

            appWindow.SetTitle(windowTitle);
            lastTime = currentTime;

            /* Render */
            glClear(GL_COLOR_BUFFER_BIT);

            shaderProgram.Bind();
            shaderProgram.setUniform1f("u_Time", currentTime);

            vao.Bind();
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            vao.Unbind();
            shaderProgram.Unbind();

            /* Swap front and back buffers */
            glfwSwapBuffers(appWindow.GetWindowPointerTemp());
        }
        /* Poll for and process events */
        glfwPollEvents();
    }
}