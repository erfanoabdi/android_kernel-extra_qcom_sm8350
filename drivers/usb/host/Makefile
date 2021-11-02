# SPDX-License-Identifier: GPL-2.0
#
# Makefile for USB Host Controller Drivers
#

ccflags-y += -I$(srctree)/drivers/usb/host/

# tell define_trace.h where to find the xhci trace header
CFLAGS_xhci-trace.o := -I$(src)

xhci-hcd-y := xhci.o xhci-mem.o xhci-ext-caps.o
xhci-hcd-y += xhci-ring.o xhci-hub.o xhci-dbg.o
xhci-hcd-y += xhci-trace.o

ifneq ($(CONFIG_USB_XHCI_DBGCAP), )
	xhci-hcd-y += xhci-dbgcap.o xhci-dbgtty.o
endif

ifneq ($(CONFIG_USB_XHCI_MTK), )
	xhci-hcd-y += xhci-mtk-sch.o
endif

xhci-plat-hcd-y := xhci-plat.o
ifneq ($(CONFIG_USB_XHCI_MVEBU), )
	xhci-plat-hcd-y		+= xhci-mvebu.o
endif
ifneq ($(CONFIG_USB_XHCI_RCAR), )
	xhci-plat-hcd-y		+= xhci-rcar.o
endif

ifneq ($(CONFIG_DEBUG_FS),)
	xhci-hcd-y		+= xhci-debugfs.o
endif
obj-$(CONFIG_USB_XHCI_HCD)	+= xhci-hcd.o
obj-$(CONFIG_USB_XHCI_PCI_RENESAS)	+= xhci-pci-renesas.o
obj-$(CONFIG_USB_XHCI_PCI)	+= xhci-pci.o
obj-$(CONFIG_USB_XHCI_PLATFORM) += xhci-plat-hcd.o
obj-$(CONFIG_USB_XHCI_HISTB)	+= xhci-histb.o
obj-$(CONFIG_USB_XHCI_MTK)	+= xhci-mtk.o
obj-$(CONFIG_USB_XHCI_TEGRA)	+= xhci-tegra.o
