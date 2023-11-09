#include <assert.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "../../vkbasic/include/vkbasic.h"
#include "../include/renderpass.h"
#include "../include/pipeline.h"
#include "../include/vkbasic3d.h"

Vkbasic3d* vkbasic3d_new(Vkbasic* vb) {
	Vkbasic3d* vb3 = malloc(sizeof(Vkbasic3d));
	vb3->renderpass = vkbasic3d_renderpass(
		vb->device,
		vb->scsi.format.format,
		vb->depth_format
	);
	vkbasic3d_pipeline_new(vb3, vb->device);
	return vb3;
}

void vkbasic3d_destroy(Vkbasic3d* vb3, VkDevice device) {
	vkDestroyPipelineLayout(device, vb3->pipelinelayout, NULL);
	vkDestroyPipeline(device, vb3->pipeline, NULL);
	vkDestroyRenderPass(device, vb3->renderpass, NULL);
	free(vb3);
}

void vkbasic3d_build_command(
	Vkbasic3d* vb3,
	VkCommandBuffer commandbuffer,
	VkFramebuffer framebuffer,
	uint32_t width,
	uint32_t height
) {
	{
		VkCommandBufferBeginInfo info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		};
		assert(0 == vkBeginCommandBuffer(commandbuffer, &info));
	}

	static const VkClearValue clear_color = {
		.color.float32 = {0.0f, 0.2f, 0.0f, 1.0f},
	};
	static const VkClearValue clear_depthstencil = {
		.depthStencil.depth = 0,
		.depthStencil.stencil = 0.0,
	};
	VkClearValue clear_values[2] = {clear_color, clear_depthstencil};

	{
		VkRenderPassBeginInfo info = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = vb3->renderpass,
			.framebuffer = framebuffer,
			.renderArea.offset.x = 0,
			.renderArea.offset.y = 0,
			.renderArea.extent.width = width,
			.renderArea.extent.height = height,
			.clearValueCount = 2,
			.pClearValues = clear_values,
		};
		vkCmdBeginRenderPass(
			commandbuffer,
			&info,
			VK_SUBPASS_CONTENTS_INLINE
		);
	}
	vkCmdBindPipeline(
		commandbuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		vb3->pipeline
	);
	vkCmdDraw(commandbuffer, 3, 1, 0, 0);
	vkCmdEndRenderPass(commandbuffer);
}
