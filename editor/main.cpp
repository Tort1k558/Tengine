#include"Editor.h"
#include"Core/Logger.h"

using namespace TengineEditor;
int main(int argc, char** argv)
{
    Logger::SetLevel(LogLevel::Debug);

    Editor editor(800, 600, "Tengine");
    editor.init();
    editor.run();

    return 0;
}