# SPDX-License-Identifier: GPL-2.0
#
# Makefile for misc devices that really don't fit anywhere else.
#

obj-$(CONFIG_HDCP_QSEECOM)	+= hdcp_qseecom.o
obj-$(CONFIG_QSEECOM)		+= qseecom-mod.o
qseecom-mod-y			:= qseecom.o
qseecom-mod-$(CONFIG_COMPAT)	+= compat_qseecom.o
obj-$(CONFIG_PROFILER)			+= profiler.o
obj-$(CONFIG_WIGIG_SENSING_SPI)	+= wigig_sensing.o
