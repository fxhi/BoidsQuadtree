#version 330 core

layout (location = 0) in vec3 a_pos;

uniform mat4 transform;
uniform mat4 u_view;

void main() {
    gl_Position = u_view * transform * vec4(a_pos, 1.0);
}