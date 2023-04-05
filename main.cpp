#include <includes.h>

int main() {
    ContextType contextType;
    contextType.GRAPHICS_ONLY = true;

    FrameWork fw(contextType);

    if(! fw.InitRenderEngine()){
        Log::error("Failed to init render engine");
    }

    fw.mainLoop();

    fw.cleanup();
    
    return 0;
}
