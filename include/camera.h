#ifndef _VKBASIC3D_CAMERA
#define _VKBASIC3D_CAMERA

#include <cglm/cglm.h>

typedef struct {
	mat4 view;
	mat4 proj;
	vec3 direction;
} Vkbasic3dCamera;

#endif
