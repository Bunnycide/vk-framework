#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 1) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 incolor;
layout (location = 2) in vec2 attr;

layout (location = 0) out vec2 texcoord;
layout (location = 1) out vec3 outcolor;
void main() {
    texcoord = attr;
    outcolor = incolor;

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(pos, 1.0f);
}
