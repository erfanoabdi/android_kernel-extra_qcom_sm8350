# SPDX-License-Identifier: GPL-2.0-only
obj-$(CONFIG_QRTR) := qrtr.o ns.o

obj-$(CONFIG_QRTR_SMD) += qrtr-smd.o
qrtr-smd-y	:= smd.o
obj-$(CONFIG_QRTR_TUN) += qrtr-tun.o
qrtr-tun-y	:= tun.o
obj-$(CONFIG_QRTR_MHI) += qrtr-mhi.o
qrtr-mhi-y	:= mhi.o
obj-$(CONFIG_QRTR_MHI_DEV) += qrtr-mhi-dev.o
qrtr-mhi-dev-y	:= mhi_dev.o
obj-$(CONFIG_QRTR_HAVEN) += qrtr-haven.o
qrtr-haven-y	:= haven.o

CONFIG_QRTR_NODE_ID?=1
CONFIG_QRTR_WAKEUP_MS?=0

ccflags-y += -DCONFIG_QRTR_NODE_ID=$(CONFIG_QRTR_NODE_ID)
ccflags-y += -DCONFIG_QRTR_WAKEUP_MS=$(CONFIG_QRTR_WAKEUP_MS)
