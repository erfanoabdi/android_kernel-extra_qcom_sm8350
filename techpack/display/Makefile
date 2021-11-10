# SPDX-License-Identifier: GPL-2.0-only

# auto-detect subdirs
ifeq ($(CONFIG_ARCH_KONA), y)
include $(M)/techpack/display/config/konadisp.conf
endif

ifeq ($(CONFIG_ARCH_KONA), y)
LINUXINCLUDE    += -include $(M)/techpack/display/config/konadispconf.h
endif

ifeq ($(CONFIG_ARCH_LAHAINA), y)
     ifeq ($(CONFIG_QGKI), y)
		include $(M)/techpack/display/config/lahainadisp.conf
LINUXINCLUDE    += -include $(M)/techpack/display/config/lahainadispconf.h
     else
		include $(M)/techpack/display/config/gki_lahainadisp.conf
LINUXINCLUDE    += -include $(M)/techpack/display/config/gki_lahainadispconf.h
     endif
endif

ifeq ($(CONFIG_ARCH_HOLI), y)
     ifeq ($(CONFIG_QGKI), y)
		include $(M)/techpack/display/config/holidisp.conf
LINUXINCLUDE    += -include $(M)/techpack/display/config/holidispconf.h
     else
		include $(M)/techpack/display/config/gki_holidisp.conf
LINUXINCLUDE    += -include $(M)/techpack/display/config/gki_holidispconf.h
     endif
endif

LINUXINCLUDE    += \
		   -I$(M)/techpack/display/include/uapi/display \
		   -I$(M)/techpack/display/include
USERINCLUDE     += -I$(M)/techpack/display/include/uapi/display

ifeq ($(CONFIG_ARCH_LITO), y)
include $(M)/techpack/display/config/saipdisp.conf
endif

ifeq ($(CONFIG_ARCH_LITO), y)
LINUXINCLUDE    += -include $(M)/techpack/display/config/saipdispconf.h
endif

ifeq ($(CONFIG_ARCH_BENGAL), y)
include $(M)/techpack/display/config/bengaldisp.conf
endif

ifeq ($(CONFIG_ARCH_BENGAL), y)
LINUXINCLUDE    += -include $(M)/techpack/display/config/bengaldispconf.h
endif

obj-$(CONFIG_DRM_MSM) += msm/
