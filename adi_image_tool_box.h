/*****************************************************************************
Copyright (c) 2009 - 2014 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 14166 $
$Date: 2014-06-24 06:17:16 +0530 (Tue, 24 Jun 2014) $

Title       : adi_image_tool_box.h

Description : APIs supported by image tool box library.

*****************************************************************************/
#ifndef __ADI_IMAGE_TOOL_BOX_H__
#define __ADI_IMAGE_TOOL_BOX_H__

/*=============  I N C L U D E S   =============*/
//#if defined _LANGUAGE_C
/* #include <float16.h> */
//#include "adi_float16.h"
//#endif /*_LANGUAGE_C  */
#include "adi_tool_chain.h"
#include "adi_image_tool_box_defines.h"
#include "adi_contour_mem_alloc.h"

/*==============  D E F I N E S  ===============*/

/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/

/*
**  External Function Prototypes
*/
#if defined(_LANGUAGE_C)
extern
void                    adi_RGB2YCbCrBT601Range16to235(
                                                       const uint8_t   *pInBuff,
                                                       uint8_t         *pOutBuff,
                                                       uint32_t        nInputPixels
                                                       );

extern
void                    adi_RGB2GRAY(
                                     const uint8_t   *pInBuff,
                                     uint8_t         *pOutBuff,
                                     uint32_t        nInputPixels
                                     );

extern
void                    adi_RGB2HSV(
                                    const uint8_t   *pInBuff,
                                    uint8_t         *pOutBuff,
                                    uint32_t        nInputPixels
                                    );

extern
void                    adi_YCbCr2RGBBT601Range16to235(
                                                       const uint8_t   *pInBuff,
                                                       uint8_t         *pOutBuff,
                                                       uint32_t        nInputPixels
                                                       );

extern
void                    adi_GRAY2RGB(
                                     const uint8_t   *pInBuff,
                                     uint8_t         *pOutBuff,
                                     uint32_t        nInputPixels
                                     );

extern
void                    adi_HSV2RGB(
                                    const uint8_t   *pInBuff,
                                    uint8_t         *pOutBuff,
                                    uint32_t        nInputPixels
                                    );

extern
void                    adi_RGB2YCbCrBT601Range0to255(
                                                      const uint8_t   *pInBuff,
                                                      uint8_t         *pOutBuff,
                                                      uint32_t        nInputPixels
                                                      );

extern
void                    adi_YCbCr2RGBBT601Range0to255(
                                                      const uint8_t   *pInBuff,
                                                      uint8_t         *pOutBuff,
                                                      uint32_t        nInputPixels
                                                      );

extern
void                    adi_RGB2XYZ(
                                    const uint8_t   *pInBuff,
                                    uint8_t         *pOutBuff,
                                    uint32_t        nInputPixels
                                    );
extern
void                    adi_YUV444toRGB888_Planar(
                                                  uint8_t  *pIn,
                                                  uint8_t  *pOut,
                                                  uint32_t  nNumPixels
                                                 );

extern
void                    adi_UYVY2RGB(
                                     const uint8_t   *pInBuff,
                                     uint8_t         *pOutBuff,
                                     uint32_t        nNumPixels
                                    );

extern
void                    adi_RGB2UYVY(
                                     const uint8_t *pInBuff,
                                     uint8_t       *pOutBuff,
                                     uint32_t      nInputPixels
                                    );

extern
void                    adi_conv2D3by3_8(
                                         const uint8_t   *pInBuff,
                                         uint32_t        dimY,
                                         uint32_t        dimX,
                                         const int16_t   *pMask,
                                         uint8_t         *pOutBuff,
                                         uint32_t        nShift
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

extern
void                    adi_conv2D3by3_16(
                                          const int16_t   *pInBuff,
                                          uint32_t        dimY,
                                          uint32_t        dimX,
                                          const int16_t   *pMask,
                                          int16_t         *pOutBuff,
                                          uint32_t        nShift
                                          );

extern
void                    adi_conv2D5by5_16(
                                          const int16_t   *pInBuff,
                                          uint32_t        dimY,
                                          uint32_t        dimX,
                                          const int16_t   *pMask,
                                          int16_t         *pOutBuff,
                                          uint32_t        nShift
                                          );

extern
void                    adi_corr2D3by3_8(
                                         const uint8_t   *pInBuff,
                                         uint32_t        dimY,
                                         uint32_t        dimX,
                                         const int16_t   *pMask,
                                         int32_t         *pOutBuff
                                         );

extern
void                    adi_corr2D5by5_8(
                                         const uint8_t   *pInBuff,
                                         uint32_t        dimY,
                                         uint32_t        dimX,
                                         const int16_t   *pMask,
                                         int32_t         *pOutBuff
                                         );

extern
void                    adi_corr2D3by3_16(
                                          const int16_t   *pInBuff,
                                          uint32_t        dimY,
                                          uint32_t        dimX,
                                          const int16_t   *pMask,
                                          int32_t         *pOutBuff
                                          );

extern
void                    adi_corr2D5by5_16(
                                          const int16_t   *pInBuff,
                                          uint32_t        dimY,
                                          uint32_t        dimX,
                                          const int16_t   *pMask,
                                          int32_t         *pOutBuff
                                          );

extern
void                    adi_erosion(
                                    const uint8_t   *pInBuff,
                                    uint32_t        dimY,
                                    uint32_t        dimX,
                                    const int16_t   *pMask,
                                    uint8_t         *pOutBuff,
                                    ADI_MASK_OPTION eMaskOption
                                    );

extern
void                    adi_dilation(
                                     const uint8_t   *pInBuff,
                                     uint32_t        dimY,
                                     uint32_t        dimX,
                                     const int16_t   *pMask,
                                     uint8_t         *pOutBuff,
                                     ADI_MASK_OPTION eMaskOption
                                     );

extern
void                    adi_morph_open(
                                       const uint8_t   *pInBuff,
                                       uint32_t        dimY,
                                       uint32_t        dimX,
                                       uint8_t         *pTempBuff,
                                       const int16_t   *pMask,
                                       uint8_t         *pOutBuff,
                                       ADI_MASK_OPTION eMaskOption
                                       );

extern
void                    adi_morph_close(
                                        const uint8_t   *pInBuff,
                                        uint32_t        dimY,
                                        uint32_t        dimX,
                                        uint8_t         *pTempBuff,
                                        const int16_t   *pMask,
                                        uint8_t         *pOutBuff,
                                        ADI_MASK_OPTION eMaskOption
                                        );

extern
void                    adi_morph_tophat(
                                         const uint8_t   *pInBuff,
                                         uint32_t        dimY,
                                         uint32_t        dimX,
                                         uint8_t         *pTempBuff,
                                         const int16_t   *pMask,
                                         uint8_t         *pOutBuff,
                                         ADI_MASK_OPTION eMaskOption
                                         );

extern
void                    adi_morph_blackhat(
                                           const uint8_t   *pInBuff,
                                           uint32_t        dimY,
                                           uint32_t        dimX,
                                           uint8_t         *pTempBuff,
                                           const int16_t   *pMask,
                                           uint8_t         *pOutBuff,
                                           ADI_MASK_OPTION eMaskOption
                                           );

extern
void                    adi_morph_gradient(
                                           const uint8_t   *pInBuff,
                                           uint32_t        dimY,
                                           uint32_t        dimX,
                                           uint8_t         *pTempBuff,
                                           const int16_t   *pMask,
                                           uint8_t         *pOutBuff,
                                           ADI_MASK_OPTION eMaskOption
                                           );

extern
void                    adi_erosion_binary(
                                           const uint8_t   *pInBuff,
                                           uint32_t        dimY,
                                           uint32_t        dimX,
                                           const int16_t   *pMask,
                                           uint8_t         *pOutBuff,
                                           ADI_MASK_OPTION eMaskOption
                                           );

extern
void                    adi_dilation_binary(
                                            const uint8_t   *pInBuff,
                                            uint32_t        dimY,
                                            uint32_t        dimX,
                                            const int16_t   *pMask,
                                            uint8_t         *pOutBuff,
                                            ADI_MASK_OPTION eMaskOption
                                            );

extern
uint32_t                adi_morph_skeleton(
                                           const uint8_t   *pInBuff,
                                           uint32_t        dimY,
                                           uint32_t        dimX,
                                           uint8_t         *pTempBuff,
                                           const int16_t   *pMask,
                                           uint8_t         *pOutBuff
                                           );

extern
void                    adi_perimeter_i8(
                                         const uint8_t   *pInBuff,
                                         uint32_t        dimY,
                                         uint32_t        dimX,
                                         uint8_t         *pOutBuff
                                         );

extern
void                    adi_perimeter_i16s(
                                           const int16_t   *pInBuff,
                                           uint32_t        dimY,
                                           uint32_t        dimX,
                                           int16_t         *pOutBuff
                                           );

extern
void                    adi_floodfill(
                                      uint8_t             *pImage,
                                      uint32_t            dimY,
                                      uint32_t            dimX,
                                      uint8_t             *pMask,
                                      int32_t             nSeedX,
                                      int32_t             nSeedY,
                                      const uint8_t       *newVal,
                                      const uint8_t       *d_lw,
                                      const uint8_t       *d_up,
                                      ADI_FLOODFILL_DATA  *pFillinfo,
                                      uint32_t            nFlags
                                      );

extern
int32_t                 adi_boundary_i8(
                                        const uint8_t   *pInBuff,
                                        uint32_t        dimY,
                                        uint32_t        dimX,
                                        uint32_t        *pCordinates,
                                        uint8_t         *pOutBuff
                                        );

extern
int32_t                 adi_boundary_i16s(
                                          const int16_t   *pInBuff,
                                          uint32_t        dimY,
                                          uint32_t        dimX,
                                          uint32_t        *pCordinates,
                                          int16_t         *pOutBuff
                                          );
extern
void                    adi_sobel_3x3_horz_i8(
                                              const uint8_t   *pInBuff,
                                              uint32_t        dimY,
                                              uint32_t        dimX,
                                              int16_t         *pOutBuff
                                              );

extern
void                    adi_sobel_3x3_vert_i8(
                                              const uint8_t   *pInBuff,
                                              uint32_t        dimY,
                                              uint32_t        dimX,
                                              int16_t         *pOutBuff
                                              );

extern
void                    adi_sobel_3x3_cross_i8(
                                               const uint8_t   *pInBuff,
                                               uint32_t        dimY,
                                               uint32_t        dimX,
                                               int16_t         *pOutBuff
                                               );

extern
void                    adi_sobel_3x3_horz_i16(
                                               const int16_t   *pInBuff,
                                               uint32_t        dimY,
                                               uint32_t        dimX,
                                               int32_t         *pOutBuff
                                               );

extern
void                    adi_sobel_3x3_vert_i16(
                                               const int16_t   *pInBuff,
                                               uint32_t        dimY,
                                               uint32_t        dimX,
                                               int32_t         *pOutBuff
                                               );

extern
void                    adi_sobel_3x3_cross_i16(
                                                const int16_t   *pInBuff,
                                                uint32_t        dimY,
                                                uint32_t        dimX,
                                                int32_t         *pOutBuff
                                                );

extern
void                    adi_sobel_5x5_horz_i8(
                                              const uint8_t   *pInBuff,
                                              uint32_t        dimY,
                                              uint32_t        dimX,
                                              int16_t         *pOutBuff
                                              );

extern
void                    adi_sobel_5x5_vert_i8(
                                              const uint8_t   *pInBuff,
                                              uint32_t        dimY,
                                              uint32_t        dimX,
                                              int16_t         *pOutBuff
                                              );

extern
void                    adi_sobel_5x5_cross_i8(
                                               const uint8_t   *pInBuff,
                                               uint32_t        dimY,
                                               uint32_t        dimX,
                                               int16_t         *pOutBuff
                                               );

extern
void                    adi_sobel_5x5_horz_i16(
                                               const int16_t   *pInBuff,
                                               uint32_t        dimY,
                                               uint32_t        dimX,
                                               int32_t         *pOutBuff
                                               );

extern
void                    adi_sobel_5x5_vert_i16(
                                               const int16_t   *pInBuff,
                                               uint32_t        dimY,
                                               uint32_t        dimX,
                                               int32_t         *pOutBuff
                                               );

extern
void                    adi_sobel_5x5_cross_i16(
                                                const int16_t   *pInBuff,
                                                uint32_t        dimY,
                                                uint32_t        dimX,
                                                int32_t         *pOutBuff
                                                );
extern
void                     adi_sobel_3x3_horz_circ_i8(
                                                   const uint8_t  *pInBuff,
                                                   uint32_t       dimY,
                                                   uint32_t       dimX,
                                                   int16_t        *pOutBuff,
                                                   uint8_t        *pCircBufferBase,
                                                   uint32_t        nCircBufferLength
                                                  );
extern
void                     adi_sobel_3x3_vert_circ_i8(
                                                   const uint8_t  *pInBuff,
                                                   uint32_t       dimY,
                                                   uint32_t       dimX,
                                                   int16_t        *pOutBuff,
                                                   uint8_t        *pCircularBufferBase,
                                                   uint32_t       nCircBufferLength
                                                  );
extern
void                     adi_sobel_3x3_cross_circ_i8(
                                                    const uint8_t *pInBuff,
                                                    uint32_t      dimY,
                                                    uint32_t      dimX,
                                                    int16_t       *pOutBuff,
                                                    uint8_t       *pCircBufferBase,
                                                    uint32_t       nCircBufferLength
                                                    );

extern
void                     adi_sobel_5x5_horz_circ_i8(
                                                   const uint8_t  *pInBuff,
                                                   uint32_t       dimY,
                                                   uint32_t       dimX,
                                                   int16_t        *pOutBuff,
                                                   uint8_t        *pCircBufferBase,
                                                   uint32_t        nCircBufferLength
                                                  );
extern
void                     adi_sobel_5x5_vert_circ_i8(
                                                   const uint8_t  *pInBuff,
                                                   uint32_t       dimY,
                                                   uint32_t       dimX,
                                                   int16_t        *pOutBuff,
                                                   uint8_t        *pCircularBufferBase,
                                                   uint32_t       nCircBufferLength
                                                  );
extern
void                     adi_sobel_5x5_cross_circ_i8(
                                                    const uint8_t *pInBuff,
                                                    uint32_t      dimY,
                                                    uint32_t      dimX,
                                                    int16_t       *pOutBuff,
                                                    uint8_t       *pCircBufferBase,
                                                    uint32_t       nCircBufferLength
                                                    );

extern
void                     adi_sobel_3x3_horz_circ_i16(
                                                    const int16_t *pInBuff,
                                                    uint32_t      dimY,
                                                    uint32_t      dimX,
                                                    int32_t       *pOutBuff,
                                                    int16_t       *pCircBufferBase,
                                                    uint32_t       nCircBufferLength
                                                    );

extern
void                     adi_sobel_3x3_vert_circ_i16(
                                                    const int16_t *pInBuff,
                                                    uint32_t      dimY,
                                                    uint32_t      dimX,
                                                    int32_t       *pOutBuff,
                                                    int16_t       *pCircBufferBase,
                                                    uint32_t       nCircBufferLength
                                                    );

extern
void                     adi_sobel_3x3_cross_circ_i16(
                                                    const int16_t *pInBuff,
                                                    uint32_t      dimY,
                                                    uint32_t      dimX,
                                                    int32_t       *pOutBuff,
                                                    int16_t       *pCircBufferBase,
                                                    uint32_t       nCircBufferLength
                                                     );

extern
void                     adi_sobel_5x5_horz_circ_i16(
                                                    const int16_t *pInBuff,
                                                    uint32_t      dimY,
                                                    uint32_t      dimX,
                                                    int32_t       *pOutBuff,
                                                    int16_t       *pCircBufferBase,
                                                    uint32_t       nCircBufferLength
                                                    );

extern
void                     adi_sobel_5x5_vert_circ_i16(
                                                    const int16_t *pInBuff,
                                                    uint32_t      dimY,
                                                    uint32_t      dimX,
                                                    int32_t       *pOutBuff,
                                                    int16_t       *pCircBufferBase,
                                                    uint32_t       nCircBufferLength
                                                    );

extern
void                     adi_sobel_5x5_cross_circ_i16(
                                                    const int16_t *pInBuff,
                                                    uint32_t      dimY,
                                                    uint32_t      dimX,
                                                    int32_t       *pOutBuff,
                                                    int16_t       *pCircBufferBase,
                                                    uint32_t       nCircBufferLength
                                                     );

extern
uint8_t                 adi_mean_8bit(
                                      const uint8_t   *pInBuff,
                                      uint32_t        nInputPixels,
                                      uint32_t        *pSum
                                      );

extern
int16_t                 adi_mean_16bit(
                                       const int16_t   *pInBuff,
                                       uint32_t        nInputPixels,
                                       int32_t         *pSum
                                       );
extern
void                    adi_AbsDiff_8bit(
                                         uint8_t  *pOutBuff,
                                         uint8_t  *pInBuff1,
                                         uint8_t  *pInBuff2,
                                         uint16_t  nCols,
                                         uint16_t  nRows
                                        );
extern
void                    adi_AbsDiff_16bit(
                                          int16_t  *pOutBuff,
                                          int16_t  *pInBuff1,
                                          int16_t  *pInBuff2,
                                          uint16_t  nCols,
                                          uint16_t  nRows
                                         );
extern
uint8_t                 adi_TriggerObject_8bit(
                                               const uint8_t   *pInBuff1,
                                               const uint8_t   *pInBuff2,
                                               uint32_t        nInputPixels,
                                               uint8_t         nLevel
                                               );

extern
uint8_t                 adi_TriggerObject_16bit(
                                                const int16_t   *pInBuff1,
                                                const int16_t   *pInBuff2,
                                                uint32_t        nInputPixels,
                                                uint16_t        nLevel
                                                );

extern
void                    adi_hist_i8(
                                    const uint8_t   *pInBuff,
                                    int32_t         *pHist,
                                    uint32_t        nSize,
                                    uint32_t        nStep,
                                    uint32_t        nRangeStart,
                                    uint32_t        nRangeEnd,
                                    uint32_t        nBins
                                    );

extern
void                    adi_hist_i16(
                                     const int16_t   *pInBuff,
                                     int32_t         *pHist,
                                     uint32_t        nSize,
                                     uint32_t        nStep,
                                     int32_t         nRangeStart,
                                     int32_t         nRangeEnd,
                                     uint32_t        nBins
                                     );

extern
void                    adi_hist_i10(
                                     const uint16_t  *pInBuff,
                                     int32_t         *pHist,
                                     uint32_t        nSize
                                     );

extern
void                    adi_FindCDF(
                                    int32_t     pInput[],
                                    int32_t     pCdf[],
                                    uint32_t    nBins,
                                    int32_t     *pCdfMin
                                    );

extern
void                    adi_histequalize_i8(
                                            const uint8_t   pInBuff[],
                                            int32_t         pCdf[],
                                            int32_t         nCdfMin,
                                            uint16_t        nWidth,
                                            uint16_t        nHeight,
                                            uint32_t        nTotalPixels,
                                            uint8_t         pOutBuff[]
                                            );

extern
void                    adi_histequalize_i16(
                                             const uint16_t  pInBuff[],
                                             int32_t         pCdf[],
                                             int32_t         nCdfMin,
                                             uint16_t        nWidth,
                                             uint16_t        nHeight,
                                             uint32_t        nTotalPixels,
                                             uint16_t        pOutBuff[]
                                             );

extern
uint32_t                adi_std_i8(
                                   const uint8_t   *pInBuff,
                                   uint32_t        dimY,
                                   uint32_t        dimX,
                                   uint32_t        nStride,
                                   uint32_t        *pMean
                                   );

extern
uint32_t                adi_gradient_i8(
                                        const uint8_t   *pInBuff,
                                        uint32_t        dimY,
                                        uint32_t        dimX,
                                        uint32_t        nStride
                                        );

extern
void                    adi_DistanceTransform_i8(
                                                 const void      *pInBuff,
                                                 uint32_t        dimY,
                                                 uint32_t        dimX,
                                                 int16_t         *pOutBuff,
                                                 int16_t         *pColBuff,
                                                 int16_t         *pRowBuff,
                                                 ADI_DT_METRIC   eMetric,
                                                 ADI_DT_PROCESS  eProcess
                                                 );

extern
float16                 adi_Mahalanobis_fl16(
                                             float16     *pInVec1,
                                             float16     *pInVec2,
                                             float16     *pInMat,
                                             uint32_t    nDim,
                                             float16     *pBuffer
                                             );

extern
void                    adi_Covariance_fl16(
                                            float16             *pInVec,
                                            uint32_t            nDim,
                                            uint32_t            nCount,
                                            float16             *pOutAvg,
                                            float16             *pOutCov,
                                            ADI_COVAR_OPTION    eFlag,
                                            float16             *pInTemp
                                            );

extern
uint32_t                adi_SumAbsDiff_8bit(
                                            const uint8_t   *pInBuff1,
                                            const uint8_t   *pInBuff2,
                                            uint32_t        nInputPixels
                                            );

extern
uint32_t                adi_SumAbsDiff_16bit(
                                             const int16_t   *pInBuff1,
                                             const int16_t   *pInBuff2,
                                             uint32_t        nInputPixels
                                             );

extern
uint32_t                adi_SumOfSquares_i8(
                                            const uint8_t  *pInBuff,
                                            uint32_t       nInputPixels
                                           );

extern
uint32_t                adi_SumOfPixels_i8(
                                           const uint8_t  *pInBuff,
                                           uint32_t       nInputPixels
                                          );

extern
uint32_t                adi_MeanGeneric_i8(
                                           const uint8_t  *pInBuff,
                                           uint32_t       nInputPixels
                                          );
extern
void adi_HoughTransformInit(
                            const uint8_t   *pInBuff,
                            uint16_t        dimY,
                            uint16_t        dimX,
                            int16_t         nThetaStart,
                            int16_t         nThetaEnd,
                            uint16_t        nRhoStep,
                            uint16_t        nThetaStep,
                            int16_t         *pSinBuff,
                            int16_t         *pCosBuff,
                            int16_t         nImageStride,
                            uint16_t        *pEdgeBuffer,
                            uint16_t        nNumAccumulatorLines,
                            uint16_t        nSkipCount,
                            uint32_t        nMaxEdgePoints,
                            ADI_HOUGH_DATA  *pAdiHough
                           );

extern
ADI_ITB_MODULE_STATUS   adi_HoughVoting(
                                        ADI_HOUGH_DATA  *pAdiHough,
                                        uint16_t        *pAccumulator,
                                        uint32_t        nAccumulatorSizePassed
                                        );
extern
void                    adi_FindLinesOfMaxLength(
                                                 ADI_HOUGH_DATA     *pAdiHough,
                                                 const uint16_t     *pAccumulator,
                                                 uint16_t           nMaxLinesToDetect,
                                                 uint16_t           nThreshold,
                                                 ADI_HOUGH_OUTPUT   *pDetectedLine,
                                                 uint16_t           nRhoNoise,
                                                 uint16_t           nThetaNoise
                                                );
extern
ADI_ITB_MODULE_STATUS adi_HoughTransform(
                                         ADI_HOUGH_DATA    *pAdiHough,
                                         uint16_t          *pAccumulator,
                                         uint32_t          nAccumulatorSizePassed,
                                         uint16_t          nThreshold,
                                         uint16_t          nMaxLines,
                                         ADI_HOUGH_OUTPUT  *pDetectedLines,
                                         uint16_t          nRhoNoise,
                                         uint16_t          nThetaNoise
                                        );

extern
uint32_t    adi_Find_Edges(
                           uint8_t           *pInputImage,
                           ADI_HOUGH_DATA    *pAdiHough,
                           uint32_t          nWidth,
                           uint32_t          nHeight,
                           uint32_t          nMaxEdgePoints
                          );

extern
void                    adi_IntegralSum_i8(
                                           const uint8_t   *pInBuff,
                                           uint32_t        *pSum,
                                           uint32_t        dimY,
                                           uint32_t        dimX,
                                           uint32_t        nStride,
                                           uint32_t        *pRowVector,
                                           uint32_t        *pColVector
                                           );

extern
void                    adi_IntegralSum_i16(
                                            const int16_t   *pInBuff,
                                            int32_t         *pSum,
                                            uint32_t        dimY,
                                            uint32_t        dimX,
                                            uint32_t        nStride,
                                            int32_t         *pRowVector,
                                            int32_t         *pColVector
                                            );

extern
void                    adi_TiltedSum_i8(
                                         const uint8_t   *pInBuff,
                                         uint32_t        *pTiltedSum,
                                         uint32_t        dimY,
                                         uint32_t        dimX,
                                         uint32_t        nStride,
                                         uint32_t        *pRowVector,
                                         uint8_t         *pPreviousImageRow
                                         );

extern
void                    adi_TiltedSum_i16(
                                          const int16_t   *pInBuff,
                                          int32_t         *pTiltedSum,
                                          uint32_t        dimY,
                                          uint32_t        dimX,
                                          uint32_t        nStride,
                                          int32_t         *pRowVector,
                                          int16_t         *pPreviousImageRow
                                          );

extern
void                    adi_IntegralSquareSum_i8(
                                                 const uint16_t  *pInBuff,
                                                 uint64_t        *pSqSum,
                                                 uint32_t        dimY,
                                                 uint32_t        dimX,
                                                 uint32_t        nStride,
                                                 uint64_t        *pRowVector,
                                                 uint64_t        *pColVector
                                                 );

extern
ADI_ITB_MODULE_STATUS   adi_MatrixAddition(
                                           const int8_t    pInMat1[],
                                           const int8_t    pInMat2[],
                                           uint32_t        nRows1,
                                           uint32_t        nCols1,
                                           uint32_t        nRows2,
                                           uint32_t        nCols2,
                                           int8_t          pOutMat[]
                                           );

extern
ADI_ITB_MODULE_STATUS   adi_MatrixSubtraction(
                                              const int8_t    pInMat1[],
                                              const int8_t    pInMat2[],
                                              uint32_t        nRows1,
                                              uint32_t        nCols1,
                                              uint32_t        nRows2,
                                              uint32_t        nCols2,
                                              int8_t          pOutMat[]
                                              );

extern
ADI_ITB_MODULE_STATUS   adi_MatrixMultiplication(
                                                 const int8_t    pInMat1[],
                                                 const int8_t    pInMat2[],
                                                 uint32_t        nRows1,
                                                 uint32_t        nCols1,
                                                 uint32_t        nRows2,
                                                 uint32_t        nCols2,
                                                 int8_t          pOutMat[],
                                                 uint32_t        nShift
                                                 );

extern
ADI_ITB_MODULE_STATUS   adi_MatrixMultTranspose(
                                                const int8_t    pInMat1[],
                                                const int8_t    pInMat2[],
                                                uint32_t        nRows1,
                                                uint32_t        nCols1,
                                                uint32_t        nRows2,
                                                uint32_t        nCols2,
                                                int8_t          pOutMat[],
                                                uint32_t        nShift
                                                );

extern
ADI_ITB_MODULE_STATUS   adi_MatrixTranspose(
                                            const int8_t    pInMat[],
                                            int8_t          pOutMat[],
                                            uint32_t        nRows,
                                            uint32_t        nCols
                                            );

extern
ADI_ITB_MODULE_STATUS   adi_MatInverseFloat16(
                                              float16     pInMat[],
                                              uint32_t    nRows,
                                              uint32_t    nCols,
                                              float16     pInverseMat[],
                                              float16     *pTempBuff
                                              );

extern
ADI_ITB_MODULE_STATUS   adi_MatInverseFloat(
                                            float32_t    pInMat[],
                                            uint32_t     nRows,
                                            uint32_t     nCols,
                                            float32_t    pInverseMat[],
                                            float32_t    *pTempBuff
                                           );

extern
void                    adi_median_i8(
                                      const uint8_t   *pInBuff,
                                      uint32_t        dimY,
                                      uint32_t        dimX,
                                      uint8_t         *pOutBuff
                                      );

extern
void                    adi_median_i16(
                                       const int16_t   *pInBuff,
                                       uint32_t        dimY,
                                       uint32_t        dimX,
                                       int16_t         *pOutBuff
                                       );

extern
void                    adi_Threshold_Binary_i8(
                                                const uint8_t   *pInBuff,
                                                uint32_t        dimY,
                                                uint32_t        dimX,
                                                uint8_t         nThreshold,
                                                uint8_t         nMaxValue,
                                                uint8_t         *pOutBuff
                                                );

extern
void                    adi_Threshold_Binary_i16(
                                                 const int16_t   *pInBuff,
                                                 uint32_t        dimY,
                                                 uint32_t        dimX,
                                                 int16_t         nThreshold,
                                                 uint8_t         nMaxValue,
                                                 uint8_t         *pOutBuff
                                                 );

extern
void                    adi_Threshold_Binary_Inv_i8(
                                                    const uint8_t   *pInBuff,
                                                    uint32_t        dimY,
                                                    uint32_t        dimX,
                                                    uint8_t         nThreshold,
                                                    uint8_t         nMinValue,
                                                    uint8_t         *pOutBuff
                                                    );

extern
void                    adi_Threshold_Binary_Inv_i16(
                                                     const int16_t   *pInBuff,
                                                     uint32_t        dimY,
                                                     uint32_t        dimX,
                                                     int16_t         nThreshold,
                                                     uint8_t         nMinValue,
                                                     uint8_t         *pOutBuff
                                                     );

extern
void                    adi_Threshold_Trunc_i8(
                                               const uint8_t   *pInBuff,
                                               uint32_t        dimY,
                                               uint32_t        dimX,
                                               uint8_t         nThreshold,
                                               uint8_t         nMaxValue,
                                               uint8_t         *pOutBuff
                                               );

extern
void                    adi_Threshold_Trunc_i16(
                                                const int16_t   *pInBuff,
                                                uint32_t        dimY,
                                                uint32_t        dimX,
                                                int16_t         nThreshold,
                                                int16_t         nMaxValue,
                                                int16_t         *pOutBuff
                                                );

extern
void                    adi_Threshold_ToZero_i8(
                                                const uint8_t   *pInBuff,
                                                uint32_t        dimY,
                                                uint32_t        dimX,
                                                uint8_t         nThreshold,
                                                uint8_t         *pOutBuff
                                                );

extern
void                    adi_Threshold_ToZero_i16(
                                                 const int16_t   *pInBuff,
                                                 uint32_t        dimY,
                                                 uint32_t        dimX,
                                                 int16_t         nThreshold,
                                                 int16_t         *pOutBuff
                                                 );

extern
void                    adi_Threshold_ToZero_Inv_i8(
                                                    const uint8_t   *pInBuff,
                                                    uint32_t        dimY,
                                                    uint32_t        dimX,
                                                    uint8_t         nThreshold,
                                                    uint8_t         *pOutBuff
                                                    );

extern
void                    adi_Threshold_ToZero_Inv_i16(
                                                     const int16_t   *pInBuff,
                                                     uint32_t        dimY,
                                                     uint32_t        dimX,
                                                     int16_t         nThreshold,
                                                     int16_t         *pOutBuff
                                                     );

extern
void                    adi_Threshold_ToBlack_i8(
                                                 const uint8_t   *pInBuff,
                                                 uint32_t        dimY,
                                                 uint32_t        dimX,
                                                 uint8_t         nThreshold,
                                                 uint8_t         *pOutBuff
                                                 );

extern
void                    adi_Threshold_ToBlack_i16(
                                                  const int16_t   *pInBuff,
                                                  uint32_t        dimY,
                                                  uint32_t        dimX,
                                                  int16_t         nThreshold,
                                                  int16_t         *pOutBuff
                                                  );

extern
void                    adi_error_diffusion(
                                            const uint8_t   *pInBuff,
                                            uint32_t        dimY,
                                            uint32_t        dimX,
                                            uint8_t         nThreshold,
                                            uint16_t        *pTempBuff,
                                            uint8_t         *pOutBuff
                                            );

extern
int32_t                 adiVectorDotProduct(
                                            const int8_t    pInBuff1[],
                                            const int8_t    pInBuff2[],
                                            uint32_t        nShift,
                                            uint32_t        nLength
                                            );

extern
void                    adiVectorCrossProduct(
                                              const int8_t    pInBuff1[],
                                              const int8_t    pInBuff2[],
                                              uint32_t        nShift,
                                              int8_t          pOutBuff[]
                                              );

extern
uint32_t                adiVectorNorm(
                                      const int8_t    pInBuff1[],
                                      const int8_t    pInBuff2[],
                                      uint32_t        nLength,
                                      uint32_t        nShift,
                                      uint32_t        nType
                                      );

extern
void                    adi_pyramid_down_i8(
                                            const uint8_t   *pInBuff,
                                            uint32_t        dimY,
                                            uint32_t        dimX,
                                            const int16_t   *pKernel,
                                            void            (*pt2PyramidFunc)(const uint8_t *pInBuff,
                                                                              uint32_t      dimY,
                                                                              uint32_t      dimX,
                                                                              const int16_t *pKernel,
                                                                              uint8_t       pTempBuff[],
                                                                              uint8_t       *pOutBuff1,
                                                                              uint8_t       *pOutBuff2),
                                            uint8_t         pTempBuff[],
                                            uint8_t         *pOutBuff1,
                                            uint8_t         *pOutBuff2
                                            );

extern
void                    adi_pyramid_down_gaussian_i8(
                                                     const uint8_t   *pInBuff,
                                                     uint32_t        dimY,
                                                     uint32_t        dimX,
                                                     const int16_t   *pKernel,
                                                     uint8_t         pTempBuff[],
                                                     uint8_t         *pOutBuff1,
                                                     uint8_t         *pOutBuff2
                                                     );

extern
void                    adi_pyramid_down_grad_horz_i8(
                                                      const uint8_t   *pInBuff,
                                                      uint32_t        dimY,
                                                      uint32_t        dimX,
                                                      const int16_t   *pKernel,
                                                      uint8_t         pTempBuff[],
                                                      uint8_t         *pOutBuff1,
                                                      uint8_t         *pOutBuff2
                                                      );

extern
void                    adi_pyramid_down_grad_vert_i8(
                                                      const uint8_t   *pInBuff,
                                                      uint32_t        dimY,
                                                      uint32_t        dimX,
                                                      const int16_t   *pKernel,
                                                      uint8_t         pTempBuff[],
                                                      uint8_t         *pOutBuff1,
                                                      uint8_t         *pOutBuff2
                                                      );

extern
void                    adi_GaussianFilter_i8(
                                              const uint8_t   *pInBuff,
                                              uint32_t        dimY,
                                              uint32_t        dimX,
                                              const int16_t   *pKernel,
                                              uint8_t         *pOutBuff
                                              );

extern
void                    adi_sobel_horz_InRange_i8(
                                                  const uint8_t   *pInBuff,
                                                  uint32_t        dimY,
                                                  uint32_t        dimX,
                                                  uint8_t         *pOutBuff
                                                  );

extern
void                    adi_sobel_vertical_InRange_i8(
                                                      const uint8_t   *pInBuff,
                                                      uint32_t        dimY,
                                                      uint32_t        dimX,
                                                      uint8_t         *pOutBuff
                                                      );

extern
void                    adi_Downsampling_i8(
                                            const uint8_t   *pInBuff,
                                            uint32_t        dimY,
                                            uint32_t        dimX,
                                            uint8_t         *pOutBuff
                                            );

extern
void                    adi_Upsampling_i8(
                                          const uint8_t   *pInBuff,
                                          uint32_t        dimY,
                                          uint32_t        dimX,
                                          uint8_t         *pOutBuff
                                          );

extern
void                    adi_ConvertScale_Abs_i16_o8(
                                                    const int16_t   *pInBuff,
                                                    uint32_t        dimY,
                                                    uint32_t        dimX,
                                                    int16_t         nScale,
                                                    int16_t         nShift,
                                                    uint8_t         *pOutBuff
                                                    );

extern
void                    adi_ConvertScale_Abs_i32_o16(
                                                     const int32_t   *pInBuff,
                                                     uint32_t        dimY,
                                                     uint32_t        dimX,
                                                     int16_t         nScale,
                                                     int16_t         nShift,
                                                     int16_t         *pOutBuff
                                                     );

extern
void                    adi_pyramid_down_i16(
                                             const int16_t  *pInBuff,
                                             uint32_t       dimY,
                                             uint32_t       dimX,
                                             const int16_t  *pKernel,
                                             void           (*pt2PyramidFunc)(
                                                                               const int16_t *pInBuff,
                                                                               uint32_t      dimY,
                                                                               uint32_t      dimX,
                                                                               const int16_t *pKernel,
                                                                               int16_t       pTempBuff[],
                                                                               int16_t       *pOutBuff1,
                                                                               int16_t       *pOutBuff2
                                                                               ),
                                            int16_t         pTempBuff[],
                                            int16_t        *pOutBuff1,
                                            int16_t        *pOutBuff2
                                            );

extern
void                    adi_pyramid_down_grad_vert_i16(
                                                       const int16_t   *pInBuff,
                                                       uint32_t        dimY,
                                                       uint32_t        dimX,
                                                       const int16_t   *pKernel,
                                                       int16_t         pTempBuff[],
                                                       int16_t         *pOutBuff1,
                                                       int16_t         *pOutBuff2
                                                       );

extern
void                    adi_pyramid_down_grad_horz_i16(
                                                       const int16_t   *pInBuff,
                                                       uint32_t        dimY,
                                                       uint32_t        dimX,
                                                       const int16_t   *pKernel,
                                                       int16_t         pTempBuff[],
                                                       int16_t         *pOutBuff1,
                                                       int16_t         *pOutBuff2
                                                       );

extern
void                    adi_pyramid_down_gaussian_i16(
                                                      const int16_t   *pInBuff,
                                                      uint32_t        dimY,
                                                      uint32_t        dimX,
                                                      const int16_t   *pKernel,
                                                      int16_t         pTempBuff[],
                                                      int16_t         *pOutBuff1,
                                                      int16_t         *pOutBuff2
                                                      );

extern
void                    adi_pyramid_up_gaussian_i8(
                                                   const uint8_t   *pInBuff,
                                                   uint32_t        dimY,
                                                   uint32_t        dimX,
                                                   const int16_t   *pKernel,
                                                   uint8_t         pTempBuff[],
                                                   uint8_t         *pOutBuff1,
                                                   uint8_t         *pOutBuff2
                                                   );

extern
void                    adi_pyramid_up_gaussian_i16(
                                                    const int16_t   *pInBuff,
                                                    uint32_t        dimY,
                                                    uint32_t        dimX,
                                                    const int16_t   *pKernel,
                                                    int16_t         pTempBuff[],
                                                    int16_t         *pOutBuff1,
                                                    int16_t         *pOutBuff2
                                                    );

extern
void                    adi_sobel_vertical_InRange_i16(
                                                       const int16_t   *pInBuff,
                                                       uint32_t        dimY,
                                                       uint32_t        dimX,
                                                       int16_t         *pOutBuff
                                                       );

extern
void                    adi_sobel_horz_InRange_i16(
                                                   const int16_t   *pInBuff,
                                                   uint32_t        dimY,
                                                   uint32_t        dimX,
                                                   int16_t         *pOutBuff
                                                   );

extern
void                    adi_GaussianFilter_i16(
                                               const int16_t   *pInBuff,
                                               uint32_t        dimY,
                                               uint32_t        dimX,
                                               const int16_t   *pKernel,
                                               int16_t         *pOutBuff
                                               );

extern
void                    adi_Downsampling_i16(
                                             const int16_t   *pInBuff,
                                             uint32_t        dimY,
                                             uint32_t        dimX,
                                             int16_t         *pOutBuff
                                             );

extern
void                    adi_ImageDownScaleby4(
                                              const uint8_t   *pInBuff,
                                              uint32_t        dimY,
                                              uint32_t        dimX,
                                              uint8_t         *pOutBuff
                                              );

extern
void                    adi_Upsampling_i16(
                                           const int16_t   *pInBuff,
                                           uint32_t        dimY,
                                           uint32_t        dimX,
                                           int16_t         *pOutBuff
                                           );

extern
void                    adi_pyramid_up_i8(
                                          const uint8_t *pInBuff,
                                          uint32_t      dimY,
                                          uint32_t      dimX,
                                          const int16_t *pKernel,
                                          void          (*pt2PyramidFunc)(const uint8_t *pInBuff,
                                                                          uint32_t      dimY,
                                                                          uint32_t      dimX,
                                                                          const int16_t *pKernel,
                                                                          uint8_t       pTempBuff[],
                                                                          uint8_t       *pOutBuff1,
                                                                          uint8_t       *pOutBuff2),
                                          uint8_t       pTempBuff[],
                                          uint8_t       *pOutBuff1,
                                          uint8_t       *pOutBuff2
                                          );

extern
void                    adi_pyramid_up_i16(
                                           const int16_t    *pInBuff,
                                           uint32_t         dimY,
                                           uint32_t         dimX,
                                           const int16_t    *pKernel,
                                           void             (*pt2PyramidFunc)(const int16_t *pInBuff,
                                                                              uint32_t      dimY,
                                                                              uint32_t      dimX,
                                                                              const int16_t *pKernel,
                                                                              int16_t       pTempBuff[],
                                                                              int16_t       *pOutBuff1,
                                                                              int16_t       *pOutBuff2),
                                           int16_t          pTempBuff[],
                                           int16_t          *pOutBuff1,
                                           int16_t          *pOutBuff2
                                           );

extern
ADI_ITB_MODULE_STATUS   adi_KalmanCreate(
                                         ADI_KALMAN  *pKalman,
                                         int8_t      *pMatrixMemory,
                                         uint32_t    nSize,
                                         uint32_t    nShift
                                         );

extern
ADI_ITB_MODULE_STATUS   adi_KalmanPredict(
                                          const ADI_KALMAN    *pKalman,
                                          ADI_MAT             *pMatU
                                          );

extern
ADI_ITB_MODULE_STATUS   adi_KalmanCorrect(
                                          ADI_KALMAN  *pKalman,
                                          ADI_MAT     *pMatZ
                                          );

extern
void                    adi_OpticalFlowLK_i16(
                                              ADI_OPTICALFLOW     *pOptical,
                                              ADI_OPTICALLKBUFF   *pBuffer,
                                              uint32_t            nFlag
                                              );

extern
void                    adi_OpticalFlowBM_i8(
                                             const ADI_OPTICALFLOW * const  poOpticalFlowInp,
                                             void                           *pTemp,
                                             uint32_t                       nUsePrev
                                             );

extern
void                    adi_OpticalFlowBM_i16(
                                              const ADI_OPTICALFLOW * const poOpticalFlowInp,
                                              void                          *pTemp,
                                              uint32_t                      nUsePrev
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
ADI_ITB_MODULE_STATUS   adi_contour_RLE(
                                        ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                        const uint8_t                   *pImage,
                                        uint32_t                        nWidth,
                                        uint32_t                        nRowNum,
                                        ADI_CONT_RUN_LEN_HDR            *pRunLenListHdr
                                        );

extern
ADI_ITB_MODULE_STATUS   adi_contour_segment_image(
                                                  ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                                  const ADI_CONT_RUN_LEN_HDR      *pRunLenListHdr,
                                                  uint32_t                        nHeight,
                                                  ADI_CONT_SEGMENT_HDR            **pSegmentListHdr,
                                                  uint32_t                        *pNumSegments
                                                  );

extern
void                    adi_contour_BoundingRectangle(
                                                      const ADI_CONT_SEGMENT_HDR  *pSegmentHdr,
                                                      ADI_CONT_BOUNDING_RECTANGLE *pBoundingRectangle
                                                      );

extern
void                    adi_contour_SplitSegmentRowWise(
                                                        const ADI_CONT_SEGMENT_HDR          *pSegmentListHdr,
                                                        ADI_CONT_SEGMENT_HDR_ROW_WISE       *pSegmentListRowWiseHdr,
                                                        const ADI_CONT_BOUNDING_RECTANGLE   *pBoundingRectangle
                                                        );

extern
ADI_ITB_MODULE_STATUS   adi_contour_TraceContour(
                                                 ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                                 ADI_CONT_SEGMENT_HDR_ROW_WISE   *pSegmentListRowWiseHdr,
                                                 ADI_CONT_BOUNDING_RECTANGLE     *pBoundingRectangle,
                                                 ADI_CONT_HDR                    **pContourListHdr
                                                 );

extern
uint32_t                adi_contour_ArcLenDiagonal(
                                                   const ADI_CONT_HDR  *pContourListHdr,
                                                   const ADI_POINT     oStartPointOfArc,
                                                   const ADI_POINT     oEndPointOfArc,
                                                   uint32_t            bIsClosed
                                                   );

extern
ADI_ITB_MODULE_STATUS   adi_contour_freemanchain(
                                                 ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                                 const ADI_CONT_HDR              *pContourListHdr,
                                                 ADI_CONT_FREEMAN_HDR            *pFreemanHdr
                                                 );

extern
ADI_ITB_MODULE_STATUS   adi_contour_freemanchain_approx(
                                                        ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                                        const ADI_CONT_HDR              *pContourListHdr,
                                                        ADI_CONT_FREEMAN_HDR            *pFreemanApproxHdr
                                                        );

extern
ADI_ITB_MODULE_STATUS   adi_contour_chain_vertex(
                                                 ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                                 const ADI_CONT_FREEMAN_HDR      *pFreemanApproxHdr,
                                                 ADI_CONT_NODE                   **pChainApproxHdr
                                                 );

extern
ADI_ITB_MODULE_STATUS   adi_contour_moments_spatial(
                                                    const ADI_CONT_HDR          *pContourListHdr,
                                                    ADI_CONT_MOMENTS_SPATIAL    *pSpatialMoments
                                                    );

extern
ADI_ITB_MODULE_STATUS   adi_contour_moments_central(
                                                    const ADI_CONT_HDR              *pContourListHdr,
                                                    const ADI_CONT_MOMENTS_SPATIAL  *pSpatialMoments,
                                                    ADI_CONT_MOMENTS_CENTRAL        *pCentralMoments
                                                    );

extern
ADI_ITB_MODULE_STATUS   adi_contour_moments_cent_norm(
                                                      const ADI_CONT_MOMENTS_SPATIAL  *pSpatialMoments,
                                                      const ADI_CONT_MOMENTS_CENTRAL  *pCentralMoments,
                                                      ADI_CONT_MOMENTS_CENT_NORM      *pCentralMomentsNorm
                                                      );

extern
ADI_ITB_MODULE_STATUS   adi_contour_moments_hu(
                                               const ADI_CONT_MOMENTS_CENT_NORM    *pCentralMomentsNorm,
                                               ADI_CONT_MOMENTS_HU                 *pHuMoments
                                               );

extern
ADI_ITB_MODULE_STATUS   adi_contour_SegmentArea(
                                                ADI_CONT_SEGMENT_HDR  *pSegmentHdr,
                                                uint32_t              *pnSegmentArea
                                                );

extern
ADI_ITB_MODULE_STATUS   adi_contour_ContourAreaApprox(
                                                      const ADI_CONT_SEGMENT_HDR_ROW_WISE *pSegmentListRowWiseHdr,
                                                      const ADI_CONT_BOUNDING_RECTANGLE   *pBoundingRectangle,
                                                      uint32_t                            *pnContourAreaApprox
                                                      );

extern
ADI_ITB_MODULE_STATUS   adi_contour_CreateSegmentImage(
                                                       const ADI_CONT_SEGMENT_HDR_ROW_WISE    *pSegmentListRowWiseHdr,
                                                       const ADI_CONT_BOUNDING_RECTANGLE      *pBoundingRectangle,
                                                       uint8_t                                *pImage,
                                                       uint8_t                                nFillVal
                                                       );

extern
void                    adi_ReScaleFeaturesForSubWindow(
                                                        ADI_HAARCLASSIFIERCASCADE   *pCascadeClassifier,
                                                        const ADI_IMAGE_HAAR_DATA   *pImageData,
                                                        int32_t                     nScale
                                                        );

extern
int32_t                 adi_EvaluateSubWindow(
                                              const ADI_HAARCLASSIFIERCASCADE *pCascadeClassifier,
                                              const ADI_IMAGE_HAAR_DATA       *pImageData,
                                              ADI_POINT                       oStartPoint,
                                              int32_t                         nStartStage
                                              );

extern
ADI_ITB_MODULE_STATUS   adi_RLE_8bit(
                                     const uint8_t           *pImage,
                                     uint32_t                nInputPixels,
                                     ADI_RUN_LEN_NODE_8BIT   *pRunLenNodeArray,
                                     uint32_t                nNumEmptyRLENodes,
                                     uint32_t                *pnNumUsedRLENodes
                                     );

extern
ADI_ITB_MODULE_STATUS   adi_RLE_16bit(
                                      const int16_t           *pImage,
                                      uint32_t                nInputPixels,
                                      ADI_RUN_LEN_NODE_16BIT  *pRunLenNodeArray,
                                      uint32_t                nNumEmptyRLENodes,
                                      uint32_t                *pnNumUsedRLENodes
                                      );

extern
ADI_ITB_MODULE_STATUS   adi_RLD_8bit(
                                     const ADI_RUN_LEN_NODE_8BIT *pRunLenNodeArray,
                                     uint32_t                    *pnNumRLENodes,
                                     uint8_t                     *pBuff,
                                     uint32_t                    nSizeOfBuff,
                                     uint32_t                    *pnOutputPixels
                                     );

extern
ADI_ITB_MODULE_STATUS   adi_RLD_16bit(
                                      const ADI_RUN_LEN_NODE_16BIT    *pRunLenNodeArray,
                                      uint32_t                        *pnNumRLENodes,
                                      int16_t                         *pBuff,
                                      uint32_t                        nSizeOfBuff,
                                      uint32_t                        *pnOutputPixels
                                      );

extern
ADI_ITB_MODULE_STATUS   adi_RLD_32bit(
                                      const ADI_RUN_LEN_NODE_32BIT    *pRunLenNodeArray,
                                      uint32_t                        *pnNumRLENodes,
                                      uint32_t                        *pBuff,
                                      uint32_t                        nSizeOfBuff,
                                      uint32_t                        *pnOutputValues
                                      );

extern
ADI_ITB_MODULE_STATUS   adi_RLE_packed_init(
                                            ADI_PACKED_RLE  *pRLEData
                                            );

extern
ADI_ITB_MODULE_STATUS   adi_RLE_unpack(
                                       const uint32_t          *pPackets,
                                       uint32_t                nNumPackets,
                                       ADI_RUN_LEN_NODE_32BIT  *pRunLenNodeArray,
                                       ADI_PACKET_MODE         ePacketMode,
                                       uint32_t                *pnNumRLENodes
                                       );

extern
ADI_ITB_MODULE_STATUS   adi_UKF_MemInit(
                                        ADI_UKF     *pUKF,
                                        int8_t      *pMem,
                                        uint32_t    nMemSize
                                        );

extern
void                    adi_UnscentedTransform(
                                               ADI_UKF          *pUKF,
                                               void             (*pfSys)(
                                                                         int32_t    *InMat,
                                                                         int32_t    *OutMat,
                                                                         uint32_t   nScale
                                                                         ),
                                               const int32_t    *X,
                                               const int32_t    *R,
                                               uint32_t         nVars,
                                               int32_t          *y,
                                               int32_t          *Y,
                                               int32_t          *P,
                                               int32_t          *Y1
                                               );

extern
ADI_ITB_MODULE_STATUS   adi_ukf(
                                ADI_UKF *pUKF,
                                void    (*f)(
                                             int32_t    *InMat,
                                             int32_t    *OutMat,
                                             uint32_t   nScale
                                             ),
                                void    (*h)(
                                             int32_t    *InMat,
                                             int32_t    *OutMat,
                                             uint32_t   nScale
                                             )
                                );

extern
void                    adi_Bayer2RGBNeighbor_BotRight(
                                                       const uint8_t   *pInBuff,
                                                       uint32_t        dimY,
                                                       uint32_t        dimX,
                                                       uint8_t         *pOutBuff
                                                       );

extern
void                    adi_Bayer2RGBNeighbor_TopLeft(
                                                      const uint8_t   *pInBuff,
                                                      uint32_t        dimY,
                                                      uint32_t        dimX,
                                                      uint8_t         *pOutBuff
                                                      );

extern
void                    adi_Bayer2RGBBilinear(
                                              const uint8_t   *pInBuff,
                                              uint32_t        dimY,
                                              uint32_t        dimX,
                                              uint8_t         *pOutBuff
                                              );

extern
void                    adi_Bayer2RGBNbr_BotRt_i16_o8(
                                                      const uint16_t   *pInBuff,
                                                      uint32_t        dimY,
                                                      uint32_t        dimX,
                                                      uint8_t         *pOutBuff,
                                                      uint32_t        nShift
                                                     );

extern
void                    adi_Bayer2RGBNbr_TopLt_i16_o8(
                                                      const uint16_t   *pInBuff,
                                                      uint32_t        dimY,
                                                      uint32_t        dimX,
                                                      uint8_t         *pOutBuff,
                                                      uint32_t        nShift
                                                     );

extern
void                    adi_Bayer2RGBBilinear_i16_o8(
                                                     const uint16_t   *pInBuff,
                                                     uint32_t        dimY,
                                                     uint32_t        dimX,
                                                     uint8_t         *pOutBuff,
                                                     uint32_t        nShift
                                                    );

extern
void                    adi_Bayer2RGBNbr_BotRt_i16_o16(
                                                       const uint16_t  *pInBuff,
                                                       uint32_t        dimY,
                                                       uint32_t        dimX,
                                                       uint16_t        *pOutBuff
                                                      );

extern
void                    adi_Bayer2RGBNbr_TopLt_i16_o16(
                                                       const uint16_t  *pInBuff,
                                                       uint32_t        dimY,
                                                       uint32_t        dimX,
                                                       uint16_t        *pOutBuff
                                                      );

extern
void                    adi_Bayer2RGBBilinear_i16_o16(
                                                      const uint16_t  *pInBuff,
                                                      uint32_t        dimY,
                                                      uint32_t        dimX,
                                                      uint16_t        *pOutBuff
                                                     );

extern
void                    adi_Bayer2RGBPnrBilinear_i16_o8(
                                                        const uint16_t  *pInBuff,
                                                        uint32_t        dimY,
                                                        uint32_t        dimX,
                                                        uint8_t         *pOutBuff,
                                                        uint32_t        nShift,
                                                        uint32_t        nOutOffset
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
uint32_t                adi_Populate_EdgePoints(
                                                uint8_t    *pEdgeImage,
                                                int32_t    *pEdgePoints,
                                                uint32_t    nLineNo,
                                                uint32_t   nWidth,
                                                uint32_t   nHeight,
                                                uint32_t   nOffset
                                               );
extern
uint32_t                adi_Ransac_Circ_Curve_Detect(
                                                     uint16_t                  *pEdgeArray,
                                                     uint32_t                  nTotalEdgePoints,
                                                     uint16_t                  nIterations,
                                                     uint32_t                  nMinDistanceRandomPoints,
                                                     int32_t                   *pTempMemory,
                                                     uint32_t                  nMinInliers,
                                                     int32_t                   nRadiusThreshold,
                                                     int32_t                   nCentreNoise,
                                                     int32_t                   nRadiusNoise,
                                                     uint32_t                  nMaxCurvesToDetect,
                                                     ADI_RANSAC_CURVE_OUTPUT   *pDetectedCircle
                                                    );
//#else /*  assembly language specific macros and extern declarations. */
#endif /* if !defined(_LANGUAGE_C) */

/*=============  D A T A    T Y P E S   =============*/
#endif /* __ADI_IMAGE_TOOL_BOX_H__ */

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/tags/release_2_4_0_imagetoolbox_140807/include/adi_image_tool_box.h $
**
*/
