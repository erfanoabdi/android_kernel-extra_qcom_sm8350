# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the kernel character device drivers.
#

obj-$(CONFIG_HW_RANDOM)		+= hw_random/
obj-$(CONFIG_MSM_SMD_PKT)       += msm_smd_pkt.o
frpc-adsprpc-objs		+= adsprpc.o
ifdef CONFIG_COMPAT
  frpc-adsprpc-objs		+= adsprpc_compat.o
endif
obj-$(CONFIG_MSM_ADSPRPC)	+= frpc-adsprpc.o
vfastrpc-objs			+= virtio_fastrpc.o
ifdef CONFIG_COMPAT
  vfastrpc-objs			+= adsprpc_compat.o
endif
obj-$(CONFIG_VIRTIO_FASTRPC)	+= vfastrpc.o
obj-$(CONFIG_MSM_RDBG)		+= rdbg.o
obj-$(CONFIG_MSM_FASTCVPD)	+= fastcvpd.o
obj-$(CONFIG_VIRTIO_EAVB)       += virtio_eavb/
