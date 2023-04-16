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

    shaderModules[ShaderType_VERTEX] = *new VkShaderModule;
    shaderModules[ShaderType_FRAGMENT] = *new VkShaderModule;

    const std::vector<uint32_t> vertCompileResult = compileShaderToSpv(vertShaderSource, VK_SHADER_STAGE_VERTEX_BIT);
    const std::vector<uint32_t> fragCompileResult = compileShaderToSpv(fragShaderSource, VK_SHADER_STAGE_FRAGMENT_BIT);

    Shader::vertexInfo = generateVertexInfo(vertCompileResult);

    std::vector<DescriptorSetLayoutData> vertDesc = generateDescriptorInfo(vertCompileResult);
    std::vector<DescriptorSetLayoutData> fragDesc = generateDescriptorInfo(fragCompileResult);

    for(auto& desc : vertDesc){
        for(auto& binding : desc.bindings){
            layout_bindings.push_back(binding);
        }
    }

    for(auto& desc : fragDesc){
        for(auto& binding : desc.bindings){
            layout_bindings.push_back(binding);
        }
    }

    VK_CHECK_RESULT(buildShaderFromSource(logicalDevice,
                                          vertCompileResult,
                                          &shaderModules[ShaderType_VERTEX]))

    VK_CHECK_RESULT(buildShaderFromSource(logicalDevice,
                                          fragCompileResult,
                                          &shaderModules[ShaderType_FRAGMENT]))
}

void Shader::deleteShader(VkDevice logicalDevice){
    for(auto const &[key, value] : shaderModules){
        vkDestroyShaderModule(logicalDevice, value, nullptr);
    }
}

VkShaderModule Shader::getShaderModule(ShaderTypes type){
    return shaderModules[type];
}

void getShaderString(const std::string& relativePath,
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

std::vector<uint32_t> compileShaderToSpv(std::string &shaderSource,
                                    VkShaderStageFlagBits type){
    shaderc::Compiler       compiler;
    shaderc::CompileOptions options;
    std::vector<uint32_t>   compilerResult;

    shaderc::SpvCompilationResult spvShader = compiler.CompileGlslToSpv(shaderSource.c_str(),
                                                                        shaderSource.size(),
                                                                        getShadercShaderType(type),
                                                                        "main");

    if(spvShader.GetCompilationStatus() !=
        shaderc_compilation_status_success){
        return  compilerResult;
    }


    compilerResult.assign(spvShader.begin(), spvShader.end());

    return compilerResult;
}

VkResult buildShaderFromSource(VkDevice logicalDevice,
                               const std::vector<uint32_t>& compilerResult,
                               VkShaderModule* shaderOut){

    // Build vulkan shader module
    VkShaderModuleCreateInfo shaderModuleCreateInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = compilerResult.size() * sizeof(uint32_t),
        .pCode = compilerResult.data()
    };

    VkResult result = vkCreateShaderModule(logicalDevice,  &shaderModuleCreateInfo, nullptr, shaderOut);

    return result;
}
