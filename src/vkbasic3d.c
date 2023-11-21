#include <assert.h>
#include <cglm/cglm.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "../../vkhelper/include/buffer.h"
#include "../../vkhelper/include/desc.h"
#include "../../vkhelper/include/renderpass.h"
#include "../../vkstatic/include/vkstatic.h"
#include "../include/camera.h"
#include "../include/model.h"
#include "../include/pipeline.h"
#include "../include/vertex.h"
#include "../include/vkbasic3d.h"

void vkbasic3d_init(
	Vkbasic3d* vb3,
	Vkstatic* vks
) {
	VkhelperRenderpassConf renderpass_conf;

	vkhelper_renderpass_config(
		&renderpass_conf,
		vks->device,
		vks->surface_format.format,
		vks->depth_format
	);

	VkSubpassDescription subpass = {
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &renderpass_conf.color_ref,
		.pDepthStencilAttachment = &renderpass_conf.depth_ref,
	};
	renderpass_conf.info.subpassCount = 1;
	renderpass_conf.info.pSubpasses = &subpass;
	vkhelper_renderpass_build(
		&vb3->renderpass,
		&renderpass_conf,
		vks->device
	);
	vkhelper_buffer_init_cpu(
		&vb3->vbufc, sizeof(Vkbasic3dVertex) * VKBASIC3D_MAX_VERTEX,
		vks->device, vks->memprop);
	vkhelper_buffer_init_gpu(
		&vb3->vbufg, sizeof(Vkbasic3dVertex) * VKBASIC3D_MAX_VERTEX,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		vks->device, vks->memprop);
	vkhelper_buffer_init_cpu(
		&vb3->ubufc, sizeof(Vkbasic3dCamera),
		vks->device, vks->memprop);
	vkhelper_buffer_init_gpu(
		&vb3->ubufg, sizeof(Vkbasic3dCamera),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		vks->device, vks->memprop);
	assert(0 == vkMapMemory(vks->device, vb3->ubufc.memory, 0,
		vb3->ubufc.size, 0, (void**)&vb3->camera));
	VkhelperDescConf conf;
	vkhelper_desc_config(&conf, 1);
	vkhelper_desc_build(&vb3->uniform, &conf, vks->device);
	VkDescriptorBufferInfo bufferinfo = {
		.buffer = vb3->ubufg.buffer,
		.offset = 0,
		.range = sizeof(Vkbasic3dCamera),
	};
	VkWriteDescriptorSet descwrite = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = vb3->uniform.set,
		.dstBinding = 0,
		.dstArrayElement = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.pBufferInfo = &bufferinfo,
	};
	vkUpdateDescriptorSets(vks->device, 1, &descwrite, 0, NULL);
	vkbasic3d_pipeline_init(vb3, vks->device);
}

void vkbasic3d_deinit(Vkbasic3d* vb3, VkDevice device) {
	vkDestroyPipeline(device, vb3->ppl_model, NULL);
	vkDestroyPipelineLayout(device, vb3->ppll_model, NULL);
	vkDestroyPipeline(device, vb3->ppl_grid, NULL);
	vkDestroyPipelineLayout(device, vb3->ppll_grid, NULL);
	vkDestroyRenderPass(device, vb3->renderpass, NULL);
	vkhelper_desc_deinit(&vb3->uniform, device);
	vkhelper_buffer_deinit(&vb3->ubufc, device);
	vkhelper_buffer_deinit(&vb3->ubufg, device);
	vkhelper_buffer_deinit(&vb3->vbufc, device);
	vkhelper_buffer_deinit(&vb3->vbufg, device);
}

void vkbasic3d_build_command(
	Vkbasic3d* vb3,
	Vkstatic* vks,
	VkCommandBuffer cbuf,
	VkFramebuffer framebuffer,
	uint32_t width,
	uint32_t height
) {
	{
		VkCommandBufferBeginInfo info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		};
		assert(0 == vkBeginCommandBuffer(cbuf, &info));
	}
	VkBufferCopy copy = { .size = sizeof(Vkbasic3dCamera) };
	vkCmdCopyBuffer(cbuf, vb3->ubufc.buffer, vb3->ubufg.buffer,
		1, &copy);
	if (vb3->vertex_update) {
		// printf("vertex update\n");
		copy.size = vb3->vlen * sizeof(Vkbasic3dVertex);
		vkCmdCopyBuffer(cbuf,
			vb3->vbufc.buffer, vb3->vbufg.buffer, 1, &copy);
		vb3->vertex_update = false;
	}
	static const VkClearValue clear_color = {
		.color.float32 = {0.0f, 0.0f, 0.0f, 1.0f},
	};
	static const VkClearValue clear_depthstencil = {
		.depthStencil.depth = 1.0f,
		.depthStencil.stencil = 0,
	};
	VkClearValue clear_values[2] = {clear_color, clear_depthstencil};

	VkViewport viewport = {0.0f, 0.0f,
		(float)width, (float)height,
		0.0f, 1.0f};
	VkRect2D scissor = {{0.0f, 0.0f}, {width, height}};
	vkCmdSetViewport(cbuf, 0, 1, &viewport);
	vkCmdSetScissor(cbuf, 0, 1, &scissor);

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
			cbuf,
			&info,
			VK_SUBPASS_CONTENTS_INLINE
		);
	}

	vkCmdBindPipeline(
		cbuf,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		vb3->ppl_model);
	vkCmdBindDescriptorSets(
		cbuf,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		vb3->ppll_model,
		0, 1,
		&vb3->uniform.set,
		0, NULL);
	VkDeviceSize zero = 0;
	vkCmdBindVertexBuffers(cbuf, 0, 1,
		&vb3->vbufg.buffer, &zero);
	vkCmdDraw(cbuf, vb3->vlen, 1, 0, 0);
	vkCmdBindPipeline(
		cbuf,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		vb3->ppl_grid);
	vkCmdBindDescriptorSets(
		cbuf,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		vb3->ppll_grid,
		0, 1,
		&vb3->uniform.set,
		0, NULL);
	vkCmdDraw(cbuf, 6, 1, 0, 0);
	vkCmdEndRenderPass(cbuf);
	assert(0 == vkEndCommandBuffer(vks->cbuf));
}
