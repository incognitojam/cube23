#version 420 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

layout(std140, binding = 0) uniform CameraData {
    mat4 u_viewProjection;
};

layout(std140, binding = 1) uniform ObjectData {
    mat4 u_transform;
};

layout(location = 0) out vec2 v_texCoord;

void main() {
    v_texCoord = a_texCoord;
    gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
}