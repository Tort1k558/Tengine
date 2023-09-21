#include"Editor.h"
#include"Core/Logger.h"

using namespace Tengine;
int main(int argc, char** argv)
{
    Logger::SetLevel(LogLevel::Debug);
    Logger::SetPattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    Editor editor(800, 600, "Tengine");
    editor.init();
    editor.run();

    return 0;
}