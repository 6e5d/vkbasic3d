#include <vulkan/vulkan.h>

#include "../build/vkbasic3d.h"

static void pipeline_init_grid(Vkbasic3d() *vb3, VkDevice device) {
	Vkhelper2(PipelineConfig) vpc = {0};
	vkhelper2(pipeline_config)(&vpc, 0, 0, 1);
	vkhelper2(pipeline_simple_shader)(&vpc, device,
		__FILE__, "../../vkbasic3d_shader/build/grid");
	vpc.desc[0] = vb3->uniform.layout;
	vkhelper2(pipeline_build)(&vb3->ppll_grid, &vb3->ppl_grid,
		&vpc, vb3->renderpass, device, 0);
	vkhelper2(pipeline_config_deinit)(&vpc, device);
}

static void pipeline_init_model(Vkbasic3d() *vb3, VkDevice device) {
	Vkhelper2(PipelineConfig) vpc = {0};
	vkhelper2(pipeline_config)(&vpc, 1, 5, 1);
	vkhelper2(pipeline_simple_shader)(&vpc, device,
		__FILE__, "../../vkbasic3d_shader/build/model");
	vpc.vib[0] = (VkVertexInputBindingDescription) {
		.binding = 0,
		.stride = sizeof(Vkbasic3d(Vertex)),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};
	vpc.via[0] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 0,
		.format = VK_FORMAT_R32G32B32A32_SFLOAT,
		.offset = 0,
	};
	vpc.via[1] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 1,
		.format = VK_FORMAT_R32G32B32A32_SFLOAT,
		.offset = 16,
	};
	vpc.via[2] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 2,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = 32,
	};
	vpc.via[3] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 3,
		.format = VK_FORMAT_R32_SINT,
		.offset = 44,
	};
	vpc.via[4] = (VkVertexInputAttributeDescription) {
		.binding = 0,
		.location = 4,
		.format = VK_FORMAT_R32G32_SFLOAT,
		.offset = 48,
	};

	vpc.desc[0] = vb3->uniform.layout;
	vkhelper2(pipeline_build)(&vb3->ppll_model, &vb3->ppl_model,
		&vpc, vb3->renderpass, device, 0);
	vkhelper2(pipeline_config_deinit)(&vpc, device);
}

void vkbasic3d(pipeline_init)(Vkbasic3d() *vb3, VkDevice device) {
	pipeline_init_grid(vb3, device);
	pipeline_init_model(vb3, device);
}
