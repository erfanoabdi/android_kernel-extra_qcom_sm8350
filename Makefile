LINUXINCLUDE    += \
                -I$(M)/include \
                -I$(M)/include/uapi \
                -I$(M)/arch/arm64/include

include $(M)/config/lahaina_GKI.config
export $(shell sed 's/=.*//' $(M)/config/lahaina_GKI.config)

LINUXINCLUDE    += \
                -include $(M)/config/lahaina_GKI.h

obj-y	:= drivers/ fs/ kernel/ net/ sound/ techpack/
