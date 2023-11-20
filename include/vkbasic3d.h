#ifndef INCLUDEGUARD_VKBASIC3D_VKBASIC3DH
#define INCLUDEGUARD_VKBASIC3D_VKBASIC3DH

#include <stdbool.h>
#include <stdint.h>
#include <vulkan/vulkan.h>

#include "../../vkhelper/include/desc.h"
#include "../../vkhelper/include/buffer.h"
#include "../../vkstatic/include/vkstatic.h"
#include "../include/camera.h"

static const size_t VKBASIC3D_MAX_VERTEX = 100000;

typedef struct {
	VkRenderPass renderpass;
	VkPipeline pipeline;
	VkPipelineLayout pipelinelayout;
	VkPipeline pipelineg;
	VkPipelineLayout pipelineglayout;
	VkhelperBuffer vbufc;
	VkhelperBuffer vbufg;
	uint32_t vlen;
	bool vertex_update;
	bool recreate_pipeline;
	VkhelperDesc uniform;
	VkhelperBuffer ubufg;
	VkhelperBuffer ubufc;
	Vkbasic3dCamera* camera; // mapped, no free
} Vkbasic3d;

void vkbasic3d_init(Vkbasic3d* vb3, Vkstatic* vks);
void vkbasic3d_deinit(Vkbasic3d* vb3, VkDevice device);

void vkbasic3d_build_command(
	Vkbasic3d* vb3,
	Vkstatic* vks,
	VkCommandBuffer commandbuffer,
	VkFramebuffer framebuffer,
	uint32_t width,
	uint32_t height
);

#endif
