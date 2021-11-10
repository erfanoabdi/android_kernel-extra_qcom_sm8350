/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2016-2019, The Linux Foundation. All rights reserved. */

#ifndef __SOC_QCOM_RPMH_CAF_H__
#define __SOC_QCOM_RPMH_CAF_H__

#include <soc/qcom/rpmh.h>

#if IS_ENABLED(CONFIG_QCOM_RPMH)
int rpmh_mode_solver_set(const struct device *dev, bool enable);

int rpmh_ctrlr_idle(const struct device *dev);

int rpmh_write_pdc_data(const struct device *dev,
			const struct tcs_cmd *cmd, u32 n);

#else
static inline int rpmh_mode_solver_set(const struct device *dev, bool enable)
{ return -ENODEV; }

static inline int rpmh_ctrlr_idle(const struct device *dev)
{ return -ENODEV; }

static inline int rpmh_write_pdc_data(const struct device *dev,
				      const struct tcs_cmd *cmd, u32 n)
{ return -ENODEV; }
#endif /* CONFIG_QCOM_RPMH */

#endif /* __SOC_QCOM_RPMH_CAF_H__ */
