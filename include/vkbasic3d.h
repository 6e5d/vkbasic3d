#include <vulkan/vulkan.h>

typedef struct {
	Vkbasic* vb;
	VkRenderPass renderpass;
	VkPipeline pipeline;
} Vkbasic3d;

Vkbasic3d* vkbasic3d_new(Vkbasic* vb);
void vkbasic3d_destroy(Vkbasic3d* vb3);
void vkbasic3d_swapchain_update(
	Vkbasic3d* vb3,
	uint32_t width,
	uint32_t height
);

void vkbasic3d_build_command(
	Vkbasic3d* vb3,
	uint32_t index,
	uint32_t width,
	uint32_t height
);
