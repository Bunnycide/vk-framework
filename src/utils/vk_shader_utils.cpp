//
// Created by jae on 06/04/23.
//

#include "includes.h"

void Shader::setValue() {

}

void Shader::compileShader(VkDevice logicalDevice,
                           const char *vertShaderPath,
                           const char *fragShaderPath) {

    std::string vertShaderSource; getShaderString(vertShaderPath, vertShaderSource);
    std::string fragShaderSource; getShaderString(fragShaderPath, fragShaderSource);
    VkShaderModule *vertShader = nullptr, *fragShader = nullptr;
    VK_CHECK_RESULT(buildShaderFromSource(logicalDevice,
                                          vertShaderSource,
                                          VK_SHADER_STAGE_VERTEX_BIT,
                                          vertShader))

    VK_CHECK_RESULT(buildShaderFromSource(logicalDevice,
                                          fragShaderSource,
                                          VK_SHADER_STAGE_FRAGMENT_BIT,
                                          fragShader))
}

void Shader::getShaderString(const std::string& relativePath,
                             std::string &shaderData) {

    std::string assetPath;
    getAssetPath(relativePath, assetPath);

    Log::info(assetPath.c_str());

    std::ifstream shaderFile;
    shaderFile.open(assetPath);

    std::string line;

    std::stringbuf buf;

    if(shaderFile.is_open()){
        while(getline(shaderFile, line)){
            buf.sputn(line.c_str(), line.size());
            buf.sputc('\n');
        }
    }

    std::string shaderString = buf.str();
    shaderData = shaderString;
}

shaderc_shader_kind getShadercShaderType(VkShaderStageFlagBits type) {
    switch (type) {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return shaderc_glsl_vertex_shader;
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return shaderc_glsl_fragment_shader;
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
            return shaderc_glsl_tess_control_shader;
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
            return shaderc_glsl_tess_evaluation_shader;
        case VK_SHADER_STAGE_GEOMETRY_BIT:
            return shaderc_glsl_geometry_shader;
        case VK_SHADER_STAGE_COMPUTE_BIT:
            return shaderc_glsl_compute_shader;
        default:
            Log::info("invalid VKShaderStageFlagBits",
                                 "tutorial06_texture", "type = %08x", type);
    }
    return static_cast<shaderc_shader_kind>(-1);
}

VkResult buildShaderFromSource(VkDevice logicalDevice,
                               std::string &shaderSource,
                               VkShaderStageFlagBits type,
                               VkShaderModule* shaderOut){
    shaderc::Compiler compiler;

//    // Build vulkan shader module
//    VkShaderModuleCreateInfo shaderModuleCreateInfo{
//        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
//        .pNext = nullptr,
//        .flags = 0,
//        .codeSize = shaderc_result_get_length(spvShader),
//        .pCode = (const uint32_t*) shaderc_result_get_bytes(spvShader)
//    };
//
//    VkResult result = vkCreateShaderModule(logicalDevice,  &shaderModuleCreateInfo, nullptr, shaderOut);

    return VK_ERROR_INITIALIZATION_FAILED;
}