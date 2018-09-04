/*****************************************************************************
Copyright (c) 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 9297 $
$Date: 2012-10-26 12:58:12 +0530 (Fri, 26 Oct 2012) $

Title       : adi_harris_corner_detect_wrapper.c

Description : Wrapper codes for Harris Corner Detection

*****************************************************************************/
#include "stdafx.h"
/*=============  I N C L U D E S   =============*/
#include "adi_image_tool_box.h"
//#include "adi_utilities.h"
#include "adi_pyr_opticalflow.h"

/*==============  D E F I N E S  ===============*/

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

  Function              : adi_harrisMarkCorners

  Function description  : This function is used to mark the corner coordiantes on
                          the image with 3x3 window of zero pixels.

  Parameters            :
    pPyrOptflSelectGoodFeatures(In/Out) -   Pointer to structure

  Returns               : Nothing

  Notes                 :

******************************************************************************/
#pragma section("adi_slow_prio0_code")
void adi_harrisMarkCorners(
                           ADI_PYR_OPTFL_SEL_GD_FEAT   *pPyrOptflSelectGoodFeatures
                           )
{
    uint8_t                     *pImgIn;
    uint8_t                     *pImgOut;
    uint32_t                    nImgWidth;
    uint32_t                    nImgHeight;
    ADI_PYR_OPTFL_GD_FEAT_PTR   pGoodFeatures;
    uint32_t                    i, j, k;
    uint32_t                    X, Y;
    uint16_t                    n;

    pImgIn = pPyrOptflSelectGoodFeatures->pImgIn;
    pImgOut = pPyrOptflSelectGoodFeatures->pImgOut;
    nImgWidth = pPyrOptflSelectGoodFeatures->nImageWidth;
    nImgHeight = pPyrOptflSelectGoodFeatures->nImageHeight;
    pGoodFeatures = pPyrOptflSelectGoodFeatures->pGoodFeatures;
    n = pPyrOptflSelectGoodFeatures->nNumGoodFeatures;

    for (i = 0; i < nImgHeight; i++)
    {
        for (j = 0; j < nImgWidth; j++)
        {
            pImgOut[i * nImgWidth + j] = pImgIn[i * nImgWidth + j];
        }
    }

    for (k = 0; k < n; k++)
    {
        X = pGoodFeatures->nCoordX;
        Y = pGoodFeatures->nCoordY;
        pGoodFeatures++;
        for (j = Y - 1; j < Y + 1; j++)
        {
            for (i = X - 1; i < X + 1; i++)
            {
                pImgOut[j * nImgWidth + i] = 0;
            }
        }
    }
}

/******************************************************************************

  Function              : adi_SelGoodFeatPreProc_i8

  Function description  : Smooths the input image and calculates the gradients

  Parameters            :
    pPyrOptflSelectGoodFeatures(In/Out) -   Pointer to structure
    pKernelSmooth(In)                   -   Kernel to smooth the image

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_SelGoodFeatPreProc_i8(
                               ADI_PYR_OPTFL_SEL_GD_FEAT   *pPyrOptflSelectGoodFeatures,
                               int16_t                     *pKernelSmooth
                               )
{
    uint8_t     *pImgIn;
    uint32_t    nImageWidth;
    uint32_t    nImageHeight;
    uint8_t     *pSmoothImgOut;
    int16_t     *pGradX;
    int16_t     *pGradY;

    uint32_t    nFilterOrder;

    pImgIn = pPyrOptflSelectGoodFeatures->pImgIn;
    nImageWidth = pPyrOptflSelectGoodFeatures->nImageWidth;
    nImageHeight = pPyrOptflSelectGoodFeatures->nImageHeight;
    pSmoothImgOut = pPyrOptflSelectGoodFeatures->pSmoothImgOut;
    pGradX = pPyrOptflSelectGoodFeatures->pGradX;
    pGradY = pPyrOptflSelectGoodFeatures->pGradY;
    nFilterOrder = 5;

    adi_GaussianFilter_i8(pImgIn,
                          nImageHeight + nFilterOrder - 1,
                          nImageWidth,
                          pKernelSmooth,
                          pSmoothImgOut);

    nFilterOrder = 3;
    adi_sobel_3x3_horz_shift3_i8(pSmoothImgOut,
                                 nImageHeight + nFilterOrder - 1,
                                 nImageWidth,
                                 pGradY);
    adi_sobel_3x3_vert_shift3_i8(pSmoothImgOut,
                                 nImageHeight + nFilterOrder - 1,
                                 nImageWidth,
                                 pGradX);
}

/******************************************************************************

  Function              : adi_SelectGoodFeatures_Appl_i8

  Function description  : Selects the good features for input image

  Parameters            :
      pOpticalFlowSelectGoodFeatures(In/Out)    -   Pointer to structure

  Returns               : Status (Zero/Non-Zero)

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_SelectGoodFeatures_Appl_i8(
                                    ADI_PYR_OPTFL_SEL_GD_FEAT   *pOpticalFlowSelectGoodFeatures
                                    )
{
    int16_t                 *pScore;
    int16_t                 *pGradX;
    int16_t                 *pGradY;
    uint8_t                 *pTempBuffL1;
    ADI_PYR_OPTFL_GD_FEAT   *pGoodFeatures;
    uint16_t                *pNumGoodFeatures;
    uint8_t                 *pImgIn, *pImgOut;
    uint16_t                *pStoreCoord;
    uint32_t                nImageWidth;
    uint32_t                nImageHeight;
    int16_t                 nSenstivity;
    int32_t                 nThreshold;
    uint16_t                nMinDistance;
    int16_t                 i;
  
    pImgIn = pOpticalFlowSelectGoodFeatures->pImgIn;
    pImgOut = pOpticalFlowSelectGoodFeatures->pImgOut;
    nImageWidth = pOpticalFlowSelectGoodFeatures->nImageWidth;
    pGradX = pOpticalFlowSelectGoodFeatures->pGradX + 
             (ROWS_LOST_IN_FILTER-HARRIS_BLOCK_SIZE_HALF) * nImageWidth;
    pGradY = pOpticalFlowSelectGoodFeatures->pGradY + 
             (ROWS_LOST_IN_FILTER-HARRIS_BLOCK_SIZE_HALF) * nImageWidth;
    pGoodFeatures = pOpticalFlowSelectGoodFeatures->pGoodFeatures;
    pNumGoodFeatures = &pOpticalFlowSelectGoodFeatures->nNumGoodFeatures;
    pScore = pOpticalFlowSelectGoodFeatures->pScore + 
             (ROWS_LOST_IN_FILTER-HARRIS_BLOCK_SIZE_HALF) * nImageWidth;
    
    nImageHeight = pOpticalFlowSelectGoodFeatures->nImageHeight - 
                  (ROWS_LOST_IN_FILTER*2) + (HARRIS_BLOCK_SIZE_HALF*2);
    nSenstivity = pOpticalFlowSelectGoodFeatures->nSenstivity;
    nThreshold = pOpticalFlowSelectGoodFeatures->nThreshold;
    nMinDistance = pOpticalFlowSelectGoodFeatures->nMinDistance;

    pTempBuffL1 = pOpticalFlowSelectGoodFeatures->pScratchBufL1;
    pStoreCoord = pOpticalFlowSelectGoodFeatures->pFeatureCoord;

    ADIHarrisCornerScoreWrapper(pGradX,
                                pGradY,
                                pScore,
                                nImageWidth,
                                nImageHeight,
                                nSenstivity,
                                (int32_t *)pTempBuffL1);
                                
    
 
    *pNumGoodFeatures = ADIharrisNonMaxSupresWrapper(pScore,
                                                     pStoreCoord,
                                                     nImageWidth,
                                                     nImageHeight,
                                                     nThreshold,
                                                     nMinDistance,
                                                     (uint8_t *)pTempBuffL1);

    for (i = 0; i < *pNumGoodFeatures; i++)
    {
        pGoodFeatures->nCoordX = *pStoreCoord++;
        pGoodFeatures->nCoordY = *pStoreCoord++;
        pGoodFeatures->nCoordY += (24-HARRIS_BLOCK_SIZE_HALF); 
        pGoodFeatures++;
    }

    return;
}

/******************************************************************************

  Function              : ADIharrisNonMaxSupresWrapper

  Function description  : Computes Harris Corner Score over an image
                          This function is a wrapper function which
                          call the library API over slices for efficient
                          performance. The data is moved in/out from the
                          image memory in L3 through memory copy. The memory
                          copy can be replaced with dma movement in a real
                          system

  Parameters            :
    pInScore(In)    -   Pointer to the corner score
    pOutFeat(Out)   -   Pointer to store coord
    nImgWidth(In)   -   Image Width
    nImgHeight(In)  -   Image Height
    nThreshold(In)  -   Threshold for detection
    nMinDist(In)    -   Minimum distance to be enforced between features
    pBuffer(Temp)   -   Pointer to scratch buffer for intermediate compute

  Returns               : Number of Good Features

  Notes                 :

******************************************************************************/
int16_t ADIharrisNonMaxSupresWrapper(
                                     int16_t     *pInScore,
                                     uint16_t    *pOutFeat,
                                     uint32_t    nImgWidth,
                                     uint32_t    nImgHeight,
                                     int16_t     nThreshold,
                                     int16_t     nMinDist,
                                     uint8_t     *pBuffer
                                     )
{
    uint32_t    i, j;
    int32_t     nBytesPerGradVal;
    int16_t     *pInBufL1;
    uint8_t     *pStateBufL1;
    int16_t     *pInCurr;
    int16_t     *pInCurrL1;
    int16_t     *pInBufL1_Sec;
    int16_t     *pInBufL1_Last;
    uint32_t    nSize, nSize1;
    int16_t     nNumFeatures;
    uint16_t    nNumFeaturesInRow;

    nBytesPerGradVal = sizeof(int16_t);

    nNumFeatures = 0;
    nNumFeaturesInRow = 0;

    pInBufL1 = (int16_t *)pBuffer + (HARRIS_BLOCK_SIZE_HALF + 1); /* Cushion for 4 columns */
    pStateBufL1 = (uint8_t *) (pInBufL1 + HARRIS_BLOCK_SIZE * nImgWidth);

    pInCurr = pInScore;
    pInCurrL1 = pInBufL1 + (HARRIS_BLOCK_SIZE_HALF) * nImgWidth;
    pInBufL1_Sec = pInBufL1 + nImgWidth;
    pInBufL1_Last = pInBufL1 + (HARRIS_BLOCK_SIZE - 1) * nImgWidth;
    nSize = HARRIS_BLOCK_SIZE * nImgWidth;
    nSize1 = (HARRIS_BLOCK_SIZE - 1) * nImgWidth;

    /* Initialization code */
    adi_HarrisNonMaxSupresInit(pStateBufL1,
                               (nMinDist + 1) * nImgWidth);

    /*
    copy 7 lines from the Image Buffer in L3 to L1 memory for first time
    subsequently copy just one row
    */

    /* This can be moved to DMA */
    adi_MemCopy((uint8_t *)pInBufL1,
                (uint8_t *)pInCurr,
                (nSize * nBytesPerGradVal));
    pInCurr += nSize;

    nSize = nImgWidth;

    for
    (
        i = HARRIS_BLOCK_SIZE_HALF;
        i < nImgHeight - HARRIS_BLOCK_SIZE_HALF;
        i++
    )
    {
        /* get one row of output */
        nNumFeaturesInRow = adi_HarrisNonMaxSupres7x7(pInCurrL1,
                                                      pOutFeat,
                                                      nImgWidth,
                                                      1,    /* slice is just 1 row */
                                                      nThreshold,
                                                      nMinDist,
                                                      pStateBufL1);

        /* Y value is written here */
        for (j = 0; j < nNumFeaturesInRow; j++)
        {
            pOutFeat++;
            *pOutFeat++ = i;
        }

        nNumFeatures += nNumFeaturesInRow;

        /* re-align the rest of the rows in L1 input buffer */
        adi_MemCopy((uint8_t *)pInBufL1,
                    (uint8_t *)pInBufL1_Sec,
                    (nSize1 * nBytesPerGradVal));

        /* copy one row from Image Buffer in L3 to L1 */

        /* This can be moved to DMA */
        adi_MemCopy((uint8_t *)pInBufL1_Last,
                    (uint8_t *)pInCurr,
                    (nSize * nBytesPerGradVal));
        pInCurr += nSize;
    }

    return (nNumFeatures);
}

/******************************************************************************

  Function                : ADIHarrisCornerScoreWrapper

  Function description    : Computes Harris Corner Score over an image
                            This function is a wrapper function which
                            call the library API over slices for efficient
                            performance. The data is moved in/out from the
                            image memory in L3 through memory copy. The memory
                            copy can be replaced with dma movement in a real
                            system

  Parameters              :
      pGradX(In)        -   Gradient of Image in X direction
      pGradY(In)        -   Gradient of Image in Y direction
      pOutScore(Out)    -   Harris Score
      nImgWidth(In)     -   Image Width
      nImgHeight(In)    -   Image Height
      nSenstivity(In)   -   Senstivity
      pBuffer(Temp)     -   Pointer to scratch buffer for intermediate compute

  Returns                 :   Nothing

  Notes                   :

******************************************************************************/
void ADIHarrisCornerScoreWrapper(
                                 int16_t     *pGradX,
                                 int16_t     *pGradY,
                                 int16_t     *pOutScore,
                                 uint32_t    nImgWidth,
                                 uint32_t    nImgHeight,
                                 int16_t     nSenstivity,
                                 int32_t     *pBuffer
                                 )
{
    uint32_t    i;
    int32_t     nBytesPerGradVal;
    int16_t     *pInBufL1X;
    int16_t     *pInCurrX;
    int16_t     *pInCurrL1X;
    int16_t     *pInBufL1_SecX;
    int16_t     *pInBufL1_LastX;

    int16_t     *pInBufL1Y;
    int16_t     *pInCurrY;
    int16_t     *pInCurrL1Y;
    int16_t     *pInBufL1_SecY;
    int16_t     *pInBufL1_LastY;

    int16_t     *pOutBufL1;
    int16_t     *pOutCurr;

    int32_t     *pTempBufL1;
    uint32_t    nSize, nSize1;
    int16_t     nNumFeatures, nNumFeatures1;

    nNumFeatures = 0;
    nNumFeatures1 = 0;
    nBytesPerGradVal = sizeof(int16_t);

    pInBufL1X = (int16_t *)pBuffer;
    pInBufL1Y = (pInBufL1X + nImgWidth * HARRIS_BLOCK_SIZE);
    pOutBufL1 = (pInBufL1Y + nImgWidth * HARRIS_BLOCK_SIZE);
    pTempBufL1 = (int32_t *) (pOutBufL1 + nImgWidth);

    pInCurrX = pGradX;
    pInCurrL1X = pInBufL1X + (HARRIS_BLOCK_SIZE_HALF) * nImgWidth;
    pInBufL1_SecX = pInBufL1X + nImgWidth;
    pInBufL1_LastX = pInBufL1X + (HARRIS_BLOCK_SIZE - 1) * nImgWidth;

    pInCurrY = pGradY;
    pInCurrL1Y = pInBufL1Y + (HARRIS_BLOCK_SIZE_HALF) * nImgWidth;
    pInBufL1_SecY = pInBufL1Y + nImgWidth;
    pInBufL1_LastY = pInBufL1Y + (HARRIS_BLOCK_SIZE - 1) * nImgWidth;

    pOutCurr = pOutScore + (HARRIS_BLOCK_SIZE_HALF) * nImgWidth;

    nSize = HARRIS_BLOCK_SIZE * nImgWidth;
    nSize1 = (HARRIS_BLOCK_SIZE - 1) * nImgWidth;

    /*
    copy 7 lines from the Image Buffer in L3 to L1 memory for first time
    subsequently copy just one row. This can be done in DMA
    */
    adi_MemCopy((uint8_t *)pInBufL1X,
                (uint8_t *)pInCurrX,
                (nSize * nBytesPerGradVal));

    pInCurrX += nSize;

    adi_MemCopy((uint8_t *)pInBufL1Y,
                (uint8_t *)pInCurrY,
                (nSize * nBytesPerGradVal));

    pInCurrY += nSize;
    nSize = nImgWidth;
    for
    (
        i = HARRIS_BLOCK_SIZE_HALF;
        i < nImgHeight - HARRIS_BLOCK_SIZE_HALF;
        i++
    )
    {

        /* get one row of output */
        adi_HarrisComputeCornerScore7x7(pInCurrL1X,
                                        pInCurrL1Y,
                                        pOutBufL1,
                                        nImgWidth,
                                        1,  /* slice is just 1 row */
                                        nSenstivity,
                                        pTempBufL1);

        /* re-align the rest of the rows in L1 input buffer */
        adi_MemCopy((uint8_t *)pInBufL1X,
                    (uint8_t *)pInBufL1_SecX,
                    (nSize1 * nBytesPerGradVal));
        adi_MemCopy((uint8_t *)pInBufL1Y,
                    (uint8_t *)pInBufL1_SecY,
                    (nSize1 * nBytesPerGradVal));

        /* copy one row from Image Buffer in L3 to L1 */

        /* This can be done in DMA */
        adi_MemCopy((uint8_t *)pInBufL1_LastX,
                    (uint8_t *)pInCurrX,
                    (nSize * nBytesPerGradVal));

        pInCurrX += nSize;

        adi_MemCopy((uint8_t *)pInBufL1_LastY,
                    (uint8_t *)pInCurrY,
                    (nSize * nBytesPerGradVal));
        pInCurrY += nSize;

        adi_MemCopy((uint8_t *)pOutCurr,
                    (uint8_t *)pOutBufL1,
                    (nSize * nBytesPerGradVal));
        pOutCurr += nSize;
    }

    return;
}

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/tags/release_2_4_0_imagetoolbox_140807/example/pyr_opticalflow/adi_harris_corner_detect_wrapper.c $
**
*/
