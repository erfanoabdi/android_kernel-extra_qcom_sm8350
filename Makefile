LINUXINCLUDE    += \
                -I$(M)/include \
                -I$(M)/include/uapi

obj-y	:= fs/ kernel/ net/ sound/ techpack/
