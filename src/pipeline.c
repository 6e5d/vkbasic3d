#include <assert.h>
#include <libgen.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>

#include "../../ppath/include/ppath.h"
#include "../../vkhelper/include/pipeline.h"
#include "../../vkhelper/include/shader.h"
#include "../include/camera.h"
#include "../include/pipeline.h"
#include "../include/vertex.h"
#include "../include/vkbasic3d.h"

void vkbasic3d_pipeline_new(
	Vkbasic3d* vb3,
	VkDevice device,
	uint32_t width,
	uint32_t height
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
	vkhelper_pipeline_configure(&conf, vert, frag, width, height);
	conf.pl.setLayoutCount = 1;
	conf.pl.pSetLayouts = &vb3->descset_layout;
	vkhelper_pipeline_standard(
		&vb3->pipelineg,
		&vb3->pipelineglayout,
		&conf,
		vb3->renderpass,
		device,
		0
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
	vkhelper_pipeline_configure(&conf, vert, frag, width, height);
	vb3->vib = (VkVertexInputBindingDescription) {
		.binding = 0,
		.stride = sizeof(Vkbasic3dVertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};

	vb3->via[0] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 0,
		.format = VK_FORMAT_R32G32B32A32_SFLOAT,
		.offset = offsetof(Vkbasic3dVertex, pos),
	};
	vb3->via[1] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 1,
		.format = VK_FORMAT_R32G32B32A32_SFLOAT,
		.offset = offsetof(Vkbasic3dVertex, color),
	};
	vb3->via[2] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 2,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = offsetof(Vkbasic3dVertex, normal),
	};
	vb3->via[3] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 3,
		.format = VK_FORMAT_R32_SINT,
		.offset = offsetof(Vkbasic3dVertex, layer),
	};
	vb3->via[4] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 4,
		.format = VK_FORMAT_R32G32_SFLOAT,
		.offset = offsetof(Vkbasic3dVertex, uv),
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

void vkbasic3d_pipeline_destroy(Vkbasic3d* vb3, VkDevice device) {
	vkDestroyPipelineLayout(device, vb3->pipelinelayout, NULL);
	vkDestroyPipeline(device, vb3->pipeline, NULL);
	vkDestroyPipelineLayout(device, vb3->pipelineglayout, NULL);
	vkDestroyPipeline(device, vb3->pipelineg, NULL);
}
