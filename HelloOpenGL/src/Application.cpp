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
#include "LightSource.h"
#include "Material.h"

int main(void)
{
    ApplicationWindow appWindow("Hello OpenGL", 1080, 720);

    std::cout << glGetString(GL_VERSION) << std::endl;

    //TODO: everything below in a Renderer class or stg like that
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader simpleColorShader("assets/shaders/basic.vert", "assets/shaders/simpleColor.frag");
    // Double texture shade
    Shader doubleTextureShader("assets/shaders/basic.vert", "assets/shaders/doubleTexture.frag");
    doubleTextureShader.Bind();

    Texture texture0("assets/textures/test1.png");
    Texture texture1("assets/textures/test.png");
    Texture diffuseMapTexture("assets/textures/container_diffuse.png");
    Texture specularMapTexture("assets/textures/container_specular.png");
    Texture emissionMapTexture("assets/textures/container_emission.png");
    diffuseMapTexture.Bind(10);
    specularMapTexture.Bind(11);
    emissionMapTexture.Bind(12);
    // Material => link Texture&Slot and just all the loaded textures in a list in the Renderer?
    // Also, could be a sampler2D array, each vertex has a texIndex, and each frame glBindTextureUnit
    texture0.Bind(0);
    doubleTextureShader.SetUniform1i("u_Texture0", 0);
    texture1.Bind(1);
    doubleTextureShader.SetUniform1i("u_Texture1", 1);

    doubleTextureShader.Unbind();

    // Light part
    Shader lightSourceShader("assets/shaders/lightSource.vert", "assets/shaders/simpleColor.frag");
    Shader basicLightningShader("assets/shaders/basicLightning.vert", "assets/shaders/basicLightning.frag");

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
    const unsigned int fpsHistoryLimit = 1000;
    const unsigned int averageFpsRefreshRate = 100;
    double fpsHistory[fpsHistoryLimit] = { 0 };
    unsigned int fpsPointer = 0;
    int averageFps = 0;

    // Scene part
    Camera camera(appWindow.GetWindowPointer(), appWindow.GetAspectRatio());

    Square square(glm::vec3(0.0f), glm::vec3(0.3f, 0.0f, 0.8f));
    square.SetEulerRotation(glm::vec3(-35.0f, 0.0f, 0.0f));
    square.SetScale(glm::vec3(0.5f));

    Material mat1(glm::vec3(1.0f, 0.5f, 1.0f), glm::vec3(1.0f, 0.5f, 1.0f), glm::vec3(0.8f, 0.8f, 0.8f), 64);
    Cube cube1(glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(1.0f));
    LightSource lightSourceCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f));
    double rotationRadius = 2.0f;

    float fogMinDist = 10.0f;
    float fogMaxDist = 50.0f;
    bool enableMsaa = 1;
    bool moveLight = 1;

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
            if (Input::IsKeyPressed(GLFW_KEY_R))
            {
                simpleColorShader.Reload();
                doubleTextureShader.Reload();
                lightSourceShader.Reload();
                basicLightningShader.Reload();
            }
            appWindow.Update();
            camera.Update(deltaTime);
            camera.SetAspectRatio(appWindow.GetAspectRatio()); //TODO: replace with events

            glm::mat4 view = camera.GetView();
            glm::mat4 projection = camera.GetProj();

            /* Render */
            if (enableMsaa) glEnable(GL_MULTISAMPLE); // Enable MSAA (even if it may already be enabled)
            else glDisable(GL_MULTISAMPLE);

            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            doubleTextureShader.Bind();
            doubleTextureShader.SetUniform1f("u_Time", currentTime);

            // My eyes are bleeding with all of these duplicated lines but it'll be changed soon (it's just the Cube/Square implementation should be rewritten it's bad)
            glm::mat4 model = square.GetModelMatrix();
            glm::mat4 MVP = projection * view * model;
            doubleTextureShader.SetUniformMat4f("u_View", view);
            doubleTextureShader.SetUniformMat4f("u_Projection", projection);
            doubleTextureShader.SetUniformMat4f("u_Model", model);
            doubleTextureShader.SetUniformMat4f("u_MVP", MVP);
            square.Draw();

            doubleTextureShader.Unbind();

            basicLightningShader.Bind();
            model = cube1.GetModelMatrix();
            MVP = projection * view * model;
            basicLightningShader.SetUniformMat4f("u_Model", model);
            basicLightningShader.SetUniformMat4f("u_View", view);
            basicLightningShader.SetUniformMat4f("u_MVP", MVP);
            basicLightningShader.SetUniform1f("u_FogMin", fogMinDist);
            basicLightningShader.SetUniform1f("u_FogMax", fogMaxDist);
            basicLightningShader.SetUniform1f("u_Time", currentTime); // for emission texture cool animation
            basicLightningShader.SetUniformVec3f("u_CameraPos", camera.GetPos());
            basicLightningShader.SetUniform1i("material.diffuseMap", 10); // set value for now
            basicLightningShader.SetUniform1i("material.specularMap", 11); // set value for 
            basicLightningShader.SetUniform1i("material.emissionMap", 12); // set value for now
            basicLightningShader.SetUniform1f("material.shininess", mat1.GetSpecularShininess());
            basicLightningShader.SetUniformVec3f("light.position", lightSourceCube.GetPosition());
            basicLightningShader.SetUniformVec3f("light.ambient", lightSourceCube.GetAmbientColor());
            basicLightningShader.SetUniformVec3f("light.diffuse", lightSourceCube.GetDiffuseColor());
            basicLightningShader.SetUniformVec3f("light.specular", lightSourceCube.GetSpecularColor());
            cube1.Draw();
            basicLightningShader.Unbind();

            lightSourceShader.Bind();
            if(moveLight) lightSourceCube.SetPosition(glm::vec3(rotationRadius*cos(currentTime), 1.5f+sin(currentTime), rotationRadius * sin(currentTime)));
            model = lightSourceCube.GetModelMatrix();
            MVP = projection * view * model;
            lightSourceShader.SetUniformMat4f("u_View", view);
            lightSourceShader.SetUniformMat4f("u_Projection", projection);
            lightSourceShader.SetUniformMat4f("u_Model", model);
            lightSourceShader.SetUniformMat4f("u_MVP", MVP);
            lightSourceShader.SetUniformVec3f("u_LightColor", lightSourceCube.GetDiffuseColor());
            lightSourceCube.Draw();
            lightSourceShader.Unbind();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                glm::vec3 cubeScale(cube1.GetScale());
                glm::vec3 lightSourcePos(lightSourceCube.GetPosition());
                ImGui::Begin("Scene editor");
                bool lightOpen = ImGui::TreeNode("Light");
                if (lightOpen)
                {
                    ImGui::Checkbox("Move", &moveLight);
                    ImGui::SliderFloat3("Position", &lightSourcePos[0], -5.0f, 5.0f);
                    ImGui::ColorEdit3("AmbientColor", &lightSourceCube.GetAmbientColor()[0]);
                    ImGui::ColorEdit3("DiffuseColor", &lightSourceCube.GetDiffuseColor()[0]);
                    ImGui::ColorEdit3("SpecularColor", &lightSourceCube.GetSpecularColor()[0]);
                    ImGui::TreePop();
                }
                bool cubeOpen = ImGui::TreeNode("Cube");
                if (cubeOpen)
                {
                    ImGui::SliderFloat3("Size", &cubeScale[0], 0.5f, 2.0f);
                    //ImGui::ColorEdit3("AmbientColor", &mat1.GetAmbientColor()[0]);
                    //ImGui::ColorEdit3("DiffuseColor", &mat1.GetDiffuseColor()[0]);
                    //ImGui::ColorEdit3("SpecularColor", &mat1.GetSpecularColor()[0]);
                    ImGui::SliderFloat("SpecShininess", &mat1.GetSpecularShininess(), 0.0f, 512.0f);
                    ImGui::TreePop();
                }
                const glm::vec3& camPos = camera.GetPos();
                ImGui::Text("Camera: %.2f;%.2f;%.2f (%.2f;%.2f) - FOV: %.1f", camPos.x, camPos.y, camPos.z, camera.GetYaw(), camera.GetPitch(), camera.GetFOV());
                ImGui::SliderFloat("FogMin", &fogMinDist, 0.0f, 100.0f);
                ImGui::SliderFloat("FogMax", &fogMaxDist, 0.0f, 100.0f);
                ImGui::Checkbox("MSAA", &enableMsaa);
                float imguiFps = ImGui::GetIO().Framerate;
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0/imguiFps, imguiFps);
                ImGui::End();

                cube1.SetScale(cubeScale);
                lightSourceCube.SetPosition(lightSourcePos);
            }
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(appWindow.GetWindowPointer());
        }
    }
}