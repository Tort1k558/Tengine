#include<Core/Logger.h>

#include"Editor.h"

using namespace TengineEditor;
int main(int argc, char** argv)
{

    Editor editor(800, 600, "Tengine");
    editor.init();
    editor.run();

    return 0;
}