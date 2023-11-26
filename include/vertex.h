#ifndef INCLUDEGUARD_VKBASIC3D_VERTEXH
#define INCLUDEGUARD_VKBASIC3D_VERTEXH

#include <stdint.h>

typedef struct {
	float pos[4];
	float color[4];
	float normal[3];
	int32_t layer;
	float uv[2];
} Vkbasic3dVertex;

#endif
