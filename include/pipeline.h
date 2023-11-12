#ifndef INCLUDEGUARD_VKBASIC3D_PIPELINEH
#define INCLUDEGUARD_VKBASIC3D_PIPELINEH

#include <vulkan/vulkan.h>

#include "../include/vkbasic3d.h"

void vkbasic3d_pipeline_new(
	Vkbasic3d* vb3,
	VkDevice device,
	uint32_t width,
	uint32_t height
);

void vkbasic3d_pipeline_destroy(Vkbasic3d* vb3, VkDevice device);

#endif
