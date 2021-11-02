/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DMA_NONCOHERENT_CAF_H
#define _LINUX_DMA_NONCOHERENT_CAF_H 1

#include <linux/dma-noncoherent.h>

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
