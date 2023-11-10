#include <assert.h>
#include <vulkan/vulkan.h>

#include "../../vkstatic/include/vkstatic.h"
#include "../../modelobj/include/modelobj.h"
#include "../../vkhelper/include/buffer.h"
#include "../include/vkbasic3d.h"
#include "../include/vertex.h"

void vkbasic3d_model_upload(
	Vkstatic* vs,
	Vkbasic3d* vb3,
	Modelobj* model
) {
	// TODO: check length limit
	{
		Vkbasic3dVertex* target;
		vkhelper_buffer_map(vs->device, (void**)&target, &vb3->vbuf);
		for (size_t idx = 0; idx < model->v_len; idx += 1) {
			target[idx] = (Vkbasic3dVertex) {
				.color = {0.5, 1.0, 1.0, 1.0},
			};
			for (size_t idy = 0; idy < 3; idy += 1) { // FIXME
				target[idx].pos[idy] = model->vs[idx][idy];
			}
			target[idx].pos[3] = 1.0;
		}
		vkhelper_buffer_unmap(vs->device, &vb3->vbuf);
	}
	{
		uint32_t* target;
		vkhelper_buffer_map(vs->device, (void**)&target, &vb3->ibuf);
		for (size_t idx = 0; idx < model->f_len; idx += 1) {
			for (size_t idy = 0; idy < 3; idy += 1) {
				assert(0 <= model->fs[idx].vids[idy]);
				target[idx * 3 + idy] =
					(uint32_t)model->fs[idx].vids[idy];
			}
		}
		vb3->index_len = (uint32_t)model->f_len * 3;
		vkhelper_buffer_unmap(vs->device, &vb3->ibuf);
	}

	vkhelper_buffer_sync(&vb3->vbuf, sizeof(Vkbasic3dVertex) * model->v_len,
		vs->device, vs->queue, vs->cpool);
	vkhelper_buffer_sync(&vb3->ibuf, sizeof(uint32_t) * vb3->index_len,
		vs->device, vs->queue, vs->cpool);
}
