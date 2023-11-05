#ifndef _VKBASIC3D_VKBASIC3DH
#define _VKBASIC3D_VKBASIC3DH

#include <vulkan/vulkan.h>

#include "../../vkbasic/include/vkbasic.h"

typedef struct {
	VkRenderPass renderpass;
	VkPipeline pipeline;
	VkPipelineLayout pipelinelayout;
} Vkbasic3d;

Vkbasic3d* vkbasic3d_new(Vkbasic* vb);
void vkbasic3d_destroy(Vkbasic3d* vb3, VkDevice device);

void vkbasic3d_build_command(
	Vkbasic3d* vb3,
	VkCommandBuffer commandbuffer,
	VkFramebuffer framebuffer,
	uint32_t width,
	uint32_t height
);

#endif
