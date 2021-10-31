LINUXINCLUDE    += \
                -I$(M)/include \
                -I$(M)/include/uapi

include $(M)/config/lahaina_GKI.config
export $(shell sed 's/=.*//' $(M)/config/lahaina_GKI.config)

LINUXINCLUDE    += \
                -include $(M)/config/lahaina_GKI.h

obj-y	:= fs/ kernel/ net/ sound/ techpack/
