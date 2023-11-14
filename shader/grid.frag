#version 450

layout(location = 0) in vec3 pc;
layout(location = 1) in vec3 pf;
layout(location = 0) out vec4 o_color;

layout(set = 0, binding = 0) uniform Data {
	mat4 view;
	mat4 proj;
	vec3 direction;
} uniforms;

vec4 grid(vec3 pos, float scale, float d) {
	vec2 coord = pos.xz * scale;
	vec2 derivative = fwidth(coord);
	vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
	float line = min(grid.x, grid.y);
	float alpha = 1.0 - min(line, 1.0); // the base wire alpha
	alpha *= min(1.0, exp(0.5 * d));
	vec4 color = vec4(0.5, 0.8, 0.8, alpha);
	return color;
}

// camera space depth used in drawing
// viewport depth for correctly handling depth buffer
float depth_camera(vec3 pos) {
	vec4 proj = uniforms.view * vec4(pos.xyz, 1.0);
	return (proj.z / proj.w);
}
float depth_viewport(vec3 pos) {
	vec4 proj = uniforms.proj * uniforms.view * vec4(pos.xyz, 1.0);
	return (proj.z / proj.w);
}

void main() {
	float t = -pc.y / (pf.y - pc.y);
	vec3 unproj = pc + t * (pf - pc);
	float d = depth_camera(unproj);
	float dv = depth_viewport(unproj);
	gl_FragDepth = dv;
	o_color = grid(unproj, 10.0, d) * float(t > 0);
}
