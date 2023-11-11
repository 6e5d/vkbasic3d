#version 450

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 norm;
layout(location = 3) in int tex_layer;
layout(location = 4) in vec2 tex_coord;

layout(location = 0) out vec4 f_color;
layout(location = 1) out vec3 f_norm;
layout(location = 2) out int f_tex_layer;
layout(location = 3) out vec2 f_tex_coord;

layout(set = 0, binding = 0) uniform Data {
	mat4 view;
	mat4 proj;
	vec3 direction;
} uniforms;

mat4 view2 = {
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0},
};
mat4 proj2 = {
	{1.0, 0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0, 0.0},
	{0.0, 0.0, 0.0, 1.0},
};
vec4 colordebug(float x) {
	if (isinf(x) || isnan(x)) {
		return vec4(1.0, 1.0, 1.0, 1.0);
	}
	if (x < -1.0) {
		return vec4(1.0, 0.0, log(-x) / 5.0, 1.0);
	} else if (x <= 0.0) {
		return vec4(-x, 0.0, 0.0, 1.0);
	} else if (x < 1.0) {
		return vec4(0.0, 0.0, x, 1.0);
	} else if (x >= 1.0) {
		return vec4(log(x) / 5.0, 0.0, 1.0, 1.0);
	}
}
void main() {
	// vec4 r = uniforms.proj * uniforms.view * pos;
	// r.xyz *= 0;
	// gl_Position = pos;
	gl_Position = uniforms.proj * uniforms.view * pos;
	// gl_Position = vec4(pos.xyz * 0.5, 1.0);
	// f_color = colordebug(pos.z);
	f_color = color;
	f_norm = norm;
	f_tex_coord = tex_coord;
	f_tex_layer = tex_layer;
}
