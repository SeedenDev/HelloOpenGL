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
#include <vector>

#include "ApplicationWindow.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Texture.h"
#include "Scene/Camera.h"
#include "Scene/Square.h"
#include "Scene/Cube.h"
#include "Input.h"
#include "Lights/LightSource.h"
#include "Rendering/Material.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Lights/Spotlight.h"
#include "Lights/Flashlight.h"
#include "Lights/GlobalLight.h"
#include "Rendering/ModelLoader.h"

//TODO: proper logger because logging takes so much time it's useful to be able to be able to partially turn it off quickly

int main(void)
{
    ApplicationWindow appWindow("Hello OpenGL", 1080, 720);

    std::cout << glGetString(GL_VERSION) << std::endl;

    //TODO: everything below in a Renderer class or smth like that
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::string imgData = Curl::GetRemoteImage("https://media.tenor.com/H-5K98Y4FhQAAAAM/cat-buh.gif");
    std::string imgData2 = Curl::GetRemoteImage("https://pbs.twimg.com/media/HBkHsm6bUAAAkXO?format=jpg&name=4096x4096");

    Texture white("assets/textures/1x1_white.png");
    Texture texture0((unsigned char*)imgData.c_str(), imgData.size());
    Texture texture1((unsigned char*)imgData2.c_str(), imgData2.size());
    Texture diffuseMapTexture("assets/textures/container_diffuse.png");
    Texture specularMapTexture("assets/textures/container_specular.png");
    Texture emissiveMapTexture("assets/textures/container_emissive.png");
    // Material => link Texture&Slot and just all the loaded textures in a list in the Renderer?
    // Also, could be a sampler2D array, each vertex has a texIndex, and each frame glBindTextureUnit?
    white.Bind(5);
    texture0.Bind(0);
    texture1.Bind(1);
    diffuseMapTexture.Bind(10);
    specularMapTexture.Bind(11);
    emissiveMapTexture.Bind(12);

    Shader basicUnlitShader("assets/shaders/basicUnlit.vert", "assets/shaders/basicUnlit.frag");
    basicUnlitShader.Bind();
    basicUnlitShader.SetUniform1i("u_Texture", 5);
    basicUnlitShader.Unbind();
    Shader doubleTextureShader("assets/shaders/basicUnlit.vert", "assets/shaders/doubleTexture.frag");
    doubleTextureShader.Bind();
    doubleTextureShader.SetUniform1i("u_TextureLower", 0);
    doubleTextureShader.SetUniform1i("u_TextureUpper", 1);
    doubleTextureShader.Unbind();

    // Light part
    Shader basicLightningShader("assets/shaders/basicLightning.vert", "assets/shaders/basicLightning.frag");
    /*SPOTLIGHT TEST:
    basicLightningShader.Bind();
    Texture spotlightTexture("assets/textures/test2.png");
    spotlightTexture.Bind(5);
    basicLightningShader.SetUniform1i("u_SpotlightTexture", 5);
    basicLightningShader.Unbind();*/

    // Model loading tests
    //Model customModel("assets/models/columbina/columbina.obj", 0);
    //Model customModel("assets/models/backpack/backpack.obj", 1);
    Model customModel("assets/models/sponza-glTF/Sponza.gltf", 1);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // ----- END OF "should be in a sorta Renderer file" 

    // Scene part
    Camera camera(appWindow.GetWindowPointer(), appWindow.GetAspectRatio());

    //TODO: add again the possibility of having 2D draw on screen like HUD

    Square square(glm::vec3(0.0f)/*, glm::vec4(0.3f, 0.0f, 0.8f, 1.f)*/);
    square.SetEulerRotation(glm::vec3(-35.0f, 0.0f, 0.0f));
    square.SetScale(glm::vec3(0.5f));

    std::vector<Cube*> cubes = {
        new Cube(glm::vec3(0.0f,  0.0f,  0.0f)),
        new Cube(glm::vec3(2.0f,  5.0f, -15.0f)),
        new Cube(glm::vec3(-1.5f, -2.2f, -2.5f)),
        new Cube(glm::vec3(-3.8f, -2.0f, -12.3f)),
        new Cube(glm::vec3(2.4f, -0.4f, -3.5f)),
        new Cube(glm::vec3(-1.7f,  3.0f, -7.5f)),
        new Cube(glm::vec3(1.3f, -2.0f, -2.5f)),
        new Cube(glm::vec3(1.5f,  2.0f, -2.5f)),
        new Cube(glm::vec3(1.5f,  0.2f, -1.5f)),
        new Cube(glm::vec3(-1.3f,  1.0f, -1.5f)),
        new Cube(glm::vec3(0.0f, 1.5f, 0.0f))
    };

    // Try to set dynamic array for lights (maybe memory leaks bc when removed they are not freed lmao)
    std::vector<LightSource*> lightsStack;
    lightsStack.emplace_back(new GlobalLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new DirectionalLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new DirectionalLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new PointLight(glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new PointLight(glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new PointLight(glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new PointLight(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new Spotlight(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new Spotlight(glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(-0.3f, -0.3f, -0.3f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new Spotlight(glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.9f, -0.3f, 0.2f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));
    lightsStack.emplace_back(new Flashlight(&camera, glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)));

    struct LightStruct {
        glm::vec4 position = glm::vec4(0.0f);
        glm::vec4 direction = glm::vec4(1.0f);
        glm::vec4 ambient = glm::vec4(0.2f);
        glm::vec4 diffuse = glm::vec4(0.5f);
        glm::vec4 specular = glm::vec4(0.8f);
        float constant = 0.0f;
        float linear = 0.0f;
        float quadratic = 0.0f;
        float innerCutOff = 0.0f;
        float outerCutOff = 0.0f;
        float type = 0;
        float a = 0;
        float b = 0;
    };
    std::vector<LightStruct> lightsData;

    GLuint ssbLocation = glGetProgramResourceIndex(basicLightningShader.GetHandlerID(), GL_SHADER_STORAGE_BLOCK, "u_LightsBuffer");
    unsigned int ssbBinding = 0;
    glShaderStorageBlockBinding(basicLightningShader.GetHandlerID(), ssbLocation, ssbBinding);
    unsigned int lightsSSBO;
    glGenBuffers(1, &lightsSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbBinding, lightsSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    //TODO: not sure about this shit lmao, surely there is a way to do it on the stack
    //Note: not used anymore (for now)(reason: test for infinite lights count)
    LightSource* lights0[] = {
        new GlobalLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),

        new DirectionalLight(glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),
        new DirectionalLight(glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),

        new PointLight(glm::vec3(0.7f,  0.2f,  2.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),
        new PointLight(glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),
        new PointLight(glm::vec3(-4.0f,  2.0f, -12.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),
        new PointLight(glm::vec3(0.0f,  0.0f, -3.0f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),

        new Spotlight(glm::vec3(0.7f,  0.2f,  2.0f), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),
        new Spotlight(glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(-0.3f, -0.3f, -0.3f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),
        new Spotlight(glm::vec3(-4.0f,  2.0f, -12.0f), glm::vec3(0.9f, -0.3f, 0.2f), glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f)),

        new Flashlight(&camera, glm::vec3(0.1f), glm::vec3(0.8f), glm::vec3(0.5f))
    };

    glm::vec3 clearColor(0.0f);
    float fogMinDist = 20.0f;
    float fogMaxDist = 100.0f;
    bool enableFog = 0;
    bool enableMsaa = 1;
    int tab = 0;
    int selectedItem = -1;

    double lastTime = glfwGetTime();
    // Graphics settings for fps: [SET/UNLIMITED/VSYNC]
    double fpsLimit = 60.0;
    double deltaTimeLimit = 1.0 / fpsLimit;
    bool unlimitedFPS = 1;
    if (unlimitedFPS || fpsLimit != 60.0)
        appWindow.ToggleVsync(); // Toggle off V-Sync

    // Average fps
    int averageFps = 0;
    const size_t avgBufferSize = 50;
    unsigned int avgBufferPtr = 0;
    double avgBuffer[avgBufferSize] = { 0 };

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

        if (vSync || (!vSync && (!unlimitedFPS && deltaTime >= deltaTimeLimit) || (unlimitedFPS)))
        {
            /* Fps counter */
            double fps = 1 / deltaTime;
            double msPerFrame = 1000.0 / fps;

            avgBuffer[avgBufferPtr++] = fps;
            if (avgBufferPtr >= avgBufferSize) avgBufferPtr = 0;
            averageFps = 0;
            for (double v : avgBuffer) averageFps += v;
            averageFps /= avgBufferSize; /*if using msPerFrame: averageFps = 1000 * avgBufferSize / averageFps;*/

            std::string windowTitle =
                "Hello OpenGL (FPS: " + std::to_string(fps) + " (avg:"+std::to_string(averageFps) + ") - " + std::to_string(msPerFrame) + "ms) DeltaTime:" + std::to_string(deltaTime);

            appWindow.SetTitle(windowTitle);
            lastTime = currentTime;

            /* Update */
            if (Input::IsKeyPressed(GLFW_KEY_R))
            {
                basicUnlitShader.Reload();
                basicUnlitShader.Bind();
                basicUnlitShader.SetUniform1i("u_Texture", 5);
                doubleTextureShader.Reload();
                doubleTextureShader.Bind();
                doubleTextureShader.SetUniform1i("u_TextureLower", 0);
                doubleTextureShader.SetUniform1i("u_TextureUpper", 1);
                doubleTextureShader.Unbind(); // not really necessary because next call is the same bind
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
            //glDepthFunc(GL_LEQUAL);
            //glDepthMask(GL_FALSE); => depth buffer read-only if needed

            doubleTextureShader.Bind();
            doubleTextureShader.SetUniform1f("u_Time", currentTime);

            // My eyes are bleeding with all of these duplicated lines but it'll be changed soon (it's just the Cube/Square implementation should be rewritten it's bad but idc for now)
            glm::mat4 model = square.GetModelMatrix();
            glm::mat4 MVP = projection * view * model;
            doubleTextureShader.SetUniformMat4f("u_View", view);
            doubleTextureShader.SetUniformMat4f("u_Projection", projection);
            doubleTextureShader.SetUniformMat4f("u_Model", model);
            doubleTextureShader.SetUniformMat4f("u_MVP", MVP);
            doubleTextureShader.SetUniformVec4f("u_DynamicColor", square.GetColor());
            square.Draw();
            doubleTextureShader.Unbind();

            basicUnlitShader.Bind();
            //for (int i = 0; i < sizeof(lights) / sizeof(LightSource*); i++)
            for (int i = 0; i < lightsStack.size(); i++)
            {
                LightSource* light = lightsStack[i];//lights[i];
                if (light->IsToggled()) light->DrawDebugCube(basicUnlitShader, view, projection);
            }
            basicUnlitShader.Unbind();

            basicLightningShader.Bind();
            basicLightningShader.SetUniformMat4f("u_View", view);
            basicLightningShader.SetUniformMat4f("u_Projection", projection);
            basicLightningShader.SetUniform1f("u_FogEnabled", enableFog);
            basicLightningShader.SetUniform1f("u_FogMin", fogMinDist);
            basicLightningShader.SetUniform1f("u_FogMax", fogMaxDist);
            basicLightningShader.SetUniform1f("u_Time", currentTime); // for emissive texture cool animation
            basicLightningShader.SetUniformVec3f("u_CameraPos", camera.GetPosition());
            // everything set by hand because Material can't really retrieve a texture because for now the system
            // is tied to the Model loader and not to an asset system
            basicLightningShader.SetUniform1i("u_Material.hasDiffuse", 1);
            basicLightningShader.SetUniform1i("u_Material.hasSpecular", 1);
            basicLightningShader.SetUniform1i("u_Material.hasEmissive", 1);
            basicLightningShader.SetUniform1i("u_Material.diffuseMap", 10);
            basicLightningShader.SetUniform1i("u_Material.specularMap", 11);
            basicLightningShader.SetUniform1i("u_Material.emissiveMap", 12);
            basicLightningShader.SetUniform1f("u_Material.shininess", 64.f);
            basicLightningShader.SetUniform1f("u_Material.specularStrength", 1.0f);

            lightsData.reserve(lightsStack.size());
            //TODO: the Common interfaces thing could be removed with proper implementations i think.. (btw I read CPP casts are now faster that C-style ones on modern compiler so..)
            int globalLightCount = 0, directionalLightCount = 0, pointLightCount = 0, spotlightCount = 0;
            //for (int i = 0; i < sizeof(lights) / sizeof(LightSource*); i++)
            for(int i = 0; i < lightsStack.size(); i++)
            {
                LightSource* light = lightsStack[i];//lights[i];
                if (!light->IsToggled()) continue;

                std::string uniform;
                LightStruct data;
                data.type = light->GetType();
                data.ambient = glm::vec4(light->GetAmbientColor(), 0.0f);
                data.diffuse = glm::vec4(light->GetDiffuseColor(), 0.0f);
                data.specular = glm::vec4(light->GetSpecularColor(), 0.0f);

                switch (light->GetType())
                {
                case LightType::GLOBAL:
                    {
                        GlobalLight* gl = static_cast<GlobalLight*>(light);
                        uniform = "u_GlobalLights[" + std::to_string(globalLightCount) + "]";
                        //basicLightningShader.SetUniformVec3f(uniform + ".position", static_cast<Common::HasPosition*>(gl)->GetPosition());
                        data.position = glm::vec4(static_cast<Common::HasPosition*>(gl)->GetPosition(), 0.0f);
                        globalLightCount++;
                    }   break;
                case LightType::DIRECTIONAL:
                    {
                        DirectionalLight* dl = static_cast<DirectionalLight*>(light);
                        uniform = "u_DirectionalLights[" + std::to_string(directionalLightCount) + "]";
                        //basicLightningShader.SetUniformVec3f(uniform + ".direction", static_cast<Common::HasDirection*>(dl)->GetDirection());
                        data.direction = glm::vec4(static_cast<Common::HasDirection*>(dl)->GetDirection(), 0.0f);
                        directionalLightCount++;
                    }   break;
                case LightType::POINT:
                    {
                        PointLight* pl = static_cast<PointLight*>(light);
                        uniform = "u_PointLights[" + std::to_string(pointLightCount) + "]";
                        //basicLightningShader.SetUniformVec3f(uniform + ".position", static_cast<Common::HasPosition*>(pl)->GetPosition());
                        //basicLightningShader.SetUniform1f(uniform + ".constant", pl->GetConstant());
                        //basicLightningShader.SetUniform1f(uniform + ".linear", pl->GetLinear());
                        //basicLightningShader.SetUniform1f(uniform + ".quadratic", pl->GetQuadratic());
                        data.position = glm::vec4(static_cast<Common::HasPosition*>(pl)->GetPosition(), 0.0f);
                        data.constant = pl->GetConstant();
                        data.linear = pl->GetLinear();
                        data.quadratic = pl->GetQuadratic();
                        pointLightCount++;
                    }   break;
                case LightType::SPOTLIGHT:
                    {
                        Spotlight* sl = static_cast<Spotlight*>(light);
                        uniform = "u_Spotlights[" + std::to_string(spotlightCount) + "]";
                        //basicLightningShader.SetUniformVec3f(uniform + ".position", static_cast<Common::HasPosition*>(sl)->GetPosition());
                        //basicLightningShader.SetUniformVec3f(uniform + ".direction", static_cast<Common::HasDirection*>(sl)->GetDirection());
                        //basicLightningShader.SetUniform1f(uniform + ".innerCutOff", sl->GetComputedInnerCutOff());
                        //basicLightningShader.SetUniform1f(uniform + ".outerCutOff", sl->GetComputedOuterCutOff());
                        data.position = glm::vec4(static_cast<Common::HasPosition*>(sl)->GetPosition(), 0.0f);
                        data.direction = glm::vec4(static_cast<Common::HasDirection*>(sl)->GetDirection(), 0.0f);
                        data.innerCutOff = sl->GetComputedInnerCutOff();
                        data.outerCutOff = sl->GetComputedOuterCutOff();
                        spotlightCount++;
                    }   break;
                case LightType::FLASHLIGHT:
                {
                    Flashlight* fl = static_cast<Flashlight*>(light); 
                    data.position = glm::vec4(fl->GetPosition(), 0.0f);
                    data.direction = glm::vec4(fl->GetDirection(), 0.0f);
                    data.innerCutOff = fl->GetComputedInnerCutOff();
                    data.outerCutOff = fl->GetComputedOuterCutOff();
                    spotlightCount++;
                }   break;
                }
                //basicLightningShader.SetUniformVec3f(uniform + ".ambient", light->GetAmbientColor());
                //basicLightningShader.SetUniformVec3f(uniform + ".diffuse", light->GetDiffuseColor());
                //basicLightningShader.SetUniformVec3f(uniform + ".specular", light->GetSpecularColor());

                lightsData.emplace_back(data);
            }
            basicLightningShader.SetUniform1i("u_LightCount", lightsData.size());
            if (lightsData.size() > 0)
            {
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsSSBO);
                glBufferData(GL_SHADER_STORAGE_BUFFER, lightsData.size() * sizeof(LightStruct), &lightsData[0], GL_STREAM_DRAW);
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            }
            lightsData.clear();

            //basicLightningShader.SetUniform1i("u_GlobalLightCount", globalLightCount);
            //basicLightningShader.SetUniform1i("u_DirectionalLightCount", directionalLightCount);
            //basicLightningShader.SetUniform1i("u_PointLightCount", pointLightCount);
            //basicLightningShader.SetUniform1i("u_SpotlightCount", spotlightCount);

            // very bad because it's not instanced rendering
            for (unsigned int i = 0; i < cubes.size(); i++)
            {
                Cube* cube = cubes[i];
                float angle = 20.0f * i;
                cube->SetEulerRotation(glm::vec3(angle, angle * 0.3, angle * 0.5));
                model = cube->GetModelMatrix();
                MVP = projection * view * model;
                basicLightningShader.SetUniformMat4f("u_Model", model);
                basicLightningShader.SetUniformMat4f("u_MVP", MVP);
                //cube->Draw();
            }

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(30.0f));
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            MVP = projection * view * model;
            basicLightningShader.SetUniformMat4f("u_Model", model);
            basicLightningShader.SetUniformMat4f("u_MVP", MVP);
            customModel.Draw(basicLightningShader);
            basicLightningShader.Unbind();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Scene editor");
                ImGui::BeginTabBar("Objects");
                if (ImGui::BeginTabItem("Lights"))
                {
                    if (ImGui::Button("Add Light"))
                    {
                        lightsStack.push_back(new PointLight(glm::vec3(0.0f), glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f)));
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Remove Light"))
                    {
                        if (selectedItem != -1 && selectedItem<lightsStack.size())
                        {
                            lightsStack.erase(lightsStack.begin() + selectedItem);
                        }
                    }
                    ImGui::BeginTabBar("lightsseparator");
                    ImGui::EndTabBar();
                    //for (int i = 0; i < sizeof(lights) / sizeof(LightSource*); i++)
                    for (int i = 0; i < lightsStack.size(); i++)
                    {
                        if (ImGui::Button((GetTypeString(/*lights*/lightsStack[i]->GetType()) + "Light " + std::to_string(i)).c_str()))
                        {
                            if (selectedItem == i) selectedItem = -1;
                            else selectedItem = i;
                        }
                        if (selectedItem == i)
                        {
                            /*lights*/lightsStack[selectedItem]->ImGuiDebugDraw();
                        }
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Cubes"))
                {
                    if (ImGui::Button("Add Cube"))
                    {
                        cubes.push_back(new Cube(glm::vec3(0.0f)));
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Remove Cube"))
                    {
                        if (selectedItem != -1 && selectedItem < cubes.size())
                        {
                            cubes.erase(cubes.begin() + selectedItem);
                        }
                    }
                    ImGui::BeginTabBar("cubesseparator");
                    ImGui::EndTabBar();
                    for (int i = 0; i < cubes.size(); i++)
                    {
                        if (ImGui::Button(("Cube " + std::to_string(i)).c_str()))
                        {
                            if (selectedItem == i) selectedItem = -1;
                            else selectedItem = i;
                        }
                        if (selectedItem == i)
                        {
                            Cube* cube = cubes[selectedItem];
                            glm::vec3 cubePos(cube->GetPosition());
                            glm::vec3 cubeRot(cube->GetEulerRotation());
                            glm::vec3 cubeScale(cube->GetScale());
                            if (ImGui::SliderFloat3("Position", &cubePos[0], -150.0f, 150.0f)) cube->SetPosition(cubePos);
                            if (ImGui::SliderFloat3("Rotation", &cubeRot[0], -360.0f, 360.0f)) cube->SetEulerRotation(cubeRot);
                            if (ImGui::SliderFloat3("Scale", &cubeScale[0], 0.5f, 10000.0f)) cube->SetScale(cubeScale);
                            //ImGui::SliderFloat("SpecShininess", &mat1.GetSpecularShininess(), 0.0f, 512.0f);
                        }
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
                ImGui::BeginTabBar("firstseparator");
                ImGui::EndTabBar();
                //--------------------------------------
                const glm::vec3& camPos = camera.GetPosition();
                ImGui::Text("Camera: %.2f;%.2f;%.2f (%.2f;%.2f) - FOV: %.1f", camPos.x, camPos.y, camPos.z, camera.GetYaw(), camera.GetPitch(), camera.GetFOV());
                float camSpeed[] = { camera.GetHorizontalSpeed(), camera.GetVerticalSpeed() };
                if (ImGui::SliderFloat2("CamSpeed", camSpeed, 0.0f, 1000.0f))
                {
                    camera.SetHorizontalSpeed(camSpeed[0]);
                    camera.SetVerticalSpeed(camSpeed[1]);
                }
                ImGui::Checkbox("Fog", &enableFog);
                ImGui::SliderFloat("FogMin", &fogMinDist, 0.0f, 1000.0f);
                ImGui::SliderFloat("FogMax", &fogMaxDist, 0.0f, 1000.0f);
                ImGui::Checkbox("MSAA", &enableMsaa);
                if (ImGui::ColorEdit3("ClearColor", &clearColor[0])) glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
                float imguiFps = ImGui::GetIO().Framerate;
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0/imguiFps, imguiFps);
                ImGui::End();                
            }
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(appWindow.GetWindowPointer());
        }
    }
}