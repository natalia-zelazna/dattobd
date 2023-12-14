// SPDX-License-Identifier: GPL-2.0-only

/*
 * Copyright (C) 2022-2023 Datto Inc.
 */

#ifndef MRF_TYPE_H_
#define MRF_TYPE_H_

#include "includes.h"

#if defined HAVE_MAKE_REQUEST_FN_INT

#define MRF_RETURN_TYPE int
#define MRF_RETURN(ret) return ret


#elif defined HAVE_MRF_RETURN_TYPE_INT

#define MRF_RETURN_TYPE unsigned int
#define MRF_RETURN(ret) return ret


#elif defined HAVE_MAKE_REQUEST_FN_VOID

#define MRF_RETURN_TYPE void
#define MRF_RETURN(ret) return


#elif defined HAVE_NONVOID_SUBMIT_BIO_1

#define MRF_RETURN_TYPE blk_qc_t
#define MRF_RETURN(ret) return BLK_QC_T_NONE
#ifdef USE_BDOPS_SUBMIT_BIO
#define NO_DATTOBD_CALL_MRF
#endif // USE_BDOPS_SUBMIT_BIO


#else

#define MRF_RETURN_TYPE void
#define MRF_RETURN(ret) return

#define NO_DATTOBD_CALL_MRF

#endif

#endif //MRF_TYPE_H_