#version 420 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec2 v_texCoord;

layout(binding = 0) uniform sampler2D u_texture;

void main() {
    color = texture(u_texture, v_texCoord);
}