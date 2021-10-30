# SPDX-License-Identifier: GPL-2.0
#
# Makefile for cpuidle.
#

ifeq ($(CONFIG_MSM_PM_LEGACY), y)
obj-y += lpm-levels-legacy.o lpm-levels-of-legacy.o
else
obj-$(CONFIG_ARM_QCOM_LPM_CPUIDLE)	+= lpm-levels.o
obj-$(CONFIG_ARM_QCOM_LPM_CPUIDLE)	+= lpm-levels-of.o
endif
