#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 f_color;

layout(set = 0, binding = 0) uniform Data {
	mat4 view;
	mat4 proj;
} uniforms;

void main() {
	gl_Position = uniforms.proj * uniforms.view * pos;
	f_color = color;
}
