#include <assert.h>
#include <string.h>
#include <vulkan/vulkan.h>

#include "../../vkstatic/include/vkstatic.h"
#include "../../modelobj/include/modelobj.h"
#include "../../vkhelper/include/buffer.h"
#include "../include/vkbasic3d.h"
#include "../include/vertex.h"

static size_t chk(int32_t v) {
	assert(v >= 0);
	return (size_t)v;
}

void vkbasic3d_model_upload(
	Vkstatic* vs,
	Vkbasic3d* vb3,
	Modelobj* model
) {
	// TODO: check length limit
	Vkbasic3dVertex* target;
	assert(0 == vkMapMemory(vs->device, vb3->vbuf.smemory, 0,
		vb3->vbuf.ssize, 0, (void**)&target));
	for (size_t idx = 0; idx < model->f_len; idx += 1) {
		ModelobjFace* f = &model->fs[idx];
		for (size_t idy = 0; idy < 3; idy += 1) {
			Vkbasic3dVertex* v = &target[idx * 3 + idy];
			size_t vid = chk(f->vids[idy]);
			printf("%zu\n", vid);
			size_t nid = chk(f->nids[idy]);
			*v = (Vkbasic3dVertex) {
				.color = {0.5, 0.5, 0.5, 1.0},
			};
			memcpy(v->pos, model->vs[vid], 12);
			v->pos[3] = 1.0;
			memcpy(v->normal, model->ns[nid], 12);
		}
	}
	vkUnmapMemory(vs->device, vb3->vbuf.smemory);
	vb3->vlen = (uint32_t)model->f_len * 3;
	vkhelper_buffer_sync(&vb3->vbuf, sizeof(Vkbasic3dVertex) * vb3->vlen,
		vs->device, vs->queue, vs->cpool);
}
