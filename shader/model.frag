#version 450
// #extension GL_EXT_nonuniform_qualifier: enable

layout(location = 0) in vec4 f_color;
layout(location = 1) in vec3 f_norm;
layout(location = 2) flat in int f_tex_layer;
layout(location = 3) in vec2 f_tex_coord;

layout(location = 0) out vec4 o_color;

// layout(set = 1, binding = 0) uniform sampler2D tex[];
layout(set = 0, binding = 0) uniform Data {
	mat4 view;
	mat4 proj;
	vec3 direction;
} uniforms;

void main() {
	// both need to be normalized
	float diff = dot(f_norm, uniforms.direction);
	// B: give some diffusion even at back side
	const float B = 0.3;
	if (diff < 0) {
		diff = B * (diff + 1.0);
	} else {
		diff = (1 - B) * diff + B;
	}
	vec3 light_color = vec3(1.0, 1.0, 1.0);
	vec3 color = (0.6 * diff + 0.4) * light_color;

	// if (f_tex_layer >= 0) {
	// 	o_color = texture(nonuniformEXT(tex[f_tex_layer]), f_tex_coord); } else {
	// }
	o_color.xyz = color *
		(f_color.w * f_color.xyz +
		(1.0 - f_color.w) * o_color.xyz);
	o_color.w = 1.0;
}
