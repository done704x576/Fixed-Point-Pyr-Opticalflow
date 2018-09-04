/*****************************************************************************
Copyright (c) 2012 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 9303 $
$Date: 2012-10-26 17:27:54 +0530 (Fri, 26 Oct 2012) $

Title       : adi_pyr_opticalflow_utils.c

Description : Pyramid optical flow utility functions in C
*****************************************************************************/
#include "stdafx.h"
/*=============  I N C L U D E S   =============*/
#include "adi_image_tool_box.h"
//#include "adi_utilities.h"
#include "adi_pyr_opticalflow.h"

/*==============  D E F I N E S  ===============*/
#define SCALE_FACTOR            16
#define RND_C(x)                (x + 0x4000) >> 15
#define RND_C8(x)               (x + 0x400000) >> 23
#define ROUND2POS(nValue, nPos) (nValue + (1 << (nPos - 1)))
#define ASM_MUL16(nVal1, nVal2) (((int16_t) nVal1 * (int16_t) nVal2))
/*=============  D A T A  =============*/

/*=============  C O D E  =============*/

/*
**  Function Prototype section
**  (static-scoped functions)
*/

/*
**  Function Definition section
*/

/******************************************************************************

Function              : adi_sobel_3x3_horz_shift3_i8

 Function description  : This function calculates the sobel derivative using
                         the mask shown below. The input is 8-bit
                         while the output is 16-bit signed.

                          Horizontal mask =  | -1   -2   -1  |
                                             |  0    0    0  |
                                             |  1    2    1  |

                          This function is called when dx = 0 and dy = 1
                           where dx = horizontal derivative
                                 dy = vertical derivative

  Parameters            :
    pInBuff(In)     -   pointer to the input image
    dimY(In)        -   number of rows of input slice
    dimX(In)        -   number of columns of input slice
    pOutBuff(Out)   -   pointer to the output buffer

  Returns               : Nothing
******************************************************************************/
void adi_sobel_3x3_horz_shift3_i8(
                                  const uint8_t   *pInBuff,
                                  uint32_t        dimY,
                                  uint32_t        dimX,
                                  int16_t         *pOutBuff
                                 )
{
    const uint8_t   *pInMatrix;
    uint32_t        i, j, k, m, n, p;
    int16_t         dx;
    int16_t         *FilterCoeffHorizontal;
    int16_t         FilterCoeff3x3Horizontal[3*3] = {
                                                        -1, -2,  -1,
                                                        0,   0,   0,
                                                        1,   2,   1
                                                    };

    m = 0;
    n = 0;
    p = 1;

    FilterCoeffHorizontal = FilterCoeff3x3Horizontal;
    for (i = 0; i < (dimY - 2); i++)
    {
        pInMatrix = (uint8_t *)pInBuff;
        dx = 0;
        for (j = 0; j < 3; j++)
        {
            dx += (pInMatrix[m]) * (FilterCoeffHorizontal[j * 3]);
            for (k = 0; k < 2; k++)
            {
                dx += (pInMatrix[k]) * (FilterCoeffHorizontal[j * 3 + k + 1]);
            }

            pInMatrix += dimX;
        }

        *pOutBuff++ = (dx >> 3);       /* first elemnet in row */

        for (n = 0; n < (dimX - 2); n++)
        {
            dx = 0;
            pInMatrix = (uint8_t *)pInBuff + n;

            for (j = 0; j < 3; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    dx += (pInMatrix[k]) * (FilterCoeffHorizontal[j * 3 + k]);
                }

                pInMatrix += dimX;
            }

            *pOutBuff++ = (dx >> 3);   /* middle elements */
        }

        pInMatrix = (uint8_t *)pInBuff + n;
        dx = 0;
        for (j = 0; j < 3; j++)
        {
            for (k = 0; k < 2; k++)
            {
                dx += (pInMatrix[k]) * (FilterCoeffHorizontal[j * 3 + k]);
            }

            dx += (pInMatrix[p]) * (FilterCoeffHorizontal[j * 3 + k]);
            pInMatrix += dimX;
        }

        *pOutBuff++ = (dx >> 3);       /* last element in row */
        pInBuff = pInBuff + dimX;
    }
}
/******************************************************************************

  Function              : adi_sobel_3x3_vert_shift3_i8

  Function description  : This function calculates the sobel derivative using
                          the mask shown below. The input is 8-bit
                          while the output is 16-bit signed.

                          Vertical mask =    | -1    0   1  |
                                             | -2    0   2  |
                                             | -1    0   1  |

                          This function is called when dx = 1 and dy = 0
                           where dx = horizontal derivative
                                 dy = vertical derivative

  Parameters            :
    pInBuff(In)     -   pointer to the input image
    dimY(In)        -   number of rows of input slice
    dimX(In)        -   number of columns of input slice
    pOutBuff(Out)   -   pointer to the output buffer

  Returns               : Nothing
******************************************************************************/
void adi_sobel_3x3_vert_shift3_i8(
                                  const uint8_t   *pInBuff,
                                  uint32_t        dimY,
                                  uint32_t        dimX,
                                  int16_t         *pOutBuff
                                 )
{
    const uint8_t   *pInMatrix;
    uint32_t        i, j, k, m, n, p;
    int16_t         dy;
    int16_t         *FilterCoeffVertical;
    int16_t         FilterCoeff3x3Vertical[3*3] = {
                                                    -1,   0,   1,
                                                    -2,   0,   2,
                                                    -1,   0,   1
                                                  };

    m = 0;
    n = 0;
    p = 1;

    FilterCoeffVertical = FilterCoeff3x3Vertical;
    for (i = 0; i < (dimY - 2); i++)
    {
        pInMatrix = (uint8_t *)pInBuff;
        dy = 0;
        for (j = 0; j < 3; j++)
        {
            dy += (pInMatrix[m]) * (FilterCoeffVertical[j * 3]);
            for (k = 0; k < 2; k++)
            {
                dy += (pInMatrix[k]) * (FilterCoeffVertical[j * 3 + k + 1]);
            }

            pInMatrix += dimX;
        }

        *pOutBuff++ = (dy >> 3);       /* first elemnet in row */

        for (n = 0; n < (dimX - 2); n++)
        {
            dy = 0;
            pInMatrix = (uint8_t *)pInBuff + n;
            for (j = 0; j < 3; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    dy += (pInMatrix[k]) * (FilterCoeffVertical[j * 3 + k]);
                }

                pInMatrix += dimX;
            }

            *pOutBuff++ = (dy >> 3);   /* middle elements */
        }

        pInMatrix = (uint8_t *)pInBuff + n;
        dy = 0;
        for (j = 0; j < 3; j++)
        {
            for (k = 0; k < 2; k++)
            {
                dy += (pInMatrix[k]) * (FilterCoeffVertical[j * 3 + k]);
            }

            dy += (pInMatrix[p]) * (FilterCoeffVertical[j * 3 + k]);
            pInMatrix += dimX;
        }

        *pOutBuff++ = (dy >> 3);       /* last element in row */
        pInBuff = pInBuff + dimX;
    }
}
/******************************************************************************

 Function              : adi_Gs_Deriv_3x3_i8

 Function description  : This function calculates the Gaussian derivative using
                         the mask shown below. The input is 8-bit
                         while the output is 16-bit signed.

                         Mask =    | X1   X2   X3 |
                                   | X4   X5   X6 |
                                   | X7   X8   X9 |

  Parameters            :
    pInBuff(In)     -   pointer to the input image 
    pMask(In)       -   3X1 mask in 1.15 format 
    dimY(In)        -   number of rows of input slice 
    dimX(In)        -   number of columns of input slice
    pOutBuff(Out)   -   pointer to the output buffer

  Returns               : Nothing

  Registers             : R0-R7, I0-I3, B0, B1, M0, M1, L0, L1,
                          P0, P1, P4, P5, LC0, LC1.

******************************************************************************/
void adi_Gs_Deriv_3x3_i8(
                         const uint8_t   *pInBuff,
                         int16_t         *pMask,
                         int32_t         dimY,
                         int32_t         dimX,
                         int16_t         *pOutBuff
                        )
{
    int16_t     temp_mask[9];
    uint32_t    i, j, k, n;
    uint8_t     *pInMatrix;
    int32_t     nRes;

    k = 0;
    n = 0;
    pOutBuff = pOutBuff + (dimX * 3) + 3;
    pInBuff = pInBuff + (dimX * 2) + 2;
    /* The pMask need to be inverted so as to directly apply it over the image, for convolution */
    for (i = 0U; i < 9U; i++)
    {
        temp_mask[i] = pMask[i];
    }

    for (i = 0; i < (uint32_t)(dimY - 6); i++)
    {
        for (n = 0; n < (uint32_t)(dimX - 6); n++)
        {
            nRes = 0;
            pInMatrix = (uint8_t *)pInBuff + n;

            for (j = 0; j < 3; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    nRes += ASM_MUL16((pInMatrix[k]) , (temp_mask[j * 3 + k]));
                }

                pInMatrix += dimX;
            }
               nRes = nRes >> 4;
            *pOutBuff++ = (int16_t)nRes;   /* middle elements in row */
        }
        pInBuff = pInBuff + dimX;
        pOutBuff = pOutBuff + 6;      
    }
}
/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/tags/release_2_4_0_imagetoolbox_140807/example/pyr_opticalflow/adi_pyr_opticalflow_utils.c $
**
*/
