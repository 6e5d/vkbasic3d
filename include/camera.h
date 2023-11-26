#ifndef INCLUDEGUARD_VKBASIC3D_CAMERAH
#define INCLUDEGUARD_VKBASIC3D_CAMERAH

#include <cglm/cglm.h>

typedef struct {
	mat4 view;
	mat4 proj;
	vec3 direction;
} Vkbasic3dCamera;

#endif
