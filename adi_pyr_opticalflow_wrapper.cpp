/*****************************************************************************
Copyright (c) 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 4165 $
$Date: 2010-11-12 20:00:16 +0530 (Fri, 12 Nov 2010) $

Title       : adi_pyr_opticalflow_wrapper.c

Description : Wrapper codes for pyramidal optical flow.

*****************************************************************************/
#include "stdafx.h"
/*=============  I N C L U D E S   =============*/
#include "adi_image_tool_box.h"
#include "adi_pyr_opticalflow.h"
#include "profile.h"

/*==============  D E F I N E S  ===============*/
/* takes fract value x of a 16.16 number and convert it to 1.15|1.15 of (1-x)|x format */
#define Cnvrt16_16to1_15_1_15_1minusX_X(X)  ((0x7FFF - (((X) & 0xFFFF)>>1))<<16) | (((X) & 0xFFFF)>>1)

/* converts 16.16 to 0.16 after rounding */
#define Cnvrt16_16to0_16(X)                 ((X) & 0xFFFF)

/* converts 16.16 to 16.0 after rounding */
#define Cnvrt16_16to16_0(X)                 (((X) + 0x8000)>>16)

/* finds absolute value of 16.16 number */
#define ABS_16_16(X)                        ((X) & 0x80000000) ? adi_mult1616_1616((X), 0xFFFF0000): (X)

#define WINDOW_SIZE_X   7
#define WINDOW_SIZE_Y   7
#define SCOREMAX        32767
#define SCOREMIN        -32768
#define SHIFT_VALUE     15

#define SAT(x, v1, v2)  (((x) < (v1)) ? (v1) : ((x) > (v2)) ? (v2) : (x))
#define RND_C(x)    (x + 0x4000) >> 15

#define ABS_16_16(X)                        ((X) & 0x80000000) ? adi_mult1616_1616((X), 0xFFFF0000): (X)
#define ASM_MUL16(nVal1, nVal2)             (((int16_t) nVal1 * (int16_t) nVal2) << 1)
#define SCALE_FACTOR                        16
#define ROUND2POS(nValue, nPos)             (nValue + (1 << (nPos - 1)))

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

  Function              : adi_TrackFeaturesImagePreProcess_i8

  Function description  : Does preprocessing on input image, required for tracking.
                          Smoothing, pyramid down, calculating gradients for both levels.

  Parameters            :
    pPyrOptflImageFeatures(In/Out)  -   Pointer to structure
    pKernelSmooth(In)               -   Kernel to smooth the image
    pKernelHorzDeriv(In)            -   Kernel to find gaussian derivative in horz direction
    pKernelVertDeriv(In)            -   Kernel to find gaussian derivative in vert direction

  Returns               : None

  Notes                 :

******************************************************************************/
void adi_TrackFeaturesImagePreProcess_i8(
                                         ADI_PYR_OPTFL_IMAGE_FEAT    *pPyrOptflImageFeatures,
                                         int16_t                     *pKernelSmooth,
                                         int16_t                     *pKernelHorzDeriv,
                                         int16_t                     *pKernelVertDeriv
                                         )
{
    uint8_t     *pImg;
    uint32_t    nImageWidth;
    uint32_t    nImageHeight;
    uint8_t     *pSmoothImg;
    uint8_t     *pImgPyr;
    int16_t     *pGradXImg;
    int16_t     *pGradYImg;
    int16_t     *pGradXPyr;
    int16_t     *pGradYPyr;

    uint32_t    nFilterOrder;

    pImg = pPyrOptflImageFeatures->pImg;
    nImageWidth = pPyrOptflImageFeatures->nImageWidth;
    nImageHeight = pPyrOptflImageFeatures->nImageHeight;
    pSmoothImg = pPyrOptflImageFeatures->pSmoothImg;
    pImgPyr = pPyrOptflImageFeatures->pImgPyr;
    pGradXImg = pPyrOptflImageFeatures->pGradXImg;
    pGradYImg = pPyrOptflImageFeatures->pGradYImg;
    pGradXPyr = pPyrOptflImageFeatures->pGradXPyr;
    pGradYPyr = pPyrOptflImageFeatures->pGradYPyr;

    nFilterOrder = 5;

    adi_GaussianFilter_i8(pImg,
                          nImageHeight + nFilterOrder - 1,
                          nImageWidth,
                          pKernelSmooth,
                          pSmoothImg);

    /* Calculate gaussian derivative of image data. Output gets shifted by 4 */
    adi_Gs_Deriv_3x3_i8(pSmoothImg,
                        pKernelHorzDeriv,
                        nImageHeight,
                        nImageWidth,
                        pGradXImg);
    adi_Gs_Deriv_3x3_i8(pSmoothImg,
                        pKernelVertDeriv,
                        nImageHeight,
                        nImageWidth,
                        pGradYImg);

    /* Pyramid down the image by 4 */
    adi_ImageDownScaleby4(pSmoothImg, nImageHeight, nImageWidth, pImgPyr);

    adi_Gs_Deriv_3x3_i8(pImgPyr,
                        pKernelHorzDeriv,
                        nImageHeight >> 2,
                        nImageWidth >> 2,
                        pGradXPyr);
    adi_Gs_Deriv_3x3_i8(pImgPyr,
                        pKernelVertDeriv,
                        nImageHeight >> 2,
                        nImageWidth >> 2,
                        pGradYPyr);
}



void adi_GaussianFilter_i8(
                           const uint8_t   *pInBuff,
                           uint32_t        dimY,
                           uint32_t        dimX,
                           const int16_t   *pKernel,
                           uint8_t         *pOutBuff
                           )
{
    uint32_t    nShift;

    /* Assumed that Coefficients will be in 1.15 format */
    nShift = 15U;
    adi_conv2D5by5_8(pInBuff,
                     dimY,
                     dimX,
                     pKernel,
                     pOutBuff,
                     nShift);
}

uint16_t adi_HarrisNonMaxSupres7x7(
                                   int16_t     *pScore,
                                   uint16_t    *pGoodFeaturesCoord,
                                   int32_t     nCols,
                                   int32_t     nRows,
                                   int32_t     nThreshold,
                                   uint16_t    nMinDistance,
                                   uint8_t     *pTempBufferMain
                                   )
{
    int32_t     i, j, k;
    int32_t     xx, yy;
    int32_t     nBordery, nBorderx;
    int32_t     nWinx, nWiny;
    int32_t     nDistx, nDisty;
    int16_t     *ptr_score;
    int32_t     nMax;
    uint16_t    nIndx;
    int32_t     nScoreMax, nScoreVal;
    int32_t     nAlign;
    uint8_t     *pTempBuffer, nTemp;

    k = 0;
    nMax = 0;
    nIndx = 0;

    nBordery = WINDOW_SIZE_Y >> 1;
    nBorderx = WINDOW_SIZE_X >> 1;
    nWinx = WINDOW_SIZE_X >> 1;
    nWiny = WINDOW_SIZE_Y >> 1;
    ptr_score = pScore;
    nDistx = nMinDistance;
    nDisty = nMinDistance;
    pTempBuffer = pTempBufferMain + 4;
    /* Threshold the corner scores */
    for (i = 0; i < nRows; i++)
    {
        for (j = 0; j < nCols; j++)
        {
            nScoreVal = (int32_t) pScore[i * nCols + j];
            nTemp = nScoreVal > nThreshold ? 1 : 0;    /* To do make it greater than equal to */
            pTempBuffer[i * nCols + j] = nTemp &  pTempBuffer[i * nCols + j];
        }
    }

    /*
    if the pixel is the maximum in the NxN window
    then it's a corner. Suprress all its neighbours to zero
    to enforce a minimum distance between corners
    */
    nIndx = 0;

    for (i = 0; i < nRows; i++)
    {
        for (j = 0; j < nCols; j++)
        {
            if (pTempBuffer[i * nCols + j] == 1)
            {
                nScoreVal = pScore[i * nCols + j];

                /* find maximum in the NxN neighbourhood */
                nMax = 0;

                for (yy = -nWiny; yy <= nWiny; yy++)
                {
                    for (xx = -nWinx; xx <= nWinx; xx++)
                    {
                        nScoreMax = pScore[(i + yy) * nCols + (j + xx)];
                        if (nScoreMax > nMax)
                        {
                            nMax = nScoreMax;
                        }
                    }
                }

                if (nMax == nScoreVal)
                {
                    *(pGoodFeaturesCoord++) = j;
                    *(pGoodFeaturesCoord++) = i;

                    nAlign = j % 4;
                    for (xx = 0; xx < nDistx; xx++)
                    {
                        pTempBuffer[(i) * nCols + (j + xx - nAlign)] = 0;
                    }

                    for (yy = 1; yy <= nDisty; yy++)
                    {
                        for (xx = -nDistx; xx < nDistx; xx++)
                        {
                            pTempBuffer[(i + yy) * nCols + (j + xx - nAlign)] = 0;
                        }
                    }
                    nIndx++;
                }

            }
        }

    }
    for (i = 0; i < nMinDistance; i++)
    {
         for (j = 0; j < nCols; j++)
         {
                pTempBuffer[i * nCols + j] =  pTempBuffer[(i + 1) * nCols + j];
         }
     }
     for (j = 0; j < nCols; j++)
     {
         pTempBuffer[i * nCols + j] = 1;
     }

    return (nIndx);
}

void adi_HarrisNonMaxSupresInit(
	uint8_t     *pTempBuff,
	uint32_t    nSize
	)
{
	int32_t     *pBufferPtr;
	int32_t     nVal;
	uint32_t    i;

	nVal = 0x01010101;
	nSize = nSize >> 2;
	pBufferPtr = (int32_t *)pTempBuff;
	pTempBuff += 4;
	pBufferPtr[0] = (int32_t) pTempBuff;
	pBufferPtr++;
	for (i = 0; i < nSize; i++)
	{
		pBufferPtr[i] = nVal;
	}
}

void adi_HarrisComputeCornerScore7x7(
	int16_t     *pGradx,
	int16_t     *pGrady,
	int16_t     *pHarrisScore,
	uint32_t    nCols,
	uint32_t    nRows,
	uint16_t    nSensitivity,
	int32_t     *pTemp
	)
{
	int32_t nGx, nGy;
	int32_t nGxx, nGxy, nGyy;
	int16_t nGxx_s, nGxy_s, nGyy_s;
	int32_t xx, yy;
	int16_t *pScore;

	int32_t nVal_32;
	int32_t x, y;
	int32_t nVal;

	pScore = pHarrisScore;
	for (y = 0; y <(int32_t) nRows; y++)
	{
		for (x = WINDOW_SIZE_X / 2; x < (int32_t)(nCols - (WINDOW_SIZE_X / 2)); x++)
		{
			/* Sum the gradients in the surrounding window */
			nGxx = 0;
			nGxy = 0;
			nGyy = 0;
			for (yy = y - WINDOW_SIZE_X / 2; yy <= y + WINDOW_SIZE_X / 2; yy++)
			{
				for
					(
					xx = x - WINDOW_SIZE_X / 2;
				xx <= x + WINDOW_SIZE_X / 2;
				xx++
					)
				{
					nGx = *(pGradx + nCols * yy + xx);
					nGy = *(pGrady + nCols * yy + xx);
					nGxx += nGx * nGx;
					nGxy += nGx * nGy;
					nGyy += nGy * nGy;
				}
			}

			/* Sum the gradients in the surrounding window */
			nGxx_s = (int16_t) (SAT(nGxx, SCOREMIN, SCOREMAX));
			nGyy_s = (int16_t) (SAT(nGyy, SCOREMIN, SCOREMAX));
			nGxy_s = (int16_t) (SAT(nGxy, SCOREMIN, SCOREMAX));

			nVal = harris_corner_score(nGxx_s,
				nGxy_s,
				nGyy_s,
				nSensitivity);

			nVal_32 = (int32_t) nVal;
			nVal_32 = nVal_32 >> SHIFT_VALUE;   /* alternatively divide */
			*(pScore + y * nCols + x) = nVal_32;
		}
	}

	return;
}

static int32_t harris_corner_score(
	int16_t nGxx,
	int16_t nGxy,
	int16_t nGyy,
	int16_t nSenstivity
	)
{
	int32_t a;
	int32_t b;
	int32_t c;
	int16_t c1;
	int32_t d, e;

	a = nGxx * nGyy;
	b = nGxy * nGxy;
	c = (nGxx + nGyy);

	c1 = SAT(c, SCOREMIN, SCOREMAX);

	d = (int32_t) ((nSenstivity * c1) >> 16);

	e = (int32_t) (d * c1);

	return (int32_t) ((a - b) - e);
}

void adi_MemCopy(
	uint8_t     *pDestBuff,
	uint8_t     *pSrcBuff,
	uint32_t    nBufSize
	)
{
	uint32_t    i, nLoopCount;
	uint32_t    nResult;
	uint32_t    *pDst, *pSrc;
	pDst = (uint32_t *)pDestBuff;
	pSrc = (uint32_t *)pSrcBuff;
	nLoopCount = nBufSize >> 2;
	for (i = 0; i < nLoopCount; i++)
	{
		nResult = pSrc[i];
		pDst[i] = nResult;
	}
}

void adi_ImageDownScaleby4(
	const uint8_t   *pInBuff,
	uint32_t        dimY,
	uint32_t        dimX,
	uint8_t         *pOutBuff
	)
{
	uint8_t     *pFirstRow, *pThirdRow, *pOut;
	uint32_t    i, j, k;
	uint32_t    nColSum[4];
	uint32_t    nRowSum;
	uint8_t     nResult;

	pFirstRow = (uint8_t *)pInBuff;
	pThirdRow = pFirstRow + (dimX << 1);
	pOut = pOutBuff;

	for (i = 0; i < ((dimY) >> 2); i++)
	{
		for (j = 0; j < ((dimX) >> 2); j++)
		{
			for (k = 0; k < 4; k++)
			{
				nColSum[k] = *pFirstRow++;
				nColSum[k] += *pThirdRow++;
				nColSum[k] += 1;
				nColSum[k] = nColSum[k] >> 1;
			}

			nRowSum = nColSum[0] + nColSum[1] + nColSum[2] + nColSum[3];
			nRowSum += 2;
			nRowSum = nRowSum >> 2;
			nResult = (uint8_t) nRowSum;
			*pOut++ = nResult;
		}

		pFirstRow += (dimX * 3);
		pThirdRow += (dimX * 3);
	}
}

void adi_conv2D5by5_8(
	const uint8_t   pInBuff[],
	uint32_t        dimY,
	uint32_t        dimX,
	const int16_t   pMask[],
	uint8_t         pOutBuff[],
	uint32_t        nShift
	)
{
	int16_t     temp_mask[25];
	uint8_t     *pInMatrix;
	uint32_t    i, j, n, k;
	int32_t     nRes;
	n = 0;
	k = 0;

	/* The pMask need to be inverted so as to directly apply it over the image, for convolution */
	for (i = 0U; i < 25U; i++)
	{
		temp_mask[i] = pMask[25U - i - 1U];
	}

	for (i = 0; i < (dimY - 4); i++)
	{
		pInMatrix = (uint8_t *)pInBuff;
		nRes = 0;
		for (j = 0; j < 5; j++)
		{
			nRes += (pInMatrix[0]) * (temp_mask[j * 5]);
			nRes += (pInMatrix[0]) * (temp_mask[(j * 5) + 1]);
			for (k = 0; k < 3; k++)
			{
				nRes += (pInMatrix[k]) * (temp_mask[j * 5 + k + 2]);
			}

			pInMatrix += dimX;
		}

		if (nShift == 15)
		{
			nRes = RND_C(nRes);
		}

		if (nRes >= 255)
		{
			nRes = 255;
		}
		else if (nRes < 0)
		{
			nRes = 0;
		}
		else
		{
		}

		*pOutBuff++ = (uint8_t) nRes;       /* first element in row */

		pInMatrix = (uint8_t *)pInBuff;
		nRes = 0;
		for (j = 0; j < 5; j++)
		{
			nRes += (pInMatrix[0]) * (temp_mask[j * 5]);
			for (k = 0; k < 4; k++)
			{
				nRes += (pInMatrix[k]) * (temp_mask[j * 5 + k + 1]);
			}

			pInMatrix += dimX;
		}

		if (nShift == 15)
		{
			nRes = RND_C(nRes);
		}

		if (nRes >= 255)
		{
			nRes = 255;
		}
		else if (nRes < 0)
		{
			nRes = 0;
		}
		else
		{
		}

		*pOutBuff++ = (uint8_t) nRes;       /* second element in row */

		for (n = 0; n < (dimX - 4); n++)
		{
			nRes = 0;
			pInMatrix = (uint8_t *)pInBuff + n;
			for (j = 0; j < 5; j++)
			{
				for (k = 0; k < 5; k++)
				{
					nRes += (pInMatrix[k]) * (temp_mask[j * 5 + k]);
				}

				pInMatrix += dimX;
			}

			if (nShift == 15)
			{
				nRes = RND_C(nRes);
			}

			if (nRes >= 255)
			{
				nRes = 255;
			}
			else if (nRes < 0)
			{
				nRes = 0;
			}
			else
			{
			}

			*pOutBuff++ = (uint8_t) nRes;   /* middle elements in row */
		}

		pInMatrix = (uint8_t *)pInBuff + n;

		nRes = 0;
		for (j = 0; j < 5; j++)
		{
			for (k = 0; k < 4; k++)
			{
				nRes += (pInMatrix[k]) * (temp_mask[j * 5 + k]);
			}

			nRes += (pInMatrix[k - 1]) * (temp_mask[j * 5 + k]);
			pInMatrix += dimX;
		}

		if (nShift == 15)
		{
			nRes = RND_C(nRes);
		}

		if (nRes >= 255)
		{
			nRes = 255;
		}
		else if (nRes < 0)
		{
			nRes = 0;
		}
		else
		{
		}

		*pOutBuff++ = (uint8_t) nRes;       /* second last elements in row */

		nRes = 0;
		pInMatrix = (uint8_t *)pInBuff + n + 1;
		for (j = 0; j < 5; j++)
		{
			for (k = 0; k < 3; k++)
			{
				nRes += (pInMatrix[k]) * (temp_mask[j * 5 + k]);
			}

			nRes += (pInMatrix[k - 1]) * (temp_mask[j * 5 + k]);
			nRes += (pInMatrix[k - 1]) * (temp_mask[j * 5 + k + 1]);
			pInMatrix += dimX;
		}

		if (nShift == 15)
		{
			nRes = RND_C(nRes);
		}

		if (nRes >= 255)
		{
			nRes = 255;
		}
		else if (nRes < 0)
		{
			nRes = 0;
		}
		else
		{
		}

		*pOutBuff++ = (uint8_t) nRes;       /* last elements in row */
		pInBuff = pInBuff + dimX;
	}
}

void adi_TrackFeatures_i8(
                          const ADI_PYR_OPTFL_IMAGE_FEAT_PTR  poPyrOptflImgAFeatures,
                          const ADI_PYR_OPTFL_IMAGE_FEAT_PTR  poPyrOptflImgBFeatures,
                          uint16_t                            nNumGoodFeatures,
                          ADI_PYR_OPTFL_GD_FEAT_PTR           poGoodFeatures,
                          uint8_t                             nWindowSize,
                          uint8_t                             nMaxIteration,
                          int32_t                             nMinDisplacement,
                          int16_t                             *pL1Buff
                          )
{
    PYR_OPTFL_INTERPOLATE   oPyrOptflInterpolate;
    uint32_t                nLocX;
    uint32_t                nLocY;
    uint32_t                nLocXout;
    uint32_t                nLocYout;
    uint16_t                nIndex;

    uint32_t                nImageWidth;
    uint32_t                nImageHeight;
    uint8_t                 *pImgA;
    uint8_t                 *pImgAPyr;
    int16_t                 *pGradXImgA;
    int16_t                 *pGradYImgA;
    int16_t                 *pGradXPyrA;
    int16_t                 *pGradYPyrA;
    uint8_t                 *pImgB;
    uint8_t                 *pImgBPyr;
    int16_t                 *pGradXImgB;
    int16_t                 *pGradYImgB;
    int16_t                 *pGradXPyrB;
    int16_t                 *pGradYPyrB;

    int16_t                 *pImgDiffL1;
    int16_t                 *pGradXSumL1;
    int16_t                 *pGradYSumL1;

    uint8_t                 nNumIteration;

    int32_t                 aParameter[5];
    int32_t                 dx;
    int32_t                 dy;
    uint8_t                 bOutOfBound;

    /* +1 in the size is to make sure all the L1 buffers are word aligned */
    pImgDiffL1 = pL1Buff + (((nWindowSize * nWindowSize) + 1) * 3);
    pGradXSumL1 = pImgDiffL1 + ((nWindowSize * nWindowSize) + 1);
    pGradYSumL1 = pGradXSumL1 + ((nWindowSize * nWindowSize) + 1);

    nImageWidth = poPyrOptflImgAFeatures->nImageWidth;
    nImageHeight = poPyrOptflImgAFeatures->nImageHeight;
    pImgA = poPyrOptflImgAFeatures->pSmoothImg;
    pImgAPyr = poPyrOptflImgAFeatures->pImgPyr;
    pGradXImgA = poPyrOptflImgAFeatures->pGradXImg;
    pGradYImgA = poPyrOptflImgAFeatures->pGradYImg;
    pGradXPyrA = poPyrOptflImgAFeatures->pGradXPyr;
    pGradYPyrA = poPyrOptflImgAFeatures->pGradYPyr;
    pImgB = poPyrOptflImgBFeatures->pSmoothImg;
    pImgBPyr = poPyrOptflImgBFeatures->pImgPyr;
    pGradXImgB = poPyrOptflImgBFeatures->pGradXImg;
    pGradYImgB = poPyrOptflImgBFeatures->pGradYImg;
    pGradXPyrB = poPyrOptflImgBFeatures->pGradXPyr;
    pGradYPyrB = poPyrOptflImgBFeatures->pGradYPyr;

    for (nIndex = 0; nIndex < nNumGoodFeatures; nIndex++)
    {

        bOutOfBound = 0;

        /* Only track features that are not lost */
        if ((poGoodFeatures[nIndex]).nVal_StatusCode >= 0)
        {
            nLocX = (poGoodFeatures[nIndex]).nCoordX;
            nLocY = (poGoodFeatures[nIndex]).nCoordY;

            /* If out of bounds, exit this iteration */
            if
            (
                ((nLocX + (nWindowSize / 2)) > nImageWidth)
            ||  ((nLocY + (nWindowSize / 2)) > nImageHeight)
            )
            {
                (poGoodFeatures[nIndex]).nVal_StatusCode = ((ADI_ITB_MODULE_PYR_OPTFL << 16) | ADI_ITB_STATUS_PYR_OPTFL_OOB);
                (poGoodFeatures[nIndex]).nNewCoordX = (uint16_t) (-1);
                (poGoodFeatures[nIndex]).nNewCoordY = (uint16_t) (-1);
                bOutOfBound = 1;
            }

            if(bOutOfBound == 0)
            {
                /* For top level pyramid */

                /*
                Divide location coordinates by 4 (subsampling value is 4)
                Put the number in 16.16 format.
                */
                nLocX <<= 14;   /* Convert the value in 16.16 format and then divide by 4 */
                nLocY <<= 14;
                nLocXout = nLocX;
                nLocYout = nLocY;

                /* Track feature at current resolution */
                nNumIteration = 0;

                do
                {
                    CompIntDiffGradSum(pImgAPyr,
                                       pGradXPyrA,
                                       pGradYPyrA,
                                       pImgBPyr,
                                       pGradXPyrB,
                                       pGradYPyrB,
                                       nImageWidth >> 2,
                                       nImageHeight >> 2,
                                       nNumIteration,
                                       nWindowSize,
                                       &oPyrOptflInterpolate,
                                       nLocX,
                                       nLocY,
                                       nLocXout,
                                       nLocYout,
                                       pL1Buff);

                    /*
                    aParameter array will get the output in following order
                    nGradXX, nGradXY, nGradYY, nErrorX, nErrorY
                    */
                    CompGradMatErrVect(pGradXSumL1,
                                       pGradYSumL1,
                                       pImgDiffL1,
                                       (nWindowSize * nWindowSize),
                                       aParameter);
                    Solve_Eqn(aParameter,
                              &dx,
                              &dy);

                    nLocXout += dx;
                    nLocYout += dy;

                    nNumIteration++;

                    dx = ABS_16_16(dx);
                    dy = ABS_16_16(dy);
                } while
                (
                    ((dx >= nMinDisplacement) || (dy >= nMinDisplacement))
                &&  (nNumIteration < nMaxIteration)
                );

                /* For bottom level pyramid (base level) */
                nLocX <<= 2;    /* Multiply by 4 */
                nLocY <<= 2;
                nLocXout <<= 2;
                nLocYout <<= 2;

                /* If out of bounds, exit this iteration */
                if
                (
                    (nLocXout > ((nImageWidth - (nWindowSize / 2)) << 16))
                ||  (nLocYout > ((nImageHeight - (nWindowSize / 2)) << 16))
                )
                {
                    (poGoodFeatures[nIndex]).nVal_StatusCode = ((ADI_ITB_MODULE_PYR_OPTFL << 16) | ADI_ITB_STATUS_PYR_OPTFL_OOB);
                    (poGoodFeatures[nIndex]).nNewCoordX = (uint16_t) (-1);
                    (poGoodFeatures[nIndex]).nNewCoordY = (uint16_t) (-1);
                    bOutOfBound = 1;
                }

                if(bOutOfBound == 0)
                {
                    /* Track feature at current resolution */
                    nNumIteration = 0;
                    do
                    {
                        CompIntDiffGradSum(pImgA,
                                           pGradXImgA,
                                           pGradYImgA,
                                           pImgB,
                                           pGradXImgB,
                                           pGradYImgB,
                                           nImageWidth,
                                           nImageHeight,
                                           nNumIteration,
                                           nWindowSize,
                                           &oPyrOptflInterpolate,
                                           nLocX,
                                           nLocY,
                                           nLocXout,
                                           nLocYout,
                                           pL1Buff);

                        CompGradMatErrVect(pGradXSumL1,
                                           pGradYSumL1,
                                           pImgDiffL1,
                                           (nWindowSize * nWindowSize),
                                           aParameter);
                        Solve_Eqn(aParameter,
                                  &dx,
                                  &dy);

                        nLocXout += dx;
                        nLocYout += dy;

                        nNumIteration++;

                        dx = ABS_16_16(dx);
                        dy = ABS_16_16(dy);
                    } while
                    (
                        ((dx >= nMinDisplacement) || (dy >= nMinDisplacement))
                    &&  (nNumIteration < nMaxIteration)
                    );

                    /* If max iteration reached */
                    if (nNumIteration >= nMaxIteration)
                    {
                        (poGoodFeatures[nIndex]).nVal_StatusCode = ((ADI_ITB_MODULE_PYR_OPTFL << 16) | ADI_ITB_STATUS_PYR_OPTFL_MAXITR);
                    }

                    /* If out of bounds */
                    if
                    (
                        (nLocXout > ((nImageWidth - (nWindowSize / 2)) << 16))
                    ||  (nLocYout > ((nImageHeight - (nWindowSize / 2)) << 16))
                    )
                    {
                        (poGoodFeatures[nIndex]).nVal_StatusCode = ((ADI_ITB_MODULE_PYR_OPTFL << 16) | ADI_ITB_STATUS_PYR_OPTFL_OOB);
                        (poGoodFeatures[nIndex]).nNewCoordX = (uint16_t) (-1);
                        (poGoodFeatures[nIndex]).nNewCoordY = (uint16_t) (-1);
                    }

                    (poGoodFeatures[nIndex]).nVal_StatusCode = ((ADI_ITB_MODULE_PYR_OPTFL << 16) | ADI_ITB_STATUS_SUCCESS);
                    (poGoodFeatures[nIndex]).nNewCoordX = Cnvrt16_16to16_0(nLocXout);
                    (poGoodFeatures[nIndex]).nNewCoordY = Cnvrt16_16to16_0(nLocYout);
                }
            }
        }
        else                /* for features which are lost */
        {
            (poGoodFeatures[nIndex]).nVal_StatusCode = ((ADI_ITB_MODULE_PYR_OPTFL << 16) | ADI_ITB_STATUS_PYR_OPTFL_LOST);
            (poGoodFeatures[nIndex]).nNewCoordX = (uint16_t) (-1);
            (poGoodFeatures[nIndex]).nNewCoordY = (uint16_t) (-1);
        }
    }
}

static void CompIntDiffGradSum(
                               uint8_t                     *pImgA,
                               int16_t                     *pGradXA,
                               int16_t                     *pGradYA,
                               uint8_t                     *pImgB,
                               int16_t                     *pGradXB,
                               int16_t                     *pGradYB,
                               uint32_t                    nImageWidth,
                               uint32_t                    nImageHeight,
                               uint8_t                     nNumIteration,
                               uint8_t                     nWindowSize,
                               PYR_OPTFL_INTERPOLATE_PTR   pPyrOptflInterpolate,
                               uint32_t                    nLocX,
                               uint32_t                    nLocY,
                               uint32_t                    nLocXout,
                               uint32_t                    nLocYout,
                               int16_t                     *pL1Buff
                               )
{
    int16_t     *pImgIntAL1;
    int16_t     *pGradXAL1;
    int16_t     *pGradYAL1;
    int16_t     *pImgDiffL1;
    int16_t     *pGradXSumL1;
    int16_t     *pGradYSumL1;

    uint16_t    nLocXInteger;       /* Integral part of location x */
    uint16_t    nLocYInteger;       /* Integral part of location y */
    uint16_t    nLocXoutInteger;    /* Integral part of location x out */
    uint16_t    nLocYoutInteger;    /* Integral part of location y out */
    uint32_t    nLocXFract;         /* Fract part of location x in (x, 1-x) 1.15 format */
    uint32_t    nLocYFract;         /* Fract part of location y in (y, 1-y) 1.15 format */
    uint32_t    nLocXoutFract;      /* Fract part of location x out in (x, 1-x) 1.15 format */
    uint32_t    nLocYoutFract;      /* Fract part of location y out in (y, 1-y) 1.15 format */

    uint8_t     nIndexX;
    uint8_t     nIndexY;

    pImgIntAL1 = pL1Buff;

    /* +1 in the size is to make sure all the L1 buffers are word aligned */
    pGradXAL1 = pImgIntAL1 + ((nWindowSize * nWindowSize) + 1U);
    pGradYAL1 = pGradXAL1 + ((nWindowSize * nWindowSize) + 1U);
    pImgDiffL1 = pGradYAL1 + ((nWindowSize * nWindowSize) + 1U);
    pGradXSumL1 = pImgDiffL1 + ((nWindowSize * nWindowSize) + 1U);
    pGradYSumL1 = pGradXSumL1 + ((nWindowSize * nWindowSize) + 1U);

    nLocXInteger = (uint16_t)(nLocX >> 16);
    nLocYInteger = (uint16_t)(nLocY >> 16);
    nLocXoutInteger = (uint16_t)(nLocXout >> 16);
    nLocYoutInteger = (uint16_t)(nLocYout >> 16);

    nLocXFract = Cnvrt16_16to1_15_1_15_1minusX_X(nLocX);
    nLocYFract = Cnvrt16_16to1_15_1_15_1minusX_X(nLocY);
    nLocXoutFract = Cnvrt16_16to1_15_1_15_1minusX_X(nLocXout);
    nLocYoutFract = Cnvrt16_16to1_15_1_15_1minusX_X(nLocYout);

    if (nNumIteration == 0U)
    {

        /* First iteration, interpolate image 1 also */

        /* locations of top left corner of the block */
        pPyrOptflInterpolate->pImg = (pImgA + (((nLocYInteger - (nWindowSize / 2)) * nImageWidth) + (nLocXInteger - (nWindowSize / 2))));
        pPyrOptflInterpolate->pGradX = (pGradXA + (((nLocYInteger - (nWindowSize / 2)) * nImageWidth) + (nLocXInteger - (nWindowSize / 2))));
        pPyrOptflInterpolate->pGradY = (pGradYA + (((nLocYInteger - (nWindowSize / 2)) * nImageWidth) + (nLocXInteger - (nWindowSize / 2))));
        pPyrOptflInterpolate->nCoeffX = nLocXFract;
        pPyrOptflInterpolate->nCoeffY = nLocYFract;
        pPyrOptflInterpolate->nImageWidth = nImageWidth;
        pPyrOptflInterpolate->nBlockWidth = nWindowSize;    /* window is always a square window */
        pPyrOptflInterpolate->nBlockHeight = nWindowSize;
        pPyrOptflInterpolate->pImgOut = (uint8_t *)pImgIntAL1;
        pPyrOptflInterpolate->pGradXOut = pGradXAL1;
        pPyrOptflInterpolate->pGradYOut = pGradYAL1;
        OpticalFlow_Interpolate(pPyrOptflInterpolate);
    }

    /* Interpolate image 2 */

    /* locations of top left corner of the block */
    pPyrOptflInterpolate->pImg = (pImgB + (((nLocYoutInteger - (nWindowSize/2)) * nImageWidth) + (nLocXoutInteger - (nWindowSize/2))));
    pPyrOptflInterpolate->pGradX = (pGradXB + (((nLocYoutInteger - (nWindowSize/2)) * nImageWidth) + (nLocXoutInteger - (nWindowSize/2))));
    pPyrOptflInterpolate->pGradY = (pGradYB + (((nLocYoutInteger - (nWindowSize/2)) * nImageWidth) + (nLocXoutInteger - (nWindowSize/2))));
    pPyrOptflInterpolate->nCoeffX = nLocXoutFract;
    pPyrOptflInterpolate->nCoeffY = nLocYoutFract;
    pPyrOptflInterpolate->nImageWidth = nImageWidth;
    pPyrOptflInterpolate->nBlockWidth = nWindowSize;        /* window is always a square window */
    pPyrOptflInterpolate->nBlockHeight = nWindowSize;

    /*
    pass offset address of pImgIntAL1 itself in second image too. Since output
    will be of byte size and buffer is half word size so we have enough space
    in pImgIntAL1 itself to store intensity of both images. pImgDiffL1 buffer
    will be used to store the differences, which will be of half word size.
    */
    pPyrOptflInterpolate->pImgOut = ((uint8_t *)pImgIntAL1) + ((nWindowSize * nWindowSize) + 1);
    pPyrOptflInterpolate->pGradXOut = pGradXSumL1;
    pPyrOptflInterpolate->pGradYOut = pGradYSumL1;
    OpticalFlow_Interpolate(pPyrOptflInterpolate);

    for (nIndexY = 0; nIndexY < nWindowSize; nIndexY++)
    {
        for (nIndexX = 0; nIndexX < nWindowSize; nIndexX++)
        {

            /*
            Find image intensity difference. Both the image intensities are
            stored in pImgIntAL1 buffer and are of uint8_t type. Store the
            differences in pImgDiffL1 which is of int16_t type.
            */
            *(pImgDiffL1 + ((nIndexY * nWindowSize) + nIndexX)) =
                        (int16_t)(*((uint8_t *)(pImgIntAL1) + ((nIndexY * nWindowSize) + nIndexX)))
                        - (int16_t)(*(((uint8_t *)pImgIntAL1) + ((nWindowSize  * nWindowSize) + 1) + ((nIndexY * nWindowSize) + nIndexX)));
            /* Find GradX sums */
            *(pGradXSumL1 + ((nIndexY * nWindowSize) + nIndexX)) =
                (*(pGradXAL1 + ((nIndexY * nWindowSize) + nIndexX))) +
                (*(pGradXSumL1 + ((nIndexY * nWindowSize) + nIndexX)));

            /* Find GradY sums */
            *(pGradYSumL1 + ((nIndexY * nWindowSize) + nIndexX)) =
                (*(pGradYAL1 + ((nIndexY * nWindowSize) + nIndexX))) +
                (*(pGradYSumL1 + ((nIndexY * nWindowSize) + nIndexX)));
        }
    }
}

static void CompGradMatErrVect(
	int16_t *pGradXSumL1,
	int16_t *pGradYSumL1,
	int16_t *pImgDiffL1,
	int16_t nLoopCount,
	int32_t *pOutParam
	)
{
	int32_t i;
	int32_t gradxx;
	int32_t gradyy;
	int32_t gradxy;
	int32_t errorx;
	int32_t errory;
	int16_t mx, my, intdiff;
	gradxx = 0;
	gradyy = 0;
	gradxy = 0;
	errorx = 0;
	errory = 0;

	for (i = 0; i < nLoopCount; i++)
	{
		mx = *pGradXSumL1++;
		my = *pGradYSumL1++;
		intdiff = *pImgDiffL1++;
		gradxx += mx * mx;
		gradyy += my * my;
		gradxy += mx * my;
		errorx += mx * intdiff;
		errory += intdiff * my;
	}

	*pOutParam++ = gradxx;
	*pOutParam++ = gradxy;
	*pOutParam++ = gradyy;
	*pOutParam++ = errorx;
	*pOutParam = errory;
}

static void Solve_Eqn(
	int32_t *pParams,
	int32_t *pXResult,
	int32_t *pYResult
	)
{
	int32_t     nGxx, nGxy, nGyy, nEx, nEy;
	int64_t     nDet, nDx, nDy, nTemp;
	int16_t     nExponent;
	int16_t     nMantissa;
	float16     nDetInFL16;
	int32_t     nTempVal1;
	int32_t     nTempVal2;
	float16     nOneInFL16;
	float16     nOneByDet;
	int16_t     nMultiply;
	int16_t     nExp;
	int64_t     nResult;
	int32_t     nShiftValue;
	uint32_t    nActualShift;
	uint32_t    nSplResult;

	nExponent = 0;

	nGxx = pParams[0];
	nGxy = pParams[1];
	nGyy = pParams[2];
	nEx = pParams[3];
	nEy = pParams[4];

	nDet = (int64_t) nGxx * nGyy;
	nTemp = (int64_t) nGxy * nGxy;
	nDet = nDet - nTemp;

	nDx = (int64_t)nGyy * nEx;
	nTemp = (int64_t)nGxy * nEy;
	nDx = nDx - nTemp;

	nDy = (int64_t)nGxx * nEy;
	nTemp = (int64_t)nGxy * nEx;
	nDy = nDy - nTemp;

	/* Converting nDet to Float 16 */
	nTemp = nDet;
	while ((nTemp != 0) && (nTemp != -1))
	{
		nTemp = nTemp >> 1;
		nExponent++;
	}

	nTemp = nDet;
	nShiftValue = 64 - nExponent;
	nTemp = (nTemp << (nShiftValue - 1));
	nMantissa = (int16_t) (nTemp >> 48);
	nTempVal2 = nMantissa;
	nTempVal1 = nExponent;
	nTempVal2 = nTempVal2 << 16;
	nTempVal1 = nTempVal1 + nTempVal2;
	nDetInFL16.l = nTempVal1;
	nOneInFL16.l = 0x40000001;
	nOneByDet = adi_FUNC_DIV_FL16(nOneInFL16,
		nDetInFL16);
	nExp = (int16_t) (nOneByDet.l);
	nMultiply = (int16_t) (nOneByDet.l >> 16);
	if (nExp >= -32)
	{
		nResult = nMultiply * nDx;
		nActualShift = (uint32_t)(-nExp);
		nResult = nResult >> nActualShift;
		*pXResult = (int32_t) nResult;

		nResult = nMultiply * nDy;
		nResult = nResult >> nActualShift;
		*pYResult = (int32_t) nResult;
	}
	else
	{
		nExp += 16;
		nActualShift = (uint32_t)(-nExp);
		nSplResult = ((uint16_t)nDx * nMultiply);
		nSplResult = nSplResult >> 16;
		nResult = nDx >> 16;
		nResult = nMultiply * nResult;
		nResult += nSplResult;
		nResult = nResult >> nActualShift;
		nResult <<= 1;  /* To remove redundant sign bit */
		*pXResult = (int32_t) nResult;

		nSplResult = ((uint16_t)nDy * nMultiply);
		nSplResult = nSplResult >> 16;
		nResult = nDy >> 16;
		nResult = nMultiply * nResult;
		nResult += nSplResult;
		nResult = nResult >> nActualShift;
		nResult <<= 1;  /* To remove redundant sign bit */
		*pYResult = (int32_t) nResult;
	}
}

static void OpticalFlow_Interpolate(
	PYR_OPTFL_INTERPOLATE   *poInterPolate
	)
{
	uint32_t    nBlockWidth, nBlockHeight;
	uint32_t    nImageWidth;
	uint8_t     *pImageOut;
	uint8_t     *pImage;
	int32_t     nCoeffX, nCoeffY;
	int16_t     *pGradX;
	int16_t     *pGradY;
	int16_t     *pGradXOut;
	int16_t     *pGradYOut;
	uint8_t     *pImageNextRow;
	int16_t     *pGradXNextRow;
	int16_t     *pGradYNextRow;
	int16_t     nX, nXC, nY, nYC;
	int32_t     nTemp1, nTemp2;
	int16_t     nTemp3, nTemp4;
	int32_t     nOffset;
	uint32_t    i, j;

	/* Populate Data */
	nBlockWidth = poInterPolate->nBlockWidth;
	nBlockHeight = poInterPolate->nBlockHeight;
	nImageWidth = poInterPolate->nImageWidth;
	nCoeffX = poInterPolate->nCoeffX;
	nCoeffY = poInterPolate->nCoeffY;
	pImage = poInterPolate->pImg;
	pImageOut = poInterPolate->pImgOut;
	pGradX = poInterPolate->pGradX;
	pGradY = poInterPolate->pGradY;
	pGradXOut = poInterPolate->pGradXOut;
	pGradYOut = poInterPolate->pGradYOut;
	pImageNextRow = pImage + nImageWidth;
	pGradXNextRow = pGradX + nImageWidth;
	pGradYNextRow = pGradY + nImageWidth;

	nX = (int16_t) nCoeffX;
	nY = (int16_t) nCoeffY;
	nXC = (int16_t) (nCoeffX >> 16);
	nYC = (int16_t) (nCoeffY >> 16);
	nOffset = nImageWidth - nBlockWidth;

	for (i = 0; i < nBlockHeight; i++)
	{
		for (j = 0; j < nBlockWidth; j++)
		{
			nTemp1 = ASM_MUL16(pImage[0], nXC);
			nTemp1 = ROUND2POS(nTemp1, SCALE_FACTOR);
			nTemp1 = nTemp1 >> SCALE_FACTOR;

			nTemp2 = ASM_MUL16(pImage[1], nX);
			nTemp2 = ROUND2POS(nTemp2, SCALE_FACTOR);
			nTemp2 = nTemp2 >> SCALE_FACTOR;

			nTemp3 = nTemp1 + nTemp2;

			nTemp1 = ASM_MUL16(pImageNextRow[0], nXC);
			nTemp1 = ROUND2POS(nTemp1, SCALE_FACTOR);
			nTemp1 = nTemp1 >> SCALE_FACTOR;

			nTemp2 = ASM_MUL16(pImageNextRow[1], nX);
			nTemp2 = ROUND2POS(nTemp2, SCALE_FACTOR);
			nTemp2 = nTemp2 >> SCALE_FACTOR;

			nTemp4 = nTemp1 + nTemp2;

			nTemp1 = ASM_MUL16(nTemp3, nYC);
			nTemp1 = ROUND2POS(nTemp1, SCALE_FACTOR);
			nTemp1 = nTemp1 >> SCALE_FACTOR;

			nTemp2 = ASM_MUL16(nTemp4, nY);
			nTemp2 = ROUND2POS(nTemp2, SCALE_FACTOR);
			nTemp2 = nTemp2 >> SCALE_FACTOR;

			nTemp3 = nTemp1 + nTemp2;
			*pImageOut++ = (uint8_t)nTemp3;
			pImage++;
			pImageNextRow++;

			nTemp1 = ASM_MUL16(pGradX[0], nXC);
			nTemp1 = ROUND2POS(nTemp1, SCALE_FACTOR);
			nTemp1 = nTemp1 >> SCALE_FACTOR;

			nTemp2 = ASM_MUL16(pGradX[1], nX);
			nTemp2 = ROUND2POS(nTemp2, SCALE_FACTOR);
			nTemp2 = nTemp2 >> SCALE_FACTOR;

			nTemp3 = nTemp1 + nTemp2;

			nTemp1 = ASM_MUL16(pGradXNextRow[0], nXC);
			nTemp1 = ROUND2POS(nTemp1, SCALE_FACTOR);
			nTemp1 = nTemp1 >> SCALE_FACTOR;

			nTemp2 = ASM_MUL16(pGradXNextRow[1], nX);
			nTemp2 = ROUND2POS(nTemp2, SCALE_FACTOR);
			nTemp2 = nTemp2 >> SCALE_FACTOR;

			nTemp4 = nTemp1 + nTemp2;

			nTemp1 = ASM_MUL16(nTemp3, nYC);
			nTemp1 = ROUND2POS(nTemp1, SCALE_FACTOR);
			nTemp1 = nTemp1 >> SCALE_FACTOR;

			nTemp2 = ASM_MUL16(nTemp4, nY);
			nTemp2 = ROUND2POS(nTemp2, SCALE_FACTOR);
			nTemp2 = nTemp2 >> SCALE_FACTOR;

			nTemp3 = nTemp1 + nTemp2;
			*pGradXOut++ = nTemp3;
			pGradX++;
			pGradXNextRow++;

			nTemp1 = ASM_MUL16(pGradY[0], nXC);
			nTemp1 = ROUND2POS(nTemp1, SCALE_FACTOR);
			nTemp1 = nTemp1 >> SCALE_FACTOR;

			nTemp2 = ASM_MUL16(pGradY[1], nX);
			nTemp2 = ROUND2POS(nTemp2, SCALE_FACTOR);
			nTemp2 = nTemp2 >> SCALE_FACTOR;

			nTemp3 = nTemp1 + nTemp2;

			nTemp1 = ASM_MUL16(pGradYNextRow[0], nXC);
			nTemp1 = ROUND2POS(nTemp1, SCALE_FACTOR);
			nTemp1 = nTemp1 >> SCALE_FACTOR;

			nTemp2 = ASM_MUL16(pGradYNextRow[1], nX);
			nTemp2 = ROUND2POS(nTemp2, SCALE_FACTOR);
			nTemp2 = nTemp2 >> SCALE_FACTOR;

			nTemp4 = nTemp1 + nTemp2;

			nTemp1 = ASM_MUL16(nTemp3, nYC);
			nTemp1 = ROUND2POS(nTemp1, SCALE_FACTOR);
			nTemp1 = nTemp1 >> SCALE_FACTOR;

			nTemp2 = ASM_MUL16(nTemp4, nY);
			nTemp2 = ROUND2POS(nTemp2, SCALE_FACTOR);
			nTemp2 = nTemp2 >> SCALE_FACTOR;

			nTemp3 = nTemp1 + nTemp2;
			*pGradYOut++ = nTemp3;
			pGradY++;
			pGradYNextRow++;
		}

		pImage += nOffset;
		pImageNextRow += nOffset;
		pGradX += nOffset;
		pGradXNextRow += nOffset;
		pGradY += nOffset;
		pGradYNextRow += nOffset;
	}
}

int32_t adi_mult1616_1616(
	int32_t nNum1,
	int32_t nNum2
	)
{
	int32_t nResult;
	nResult = ((int64_t) nNum1 * nNum2) >> 16;
	return (nResult);
}

float16 adi_FUNC_DIV_FL16(
	float16 x,
	float16 y
	)
{
	int32_t         i;
	int32_t         niters;
	const int32_t   one = 0x40000001;
	float16         fl;
	int32_t         xneg;
	int32_t         yneg;
	uint16_t        r;
	uint16_t        d;
	uint16_t        q;
	int16_t         tmp;
	niters = 16;
	xneg = x.l < 0;
	yneg = y.l < 0;
	r = 0u;
	d = (uint16_t) x.s.m;
	q = (uint16_t) y.s.m;

	if (y.l == 0)
	{
		fl.l = NaN_fl16;
	}
	else if (y.l == one)
	{
		fl = x;
	}
	else if (x.l == y.l)
	{
		fl.l = one;
	}
	else
	{
		fl.s.e = x.s.e - y.s.e;
		if (xneg != 0)
		{
			tmp = -(int16_t) d;
			d = (uint16_t) tmp;
		}

		if (yneg != 0)
		{
			tmp = -(int16_t) q;
			q = (uint16_t) tmp;
		}

		for (i = 0; i < niters; i++)
		{
			r <<= 1;
			if (q <= d)
			{
				d -= q;
				r |= 1u;
			}

			d <<= 1;
		}

		d = r;
		if ((d & 0x8000u) != 0)
		{

			/* overflow */
			d >>= 1;
			fl.s.e++;
		}

		if (xneg != yneg)
		{
			tmp = -(int16_t) d;
			d = (uint16_t) tmp;
		}

		fl.s.m = (int16_t) d;   /* truncation  */
		if (fl.s.m != 0)
		{
			int16_t exponent;
			exponent = (int16_t) norm_fr1x16(fl.s.m);
			fl.s.m <<= exponent;
			fl.s.e -= exponent;
		}
		else
		{
			fl.s.e = 0;
		}
	}

	if (fl.s.m == (int16_t) 0x8000)
	{
		fl.s.m = 0xC000;
		fl.s.e += 1;
	}

	return (fl);
}

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/tags/release_2_4_0_imagetoolbox_140807/example/pyr_opticalflow/adi_pyr_opticalflow_wrapper.c $
**
*/
