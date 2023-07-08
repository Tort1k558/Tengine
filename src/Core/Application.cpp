#include "Application.h"

#include"Renderer/OpenGL/RendererContextOpenGL.h"
#include"Renderer/VertexArray.h"
#include"Renderer/VertexBuffer.h"
#include"Renderer/IndexBuffer.h"
#include"Renderer/Shader.h"
#include"ECS/Object.h"
#include"ECS/SystemManager.h"
#include"Timer.h"
#include"Components/Transform.h"
#include"Systems/RendererSystem.h"
#include"Systems/UISystem.h"
#include"Logger.h"

#include<GLFW/glfw3.h>



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
    
    uniform mat4 u_modelMatrix;

    void main()
    {
        color = vertexColor;
        gl_Position = u_modelMatrix * vec4(vertexPos,1.0);
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

void Application::init()
{
	m_window->init();

    System::GetInstance<RendererSystem>()->setRendererType(RendererType::OpenGL);
    SystemManager::AddSystem<RendererSystem>();
    System::GetInstance<UISystem>()->setWindow(m_window);
    SystemManager::AddSystem<UISystem>();

    SystemManager::InitSystems();

    m_eventDispatcher = EventDispatcher();
    m_eventDispatcher.addEvent<EventMouseMoved>([](EventMouseMoved& event)
        {
            //Logger::Debug("EVENT::The Mouse Moved to {0}x{1}", event.xPos, event.yPos);
        });
    m_eventDispatcher.addEvent<EventMouseButtonPressed>([](EventMouseButtonPressed& event)
        {
            Logger::Debug("EVENT::The mouse button is pressed with the code {0} on the cordinates {1}x{2}", static_cast<int>(event.code), event.x, event.y);
        });
    m_eventDispatcher.addEvent<EventMouseButtonReleased>([](EventMouseButtonReleased& event)
        {
            Logger::Debug("EVENT::The mouse button is released with the code {0} on the cordinates {1}x{2}", static_cast<int>(event.code), event.x, event.y);
        });
    m_eventDispatcher.addEvent<EventKeyPressed>([&](EventKeyPressed& event)
        {
            Logger::Debug("EVENT::The mouse button is pressed with the code {0}", static_cast<int>(event.code));
        });
    m_eventDispatcher.addEvent<EventKeyReleased>([&](EventKeyReleased& event)
        {
            Logger::Debug("EVENT::The mouse button is released with the code {0}", static_cast<int>(event.code));;
        });
    m_eventDispatcher.addEvent<EventWindowClose>([&](EventWindowClose& event)
        {
            Logger::Debug("EVENT::The Window closed");
            m_closeWindow = true;
        });
    m_eventDispatcher.addEvent<EventWindowResize>([&](EventWindowResize& event)
        {
            Logger::Debug("EVENT::The Window resized to {0}x{1}", event.x, event.y);
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

    std::shared_ptr<Object> object = Object::Create();

    std::shared_ptr<Transform> transform = Component::Create<Transform>();
    transform->setRotation({ 0.0f,0.0f,90.0f });
    transform->setScale({ 2.0f,1.0f,1.0f });
    transform->setPosition({ 0.5f,0.0f,0.0f });
    object->addComponent(transform);
}

void Application::run()
{
    std::shared_ptr<Transform> transform = ComponentManager::getComponents<Transform>()[0];
    double delta = 0.0f;
    while (!m_closeWindow)
    {
        Timer::Start();
        delta += Timer::GetDeltaTime() * 500000;
        SystemManager::UpdateSystems();

        transform->setRotation( {delta, delta, delta });
        shader->setUniformMat4("u_modelMatrix", transform->getMatrix());
        shader->bind();
        va->bind();
        glDrawElements(GL_TRIANGLES,va->getCountOfIndices(), GL_UNSIGNED_INT, 0);
        m_window->update();
        Logger::Info("FPS::{0}", 1.0 / Timer::GetDeltaTime());
    }
}

