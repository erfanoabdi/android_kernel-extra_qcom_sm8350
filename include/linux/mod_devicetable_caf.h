/* SPDX-License-Identifier: GPL-2.0 */

#ifndef LINUX_MOD_DEVICETABLE_CAF_H
#define LINUX_MOD_DEVICETABLE_CAF_H

#include <linux/mod_devicetable.h>
#include <linux/version.h>

/* SLIMbus */
struct slim_device_id_caf {
	char name[SLIMBUS_NAME_SIZE];

	/* Data private to the driver */
	kernel_ulong_t driver_data;
};

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5, 4, 0))
#define MHI_NAME_SIZE 32

/**
 * struct mhi_device_id - MHI device identification
 * @chan: MHI channel name
 * @driver_data: driver data;
 */

struct mhi_device_id {
	const char chan[MHI_NAME_SIZE];
	kernel_ulong_t driver_data;
};
#endif

#endif /* LINUX_MOD_DEVICETABLE_CAF_H */
