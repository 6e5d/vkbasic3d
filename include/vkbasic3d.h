#include <vulkan/vulkan.h>

typedef struct {
	VkRenderPass renderpass;
	VkPipeline pipeline;
} Vkbasic3d;

Vkbasic3d* vkbasic3d_new(Vkbasic* vb);
void vkbasic3d_destroy(Vkbasic3d* vb3, VkDevice device);

void vkbasic3d_build_command(
	Vkbasic3d* vb3,
	VkCommandBuffer commandbuffer,
	VkFramebuffer framebuffer,
	uint32_t width,
	uint32_t height
);
