#include <vulkan/vulkan.h>
#include <cglm/cglm.h>

#include "../../vkhelper2/build/vkhelper2.h"
#include "../../vkstatic/build/vkstatic.h"
#include "../../modelobj/build/modelobj.h"

const static size_t vkbasic3d(max_vertex) = 100000;

void vkbasic3d(pipeline_init)(Vkbasic3d() *vb3, VkDevice device);

typedef struct {
	float pos[4];
	float color[4];
	float normal[3];
	int32_t layer;
	float uv[2];
} Vkbasic3d(Vertex);

typedef struct {
	mat4 view;
	mat4 proj;
	vec3 direction;
} Vkbasic3d(Camera);

typedef struct {
	VkRenderPass renderpass;
	VkPipeline ppl_model;
	VkPipelineLayout ppll_model;
	VkPipeline ppl_grid;
	VkPipelineLayout ppll_grid;
	Vkhelper2(Buffer) vbufc;
	Vkhelper2(Buffer) vbufg;
	uint32_t vlen;
	bool vertex_update;
	Vkhelper2(Desc) uniform;
	Vkhelper2(Buffer) ubufg;
	Vkhelper2(Buffer) ubufc;
	Vkbasic3d(Camera)* camera; // mapped, no free
} Vkbasic3d();
void vkbasic3d(init)(Vkbasic3d()* vb3, Vkstatic()* vks);
void vkbasic3d(deinit)(Vkbasic3d()* vb3, VkDevice device);
void vkbasic3d(build_command)(
	Vkbasic3d()* vb3,
	Vkstatic()* vks,
	VkCommandBuffer commandbuffer,
	VkFramebuffer framebuffer,
	uint32_t width,
	uint32_t height
);

void vkbasic3d(model_upload)(
	Vkstatic()* vs,
	Vkbasic3d()* vb3,
	Modelobj()* model
);
