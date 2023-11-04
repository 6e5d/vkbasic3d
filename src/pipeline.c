#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include <libgen.h>

#include "../include/pipeline.h"
#include "../../vkbasic/include/common.h"

// TODO: free data and pipeline!
static VkShaderModule create_shader_module(
	VkDevice device,
	char* path
) {
	printf("loading: %s\n", path);
	FILE* fp = fopen(path, "rb");
	if (fp == NULL) {
		printf("shader not found\n");
		exit(1);
	}
	fseek(fp, 0L, SEEK_END);
	size_t len = ftell(fp);
	if (len % 4 != 0) {
		printf("bad spv shader code!\n");
		exit(1);
	}
	size_t count = len / 4;
	rewind(fp);
	uint32_t* data = malloc(len);
	assert(fread(data, 4, count, fp) == count);
	VkShaderModuleCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = len,
		.pCode = data,
	};
	VkShaderModule shaderModule;
	vkbasic_check(vkCreateShaderModule(
		device, &createInfo, NULL, &shaderModule));
	free(data);
	return shaderModule;
}

VkPipeline vkbasic3d_pipeline_new(
	VkDevice device,
	VkRenderPass renderpass
) {
	size_t len1 = strlen(__FILE__);
	char shader_vert_rel[] = "/../shader/test_vert.spv";
	size_t len2 = strlen(shader_vert_rel);
	char* shader_path = malloc(len1 + len2 + 1);
	strcpy(shader_path, __FILE__);
	shader_path = dirname(shader_path);
	printf("%s\n", shader_path);
	strcat(shader_path, shader_vert_rel);
	VkShaderModule vertShaderModule =
		create_shader_module(device, shader_path);
	char shader_frag_rel[] = "/../shader/test_frag.spv";
	len2 = strlen(shader_frag_rel);
	shader_path = realloc(shader_path, len1 + len2 + 1);
	strcpy(shader_path, __FILE__);
	shader_path = dirname(shader_path);
	strcat(shader_path, shader_frag_rel);
	VkShaderModule fragShaderModule =
		create_shader_module(device, shader_path);
	free(shader_path);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vertShaderModule,
		.pName = "main",
	};

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = fragShaderModule,
		.pName = "main",
	};

	VkPipelineShaderStageCreateInfo shaderStages[] =
		{vertShaderStageInfo, fragShaderStageInfo};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 0,
		.vertexAttributeDescriptionCount = 0,
	};

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE,
	};

	VkViewport viewport = {
		.x = 0.0f,
		.y = 0.0f,
		.width = 900.0f,
		.height = 600.0f,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	VkRect2D scissor = {
		.offset = {0, 0},
		.extent = {99999, 99999},
	};

	VkPipelineViewportStateCreateInfo viewportState = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor,
	};

	VkPipelineRasterizationStateCreateInfo rasterizer = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.lineWidth = 1.0f,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
	};

	VkPipelineMultisampleStateCreateInfo multisampling = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	};

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {
		.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT,
		.blendEnable = VK_FALSE,
	};

	VkPipelineColorBlendStateCreateInfo colorBlending = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachment,
		.blendConstants[0] = 0.0f,
		.blendConstants[1] = 0.0f,
		.blendConstants[2] = 0.0f,
		.blendConstants[3] = 0.0f,
	};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 0,
		.pushConstantRangeCount = 0,
	};
	VkPipelineLayout pipeline_layout;
	vkbasic_check(vkCreatePipelineLayout(
		device, &pipelineLayoutInfo, NULL, &pipeline_layout));

	VkGraphicsPipelineCreateInfo pipelineInfo = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = 2,
		.pStages = shaderStages,
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &inputAssembly,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pColorBlendState = &colorBlending,
		.layout = pipeline_layout,
		.renderPass = renderpass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
	};

	VkPipeline result;
	vkbasic_check(vkCreateGraphicsPipelines(
		device,
		VK_NULL_HANDLE,
		1,
		&pipelineInfo,
		NULL,
		&result
	));
	vkDestroyShaderModule(device, fragShaderModule, NULL);
	vkDestroyShaderModule(device, vertShaderModule, NULL);
	return result;
}
