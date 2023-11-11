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
	char* path;
	VkShaderModule vert, frag;
	VkhelperPipelineConf conf;

	// grid
	path = ppath_rel_new(__FILE__, "../../shader/grid_vert.spv");
	vert = vkhelper_shader_module(device, path);
	free(path);
	path = ppath_rel_new(__FILE__, "../../shader/grid_frag.spv");
	frag = vkhelper_shader_module(device, path);
	free(path);
	vkhelper_pipeline_configure(&conf, vert, frag);
	conf.pl.setLayoutCount = 1;
	conf.pl.pSetLayouts = &vb3->descset_layout;
	vkhelper_pipeline_standard(
		&vb3->pipelineg,
		&vb3->pipelineglayout,
		&conf,
		vb3->renderpass,
		device,
		1
	);
	vkDestroyShaderModule(device, frag, NULL);
	vkDestroyShaderModule(device, vert, NULL);

	// model
	path = ppath_rel_new(__FILE__, "../../shader/model_vert.spv");
	vert = vkhelper_shader_module(device, path);
	free(path);
	path = ppath_rel_new(__FILE__, "../../shader/model_frag.spv");
	frag = vkhelper_shader_module(device, path);
	free(path);
	vkhelper_pipeline_configure(&conf, vert, frag);
	vb3->vib = (VkVertexInputBindingDescription) {
		.binding = 0,
		.stride = 56,
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

	conf.pl.setLayoutCount = 1;
	conf.pl.pSetLayouts = &vb3->descset_layout;

	vkhelper_pipeline_standard(
		&vb3->pipeline,
		&vb3->pipelinelayout,
		&conf,
		vb3->renderpass,
		device,
		0
	);
	vkDestroyShaderModule(device, frag, NULL);
	vkDestroyShaderModule(device, vert, NULL);
}
