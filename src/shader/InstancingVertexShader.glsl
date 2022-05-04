#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 2) in mat4 instanceMatrix;

uniform mat4 u_view;

void main() {
    gl_Position = u_view * instanceMatrix * vec4(a_pos, 1.0);
}


