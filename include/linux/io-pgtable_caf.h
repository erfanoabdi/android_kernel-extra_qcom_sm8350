/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __IO_PGTABLE_CAF_H
#define __IO_PGTABLE_CAF_H

#include <linux/io-pgtable.h>
#include <linux/scatterlist.h>

#define to_msm_io_pgtable_info(_cfg) \
	container_of(_cfg, struct msm_io_pgtable_info, pgtbl_cfg)

#define IO_PGTABLE_QUIRK_QCOM_USE_UPSTREAM_HINT	BIT(5)
#define IO_PGTABLE_QUIRK_QCOM_USE_LLC_NWA	BIT(6)

/**
 * struct msm_io_pgtable_info - MSM specific page table manipulation API for
 * IOMMU drivers, and page table configuration.
 *
 * @map_sg:		Map a scatterlist.  Returns the number of bytes mapped,
 *			or -ve val on failure.  The size parameter contains the
 *			size of the partial mapping in case of failure.
 * @is_iova_coherent:	Checks coherency of given IOVA. Returns True if coherent
 *			and False if non-coherent.
 * @iova_to_pte:	Translate iova to Page Table Entry (PTE).
 * @pgtbl_cfg:		The configuration for a set of page tables.
 * @iova_base:		Configured IOVA base
 * @iova_end:		Configured IOVA end
 */
struct msm_io_pgtable_info {
	int (*map_sg)(struct io_pgtable_ops *ops, unsigned long iova,
		      struct scatterlist *sg, unsigned int nents, int prot,
		      size_t *size);
	bool (*is_iova_coherent)(struct io_pgtable_ops *ops,
				 unsigned long iova);
	uint64_t (*iova_to_pte)(struct io_pgtable_ops *ops, unsigned long iova);
	struct io_pgtable_cfg pgtbl_cfg;
	dma_addr_t	iova_base;
	dma_addr_t	iova_end;
};

#endif /* __IO_PGTABLE_CAF_H */
