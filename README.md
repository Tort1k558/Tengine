# Tengine
## What is it?
Tengine is a custom game engine that I created for learning purposes.
The plan is to mainly target platforms like HTML5 and windows.

## Engine
The engine is divided into 2 components, the kernel itself and the editor, the editor uses api to interact with the kernel and implements additional functionality.
You can also just use the kernel itself without using editor, the kernel does not depend on editor in any way.

## Editor
![It looks like this](/images/editor.png)

## Supports builds for these platforms
- Windows
- HTML5(built with emscripten)

## Supported RenderApi
- OpenGL
- OpenGL ES 3.0 (partly because emscripten translates OpenGL calls to OpenGl ES, shaders are implemented)

## Libraries used
### Core
- glfw(to creating window)
- glad(for OpenGL)
- spdlog(for logging)
- glm(for math)
- assimp(to load meshes)
- nlohmann(to use json formats)

### Editor
- nfd(to interact with the file system)
- imgui(for UI)