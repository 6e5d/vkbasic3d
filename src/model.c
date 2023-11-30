#include <vulkan/vulkan.h>

#include "../../modelobj/include/modelobj.h"
#include "../../vkhelper2/include/vkhelper2.h"
#include "../../vkstatic/include/vkstatic.h"
#include "../include/vertex.h"
#include "../include/vkbasic3d.h"

static size_t chk(int32_t v) {
	assert(v >= 0);
	return (size_t)v;
}

void vkbasic3d_model_upload(
	Vkstatic* vs,
	Vkbasic3d* vb3,
	Modelobj* model
) {
	assert(model->v_len <= VKBASIC3D_MAX_VERTEX);
	Vkbasic3dVertex* target;
	assert(0 == vkMapMemory(vs->device, vb3->vbufc.memory, 0,
		vb3->vbufc.size, 0, (void**)&target));
	for (size_t idx = 0; idx < model->f_len; idx += 1) {
		ModelobjFace* f = &model->fs[idx];
		for (size_t idy = 0; idy < 3; idy += 1) {
			Vkbasic3dVertex* v = &target[idx * 3 + idy];
			size_t vid = chk(f->vids[idy]);
			size_t nid = chk(f->nids[idy]);
			*v = (Vkbasic3dVertex) {
				.color = {0.8f, 0.8f, 0.8f, 1.0f},
			};
			memcpy(v->pos, model->vs[vid], 12);
			v->pos[3] = 1.0f;
			memcpy(v->normal, model->ns[nid], 12);
		}
	}
	vkUnmapMemory(vs->device, vb3->vbufc.memory);
	vb3->vlen = (uint32_t)model->f_len * 3;
	vb3->vertex_update = true;
}
