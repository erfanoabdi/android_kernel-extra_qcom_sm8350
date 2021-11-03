// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/usb/gadget.h>
 *
 * We call the USB code inside a Linux-based peripheral device a "gadget"
 * driver, except for the hardware-specific bus glue.  One USB host can
 * master many USB gadgets, but the gadgets are only slaved to one host.
 *
 *
 * (C) Copyright 2002-2004 by David Brownell
 * All Rights Reserved.
 *
 * This software is licensed under the GNU GPL version 2.
 */

#ifndef __LINUX_USB_GADGET_CAF_H
#define __LINUX_USB_GADGET_CAF_H

#include <linux/usb/gadget.h>
#include <linux/pm_runtime.h>

static inline int usb_gadget_func_wakeup(struct usb_gadget *gadget,
		int interface_id)
{ return 0; }

/**
 * usb_gadget_autopm_get - increment PM-usage counter of usb gadget's parent
 * device.
 * @gadget: usb gadget whose parent device counter is incremented
 *
 * This routine should be called by function driver when it wants to use
 * gadget's parent device and needs to guarantee that it is not suspended. In
 * addition, the routine prevents subsequent autosuspends of gadget's parent
 * device. However if the autoresume fails then the counter is re-decremented.
 *
 * This routine can run only in process context.
 */
static inline int usb_gadget_autopm_get(struct usb_gadget *gadget)
{
	int status = -ENODEV;

	if (!gadget || !gadget->dev.parent)
		return status;

	status = pm_runtime_get_sync(gadget->dev.parent);
	if (status < 0)
		pm_runtime_put_sync(gadget->dev.parent);

	if (status > 0)
		status = 0;
	return status;
}

/**
 * usb_gadget_autopm_get_async - increment PM-usage counter of usb gadget's
 * parent device.
 * @gadget: usb gadget whose parent device counter is incremented
 *
 * This routine increments @gadget parent device PM usage counter and queue an
 * autoresume request if the device is suspended. It does not autoresume device
 * directly (it only queues a request). After a successful call, the device may
 * not yet be resumed.
 *
 * This routine can run in atomic context.
 */
static inline int usb_gadget_autopm_get_async(struct usb_gadget *gadget)
{
	int status = -ENODEV;

	if (!gadget || !gadget->dev.parent)
		return status;

	status = pm_runtime_get(gadget->dev.parent);
	if (status < 0 && status != -EINPROGRESS)
		pm_runtime_put_noidle(gadget->dev.parent);

	if (status > 0 || status == -EINPROGRESS)
		status = 0;
	return status;
}

/**
 * usb_gadget_autopm_get_noresume - increment PM-usage counter of usb gadget's
 * parent device.
 * @gadget: usb gadget whose parent device counter is incremented
 *
 * This routine increments PM-usage count of @gadget parent device but does not
 * carry out an autoresume.
 *
 * This routine can run in atomic context.
 */
static inline void usb_gadget_autopm_get_noresume(struct usb_gadget *gadget)
{
	if (gadget && gadget->dev.parent)
		pm_runtime_get_noresume(gadget->dev.parent);
}

/**
 * usb_gadget_autopm_put - decrement PM-usage counter of usb gadget's parent
 * device.
 * @gadget: usb gadget whose parent device counter is decremented.
 *
 * This routine should be called by function driver when it is finished using
 * @gadget parent device and wants to allow it to autosuspend. It decrements
 * PM-usage counter of @gadget parent device, when the counter reaches 0, a
 * delayed autosuspend request is attempted.
 *
 * This routine can run only in process context.
 */
static inline void usb_gadget_autopm_put(struct usb_gadget *gadget)
{
	if (gadget && gadget->dev.parent)
		pm_runtime_put_sync(gadget->dev.parent);
}

/**
 * usb_gadget_autopm_put_async - decrement PM-usage counter of usb gadget's
 * parent device.
 * @gadget: usb gadget whose parent device counter is decremented.
 *
 * This routine decrements PM-usage counter of @gadget parent device and
 * schedules a delayed autosuspend request if the counter is <= 0.
 *
 * This routine can run in atomic context.
 */
static inline void usb_gadget_autopm_put_async(struct usb_gadget *gadget)
{
	if (gadget && gadget->dev.parent)
		pm_runtime_put(gadget->dev.parent);
}

/**
 * usb_gadget_autopm_put_no_suspend - decrement PM-usage counter of usb gadget
's
 * parent device.
 * @gadget: usb gadget whose parent device counter is decremented.
 *
 * This routine decrements PM-usage counter of @gadget parent device but does
 * not carry out an autosuspend.
 *
 * This routine can run in atomic context.
 */
static inline void usb_gadget_autopm_put_no_suspend(struct usb_gadget *gadget)
{
	if (gadget && gadget->dev.parent)
		pm_runtime_put_noidle(gadget->dev.parent);
}

static inline int usb_func_wakeup(struct usb_function *func)
{
	return -EOPNOTSUPP;
}

#endif /* __LINUX_USB_GADGET_CAF_H */
