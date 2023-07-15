#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<spdlog/spdlog.h>
#include"src/Core/Application.h"
int main(int argc,char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    
    Application app(640, 480, "Tengine");
    app.init();
    app.lockFps();
    app.setMaxFps(30);
    app.run();

    return 0;
}