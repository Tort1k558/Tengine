#include "Application.h"

#include"Renderer/Renderer.h"
#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"Window.h"
#include"Renderer/VertexArray.h"
#include"Renderer/VertexBuffer.h"
#include"Renderer/IndexBuffer.h"
#include"Renderer/Shader.h"
#include"ECS/Object.h"
#include"Timer.h"

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
    
}

GLfloat posCol[] =
{
//         Positions              Colors
    -0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,      1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 0.0f
};
GLuint indices[] =
{
    0,1,2,
    0,1,3
};

const char* vertexShader =
R"(
    #version 460
    layout(location = 0) in vec3 vertexPos;
    layout(location = 1) in vec3 vertexColor;
    
    out vec3 color;
    
    uniform mat4 u_rotMatrix;

    void main()
    {
        color = vertexColor;
        gl_Position = u_rotMatrix * vec4(vertexPos,1.0);
    }
)";

const char* fragmentShader =
R"(
    #version 460

    in vec3 color;
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(color,1.0);
    }
)";

std::shared_ptr<Shader> shader;
std::shared_ptr<VertexArray> va;
std::shared_ptr<VertexBuffer> vbPosCol;
std::shared_ptr<IndexBuffer> ibPosCol;

struct PositionComponent : public Component
{
    Vec3 pos;
};

struct VelocityComponent : public Component
{
    Vec3 velocity;
};

void Application::init()
{
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
    
    shader = Shader::Create();
    shader->addShader(vertexShader, ShaderType::VertexShader);
    shader->addShader(fragmentShader, ShaderType::FragmentShader);
    shader->compile();

    va = VertexArray::Create();
    vbPosCol = VertexBuffer::Create(posCol, sizeof(posCol), BufferUsage::Static);
    BufferLayout posCol;
    posCol.push({ ElementType::Float3 });
    posCol.push({ ElementType::Float3 });

    vbPosCol->setLayout(posCol);
    va->addVertexBuffer(vbPosCol);
    ibPosCol = IndexBuffer::Create(indices, 6);
    va->setIndexBuffer(ibPosCol);
    shader->setUniformMat4("u_rotMatrix", RotateMatrix<Mat4>(Mat4(1.0f), Vec3(1.0f), 5.3f));

    std::shared_ptr<Object> object = Object::Create();
    std::shared_ptr<Object> object2 = Object::Create();
    std::shared_ptr<Object> object3 = Object::Create();

    std::shared_ptr<PositionComponent> posComp = Component::Create<PositionComponent>();
    std::shared_ptr<VelocityComponent> velComp = Component::Create<VelocityComponent>();
    posComp->pos = Vec3(1.0f, 10.0f, -40.0f);
    velComp->velocity= Vec3(99.0f, 10.0f, -5.0f);
    object->addComponent<PositionComponent>(posComp);
    object->addComponent<VelocityComponent>(velComp);
    object2->addComponent<VelocityComponent>(velComp);
}

void Application::run()
{
    auto positionComponents = ComponentManager::getComponents<PositionComponent>();
    auto velocityComponents = ComponentManager::getComponents<VelocityComponent>();
    double delta = 0.0f;
    while (!m_closeWindow)
    {
        Timer::Start();
        delta += Timer::GetDeltaTime() * 10000;
        glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader->setUniformMat4("u_rotMatrix", RotateMatrix<Mat4>(Mat4(1.0f), Vec3(1.0f), delta));
        shader->bind();
        va->bind();
        glDrawElements(GL_TRIANGLES,va->getCountOfIndices(), GL_UNSIGNED_INT, 0);
        m_window->update();
        spdlog::info("FPS:{0}", 1.0 / Timer::GetDeltaTime());
    }
}

