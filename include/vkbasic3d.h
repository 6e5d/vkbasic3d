#ifndef INCLUDEGUARD_VKBASIC3D
#define INCLUDEGUARD_VKBASIC3D

#include <vulkan/vulkan.h>

#include "../../vkhelper2/include/vkhelper2.h"
#include "../../vkstatic/include/vkstatic.h"
#include "../../modelobj/build/modelobj.h"
#include "../include/camera.h"

static const size_t VKBASIC3D_MAX_VERTEX = 100000;

typedef struct {
	VkRenderPass renderpass;
	VkPipeline ppl_model;
	VkPipelineLayout ppll_model;
	VkPipeline ppl_grid;
	VkPipelineLayout ppll_grid;
	Vkhelper2Buffer vbufc;
	Vkhelper2Buffer vbufg;
	uint32_t vlen;
	bool vertex_update;
	Vkhelper2Desc uniform;
	Vkhelper2Buffer ubufg;
	Vkhelper2Buffer ubufc;
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

void vkbasic3d_model_upload(
	Vkstatic* vs,
	Vkbasic3d* vb3,
	Com_6e5dModelobj* model
);

#endif
