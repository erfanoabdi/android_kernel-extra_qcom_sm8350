# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the linux kernel.
#


obj-$(CONFIG_QCOM_SCM)		+= _qcom_scm.o
_qcom_scm-y			+= qcom_scm.o qcom_scm-smc.o
_qcom_scm-$(CONFIG_QTEE_SHM_BRIDGE)	+= qtee_shmbridge.o

obj-$(CONFIG_ARM_SCMI_PROTOCOL)	+= arm_scmi/
obj-$(CONFIG_QTI_TZ_LOG)	+= qcom/
