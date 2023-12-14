// SPDX-License-Identifier: GPL-2.0-only

/*
 * Copyright (C) 2022-2023 Datto Inc.
 */

// 5.9 <= kernel_version

#include "includes.h"

MODULE_LICENSE("GPL");

static int dummy_bio(struct bio *bio){
	return 0;
}


static inline void dummy(void){
	struct bio b;
	struct block_device_operations bdo = {
		.submit_bio = dummy_bio,
	};

	bdo.submit_bio(&b);
}
