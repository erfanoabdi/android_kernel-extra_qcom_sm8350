/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DMA_NONCOHERENT_CAF_H
#define _LINUX_DMA_NONCOHERENT_CAF_H 1

#include <linux/version.h>
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5, 4, 0))
#include <linux/dma-noncoherent.h>
#else
#include <linux/dma-map-ops.h>
#endif

#ifdef CONFIG_DMA_COHERENT_HINT_CACHED
static inline bool dev_is_dma_coherent_hint_cached(struct device *dev)
{
	return dev->dma_coherent_hint_cached;
}
#else
static inline bool dev_is_dma_coherent_hint_cached(struct device *dev)
{
	return false;
}
#endif

#endif /* _LINUX_DMA_NONCOHERENT_CAF_H */
