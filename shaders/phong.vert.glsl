#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

uniform float scale = 0.0;

uniform mat4x4 projection;
uniform mat4x4 view;
uniform mat4x4 world;

out vec3 eye;
out vec3 fnormal;
out vec3 fposition;
out vec2 fuv;
out vec3 fcolor;
out mat3x3 view3x3;

void main() {
    gl_Position = projection * view * world * position;

	eye = -inverse(view)[3].xyz;
	view3x3 = inverse(mat3x3(view));
    fposition = gl_Position.xyz;
    fnormal = mat3x3(view * world) * normal;
    fuv = uv;
	fcolor = color;
}
