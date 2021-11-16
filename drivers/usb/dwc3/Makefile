# SPDX-License-Identifier: GPL-2.0

ccflags-y += -I$(srctree)/drivers/usb/dwc3/

CFLAGS_dwc3-msm-core.o                       := -I$(srctree)/drivers/usb/host
obj-$(CONFIG_USB_DWC3_MSM)		+= dwc3-msm.o
dwc3-msm-y				:= dwc3-msm-core.o debug_ipc.o
ifneq ($(CONFIG_ARM64),)
dwc3-msm-y				+= dwc3-msm-ops.o
endif
