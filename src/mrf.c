// SPDX-License-Identifier: GPL-2.0-only

/*
 * Copyright (C) 2022 Datto Inc.
 */

#include "mrf.h"
#include "includes.h"
#include "snap_device.h"

#ifdef HAVE_BLK_ALLOC_QUEUE
//#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
#include <linux/blk-mq.h>
#include <linux/percpu-refcount.h>
#endif

#ifdef HAVE_MAKE_REQUEST_FN_INT
int dattobd_call_mrf(make_request_fn *fn, struct request_queue *q,
                     struct bio *bio)
{
        return fn(q, bio);
}
#elif defined HAVE_MAKE_REQUEST_FN_VOID
int dattobd_call_mrf(make_request_fn *fn, struct request_queue *q,
                     struct bio *bio)
{
        fn(q, bio);
        return 0;
}
#elif !defined USE_BDOPS_SUBMIT_BIO
int dattobd_call_mrf(make_request_fn *fn, struct request_queue *q,
                     struct bio *bio)
{
        return fn(q, bio);
}
#endif

#ifdef HAVE_BLK_ALLOC_QUEUE
//#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
MRF_RETURN_TYPE dattobd_null_mrf(struct request_queue *q, struct bio *bio)
{
        percpu_ref_get(&q->q_usage_counter);
        // create a make_request_fn for the supplied request_queue.
        return blk_mq_make_request(q, bio);
}
#endif

#ifndef HAVE_BDOPS_SUBMIT_BIO
make_request_fn* dattobd_get_bd_mrf(struct block_device *bdev)
{
    return bdev->bd_disk->queue->make_request_fn;
}

void dattobd_set_bd_mrf(struct block_device *bdev, make_request_fn *mrf)
{
    bdev->bd_disk->queue->make_request_fn = mrf;
}
#endif

#ifdef USE_BDOPS_SUBMIT_BIO
MRF_RETURN_TYPE (*dattobd_blk_mq_submit_bio)(struct bio *) = (BLK_MQ_SUBMIT_BIO_ADDR != 0) ?
	(MRF_RETURN_TYPE (*)(struct bio *)) (BLK_MQ_SUBMIT_BIO_ADDR + (long long)(((void *)kfree)-(void *)KFREE_ADDR)) : NULL;

MRF_RETURN_TYPE dattobd_snap_null_mrf(struct bio *bio){
	return dattobd_blk_mq_submit_bio(bio);

}
struct block_device_operations* dattobd_get_bd_ops(struct block_device *bdev){
	return (struct block_device_operations*)bdev->bd_disk->fops;
}

#else
int dattobd_call_mrf_real(struct snap_device *dev, struct bio *bio)
{
    return dattobd_call_mrf(
        dev->sd_orig_request_fn,
        dattobd_bio_get_queue(bio), 
        bio);
}
#endif
