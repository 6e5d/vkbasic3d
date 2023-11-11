#version 450

// the close/far points are *screen pos* at z=0 and z=1
layout(location = 0) out vec3 pc;
layout(location = 1) out vec3 pf;

layout(set = 0, binding = 0) uniform Data {
	mat4 view;
	mat4 proj;
	vec3 direction;
} uniforms;

vec3 plane[6] = vec3[] (
	vec3(-1, -1, 0),
	vec3(1, 1, 0),
	vec3(-1, 1, 0),
	vec3(1, 1, 0),
	vec3(-1, -1, 0),
	vec3(1, -1, 0)
);

vec3 unproject(float x, float y, float z) {
	mat4 view = inverse(uniforms.view);
	mat4 proj = inverse(uniforms.proj);
	vec4 unprojected =  view * proj * vec4(x, y, z, 1.0);
	return unprojected.xyz / unprojected.w;
}

void main() {
	vec3 p = plane[gl_VertexIndex].xyz;
	pc = unproject(p.x, p.y, 0.0);
	pf = unproject(p.x, p.y, 1.0);
	gl_Position = vec4(p, 1.0); // use screen pos here
}
