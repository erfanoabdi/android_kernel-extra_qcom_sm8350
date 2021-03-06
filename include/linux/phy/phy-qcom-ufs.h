/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2013-2019, Linux Foundation. All rights reserved.
 */

#ifndef PHY_QCOM_UFS_H_
#define PHY_QCOM_UFS_H_

#include <linux/phy/phy.h>

void ufs_qcom_phy_ctrl_rx_linecfg(struct phy *generic_phy, bool ctrl);
void ufs_qcom_phy_set_tx_lane_enable(struct phy *generic_phy, u32 tx_lanes);
void ufs_qcom_phy_dbg_register_dump(struct phy *generic_phy);
void ufs_qcom_phy_save_controller_version(struct phy *generic_phy,
					  u8 major, u16 minor, u16 step);

#endif /* PHY_QCOM_UFS_H_ */

