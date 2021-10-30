# SPDX-License-Identifier: GPL-2.0
#
# Makefile for ALSA
#

LINUXINCLUDE    += \
                -I$(srctree)/sound/usb

snd-usb-audio-qmi-objs := usb_audio_qmi_v01.o usb_audio_qmi_svc.o
obj-$(CONFIG_SND_USB_AUDIO_QMI) += snd-usb-audio-qmi.o
