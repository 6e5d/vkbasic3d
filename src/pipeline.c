#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include <libgen.h>

#include "../include/pipeline.h"
#include "../include/camera.h"
#include "../include/vkbasic3d.h"
#include "../../vkhelper/include/pipeline.h"
#include "../../vkhelper/include/shader.h"
#include "../../ppath/include/ppath.h"

void vkbasic3d_pipeline_new(
	Vkbasic3d* vb3,
	VkDevice device
) {
	char* path = ppath_rel_new(__FILE__, "../../shader/model_vert.spv");
	VkShaderModule vert = vkhelper_shader_module(device, path);
	free(path);
	path = ppath_rel_new(__FILE__, "../../shader/model_frag.spv");
	VkShaderModule frag = vkhelper_shader_module(device, path);
	free(path);
	VkhelperPipelineConf conf;

	vkhelper_pipeline_configure(&conf, vert, frag);
	vb3->vib = (VkVertexInputBindingDescription) {
		.binding = 0,
		.stride = 32,
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};

	vb3->via[0] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 0,
		.format = VK_FORMAT_R32G32B32A32_SFLOAT,
		.offset = 0,
	};
	vb3->via[1] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 1,
		.format = VK_FORMAT_R32G32B32A32_SFLOAT,
		.offset = 16,
	};
	vb3->via[2] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 2,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = 32,
	};
	vb3->via[3] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 3,
		.format = VK_FORMAT_R32_SINT,
		.offset = 44,
	};
	vb3->via[4] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 4,
		.format = VK_FORMAT_R32G32_SFLOAT,
		.offset = 48,
	};
	conf.vis.vertexBindingDescriptionCount = 1;
	conf.vis.pVertexBindingDescriptions = &vb3->vib;
	conf.vis.vertexAttributeDescriptionCount = 5;
	conf.vis.pVertexAttributeDescriptions = vb3->via;

	// descset layout
	VkDescriptorSetLayoutBinding setbinding = {
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
	};
	VkDescriptorSetLayoutCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.bindingCount = 1,
		.pBindings = &setbinding,
	};
	assert(0 == vkCreateDescriptorSetLayout(
		device, &info, NULL, &vb3->descset_layout));
	conf.pl.setLayoutCount = 1;
	conf.pl.pSetLayouts = &vb3->descset_layout;

	vkhelper_pipeline_standard(
		&vb3->pipeline,
		&vb3->pipelinelayout,
		&conf,
		vb3->renderpass,
		device
	);
	vkDestroyShaderModule(device, frag, NULL);
	vkDestroyShaderModule(device, vert, NULL);
}
