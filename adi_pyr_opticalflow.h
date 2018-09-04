/*****************************************************************************
Copyright (c) 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 3907 $
$Date: 2010-10-27 00:17:31 +0530 (Wed, 27 Oct 2010) $

Title       : adi_pyr_opticalflow.h

Description : declarations specific to functions that finds the
              "pyramidical optical flow" between two images.

*****************************************************************************/
#ifndef __ADI_PYR_OPTICALFLOW_H__
#define __ADI_PYR_OPTICALFLOW_H__

/*=============  I N C L U D E S   =============*/
#include "adi_image_tool_box_defines.h"

/*==============  D E F I N E S  ===============*/
#define ADI_IMAGE_WIDTH         ((uint32_t) 320)
#define ADI_IMAGE_HEIGHT        ((uint32_t) 240)
#define ADI_IMAGE_SIZE          ((uint32_t) (ADI_IMAGE_WIDTH * ADI_IMAGE_HEIGHT))
#define MAX_NUM_GOOD_FEATURES   100
#define MAX_NUM_ITERATIONS      10
#define WINDOW_SIZE             7       /* window size for accumulation (it must be odd) */
#define MIN_DISPLACEMENT        0x1999  /* Min displacement = 0.1 in 0.16 format */
#define ROWS_LOST_IN_FILTER     24

/* Harris Corner */
#define HARRIS_BLOCK_SIZE       7
#define HARRIS_BLOCK_SIZE_HALF  (HARRIS_BLOCK_SIZE >> 1)
#define HARRIS_SENSTIVITY       2621
#define HARRIS_THRESHOLD        500
#define HARRIS_MINDIST          8

enum COMMAND
{
    OPTICAL_FLOW_LUCAS_KANADE_PYRAMID_8
};

/*=============  D A T A    T Y P E S   =============*/

/* ADI_PYR_OPTFL_SEL_GD_FEAT structure will hold input parameters for selecting good features */
typedef struct _adiPyrOptflSelGdFeat
{
    uint8_t                     *pImgIn;            /* Pointer to input image */
    uint8_t                     *pImgOut;           /* Pointer to output buffer */
    uint32_t                    nImageWidth;        /* Image width */
    uint32_t                    nImageHeight;       /* Image height */
    uint8_t                     *pSmoothImgOut;     /* Pointer to smoothed output image */
    int16_t                     *pGradX;            /* Pointer to gradient in x direction */
    int16_t                     *pGradY;            /* Pointer to gradient in y direction */
    int16_t                     *pScore;            /* pointer to store Harris corner score */
    uint8_t                     *pScratchBuf;       /* Pointer to scratch memory for feature detection in L3 */
    uint8_t                     *pScratchBufL1;     /* Pointer to scratch memory for feature detection in L1 */
    uint16_t                    *pFeatureCoord;     /* Pointer to feature co-ordinates */
    ADI_PYR_OPTFL_GD_FEAT_PTR   pGoodFeatures;      /* List containing good features */
    uint16_t                    nNumGoodFeatures;   /* Total number of good features returned */
    uint16_t                    nSenstivity;        /* Senstivity factore for harris corner detection */
    uint16_t                    nThreshold;         /* Threshold for harris corner detection */
    uint16_t                    nMinDistance;       /* MinDistance between 2 consecutive corners in both X and Y direction */
} ADI_PYR_OPTFL_SEL_GD_FEAT, *ADI_PYR_OPTFL_SEL_GD_FEAT_PTR;

/* PYR_OPTFL_INTERPOLATE structure will hold input parameters for interpolation while tracking features */
typedef struct _PyrOptflInterpolate
{
	uint32_t    nBlockWidth;    /* Block width */
	uint32_t    nBlockHeight;   /* Block height */
	uint8_t     *pImg;          /* Pointer to image */
	uint8_t     *pImgOut;       /* Pointer to interpolated image out */
	int16_t     *pGradXOut;     /* Pointer to interpolated gradient out in x direction */
	int16_t     *pGradYOut;     /* Pointer to interpolated gradient out in y direction */
	int16_t     *pGradX;        /* Pointer to gradient in x direction */
	int16_t     *pGradY;        /* Pointer to gradient in y direction */
	int32_t     nCoeffX;        /* Fract part of coeff X in (X, (1-X)) format (1.15, 1.15) */
	int32_t     nCoeffY;        /* Fract part of coeff Y in (X, (1-X)) format (1.15, 1.15) */
	uint32_t    nImageWidth;    /* Image width */
} PYR_OPTFL_INTERPOLATE, *PYR_OPTFL_INTERPOLATE_PTR;

/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/

/*
**  External Function Prototypes
*/
extern
void adi_sobel_3x3_horz_shift3_i8(
                                  const uint8_t   *pInBuff,
                                  uint32_t        dimY,
                                  uint32_t        dimX,
                                  int16_t         *pOutBuff
                                  );

extern
void adi_sobel_3x3_vert_shift3_i8(
                                  const uint8_t   *pInBuff,
                                  uint32_t        dimY,
                                  uint32_t        dimX,
                                  int16_t         *pOutBuff
                                  );

extern
void adi_Gs_Deriv_3x3_i8(
                         const uint8_t   *pInBuff,
                         int16_t         *pMask,
                         int32_t         dimY,
                         int32_t         dimX,
                         int16_t         *pOutBuff
                         );

extern
void adi_SelGoodFeatPreProc_i8(
                               ADI_PYR_OPTFL_SEL_GD_FEAT   *pPyrOptflSelectGoodFeatures,
                               int16_t                     *pKernelSmooth
                               );

extern
void adi_SelectGoodFeatures_Appl_i8(
                                    ADI_PYR_OPTFL_SEL_GD_FEAT   *pPyrOptflSelectGoodFeatures
                                    );

extern
void adi_TrackFeaturesImagePreProcess_i8(
                                         ADI_PYR_OPTFL_IMAGE_FEAT    *pPyrOptflImageFeatures,
                                         int16_t                     *pKernelSmooth,
                                         int16_t                     *pKernelHorzDeriv,
                                         int16_t                     *pKernelVertDeriv
                                         );

extern
void ADIHarrisCornerScoreWrapper(
                                 int16_t     *pGradX,
                                 int16_t     *pGradY,
                                 int16_t     *pOutScore,
                                 uint32_t    nImgWidth,
                                 uint32_t    nImgHeight,
                                 int16_t     nSenstivity,
                                 int32_t     *pBuffer
                                 );

extern
int16_t ADIharrisNonMaxSupresWrapper(
                                     int16_t     *pInScore,
                                     uint16_t    *pOutFeat,
                                     uint32_t    nImgWidth,
                                     uint32_t    nImgHeight,
                                     int16_t     nThreshold,
                                     int16_t     nMinDist,
                                     uint8_t     *pBuffer
                                     );

extern
void adi_harrisMarkCorners(
                           ADI_PYR_OPTFL_SEL_GD_FEAT   *pPyrOptflSelectGoodFeatures
                           );

extern
	void                    adi_TrackFeatures_i8(
	const ADI_PYR_OPTFL_IMAGE_FEAT_PTR poPyrOptflImgAFeatures,
	const ADI_PYR_OPTFL_IMAGE_FEAT_PTR poPyrOptflImgBFeatures,
	uint16_t                           nNumGoodFeatures,
	ADI_PYR_OPTFL_GD_FEAT_PTR          poGoodFeatures,
	uint8_t                            nWindowSize,
	uint8_t                            nMaxIteration,
	int32_t                            nMinDisplacement,
	int16_t                            *pL1Buff
	);

extern
	void                    adi_GaussianFilter_i8(
	const uint8_t   *pInBuff,
	uint32_t        dimY,
	uint32_t        dimX,
	const int16_t   *pKernel,
	uint8_t         *pOutBuff
	);

void                    adi_ImageDownScaleby4(
	const uint8_t   *pInBuff,
	uint32_t        dimY,
	uint32_t        dimX,
	uint8_t         *pOutBuff
	);

extern
	void                    adi_HarrisNonMaxSupresInit(
	uint8_t     *pTempBuff,
	uint32_t    nSize
	);

extern
	uint16_t                adi_HarrisNonMaxSupres7x7(
	int16_t     *pScore,
	uint16_t    *pFeatCoord,
	int32_t     nCols,
	int32_t     nRows,
	int32_t     nThreshold,
	uint16_t    nMinDistance,
	uint8_t     *pTempBuffer
	);

extern
	void                    adi_HarrisComputeCornerScore7x7(
	int16_t     *pGradx,
	int16_t     *pGrady,
	int16_t     *pHarrisScore,
	uint32_t    nCols,
	uint32_t    nRows,
	uint16_t    nSensitivity,
	int32_t     *pTemp
	);

extern
	void            adi_MemCopy(
	uint8_t     *pDestBuff,
	uint8_t     *pSrcBuff,
	uint32_t    nBufSize
	);

extern
	void                    adi_conv2D5by5_8(
	const uint8_t   *pInBuff,
	uint32_t        dimY,
	uint32_t        dimX,
	const int16_t   *pMask,
	uint8_t         *pOutBuff,
	uint32_t        nShift
	);

static int32_t harris_corner_score(
	int16_t nGxx,
	int16_t nGxy,
	int16_t nGyy,
	int16_t nSenstivity
	);

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
	);

static void CompGradMatErrVect(
	int16_t *pGradXSumL1,
	int16_t *pGradYSumL1,
	int16_t *pImgDiffL1,
	int16_t nLoopCount,
	int32_t *pOutParam
	);

static void Solve_Eqn(
	int32_t *pParams,
	int32_t *pXResult,
	int32_t *pYResult
	);

static void OpticalFlow_Interpolate(
	PYR_OPTFL_INTERPOLATE   *poInterPolate
	);
extern 
	int32_t adi_mult1616_1616(
	int32_t nNum1,
	int32_t nNum2
	);

extern
	float16         adi_FUNC_DIV_FL16(
	float16 x,
	float16 y
	);


#endif /* __ADI_PYR_OPTICALFLOW_H__ */

/*
**
** EOF: $URL: http://ipdc-autosrv1.ad.analog.com:8080/svn/ipdc-ptg/trunk/image_processing_toolbox/demo/include/adi_opticalflow.h $
**
*/
