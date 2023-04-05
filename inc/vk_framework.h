#ifndef VK_FRAMEWORK_H
#define VK_FRAMEWORK_H

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		Log::error("Fatal : VkResult is \" %s \" in %s at line %d", string_VkResult(res) , __FILE__, __LINE__); \
		assert(res == VK_SUCCESS);																		\
	}																									\
}

class FrameWork{
private:
    ContextType contextType;
    VulkanInstance vulkanInstance;
    VulkanSwapChain vulkanSwapChain;

public:
    explicit FrameWork(ContextType);
    ~FrameWork();

    bool InitRenderEngine();
    bool LoadScene(const char* path);
    bool UseRenderType(RenderStyle);
    bool LoadLightStyle(LightData*, const char* lightData);
    void mainLoop();

    void cleanup();
};

#endif // VK_FRAMEWORK_H