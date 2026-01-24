#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "ApplicationWindow.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"
#include "Square.h"
#include "Cube.h"
#include "Input.h"

int main(void)
{
    ApplicationWindow appWindow("Hello OpenGL", 720, 480);

    std::cout << glGetString(GL_VERSION) << std::endl;

    //TODO: everything below in a Renderer class or stg like that
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Double texture shade
    Shader doubleTextureShader("assets/shaders/basic.vert", "assets/shaders/doubleTexture.frag");
    doubleTextureShader.Bind();

    Texture texture0("assets/textures/test1.png");
    Texture texture1("assets/textures/test.png");
    // Material => link Texture&Slot and just all the loaded textures in a list in the Renderer?
    texture0.Bind(0);
    doubleTextureShader.SetUniform1i("u_Texture0", 0);
    texture1.Bind(1);
    doubleTextureShader.SetUniform1i("u_Texture1", 1);

    doubleTextureShader.Unbind();

    // Light shader
    Shader simpleColorShader("assets/shaders/basic.vert", "assets/shaders/simpleColor.frag");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // ----- END OF "should be in a sorta Renderer file" 

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

    // Scene part
    Camera camera(appWindow.GetWindowPointer(), appWindow.GetAspectRatio());

    Square square(glm::vec3(0.0f), glm::vec3(0.3f, 0.0f, 0.8f));
    square.SetEulerRotation(glm::vec3(-35.0f, 0.0f, 0.0f));
    square.SetScale(glm::vec3(0.5f));

    Cube cube1(glm::vec3(0.0f, 1.5f, 0.0f));
    Cube cube2(glm::vec3(0.0f, 0.0f, 0.0f));
    double rotationRadius = 2.0f;
    
    while (!appWindow.ShouldClose())
    {
        /* Poll for and process events */
        glfwPollEvents();

        if (glfwGetWindowAttrib(appWindow.GetWindowPointer(), GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

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
            if (Input::IsKeyPressed(GLFW_KEY_R)) simpleColorShader.Reload();
            appWindow.Update();
            camera.Update(deltaTime);
            camera.SetAspectRatio(appWindow.GetAspectRatio()); //TODO: replace with events

            glm::mat4 view = camera.GetView();
            glm::mat4 projection = camera.GetProj();

            /* Render */
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            doubleTextureShader.Bind();
            doubleTextureShader.SetUniform1f("u_Time", currentTime);

            doubleTextureShader.SetUniformMat4f("u_View", view);
            doubleTextureShader.SetUniformMat4f("u_Projection", projection);

            // My eyes are bleeding with all of these duplicated lines but it'll be changed soon (it's just the Cube/Square implementation should be rewritten it's bad)
            glm::mat4 model = square.GetModelMatrix();
            glm::mat4 MVP = projection * view * model;
            doubleTextureShader.SetUniformMat4f("u_Model", model);
            doubleTextureShader.SetUniformMat4f("u_MVP", MVP);
            square.Draw();
            doubleTextureShader.Unbind();

            simpleColorShader.Bind();
            model = cube1.GetModelMatrix();
            MVP = projection * view * model;
            doubleTextureShader.SetUniformMat4f("u_Model", model);
            doubleTextureShader.SetUniformMat4f("u_MVP", MVP);
            cube1.Draw();

            cube2.SetPosition(glm::vec3(rotationRadius*cos(currentTime), 0, rotationRadius*sin(currentTime)));
            model = cube2.GetModelMatrix();
            MVP = projection * view * model;
            doubleTextureShader.SetUniformMat4f("u_Model", model);
            doubleTextureShader.SetUniformMat4f("u_MVP", MVP);
            cube2.Draw();
            simpleColorShader.Unbind();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                glm::vec3 squareRot(square.GetEulerRotation());
                ImGui::Begin("Test");
                ImGui::SliderFloat3("SquareRot", &squareRot[0], -360.0f, 360.0f);
                const glm::vec3& camPos = camera.GetPos();
                ImGui::Text("Camera: %.2f;%.2f;%.2f (%.2f;%.2f) - FOV: %.1f", camPos.x, camPos.y, camPos.z, camera.GetYaw(), camera.GetPitch(), camera.GetFOV());
                float imguiFps = ImGui::GetIO().Framerate;
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0/imguiFps, imguiFps);
                ImGui::End();
                square.SetEulerRotation(squareRot);
            }
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(appWindow.GetWindowPointer());
        }
    }
}