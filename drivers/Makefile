drivers-dirs := $(shell find $(M)/ -maxdepth 1 -mindepth 1 -type d -not -name ".*")
obj-y += $(addsuffix /,$(subst $(M)/,,$(drivers-dirs)))
