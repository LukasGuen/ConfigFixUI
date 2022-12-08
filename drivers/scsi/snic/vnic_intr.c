// SPDX-License-Identifier: GPL-2.0-only
// Copyright 2014 Cisco Systems, Inc.  All rights reserved.

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include "vnic_dev.h"
#include "vnic_intr.h"

void svnic_intr_free(struct vnic_intr *intr)
{
	intr->ctrl = NULL;
}

int svnic_intr_alloc(struct vnic_dev *vdev, struct vnic_intr *intr,
	unsigned int index)
{
	intr->index = index;
	intr->vdev = vdev;

	intr->ctrl = svnic_dev_get_res(vdev, RES_TYPE_INTR_CTRL, index);
	if (!intr->ctrl) {
		pr_err("Failed to hook INTR[%d].ctrl resource\n",
			index);
		return -EINVAL;
	}

	return 0;
}

void svnic_intr_init(struct vnic_intr *intr, unsigned int coalescing_timer,
	unsigned int coalescing_type, unsigned int mask_on_assertion)
{
	iowrite32(coalescing_timer, &intr->ctrl->coalescing_timer);
	iowrite32(coalescing_type, &intr->ctrl->coalescing_type);
	iowrite32(mask_on_assertion, &intr->ctrl->mask_on_assertion);
	iowrite32(0, &intr->ctrl->int_credits);
}

void svnic_intr_clean(struct vnic_intr *intr)
{
	iowrite32(0, &intr->ctrl->int_credits);
}
