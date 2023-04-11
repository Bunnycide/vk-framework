#version 120

layout (location = 0) in vec2 texcoord;
layout (location = 1) in vec3 outcolor;

out vec4 fragColor;

void main() {
    fragColor = vec4(outcolor, 1.0f);
}
