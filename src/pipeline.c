#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include <libgen.h>

#include "../include/pipeline.h"
#include "../include/vkbasic3d.h"
#include "../../vkbasic/include/pipeline.h"
#include "../../vkbasic/include/shader.h"
#include "../../ppath/include/ppath.h"

void vkbasic3d_pipeline_new(
	Vkbasic3d* vb3,
	VkDevice device
) {
	char* path = ppath_rel_new(__FILE__, "../../shader/test_vert.spv");
	VkShaderModule vert = vkbasic_shader_module(device, path);
	free(path);
	path = ppath_rel_new(__FILE__, "../../shader/test_frag.spv");
	VkShaderModule frag = vkbasic_shader_module(device, path);
	free(path);
	VkbasicPipelineConf* conf = vkbasic_pipeline_configure(vert, frag);
	vkbasic_pipeline_standard(
		&vb3->pipeline,
		&vb3->pipelinelayout,
		conf,
		vb3->renderpass,
		device
	);
	free(conf);
	vkDestroyShaderModule(device, frag, NULL);
	vkDestroyShaderModule(device, vert, NULL);
}
