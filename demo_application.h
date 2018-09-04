/*****************************************************************************
Copyright (c) 2009 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 7037 $
$Date: 2012-03-01 12:43:54 +0530 (Thu, 01 Mar 2012) $

Title       : demo_application.h

Description : Macro definitions for demo application

*****************************************************************************/
#ifndef __DEMO_APPLICATION_H__
#define __DEMO_APPLICATION_H__

/*=============  I N C L U D E S   =============*/
#include <stdio.h>
#include "adi_tool_chain.h"

/*==============  D E F I N E S  ===============*/
#ifdef MISRA_RULES
#pragma diag(push)

/*
MISRA Rule 19.4(Req): C macros shall only expand to a braced initialiser,
a constant, a string literal, a parenthesised expression, a type qualifier,
a storage class specifier, or a do-while-zero construct.
*/
#pragma diag(suppress : misra_rule_19_4)
#endif
#define PRINT                       printf
#define IMAGE_TYPE_UNKNOWN          ((uint32_t) 0)
#define IMAGE_TYPE_RAW              ((uint32_t) 1)
#define IMAGE_TYPE_RGB_BMP          ((uint32_t) 2)
#define IMAGE_TYPE_RGB_RAW          ((uint32_t) 3)
#define IMAGE_TYPE_YUV              ((uint32_t) 4)
#define IMAGE_TYPE_YUV420           ((uint32_t) 5)
#define IMAGE_TYPE_YUV422           ((uint32_t) 6)
#define RAW_IMAGE_NUM_BYTES         ((uint32_t) 1)
#define YUV420_NUM_BYTES            ((uint32_t) (3 / 2))
#define YUV422_NUM_BYTES            ((uint32_t) 2)
#define YUV444_NUM_BYTES            ((uint32_t) 3)
#define RGB888_NUM_BYTES            ((uint32_t) 3)
#define YUV422_NUM_BYTES            ((uint32_t) 2)    
#define ADI_NUM_BYTES_PER_PIXEL_1   1
#define ADI_NUM_BYTES_PER_PIXEL_2   2

#define ADI_HALFWORD_BYTES          ((uint32_t) 2)
#define ADI_WORD_BYTES              ((uint32_t) 4)

/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/

/*
**  External Function Prototypes
*/

/*=============  D A T A    T Y P E S   =============*/
#ifdef MISRA_RULES
#pragma diag(pop)
#endif
#endif /* __DEMO_APPLICATION_H__ */

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/tags/release_2_4_0_imagetoolbox_140807/example/include/demo_application.h $
**
*/
