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
	const float C = 0.01;
	const float CH = C * 0.01;
	if ((fract(CH + coord.x * 0.1) < C) ||
		(fract(CH + coord.y * 0.1) < C)) {
		alpha *= max(0.0, 1.0 - pow(d, 20));
	} else {
		alpha *= max(0.0, 1.0 - pow(d, 5));
	}
	vec4 color = vec4(0.5, 0.5, 1.0, alpha);
	if (coord.x >= 0) {
		color.x = 0.7;
	}
	if (coord.y >= 0) {
		color.y = 0.7;
	}
	return color;
}

float depth(vec3 pos) {
	vec4 proj = uniforms.proj * uniforms.view * vec4(pos.xyz, 1.0);
	return (proj.z / proj.w);
}

void main() {
	float t = -pc.y / (pf.y - pc.y);
	vec3 unproj = pc + t * (pf - pc);
	float d = depth(unproj);
	gl_FragDepth = d;
	o_color = grid(unproj, 10.0, d) * float(t > 0);
}
