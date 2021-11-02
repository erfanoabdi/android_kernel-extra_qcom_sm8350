/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Header file for dma buffer sharing framework.
 *
 * Copyright(C) 2011 Linaro Limited. All rights reserved.
 * Author: Sumit Semwal <sumit.semwal@ti.com>
 *
 * Many thanks to linaro-mm-sig list, and specially
 * Arnd Bergmann <arnd@arndb.de>, Rob Clark <rob@ti.com> and
 * Daniel Vetter <daniel@ffwll.ch> for their support in creation and
 * refining of this idea.
 */
#ifndef __DMA_BUF_CAF_H__
#define __DMA_BUF_CAF_H__

#include <linux/dma-buf-ref.h>
#include <linux/dma-buf.h>

/**
 * dma_buf_destructor - dma-buf destructor function
 * @dmabuf:	[in]	pointer to dma-buf
 * @dtor_data:	[in]	destructor data associated with this buffer
 *
 * The dma-buf destructor which is called when the dma-buf is freed.
 *
 * If the destructor returns an error the dma-buf's exporter release function
 * won't be called.
 */
typedef int (*dma_buf_destructor)(struct dma_buf *dmabuf, void *dtor_data);

/**
 * struct msm_dma_buf - Holds the meta data associated with a shared buffer
 * object, as well as the buffer object.
 * @refs: list entry for dma-buf reference tracking
 * @i_ino: inode number
 * @dma_buf: the shared buffer object
 */
struct msm_dma_buf {
	struct list_head refs;
	unsigned long i_ino;
	struct dma_buf dma_buf;
};

/**
 * to_msm_dma_buf - helper macro for deriving an msm_dma_buf from a dma_buf.
 */
#define to_msm_dma_buf(_dma_buf) \
	container_of(_dma_buf, struct msm_dma_buf, dma_buf)

void dma_buf_put_sync(struct dma_buf *dmabuf);

#ifdef CONFIG_DMABUF_DESTRUCTOR_SUPPORT
/**
 * dma_buf_set_destructor - set the dma-buf's destructor
 * @dmabuf:		[in]	pointer to dma-buf
 * @dma_buf_destructor	[in]	the destructor function
 * @dtor_data:		[in]	destructor data associated with this buffer
 */
static inline int dma_buf_set_destructor(struct dma_buf *dmabuf,
					 dma_buf_destructor dtor,
					 void *dtor_data)
{
	dmabuf->dtor = dtor;
	dmabuf->dtor_data = dtor_data;
	return 0;
}
#else
static inline int dma_buf_set_destructor(struct dma_buf *dmabuf,
					 dma_buf_destructor dtor,
					 void *dtor_data)
{
	return -ENOTSUPP;
}
#endif

#endif /* __DMA_BUF_CAF_H__ */
