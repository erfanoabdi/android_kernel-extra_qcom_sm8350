// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2012-2014, 2019 The Linux Foundation. All rights reserved.
 * Copyright (c) 2017, Linaro Ltd.
 */
#ifndef __QMI_HELPERS_CAF_H__
#define __QMI_HELPERS_CAF_H__

#include <linux/soc/qcom/qmi.h>

#define QMI_ERR_NETWORK_NOT_READY_V01		53
#define QMI_ERR_DISABLED_V01			69

void qmi_set_sndtimeo(struct qmi_handle *qmi, long timeo);

#endif
