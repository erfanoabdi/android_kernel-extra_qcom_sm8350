# SPDX-License-Identifier: GPL-2.0

ccflags-y += -I$(srctree)/drivers/staging/android/ion/heaps/

obj-$(CONFIG_ION_MSM_HEAPS) += msm_ion_heaps.o
msm_ion_heaps-objs += msm_ion.o msm_ion_dma_buf.o ion_msm_page_pool.o \
		ion_msm_system_heap.o ion_carveout_heap.o \
		ion_system_secure_heap.o ion_cma_heap.o ion_secure_util.o
