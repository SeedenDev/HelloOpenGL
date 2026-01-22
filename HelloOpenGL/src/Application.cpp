#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "ApplicationWindow.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"

int main(void)
{
    ApplicationWindow appWindow("Hello OpenGL", 720, 480);

    std::cout << glGetString(GL_VERSION) << std::endl;

    //TODO: everything below in a Renderer class

    /* VBO(vertices), IBO(indices) & VAO(attributes + VBO/IBO) + Shader setup */

    // Blending for alpha channels
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Square part
    float squareVertices[] = {
       // pos      // color       // texture
       -0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 0.0, // 0 (bottom-left)
        0.5, -0.5, 0.0, 1.0, 0.0, 1.0, 0.0, // 1 (bottom-right)
        0.5,  0.5, 1.0, 0.0, 0.0, 1.0, 1.0, // 2 (top-right)
       -0.5,  0.5, 0.0, 0.0, 1.0, 0.0, 1.0  // 3 (top-left)
    };
    unsigned int squareIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray squareVao;
    VertexBuffer squareVbo(squareVertices, sizeof(squareVertices));
    IndexBuffer squareIbo(squareIndices, sizeof(squareIndices));

    VertexLayout squareVertexLayout;
    squareVertexLayout.AddAttr<float>(2);
    squareVertexLayout.AddAttr<float>(3);
    squareVertexLayout.AddAttr<float>(2);
    squareVao.ApplyLayout(squareVbo, squareVertexLayout);

    squareVao.Unbind();
    squareVbo.Unbind();
    squareIbo.Unbind();
    // End square

    // Cube part
    float cubeVertices[] = {
         // pos               // color       // texture
         // Back face
          0.5f, -0.5f, -0.5f, 1.0, 1.0, 1.0, 1.0f, 0.0f, // 0 right bottom far
         -0.5f, -0.5f, -0.5f, 1.0, 1.0, 1.0, 0.0f, 0.0f, // 1 left bottom far
         -0.5f,  0.5f, -0.5f, 1.0, 1.0, 1.0, 0.0f, 1.0f, // 2 left top far
          0.5f,  0.5f, -0.5f, 1.0, 1.0, 1.0, 1.0f, 1.0f, // 3 right top far

         // Front face
         -0.5f, -0.5f,  0.5f, 1.0, 1.0, 1.0, 0.0f, 0.0f, // 4 left bottom near
          0.5f, -0.5f,  0.5f, 1.0, 1.0, 1.0, 1.0f, 0.0f, // 5 right bottom near
          0.5f,  0.5f,  0.5f, 1.0, 1.0, 1.0, 1.0f, 1.0f, // 6 right top near
         -0.5f,  0.5f,  0.5f, 1.0, 1.0, 1.0, 0.0f, 1.0f, // 7 left top near

         // Left face
         -0.5f, -0.5f, -0.5f, 1.0, 1.0, 1.0, 0.0f, 1.0f, // 8 left bottom far DUP1
         -0.5f, -0.5f,  0.5f, 1.0, 1.0, 1.0, 0.0f, 0.0f, // 9 left bottom near DUP4
         -0.5f,  0.5f,  0.5f, 1.0, 1.0, 1.0, 1.0f, 0.0f, // 10 left top near DUP7
         -0.5f,  0.5f, -0.5f, 1.0, 1.0, 1.0, 1.0f, 1.0f, // 11 left top far DUP2

         // Right face
          0.5f, -0.5f,  0.5f, 1.0, 1.0, 1.0, 0.0f, 0.0f, // 12 right bottom near DP5
          0.5f, -0.5f, -0.5f, 1.0, 1.0, 1.0, 0.0f, 1.0f, // 13 right bottom far DUP0
          0.5f,  0.5f, -0.5f, 1.0, 1.0, 1.0, 1.0f, 1.0f, // 14 right top far DUP3
          0.5f,  0.5f,  0.5f, 1.0, 1.0, 1.0, 1.0f, 0.0f, // 15 right top near DUP6

          // Bottom face
          -0.5f, -0.5f, -0.5f, 1.0, 1.0, 1.0, 0.0f, 1.0f, // 16 left bottom far DUP1
           0.5f, -0.5f, -0.5f, 1.0, 1.0, 1.0, 1.0f, 1.0f, // 17 right bottom far DUP0
           0.5f, -0.5f,  0.5f, 1.0, 1.0, 1.0, 1.0f, 0.0f, // 18 right bottom near DUP5
          -0.5f, -0.5f,  0.5f, 1.0, 1.0, 1.0, 0.0f, 0.0f, // 19 left bottom near DUP4

          // Top face
          -0.5f,  0.5f,  0.5f, 1.0, 1.0, 1.0, 0.0f, 0.0f, // 20 left top near DUP7
           0.5f,  0.5f,  0.5f, 1.0, 1.0, 1.0, 1.0f, 0.0f, // 21 right top near DUP6
           0.5f,  0.5f, -0.5f, 1.0, 1.0, 1.0, 1.0f, 1.0f, // 22 right top far DUP3
          -0.5f,  0.5f, -0.5f, 1.0, 1.0, 1.0, 0.0f, 1.0f, // 23 left top far DUP2
    };
    // indices loop: 0 1 2 ; 2 3 0
    unsigned int cubeIndices[] = {
        0, 1, 2,       2, 3, 0,    // Back 
        4, 5, 6,       6, 7, 4,    // Front
        8, 9, 10,      10, 11, 8,  // Left
        12, 13, 14,    14, 15, 12, // Right
        16, 17, 18,    18, 19, 16, // Bottom
        20, 21, 22,    22, 23, 20, // Top
    };
    
    VertexArray cubeVao;
    VertexBuffer cubeVbo(cubeVertices, sizeof(cubeVertices));
    IndexBuffer cubeIbo(cubeIndices, sizeof(cubeIndices));

    VertexLayout cubeVertexLayout;
    cubeVertexLayout.AddAttr<float>(3);
    cubeVertexLayout.AddAttr<float>(3);
    cubeVertexLayout.AddAttr<float>(2);
    cubeVao.ApplyLayout(cubeVbo, cubeVertexLayout);

    cubeVao.Unbind();
    cubeVbo.Unbind();
    cubeIbo.Unbind();
    // End cube

    //squareVao.Bind();
    //cubeVao.Bind();

    Shader shaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
    shaderProgram.Bind();

    Texture texture0("assets/textures/test1.png");
    Texture texture1("assets/textures/test.png");
    // Maybe for the RenderContext, link Texture&Slot and just keep the whole loaded textures in a list in the Renderer?
    texture0.Bind(0);
    shaderProgram.SetUniform1i("u_Texture0", 0);

    texture1.Bind(1);
    shaderProgram.SetUniform1i("u_Texture1", 1);

    //squareVao.Unbind();
    //cubeVao.Unbind();
    shaderProgram.Unbind();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    double lastTime = glfwGetTime();
    // Graphics settings for fps: [SET/UNLIMITED/VSYNC]
    double fpsCount = 60.0;
    double fpsLimit = 1.0 / fpsCount;
    bool unlimitedFPS = 1;
    if (unlimitedFPS || fpsCount!=60.0)
        appWindow.ToggleVsync();

    // Average fps (not very satisfied with it)
    const unsigned int fpsHistoryLimit = 500;
    const unsigned int averageFpsRefreshRate = 100;
    double fpsHistory[fpsHistoryLimit] = { 0 };
    unsigned int fpsPointer = 0;
    int averageFps = 0;


    //TODO: In the cube/model class
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    Camera camera(appWindow.GetWindowPointer(), appWindow.GetAspectRatio());
    // For custom lookat function
    //glm::vec3 camDir(glm::normalize(camPos - camTarget)); // actually reverse dir (vector towards us)
    //glm::vec3 camRight(glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camDir)));
    //glm::vec3 camUp(glm::cross(camDir, camRight));
    //-------------

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

            fpsHistory[fpsPointer++] = fps;
            if (fpsPointer >= fpsHistoryLimit) fpsPointer = 0;
            if (fpsPointer >= averageFpsRefreshRate)
            {
                averageFps = 0;
                for (double v : fpsHistory) averageFps += v;
                averageFps /= fpsHistoryLimit;
            }

            std::string windowTitle =
                "Hello OpenGL (FPS: " + std::to_string(fps) + " (avg:"+std::to_string(averageFps) + ") - " + std::to_string(renderMs) + "ms) DeltaTime:" + std::to_string(deltaTime);

            appWindow.SetTitle(windowTitle);
            lastTime = currentTime;

            /* Update */
            appWindow.Update();
            camera.Update(deltaTime);
            camera.SetAspectRatio(appWindow.GetAspectRatio()); //TODO: replace with events

            glm::mat4 view = camera.GetView();
            glm::mat4 projection = camera.GetProj();

            /* Render */
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shaderProgram.Bind();
            shaderProgram.SetUniform1f("u_Time", currentTime);

            shaderProgram.SetUniformMat4("u_Model", model);
            shaderProgram.SetUniformMat4("u_View", view);
            shaderProgram.SetUniformMat4("u_Projection", projection);

            squareVao.Bind();
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            squareVao.Unbind();

            cubeVao.Bind();

            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 cubeModel = glm::translate(glm::mat4(1.0f), cubePositions[i]);
                cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, sin(currentTime), 0.0f));
                cubeModel = glm::rotate(cubeModel, (float)(currentTime * glm::radians(10.0 * i+1)), glm::vec3(.3f, 1.0f, 0.6f));
                cubeModel = glm::scale(cubeModel, glm::vec3(0.5f));

                shaderProgram.SetUniformMat4("u_Model", cubeModel);

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
            cubeVao.Unbind();

            shaderProgram.Unbind();

            /* Swap front and back buffers */
            glfwSwapBuffers(appWindow.GetWindowPointer());
        }
        /* Poll for and process events */
        glfwPollEvents();
    }
}