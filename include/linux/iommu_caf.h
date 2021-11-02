/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2007-2008 Advanced Micro Devices, Inc.
 * Author: Joerg Roedel <joerg.roedel@amd.com>
 */

#ifndef __LINUX_IOMMU_CAF_H
#define __LINUX_IOMMU_CAF_H

#include <linux/iommu.h>
/* Use upstream device's bus attribute */
#define IOMMU_USE_UPSTREAM_HINT	(1 << 7)

/* Use upstream device's bus attribute with no write-allocate cache policy */
#define IOMMU_USE_LLC_NWA	(1 << 8)

/* iommu fault flags */
#define IOMMU_FAULT_TRANSLATION         (1 << 2)
#define IOMMU_FAULT_PERMISSION          (1 << 3)
#define IOMMU_FAULT_EXTERNAL            (1 << 4)
#define IOMMU_FAULT_TRANSACTION_STALLED (1 << 5)

struct iommu_fault_ids {
	u32 bid;
	u32 pid;
	u32 mid;
};

/* iommu transaction flags */
#define IOMMU_TRANS_WRITE	BIT(0)	/* 1 Write, 0 Read */
#define IOMMU_TRANS_PRIV	BIT(1)	/* 1 Privileged, 0 Unprivileged */
#define IOMMU_TRANS_INST	BIT(2)	/* 1 Instruction fetch, 0 Data access */
#define IOMMU_TRANS_SEC	BIT(3)	/* 1 Secure, 0 Non-secure access*/

/* Non secure unprivileged Data read operation */
#define IOMMU_TRANS_DEFAULT	(0U)

struct iommu_pgtbl_info {
	void *ops;
};

#define to_msm_iommu_ops(_iommu_ops) \
	container_of(_iommu_ops, struct msm_iommu_ops, iommu_ops)
#define to_msm_iommu_domain(_iommu_domain) \
	container_of(_iommu_domain, struct msm_iommu_domain, iommu_domain)

#define IOMMU_DOMAIN_NAME_LEN 32

struct msm_iommu_domain {
	char name[IOMMU_DOMAIN_NAME_LEN];
	bool is_debug_domain;
	struct iommu_domain iommu_domain;
};

#define EXTENDED_ATTR_BASE			(DOMAIN_ATTR_MAX + 16)

#define DOMAIN_ATTR_PT_BASE_ADDR		(EXTENDED_ATTR_BASE + 0)
#define DOMAIN_ATTR_CONTEXT_BANK		(EXTENDED_ATTR_BASE + 1)
#define DOMAIN_ATTR_DYNAMIC			(EXTENDED_ATTR_BASE + 2)
#define DOMAIN_ATTR_TTBR0			(EXTENDED_ATTR_BASE + 3)
#define DOMAIN_ATTR_CONTEXTIDR			(EXTENDED_ATTR_BASE + 4)
#define DOMAIN_ATTR_PROCID			(EXTENDED_ATTR_BASE + 5)
#define DOMAIN_ATTR_NON_FATAL_FAULTS		(EXTENDED_ATTR_BASE + 6)
#define DOMAIN_ATTR_S1_BYPASS			(EXTENDED_ATTR_BASE + 7)
#define DOMAIN_ATTR_ATOMIC			(EXTENDED_ATTR_BASE + 8)
#define DOMAIN_ATTR_SECURE_VMID			(EXTENDED_ATTR_BASE + 9)
#define DOMAIN_ATTR_FAST			(EXTENDED_ATTR_BASE + 10)
#define DOMAIN_ATTR_PGTBL_INFO			(EXTENDED_ATTR_BASE + 11)
#define DOMAIN_ATTR_USE_UPSTREAM_HINT		(EXTENDED_ATTR_BASE + 12)
#define DOMAIN_ATTR_EARLY_MAP			(EXTENDED_ATTR_BASE + 13)
#define DOMAIN_ATTR_PAGE_TABLE_IS_COHERENT	(EXTENDED_ATTR_BASE + 14)
#define DOMAIN_ATTR_PAGE_TABLE_FORCE_COHERENT	(EXTENDED_ATTR_BASE + 15)
#define DOMAIN_ATTR_USE_LLC_NWA			(EXTENDED_ATTR_BASE + 16)
#define DOMAIN_ATTR_SPLIT_TABLES		(EXTENDED_ATTR_BASE + 17)
#define DOMAIN_ATTR_FAULT_MODEL_NO_CFRE		(EXTENDED_ATTR_BASE + 18)
#define DOMAIN_ATTR_FAULT_MODEL_NO_STALL	(EXTENDED_ATTR_BASE + 19)
#define DOMAIN_ATTR_FAULT_MODEL_HUPCF		(EXTENDED_ATTR_BASE + 20)
#define DOMAIN_ATTR_EXTENDED_MAX		(EXTENDED_ATTR_BASE + 21)

#ifdef CONFIG_IOMMU_API
/**
 * struct msm_iommu_ops - standard iommu ops, as well as additional MSM
 * specific iommu ops
 * @map_sg: map a scatter-gather list of physically contiguous memory chunks
 *          to an iommu domain
 * @iova_to_phys_hard: translate iova to physical address using IOMMU hardware
 * @is_iova_coherent: checks coherency of the given iova
 * @tlbi_domain: Invalidate all TLBs covering an iommu domain
 * @iova_to_pte: translate iova to Page Table Entry (PTE).
 * @iommu_ops: the standard iommu ops
 */
struct msm_iommu_ops {
	size_t (*map_sg)(struct iommu_domain *domain, unsigned long iova,
			 struct scatterlist *sg, unsigned int nents, int prot);
	phys_addr_t (*iova_to_phys_hard)(struct iommu_domain *domain,
					 dma_addr_t iova,
					 unsigned long trans_flags);
	bool (*is_iova_coherent)(struct iommu_domain *domain, dma_addr_t iova);
	void (*tlbi_domain)(struct iommu_domain *domain);
	uint64_t (*iova_to_pte)(struct iommu_domain *domain, dma_addr_t iova);
	struct iommu_ops iommu_ops;
};

extern size_t iommu_pgsize(unsigned long pgsize_bitmap,
			   unsigned long addr_merge, size_t size);
extern size_t default_iommu_map_sg(struct iommu_domain *domain,
				   unsigned long iova, struct scatterlist *sg,
				   unsigned int nents, int prot);
extern phys_addr_t iommu_iova_to_phys_hard(struct iommu_domain *domain,
				   dma_addr_t iova, unsigned long trans_flags);
extern bool iommu_is_iova_coherent(struct iommu_domain *domain,
				dma_addr_t iova);
extern int iommu_get_fault_ids(struct iommu_domain *domain,
				struct iommu_fault_ids *f_ids);
extern uint64_t iommu_iova_to_pte(struct iommu_domain *domain,
	    dma_addr_t iova);

static inline void iommu_tlbiall(struct iommu_domain *domain)
{
	struct msm_iommu_ops *ops = to_msm_iommu_ops(domain->ops);

	if (ops->tlbi_domain)
		ops->tlbi_domain(domain);
}

#else /* CONFIG_IOMMU_API */

static inline phys_addr_t iommu_iova_to_phys_hard(struct iommu_domain *domain,
				dma_addr_t iova, unsigned long trans_flags)
{
	return 0;
}

static inline bool iommu_is_iova_coherent(struct iommu_domain *domain,
					  dma_addr_t iova)
{
	return false;
}

static inline int iommu_get_fault_ids(struct iommu_domain *domain,
				struct iommu_fault_ids *f_ids)
{
	return -EINVAL;
}

static inline void iommu_tlbiall(struct iommu_domain *domain)
{
}

#endif /* CONFIG_IOMMU_API */

#endif /* __LINUX_IOMMU_CAF_H */
