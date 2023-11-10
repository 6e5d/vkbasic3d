#ifndef _VKBASIC3D_VKBASIC3DH
#define _VKBASIC3D_VKBASIC3DH

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "../include/camera.h"
#include "../../vkbasic/include/vkbasic.h"
#include "../../vkhelper/include/buffer.h"

static const size_t VKBASIC3D_MAX_VERTEX = 10000;
static const size_t VKBASIC3D_MAX_INDEX = 50000;

typedef struct {
	VkRenderPass renderpass;
	VkPipeline pipeline;
	VkPipelineLayout pipelinelayout;
	VkhelperBuffer vbuf;
	VkhelperBuffer ibuf;
	VkhelperBuffer ubuf;
	VkVertexInputBindingDescription vib;
	VkVertexInputAttributeDescription via[5];
	VkDeviceSize zero;
	uint32_t index_len;
	Vkbasic3dCamera* camera; // mapped
	VkDescriptorPool descpool;
	VkDescriptorSetLayout descset_layout;
	VkDescriptorSet descset;
	VkViewport viewport;
	VkRect2D scissor;
} Vkbasic3d;

void vkbasic3d_init(Vkbasic3d* vb3, Vkstatic* vks);
void vkbasic3d_deinit(Vkbasic3d* vb3, VkDevice device);

void vkbasic3d_build_command(
	Vkbasic3d* vb3,
	VkCommandBuffer commandbuffer,
	VkFramebuffer framebuffer,
	uint32_t width,
	uint32_t height
);

#endif
