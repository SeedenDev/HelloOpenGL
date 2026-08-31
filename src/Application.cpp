#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "ApplicationWindow.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Texture.h"
#include "Scene/Camera.h"
#include "Scene/Quad.h"
#include "Scene/Cube.h"
#include "Input.h"
#include "Lights/LightSource.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Lights/Spotlight.h"
#include "Lights/Flashlight.h"
#include "Lights/GlobalLight.h"
#include "Rendering/ModelLoader.h"
#include "Rendering/Framebuffer.h"
#include "GlobalUtil.h"

//TODO: proper logger because logging takes so much time it's useful to be able to be able to partially turn it off quickly

//TODO: add a "render resolution" and "resolution scale" in game settings (like WxH & 0.9/1.3 stuff)
// well maybe not a "render res" but the window size, because the render res is set with the scale or with the AA algorithm (e.g. SMAA x2/x4 the size of the output)

//TODO: surface scattering: light goes through objects (and can scatter+exits at a diff point) => use for translucent object, like curtains (currently lit on one side), ears, etc

int main()
{
    const bool SPONZA_MAP = 1, CUSTOM_FB = 1;

    ApplicationWindow appWindow("Hello OpenGL", 1080, 720);

    const GLubyte* gpuRenderer = glGetString(GL_RENDERER);
    std::cout << "GPU: " << gpuRenderer << std::endl;
    std::cout << "Driver: " << glGetString(GL_VERSION) << std::endl;
    if (GLAD_GL_ARB_bindless_texture != 0) {
        std::cout << "GL_ARB_bindless_texture is available" << std::endl;
    }
    int majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    std::cout << "GL version: " << majorVersion << "." << minorVersion << std::endl;
    //TODO: don't work anymore bc it uses the "ARB_texture_filter_anisotropic" (not included currently in my generated glad) OR is included in core 4.6
    // float maxAnisotropy;
    // glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
    // std::cout << "Max anisotropy: " << maxAnisotropy << std::endl;
    int msaaMaxSample;
    glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &msaaMaxSample);
    std::cout << "MSAA max samples: " << std::to_string(msaaMaxSample) << std::endl;

    //TODO: everything below in a Renderer class or smth like that
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    std::string imgData = Curl::GetRemoteImage("https://media.tenor.com/H-5K98Y4FhQAAAAM/cat-buh.gif");
    std::string imgData2 = Curl::GetRemoteImage("https://pbs.twimg.com/media/HBkHsm6bUAAAkXO?format=jpg&name=4096x4096");

    Texture white("assets/textures/1x1_white.png");
    Texture texture0((unsigned char*)imgData.c_str(), imgData.size());
    Texture texture1((unsigned char*)imgData2.c_str(), imgData2.size());
    Texture windowTexture("assets/textures/window.png");
    Texture diffuseMapTexture("assets/textures/container_diffuse.png");
    Texture specularMapTexture("assets/textures/container_specular.png");
    Texture emissiveMapTexture("assets/textures/container_emissive.png");
    // Material => link Texture&Slot and just all the loaded textures in a list in the Renderer?
    // Also, could be a sampler2D array, each vertex has a texIndex, and each frame glBindTextureUnit?
    white.Bind(5);
    texture0.Bind(0);
    texture1.Bind(1);
    windowTexture.Bind(2);
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
    Shader screenQuadShader("assets/shaders/basic2DQuad.vert", "assets/shaders/basic2DQuad.frag");
    basicUnlitShader.Bind();
    basicUnlitShader.SetUniform1i("u_Texture", 5); // Default is: white texture to use it as a colored quad with u_DynamicColor
    basicUnlitShader.Unbind();

    // Light part
    Shader basicLightingShader("assets/shaders/basicLighting.vert", "assets/shaders/basicLighting.frag");
    /*SPOTLIGHT TEST:
    basicLightingShader.Bind();
    Texture spotlightTexture("assets/textures/test2.png");
    spotlightTexture.Bind(5);
    basicLightingShader.SetUniform1i("u_SpotlightTexture", 5);
    basicLightingShader.Unbind();*/

    // Model loading tests
    // Model customModel("assets/models/columbina/columbina.obj", 0);
    Model backpackModel("assets/models/backpack/backpack.obj", 1);
    Model customModel("assets/models/sponza-glTF/Sponza.gltf", 0);
    // ----- END OF "should be in a sorta Renderer file" (I disagree with my past self)

    // Scene part
    Camera camera(appWindow.GetAspectRatio());

    //TODO: add again the possibility of having 2D draw on screen like HUD => the 2D shader is baaack (pos/color/uv)

    Quad quad(glm::vec3(0.0f)/*, glm::vec4(0.3f, 0.0f, 0.8f, 1.f)*/);
    quad.SetEulerRotation(glm::vec3(-35.0f, 0.0f, 0.0f));
    quad.SetScale(glm::vec3(0.5f));

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

    GLuint ssbLocation = glGetProgramResourceIndex(basicLightingShader.GetHandlerID(), GL_SHADER_STORAGE_BLOCK, "u_LightsBuffer");
    unsigned int ssbBinding = 0;
    glShaderStorageBlockBinding(basicLightingShader.GetHandlerID(), ssbLocation, ssbBinding);
    unsigned int lightsSSBO;
    glGenBuffers(1, &lightsSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsSSBO);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, lightsStack.size() * sizeof(LightStruct), nullptr, GL_DYNAMIC_STORAGE_BIT);
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
    
    /* Cubemap tests */
    const std::string skyboxAssetsPath = "assets/textures/skybox/";
    unsigned int skyboxTexture;
    glGenTextures(1, &skyboxTexture);
    glActiveTexture(GL_TEXTURE25);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    int width, height, bytesPerChannel;
    unsigned char* data;
    stbi_set_flip_vertically_on_load(0);
    for (int i = 0; i < 6; i++)
    {
        data = stbi_load((skyboxAssetsPath + std::to_string(i) + std::string(".jpg")).c_str(), &width, &height, &bytesPerChannel, 0);
        if (data)
        {
            //Note: neg_z = front / make it that way in the files
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Skybox texture " << std::to_string(i) << " couldn't be loaded." << std::endl;
            glDeleteTextures(1, &skyboxTexture);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // Don't forget to delete the texture once it'll be implemented as a class

    //TODO: I'm dying seeing this, I need to clean it with a IBO in the future
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    VertexArray skyboxVao;
    VertexBuffer skyboxVbo(skyboxVertices, sizeof(skyboxVertices));
    VertexLayout skyboxLayout;
    skyboxLayout.AddAttr<float>(3);
    skyboxVao.ApplyLayout(skyboxVbo, skyboxLayout);
    skyboxVao.Unbind();
    skyboxVbo.Unbind();

    Shader skyboxShader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
    /* ------------- */

    Framebuffer* fullscreenFb = NULL;
    //Note: not sure about having both of them. I think it's either renderRes either a scale based on the window size?
    float renderScale = 1.0f; // also add a render size in game settings (like WxH)
    unsigned int renderRes[] = { 1920, 1080 };
    unsigned int msaaSample = std::clamp(16, 0, msaaMaxSample);
    // Render resolution for imgui bc it needs int and can't use unsigned int (not good solution lmao)
    int renderResWidth = renderRes[0], renderResHeight = renderRes[1], msaaLevel = msaaSample;
    if (CUSTOM_FB) fullscreenFb = new Framebuffer(renderRes[0]*renderScale, renderRes[1]*renderScale, msaaSample); //dirty but idc atm this whole project is dirty code i just need to finish some tutorials and i'm gonna archive this shit to start anew

    glm::vec3 clearColor(0.0f);
    float fogMinDist = 20.0f;
    float fogMaxDist = 100.0f;
    bool enableFog = 0;
    bool enableMsaa = 1;
    int tab = 0;
    int selectedItem = -1;
    bool enableOutline = 0;
    glm::vec4 outlineColor(1.0f, 0.0f, 0.7f, 1.0f);
    int visualDebugMode = 0; // 0=none ; 1=depth buffer ; 2=normals
    const char* visualDebugList[]{ "None", "Depth Buffer", "Normals" };

    double lastTime = glfwGetTime();
    // Graphics settings for fps: [SET/UNLIMITED/VSYNC]
    float fpsLimit = 60.0;
    double deltaTimeLimit = 1.0 / fpsLimit;
    bool unlimitedFPS = 0;
    if (unlimitedFPS || fpsLimit != 60.0)
        appWindow.ToggleVsync(); // Toggle off V-Sync

    // Average fps
    int averageFps = 0;
    const size_t avgBufferSize = 50;
    unsigned int avgBufferPtr = 0;
    float avgBuffer[avgBufferSize] = { 0 };

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
            float fps = 1 / deltaTime;
            double msPerFrame = 1000.0 / fps;

            avgBuffer[avgBufferPtr++] = fps;
            if (avgBufferPtr >= avgBufferSize) avgBufferPtr = 0;
            averageFps = 0;
            for (float v : avgBuffer) averageFps += v;
            averageFps /= avgBufferSize; /*if using msPerFrame: averageFps = 1000 * avgBufferSize / averageFps;*/

            std::string windowTitle =
                "Hello OpenGL (FPS: " + std::to_string(fps) + " (avg:" + std::to_string(averageFps) + ") - " + std::to_string(msPerFrame) + "ms) DeltaTime:" + std::to_string(deltaTime);

            //appWindow.SetTitle(windowTitle);
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
                basicLightingShader.Reload();
                screenQuadShader.Reload();
                skyboxShader.Reload();
            }
            appWindow.Update();
            camera.Update(deltaTime);
            camera.SetAspectRatio(appWindow.GetAspectRatio()); //TODO: replace with events

            glm::mat4 view = camera.GetView();
            glm::mat4 projection = camera.GetProj();

            if (CUSTOM_FB) fullscreenFb->Bind();

            /* Render */

            //Note: glCullFace needs to be disabled for objects like 2D grass

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_STENCIL_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            //glDepthFunc(GL_LEQUAL);
            //glDepthMask(GL_FALSE); => depth buffer read-only if needed

            //glStencilMask(0xFF); (default) => bitmask ANDed with the stencil value about to be written. Ex: 0x00 => 0&x=0 => disable writing => read-
            /* Set the test function to determine if a fragment should pass or not. Params:
                the fct that will test the stencil value with the ref value ; the actual ref value ; a bitmask ANDed with both stencil value & ref before the test */
            // glStencilFunc(GL_EQUAL, 1, 0xFF);
            /* Writing to the buffer : an action for 3 cases : if the stencil test fails; passes but depth fails; passes and depth passes too(default: KEEP everywhere) */
            // glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            // => glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) also exists, else it is set for both GL_FRONT_AND_BACK

            doubleTextureShader.Bind();
            doubleTextureShader.SetUniform1f("u_Time", currentTime);

            // My eyes are bleeding with all of these duplicated lines but it'll be changed soon (it's just the Cube/Square implementation should be rewritten it's bad but idc for now)
            glm::mat4 model = quad.GetModelMatrix();
            glm::mat4 MVP = projection * view * model;
            doubleTextureShader.SetUniformMat4f("u_View", view);
            doubleTextureShader.SetUniformMat4f("u_Projection", projection);
            doubleTextureShader.SetUniformMat4f("u_Model", model);
            doubleTextureShader.SetUniformMat4f("u_MVP", MVP);
            doubleTextureShader.SetUniformVec4f("u_DynamicColor", quad.GetColor());
            quad.Draw();
            doubleTextureShader.Unbind();

            basicLightingShader.Bind();
            basicLightingShader.SetUniformMat4f("u_View", view);
            basicLightingShader.SetUniformMat4f("u_Projection", projection);
            basicLightingShader.SetUniform1i("u_FogEnabled", enableFog);
            basicLightingShader.SetUniform1f("u_FogMin", fogMinDist);
            basicLightingShader.SetUniform1f("u_FogMax", fogMaxDist);
            basicLightingShader.SetUniform1f("u_Time", currentTime); // for emissive texture cool animation
            basicLightingShader.SetUniformVec3f("u_CameraPos", camera.GetPosition());
            basicLightingShader.SetUniform1i("u_VisualDebugMode", visualDebugMode);
            
            lightsData.reserve(lightsStack.size());
            //TODO: the Common interfaces thing could be removed with proper implementations i think.. (btw I read CPP casts are now faster that C-style ones on modern compiler so..)
            int globalLightCount = 0, directionalLightCount = 0, pointLightCount = 0, spotlightCount = 0;
            //for (int i = 0; i < sizeof(lights) / sizeof(LightSource*); i++)
            for (int i = 0; i < lightsStack.size(); i++)
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
                        //basicLightingShader.SetUniformVec3f(uniform + ".position", static_cast<Common::HasPosition*>(gl)->GetPosition());
                        data.position = glm::vec4(static_cast<Common::HasPosition*>(gl)->GetPosition(), 0.0f);
                        globalLightCount++;
                    }   break;
                    case LightType::DIRECTIONAL:
                    {
                        DirectionalLight* dl = static_cast<DirectionalLight*>(light);
                        uniform = "u_DirectionalLights[" + std::to_string(directionalLightCount) + "]";
                        //basicLightingShader.SetUniformVec3f(uniform + ".direction", static_cast<Common::HasDirection*>(dl)->GetDirection());
                        data.direction = glm::vec4(static_cast<Common::HasDirection*>(dl)->GetDirection(), 0.0f);
                        directionalLightCount++;
                    }   break;
                    case LightType::POINT:
                    {
                        PointLight* pl = static_cast<PointLight*>(light);
                        uniform = "u_PointLights[" + std::to_string(pointLightCount) + "]";
                        //basicLightingShader.SetUniformVec3f(uniform + ".position", static_cast<Common::HasPosition*>(pl)->GetPosition());
                        //basicLightingShader.SetUniform1f(uniform + ".constant", pl->GetConstant());
                        //basicLightingShader.SetUniform1f(uniform + ".linear", pl->GetLinear());
                        //basicLightingShader.SetUniform1f(uniform + ".quadratic", pl->GetQuadratic());
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
                        //basicLightingShader.SetUniformVec3f(uniform + ".position", static_cast<Common::HasPosition*>(sl)->GetPosition());
                        //basicLightingShader.SetUniformVec3f(uniform + ".direction", static_cast<Common::HasDirection*>(sl)->GetDirection());
                        //basicLightingShader.SetUniform1f(uniform + ".innerCutOff", sl->GetComputedInnerCutOff());
                        //basicLightingShader.SetUniform1f(uniform + ".outerCutOff", sl->GetComputedOuterCutOff());
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
                //basicLightingShader.SetUniformVec3f(uniform + ".ambient", light->GetAmbientColor());
                //basicLightingShader.SetUniformVec3f(uniform + ".diffuse", light->GetDiffuseColor());
                //basicLightingShader.SetUniformVec3f(uniform + ".specular", light->GetSpecularColor());

                lightsData.emplace_back(data);
            }
            basicLightingShader.SetUniform1i("u_LightCount", lightsData.size());
            if (lightsData.size() > 0)
            {
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightsSSBO);
                glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, lightsData.size() * sizeof(LightStruct), &lightsData[0]);
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            }
            lightsData.clear();

            //basicLightingShader.SetUniform1i("u_GlobalLightCount", globalLightCount);
            //basicLightingShader.SetUniform1i("u_DirectionalLightCount", directionalLightCount);
            //basicLightingShader.SetUniform1i("u_PointLightCount", pointLightCount);
            //basicLightingShader.SetUniform1i("u_SpotlightCount", spotlightCount);

            //Everything below: very bad because it's not instanced rendering
            if (SPONZA_MAP)
            {
                model = glm::mat4(1.0f);
                // model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(1.0f));
                // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                MVP = projection * view * model;
                basicLightingShader.SetUniformMat4f("u_Model", model);
                basicLightingShader.SetUniformMat4f("u_MVP", MVP);
                customModel.Draw(basicLightingShader);

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.1f));
                // model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                MVP = projection * view * model;
                basicLightingShader.SetUniformMat4f("u_Model", model);
                basicLightingShader.SetUniformMat4f("u_MVP", MVP);
                backpackModel.Draw(basicLightingShader);
            }
            else
            {
                // everything set by hand because Material can't really retrieve a texture because for now the system
                // is tied to the Model loader and not to an asset system
                basicLightingShader.SetUniform1i("u_Material.hasDiffuse", 1);
                basicLightingShader.SetUniform1i("u_Material.hasSpecular", 1);
                basicLightingShader.SetUniform1i("u_Material.hasEmissive", 1);
                basicLightingShader.SetUniform1i("u_Material.diffuseMap", 10);
                basicLightingShader.SetUniform1i("u_Material.specularMap", 11);
                basicLightingShader.SetUniform1i("u_Material.emissiveMap", 12);
                basicLightingShader.SetUniform1f("u_Material.shininess", 64.f);
                basicLightingShader.SetUniform1f("u_Material.specularStrength", 1.0f);

                // Stencil testing experimentation: object outlining (based on learnopengl.com but edited because disabling depth testing caused issues with the light cubes
                if (enableOutline)
                {
                    glStencilMask(0xFF);
                    glStencilFunc(GL_ALWAYS, 1, 0xFF);
                    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                }
                for (unsigned int i = 0; i < cubes.size(); i++)
                {
                    Cube* cube = cubes[i];
                    float angle = 20.0f * i;
                    cube->SetEulerRotation(glm::vec3(angle, angle * 0.3, angle * 0.5));
                    model = cube->GetModelMatrix();
                    MVP = projection * view * model;
                    basicLightingShader.SetUniformMat4f("u_Model", model);
                    basicLightingShader.SetUniformMat4f("u_MVP", MVP);
                    cube->Draw();
                }
            }
            basicLightingShader.Unbind();
            basicUnlitShader.Bind();
            if (!SPONZA_MAP)
            {
                if (enableOutline)
                {
                    glDepthFunc(GL_ALWAYS);
                    glStencilMask(0x00);
                    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

                    basicUnlitShader.SetUniformVec4f("u_DynamicColor", outlineColor);
                    for (unsigned int i = 0; i < cubes.size(); i++)
                    {
                        Cube* cube = cubes[i];
                        float angle = 20.0f * i;
                        cube->SetEulerRotation(glm::vec3(angle, angle * 0.3, angle * 0.5));
                        model = cube->GetModelMatrix();
                        model = glm::scale(model, glm::vec3(1.1f));
                        MVP = projection * view * model;
                        basicUnlitShader.SetUniformMat4f("u_Model", model);
                        basicUnlitShader.SetUniformMat4f("u_MVP", MVP);
                        cube->Draw();
                    }
                    glDepthFunc(GL_LESS);
                    glStencilMask(0xFF);
                    glStencilFunc(GL_ALWAYS, 1, 0xFF);
                    // Both below not needed but I prefer resetting
                    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                    glClear(GL_STENCIL_BUFFER_BIT);
                }
            }

            //for (int i = 0; i < sizeof(lights) / sizeof(LightSource*); i++)
            for (int i = 0; i < lightsStack.size(); i++)
            {
                LightSource* light = lightsStack[i];//lights[i];
                if (light->IsToggled()) light->DrawDebugCube(basicUnlitShader, view, projection);
            }
            basicUnlitShader.Unbind();

            glDepthFunc(GL_LEQUAL); //default GL_LESS
            skyboxShader.Bind();
            skyboxShader.SetUniformMat4f("u_View", glm::mat4(glm::mat3(view)));
            skyboxShader.SetUniformMat4f("u_Projection", projection);
            skyboxShader.SetUniform1i("u_Skybox", 25);
            skyboxVao.Bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            skyboxShader.Unbind();
            glDepthFunc(GL_LESS);

            // Frame buffer experimentation (not including ImGui because it would need some adjustments too)
            if (CUSTOM_FB)
            {
                fullscreenFb->Unbind();
                fullscreenFb->Draw(screenQuadShader, 0, appWindow.GetWidth(), appWindow.GetHeight());
            }
            // ---------

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::BeginTabBar("Info");
                const glm::vec3& camPos = camera.GetPosition();
                ImGui::Text("Camera:");
                ImGui::Text("%.2f;%.2f;%.2f (%.2f;%.2f)", camPos.x, camPos.y, camPos.z, camera.GetYaw(), camera.GetPitch());
                ImGui::Text("FOV: %.1f", camera.GetFOV());
                float imguiFps = ImGui::GetIO().Framerate;
                ImGui::Text("Avg: %.3f ms/frame (%.1f FPS)[%.1iFPS/%.1fms]", 1000.0 / imguiFps, imguiFps, averageFps, msPerFrame);
                ImGui::EndTabBar();
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
                ImGui::End();
                ImGui::Begin("Settings");
                float camSpeed[] = { camera.GetHorizontalSpeed(), camera.GetVerticalSpeed() };
                if (ImGui::SliderFloat2("CamSpeed", camSpeed, 0.0f, 1000.0f))
                {
                    camera.SetHorizontalSpeed(camSpeed[0]);
                    camera.SetVerticalSpeed(camSpeed[1]);
                }
                ImGui::Checkbox("Fog", &enableFog);
                ImGui::SliderFloat("FogMin", &fogMinDist, 0.0f, 1000.0f);
                ImGui::SliderFloat("FogMax", &fogMaxDist, 0.0f, 1000.0f);
                ImGui::Combo("Visual debug", &visualDebugMode, visualDebugList, IM_ARRAYSIZE(visualDebugList));
                ImGui::Checkbox("Cube outline", &enableOutline);
                if (enableOutline) ImGui::ColorEdit4("Outline", &outlineColor[0]);
                if (ImGui::Checkbox("VSync", &vSync)) appWindow.ToggleVsync();
                if (!vSync)
                {
                    ImGui::Checkbox("Unlimited FPS", &unlimitedFPS);
                    if(!unlimitedFPS)
                    {
                        ImGui::SameLine();
                        if(ImGui::SliderFloat("FPS limit", &fpsLimit, 0.0, 200.0)) deltaTimeLimit = 1.0 / fpsLimit;
                    }
                }
                if (ImGui::ColorEdit3("ClearColor", &clearColor[0])) glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
                if (CUSTOM_FB)
                {
                    // Bruh having two variables but i want to bypass the unsigned int stuff for now i'm tired
                    if (ImGui::InputInt("FbWidth", &renderResWidth))
                    {
                        renderRes[0] = renderResWidth;
                        fullscreenFb->Resize(renderResWidth*renderScale, renderRes[1]*renderScale);
                    }
                    if (ImGui::InputInt("FbHeight", &renderResHeight))
                    {
                        renderRes[1] = renderResHeight;
                        fullscreenFb->Resize(renderRes[0]*renderScale, renderResHeight*renderScale);
                    }
                    if (ImGui::Checkbox("MSAA", &enableMsaa))
                    {
                        //TODO: idk if it may mess up with ImGui or actually being applied for the framebuffer
                        if (enableMsaa) glEnable(GL_MULTISAMPLE);
                        else glDisable(GL_MULTISAMPLE);
                    }
                    if (ImGui::SliderInt("FbMSAALevel", &msaaLevel, 0, msaaMaxSample))
                    {
                        fullscreenFb->SetMsaaLevel(msaaLevel);
                    }
                    if (ImGui::SliderFloat("FbRenderScale", &renderScale, 0.0f, 10.0f))
                    {
                        fullscreenFb->Resize(renderRes[0]*renderScale, renderRes[1]*renderScale);
                    }
                }
                ImGui::End();
            }
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(appWindow.GetWindowPointer());
        }
    }
}