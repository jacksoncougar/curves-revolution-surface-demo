#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

uniform mat4x4 projection;
uniform mat4x4 view;
uniform mat4x4 world;

out vec3 fcolor;

void main() {
    gl_Position = projection * view * world * position;
	fcolor = color;
}
