# SPDX-License-Identifier: GPL-2.0
#
# Makefile for MMC/SD host controller drivers
#

ccflags-y += -I$(srctree)/drivers/mmc/

obj-$(CONFIG_MMC_SDHCI_MSM)		+= sdhci-msm.o
obj-$(CONFIG_MMC_CQHCI)			+= cqhci.o
cqhci-y					+= cqhci-core.o
cqhci-$(CONFIG_MMC_CRYPTO)		+= cqhci-crypto.o
obj-$(CONFIG_MMC_CQHCI_CRYPTO_QTI)	+= cqhci-crypto-qti.o
