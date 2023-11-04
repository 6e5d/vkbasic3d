#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "../../vkbasic/include/vkbasic.h"
#include "../include/renderpass.h"
#include "../include/pipeline.h"
#include "../include/vkbasic3d.h"

#include <stdio.h>
#define X(n) printf("X %d\n", n);
Vkbasic3d* vkbasic3d_new(Vkbasic* vb) {
	Vkbasic3d* vb3 = malloc(sizeof(Vkbasic3d));
	vb3->vb = vb;
	vb3->renderpass = vkbasic3d_renderpass(
		vb->device,
		vb->scsi.format.format
	);
	vb3->pipeline = vkbasic3d_pipeline_new(vb->device, vb3->renderpass);
	return vb3;
}

void vkbasic3d_swapchain_update(
	Vkbasic3d* vb3,
	uint32_t width,
	uint32_t height
) {
	vkbasic_swapchain_update(vb3->vb, vb3->renderpass, width, height);
}

void vkbasic3d_destroy(Vkbasic3d* vb3) {
	vkDestroyPipeline(vb3->vb->device, vb3->pipeline, NULL);
	vkDestroyRenderPass(vb3->vb->device, vb3->renderpass, NULL);
	vkbasic_destroy(vb3->vb);
	free(vb3);
}

void vkbasic3d_build_command(
	Vkbasic3d* vb3,
	uint32_t index,
	uint32_t width,
	uint32_t height
) {
	static VkClearValue clearValue = {{{0.0f, 0.2f, 0.0f, 1.0f}}};
	VkCommandBuffer commandbuffer = vb3->vb->command_buffer;
	VkFramebuffer framebuffer = vb3->vb->vs->elements[index].framebuffer;

	VkRenderPassBeginInfo info = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = vb3->renderpass,
		.framebuffer = framebuffer,
		.renderArea.offset.x = 0,
		.renderArea.offset.y = 0,
		.renderArea.extent.width = width,
		.renderArea.extent.height = height,
		.clearValueCount = 1,
		.pClearValues = &clearValue,
	};
	vkCmdBeginRenderPass(commandbuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vb3->pipeline);
	vkCmdDraw(commandbuffer, 3, 1, 0, 0);
	vkCmdEndRenderPass(commandbuffer);
}
