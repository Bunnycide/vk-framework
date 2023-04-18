#include <includes.h>

int main() {
    ContextType contextType;
    contextType.GRAPHICS_ONLY = true;

    int width = 800, height = 600;

    WindowLinux windowLinux(width, height, "vk-test");

    FrameWork fw(contextType, windowLinux);

    if(! fw.InitRenderEngine()){
        Log::error("Failed to init render engine");
    }

    fw.mainLoop();

    fw.cleanup();
    
    return 0;
}
