#version 450
// #extension GL_EXT_nonuniform_qualifier: enable

layout(location = 0) in vec4 f_color;
layout(location = 1) in vec3 f_norm;
layout(location = 2) flat in int f_tex_layer;
layout(location = 3) in vec2 f_tex_coord;

layout(location = 0) out vec4 o_color;

// layout(set = 1, binding = 0) uniform sampler2D tex[];
// layout(push_constant) uniform Light {
// 	vec3 color;
// 	vec3 direction;
// } light;

void main() {
	// both need to be normalized
	// float diff = dot(f_norm, light.direction);
	float diff = dot(f_norm, vec3(0.0, 1.0, 0.0));
	// B: give some diffusion even at back side
	const float B = 0.5;
	if (diff < 0) {
		diff = B * (diff + 1.0);
	} else {
		diff = (1 - B) * diff + B;
	}
	// vec3 color = (0.8 * diff + 0.2) * light.color;
	vec3 color = (0.8 * diff + 0.2) * vec3(1.0, 1.0, 1.0);

	// if (f_tex_layer >= 0) {
	// 	o_color = texture(nonuniformEXT(tex[f_tex_layer]), f_tex_coord); } else {
	// }
	o_color = vec4(0.0, 0.0, 0.0, 1.0);
	o_color.xyz = f_color.w * f_color.xyz + (1.0 - f_color.w) * o_color.xyz;
	o_color *= vec4(color, 1.0);
}
