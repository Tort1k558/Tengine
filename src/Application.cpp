#include "Application.h"

#include"Renderer/Renderer.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"Window.h"

#include<spdlog/spdlog.h>
#include<GLFW/glfw3.h>
#include<imgui/imgui.h>
#include<imgui/backends/imgui_impl_opengl3.h>
#include<imgui/backends/imgui_impl_glfw.h>

Application::Application(unsigned int width, unsigned int height, std::string title) :
    m_closeWindow(false)
{
	m_window = std::make_shared<Window>(width, height, title);
}

Application::~Application()
{
    glfwTerminate();
}

GLfloat points[] = {
    -0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
};
GLfloat colors[] = {
     1.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 1.0f
};


const char* vertexShader =
R"(
    #version 460
    layout(location = 0) in vec3 vertexPos;
    layout(location = 1) in vec3 vertexColor;

    out vec3 color;

    void main()
    {
        color = vertexColor;
        gl_Position = vec4(vertexPos,1.0);
    }
)";

const char* fragmenShader =
R"(
    #version 460

    in vec3 color;
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(color,1.0);
    }
)";

GLuint shaderProgram;
GLuint vao;
void Application::init()
{
    if (!glfwInit())
    {
        spdlog::critical("Failed to load glfw!");
        return;
    }
	m_window->init();
    Renderer::Init();


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_window->getWindow(),true);

    m_eventDispatcher = EventDispatcher();
    m_eventDispatcher.addEvent<EventMouseMoved>([](EventMouseMoved& event)
        {
            //spdlog::debug("EVENT::The Mouse Moved to {0}x{1}", event.xPos, event.yPos);
        });
    m_eventDispatcher.addEvent<EventMouseButtonPressed>([](EventMouseButtonPressed& event)
        {
            spdlog::debug("EVENT::The mouse button is pressed with the code {0} on the cordinates {1}x{2}", static_cast<int>(event.code), event.x, event.y);
        });
    m_eventDispatcher.addEvent<EventMouseButtonReleased>([](EventMouseButtonReleased& event)
        {
            spdlog::debug("EVENT::The mouse button is released with the code {0} on the cordinates {1}x{2}", static_cast<int>(event.code), event.x, event.y);
        });
    m_eventDispatcher.addEvent<EventKeyPressed>([&](EventKeyPressed& event)
        {
            spdlog::debug("EVENT::The mouse button is pressed with the code {0}", static_cast<int>(event.code));
        });
    m_eventDispatcher.addEvent<EventKeyReleased>([&](EventKeyReleased& event)
        {
            spdlog::debug("EVENT::The mouse button is released with the code {0}", static_cast<int>(event.code));;
        });
    m_eventDispatcher.addEvent<EventWindowClose>([&](EventWindowClose& event)
        {
            spdlog::debug("EVENT::The Window closed");
            m_closeWindow = true;
        });
    m_eventDispatcher.addEvent<EventWindowResize>([&](EventWindowResize& event)
        {
            spdlog::debug("EVENT::The Window resized to {0}x{1}", event.x, event.y);
        });
    m_window->setEventCallback([&](Event& event)
        {
            m_eventDispatcher.proccess(event);
        });

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, nullptr);
    glCompileShader(vs);

    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, & success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, nullptr, infoLog);
        spdlog::critical("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{0}", infoLog);
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmenShader, nullptr);
    glCompileShader(fs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vs, 512, nullptr, infoLog);
        spdlog::critical("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{0}", infoLog);
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLuint vboPoints;
    glGenBuffers(1, &vboPoints);
    glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint vboColors;
    glGenBuffers(1, &vboColors);
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, nullptr);

}

void Application::run()
{

    while (!m_closeWindow)
    {
        glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(m_window->getWidth());
        io.DisplaySize.y = static_cast<float>(m_window->getHeight());
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window->update();
    }
}

