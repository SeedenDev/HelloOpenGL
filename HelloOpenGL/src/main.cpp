#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// From https://learnopengl.com/In-Practice/Debugging
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
// ----

static std::string ReadFileAsString(const std::string& filepath)
{
    // C++ way of reading file, on the basis of how to do it with the C API (could be a little bit quicker)
    std::ifstream stream(filepath);
    std::string contents;
    stream.seekg(0, std::ios::end);
    contents.resize(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(&contents[0], contents.size());
    stream.close();
    return contents;
}

static unsigned int CreateShader(const GLenum shaderType, const std::string& filepath)
{
    std::string fileStr = ReadFileAsString(filepath);
    const char* shaderSrc = fileStr.c_str();
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    /* Error checking */
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* log = (char*) alloca(length*sizeof(char));
        glGetShaderInfoLog(shader, length, NULL, log);
        std::cout << "ERROR::SHADER::"<< (shaderType==GL_VERTEX_SHADER?"VERTEX":"FRAGMENT") << "::COMPILATION_FAILED\n" << log << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //glfwSwapInterval(1); // = VSYNC ON by default (monitor refresh rate = fps) - Better to be on (my gpu at 70% usage lmao)

    /* GLEW init */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to init GLEW" << std::endl;
        return -1;
    }

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr); // available only since opengl 4.3 but seems to work in 3.3
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    std::cout << glGetString(GL_VERSION) << std::endl;

    /* VBO(vertices), IBO(indices) & VAO(attributes + VBO/IBO) setup */
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

    unsigned int vbo, vao, ibo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glGenVertexArrays(1, &vao);
    // Bind the VAO and now everything that follows will be in this VAO
    glBindVertexArray(vao);

    // Put the data in the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Put the data in the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Setup the vertex attributes

    // Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); //not necessary BUT note that I should not unbind the VBO & IBO before the VAO, otherwise they won't be included in it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    /* Shaders */
    unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, "assets/shaders/vertex.glsl");
    unsigned int fragShader = CreateShader(GL_FRAGMENT_SHADER, "assets/shaders/fragment.glsl");

    /* Shader program */
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    const int timeLocation = glGetUniformLocation(shaderProgram, "u_Time");

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int frameCount = 0;
    double lastTime = glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // FPS counter & deltaTime
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        frameCount++;
        if (deltaTime >= 1.0)
        {
            double renderMs = 1000.0 / frameCount;
            double fps = frameCount / deltaTime;

            std::string windowTitle = 
                "Hello OpenGL (FPS: "+ std::to_string(fps) + " - " + std::to_string(renderMs) + "ms) "
                "FrameCount: " + std::to_string(frameCount) + " - DeltaTime:" + std::to_string(deltaTime);

            glfwSetWindowTitle(window, windowTitle.c_str());
            frameCount = 0;
            lastTime = currentTime;
        }
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Use our shaders
        glUseProgram(shaderProgram);

        glUniform1f(timeLocation, glfwGetTime());

        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwTerminate();
}