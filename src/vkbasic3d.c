#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <vulkan/vulkan.h>
#include <math.h>
#include <cglm/cglm.h>

#include "../../vkstatic/include/vkstatic.h"
#include "../../vkhelper/include/renderpass.h"
#include "../../vkhelper/include/buffer.h"
#include "../include/vertex.h"
#include "../include/camera.h"
#include "../include/model.h"
#include "../include/pipeline.h"
#include "../include/vkbasic3d.h"

void vkbasic3d_init(
	Vkbasic3d* vb3,
	Vkstatic* vs
) {
	vb3->zero = 0;
	vb3->recreate_pipeline = true;
	vb3->renderpass = vkhelper_renderpass(
		vs->device,
		vs->surface_format.format,
		vs->depth_format
	);
	// descset layout
	VkDescriptorSetLayoutBinding setbinding = {
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT |
			VK_SHADER_STAGE_FRAGMENT_BIT,
	};
	VkDescriptorSetLayoutCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.bindingCount = 1,
		.pBindings = &setbinding,
	};
	assert(0 == vkCreateDescriptorSetLayout(
		vs->device, &info, NULL, &vb3->descset_layout));
	vkhelper_buffer_init(
		&vb3->vbuf, sizeof(Vkbasic3dVertex) * VKBASIC3D_MAX_VERTEX,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		vs->device, vs->memprop);
	vkhelper_buffer_init(
		&vb3->ubuf, sizeof(Vkbasic3dCamera),
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		vs->device, vs->memprop);
	assert(0 == vkMapMemory(vs->device, vb3->ubuf.smemory, 0,
		vb3->ubuf.ssize, 0, (void**)&vb3->camera));
	vkhelper_buffer_sync(&vb3->ubuf, sizeof(Vkbasic3dCamera),
		vs->device, vs->queue, vs->cpool);
	VkDescriptorPoolSize poolsize = {
		.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
	};
	VkDescriptorPoolCreateInfo poolinfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.poolSizeCount = 1,
		.pPoolSizes = &poolsize,
		.maxSets = 1,
	};
	assert(0 == vkCreateDescriptorPool(
		vs->device, &poolinfo, NULL, &vb3->descpool));
	VkDescriptorSetAllocateInfo allocinfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = vb3->descpool,
		.descriptorSetCount = 1,
		.pSetLayouts = &vb3->descset_layout,
	};
	assert(0 == vkAllocateDescriptorSets(
		vs->device, &allocinfo, &vb3->descset));
	VkDescriptorBufferInfo bufferinfo = {
		.buffer = vb3->ubuf.buffer,
		.offset = 0,
		.range = sizeof(Vkbasic3dCamera),
	};
	VkWriteDescriptorSet descwrite = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = vb3->descset,
		.dstBinding = 0,
		.dstArrayElement = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.pBufferInfo = &bufferinfo,
	};
	vkUpdateDescriptorSets(vs->device, 1, &descwrite, 0, NULL);
}

void vkbasic3d_deinit(Vkbasic3d* vb3, VkDevice device) {
	vkbasic3d_pipeline_destroy(vb3, device);
	vkDestroyRenderPass(device, vb3->renderpass, NULL);
	vkDestroyDescriptorSetLayout(device, vb3->descset_layout, NULL);
	vkDestroyDescriptorPool(device, vb3->descpool, NULL);
	vkhelper_buffer_deinit(&vb3->ubuf, device);
	vkhelper_buffer_deinit(&vb3->vbuf, device);
}

void vkbasic3d_build_command(
	Vkbasic3d* vb3,
	VkDevice device,
	VkCommandBuffer commandbuffer,
	VkFramebuffer framebuffer,
	uint32_t width,
	uint32_t height
) {
	if (vb3->recreate_pipeline) {
		if (vb3->pipeline != VK_NULL_HANDLE) {
			vkbasic3d_pipeline_destroy(vb3, device);
		}
		vkbasic3d_pipeline_new(vb3, device, width, height);
		vb3->recreate_pipeline = false;
	}
	{
		VkCommandBufferBeginInfo info = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		};
		assert(0 == vkBeginCommandBuffer(commandbuffer, &info));
	}
	VkBufferCopy copy = { .size = sizeof(Vkbasic3dCamera) };
	vkCmdCopyBuffer(commandbuffer, vb3->ubuf.sbuffer, vb3->ubuf.buffer,
		1, &copy);
	if (vb3->vertex_update) {
		// printf("vertex update\n");
		copy.size = vb3->vlen * sizeof(Vkbasic3dVertex);
		vkCmdCopyBuffer(commandbuffer,
			vb3->vbuf.sbuffer, vb3->vbuf.buffer, 1, &copy);
		vb3->vertex_update = false;
	}
	static const VkClearValue clear_color = {
		.color.float32 = {0.0f, 0.0f, 0.0f, 1.0f},
	};
	static const VkClearValue clear_depthstencil = {
		.depthStencil.depth = 1.0,
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
	vkCmdBindDescriptorSets(
		commandbuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		vb3->pipelinelayout,
		0, 1,
		&vb3->descset,
		0, NULL
	);
	vkCmdBindVertexBuffers(commandbuffer, 0, 1,
		&vb3->vbuf.buffer, &vb3->zero);
	vkCmdDraw(commandbuffer, vb3->vlen, 1, 0, 0);
	vkCmdNextSubpass(commandbuffer, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(
		commandbuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		vb3->pipelineg
	);
	vkCmdBindDescriptorSets(
		commandbuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		vb3->pipelinelayout,
		0, 1,
		&vb3->descset,
		0, NULL
	);
	vkCmdDraw(commandbuffer, 6, 1, 0, 0);
	vkCmdEndRenderPass(commandbuffer);
}
