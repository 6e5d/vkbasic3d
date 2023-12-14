#include <vulkan/vulkan.h>

#include "../../vkhelper2/include/vkhelper2.h"
#include "../include/camera.h"
#include "../include/pipeline.h"
#include "../include/vertex.h"
#include "../include/vkbasic3d.h"

static void vkbasic3d_pipeline_init_grid(Vkbasic3d *vb3, VkDevice device) {
	Vkhelper2PipelineConfig vpc = {0};
	vkhelper2_pipeline_config(&vpc, 0, 0, 1);
	vkhelper2_pipeline_simple_shader(&vpc, device,
		__FILE__, "../../vkbasic3d_shader/build/grid");
	vpc.desc[0] = vb3->uniform.layout;
	vkhelper2_pipeline_build(&vb3->ppll_grid, &vb3->ppl_grid,
		&vpc, vb3->renderpass, device, 0);
	vkhelper2_pipeline_config_deinit(&vpc, device);
}

static void vkbasic3d_pipeline_init_model(Vkbasic3d *vb3, VkDevice device) {
	Vkhelper2PipelineConfig vpc = {0};
	vkhelper2_pipeline_config(&vpc, 1, 5, 1);
	vkhelper2_pipeline_simple_shader(&vpc, device,
		__FILE__, "../../vkbasic3d_shader/build/model");
	vpc.vib[0] = (VkVertexInputBindingDescription) {
		.binding = 0,
		.stride = sizeof(Vkbasic3dVertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
	};
	typedef VkVertexInputAttributeDescription Via;
	vpc.via[0] = (Via) {
		.binding = 0,
		.location = 0,
		.format = VK_FORMAT_R32G32B32A32_SFLOAT,
		.offset = offsetof(Vkbasic3dVertex, pos),
	};
	vpc.via[1] = (Via) {
		.binding = 0,
		.location = 1,
		.format = VK_FORMAT_R32G32B32A32_SFLOAT,
		.offset = offsetof(Vkbasic3dVertex, color),
	};
	vpc.via[2] = (Via) {
		.binding = 0,
		.location = 2,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = offsetof(Vkbasic3dVertex, normal),
	};
	vpc.via[3] = (Via) {
		.binding = 0,
		.location = 3,
		.format = VK_FORMAT_R32_SINT,
		.offset = offsetof(Vkbasic3dVertex, layer),
	};
	vpc.via[4] = (Via) {
		.binding = 0,
		.location = 4,
		.format = VK_FORMAT_R32G32_SFLOAT,
		.offset = offsetof(Vkbasic3dVertex, uv),
	};

	vpc.desc[0] = vb3->uniform.layout;
	vkhelper2_pipeline_build(&vb3->ppll_model, &vb3->ppl_model,
		&vpc, vb3->renderpass, device, 0);
	vkhelper2_pipeline_config_deinit(&vpc, device);
}

void vkbasic3d_pipeline_init(Vkbasic3d *vb3, VkDevice device) {
	vkbasic3d_pipeline_init_grid(vb3, device);
	vkbasic3d_pipeline_init_model(vb3, device);
}
