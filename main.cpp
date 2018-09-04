// adi_pyr_opticalflow.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
/*=============  I N C L U D E S   =============*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adi_tool_chain.h"
#include "adi_image_tool_box.h"
#include "adi_pyr_opticalflow.h"
#include "demo_application.h"
#include "profile.h"

#include <opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <iostream> 
#include <string>
#include <vector>
#include <fstream>


/*==============  D E F I N E S  ===============*/
#define NUM_INPUT_FILES 1845
#define FRAME_START     0
/*=============  D A T A  =============*/


uint8_t aInputBufAL3[(ADI_IMAGE_SIZE + ((2 + 2) * ADI_IMAGE_WIDTH))];


uint8_t aOutBufAL3[(ADI_IMAGE_SIZE)];


uint8_t aInputBufBL3[(ADI_IMAGE_SIZE + ((2 + 2) * ADI_IMAGE_WIDTH))];


char_t  aInFilename[NUM_INPUT_FILES][40] =
{
	"demo_pyrLK-320x240_8"
};


static char_t   aOutFilename[256];


static char_t   atempFilename[256];


static char_t   aPathToMedia[256];


static char_t   aOutputPath[256];


static char_t   aOutFilenameWithPath[256];


ADI_PYR_OPTFL_SEL_GD_FEAT   oPyrOptflSelectGoodFeatures;


ADI_PYR_OPTFL_IMAGE_FEAT    oPyrOptflImgAFeatures;


ADI_PYR_OPTFL_IMAGE_FEAT    oPyrOptflImgBFeatures;


int16_t aMaskSmooth[25] =
{
	0x7F,   0x1FF,  0x2FF,  0x1FF,  0x7F,
	0x1FF,  0x7FF,  0xBFF,  0x7FF,  0x1FF,
	0x2FF,  0xBFF,  0x11FF, 0xBFF,  0x2FF,
	0x1FF,  0x7FF,  0xBFF,  0x7FF,  0x1FF,
	0x7F,   0x1FF,  0x2FF,  0x1FF,  0x7F
};

/* Gaussian derivative kernel */

int16_t aMaskHorzDeriv[9] =
{
	-5, 0, 5,
	-8, 0, 8,
	-5, 0, 5
};


int16_t aMaskVertDeriv[9] =
{
	-5, -8, -5,
	0,  0,  0,
	5,  8,  5
};


uint8_t aImgBufSmoothAL3[ADI_IMAGE_SIZE];


uint8_t aImgBufSmoothBL3[ADI_IMAGE_SIZE];


int16_t aGradXAL3[ADI_IMAGE_SIZE];


int16_t aGradYAL3[ADI_IMAGE_SIZE];


int16_t aScoreAL3[ADI_IMAGE_SIZE];


int16_t aGradXBL3[ADI_IMAGE_SIZE];


int16_t aGradYBL3[ADI_IMAGE_SIZE];


uint8_t aImgPyrAL3[ADI_IMAGE_SIZE >> 4];


uint8_t aImgPyrBL3[ADI_IMAGE_SIZE >> 4];


int16_t aGradXPyrAL3[ADI_IMAGE_SIZE >> 4];


int16_t aGradYPyrAL3[ADI_IMAGE_SIZE >> 4];


int16_t aGradXPyrBL3[ADI_IMAGE_SIZE >> 4];


int16_t aGradYPyrBL3[ADI_IMAGE_SIZE >> 4];


ADI_PYR_OPTFL_GD_FEAT   aGoodFeatures[MAX_NUM_GOOD_FEATURES];


uint8_t aTempBuffL3[ADI_IMAGE_SIZE + 8 * ADI_IMAGE_WIDTH];

/* For 250 features */

uint16_t    aL3BuffStoreCoord[500];

/* L1 buffer */

/* Used in select good feature API */

int8_t  aL1BuffTemp[ADI_IMAGE_WIDTH * 4 * sizeof(int32_t) + \
	ADI_IMAGE_WIDTH * HARRIS_BLOCK_SIZE * sizeof(int16_t) * 2 + \
	ADI_IMAGE_WIDTH * sizeof(int16_t)];

/* Used in tracking API */
/* +1 in the size is to make sure all the L1 buffers are word aligned */

int16_t aL1Buff[(((WINDOW_SIZE * WINDOW_SIZE) + 1) * 6) + 2]; /* +2 for unrolling */


void test_many_images()
{
	int32_t nInput_size;
	char input_image_name[100];
	char output_image_name[100];
	int result_num = 0;
	char result_name[100];
	cv::Mat input_image, previous_image, temp_image;

	for (int i = 1; i < NUM_INPUT_FILES; i++)
	{
		//Load input image
		sprintf(input_image_name,"../%s\\%d.jpg","image",i); 
		input_image = cv::imread(input_image_name);

		if (input_image.empty())
		{
			break;
		}
		temp_image = input_image.clone();

		printf("\nImage Num = %d, process => opticalflow - Lucas Kanade Pyramid 8bit\n", i);

		if (i > 1)
		{
			// Start a timer
			double duration;
			duration = static_cast<double>(cv::getTickCount());

			nInput_size = sizeof(uint8_t);

			memcpy(aInputBufAL3 + (ADI_IMAGE_WIDTH * 2 * nInput_size), previous_image.data, ADI_IMAGE_WIDTH * ADI_IMAGE_HEIGHT * 3 * sizeof(uint8_t));
			memcpy(aInputBufBL3 + (ADI_IMAGE_WIDTH * 2 * nInput_size), input_image.data, ADI_IMAGE_WIDTH * ADI_IMAGE_HEIGHT * 3 * sizeof(uint8_t));

			nInput_size = sizeof(int8_t);

			oPyrOptflSelectGoodFeatures.pImgIn = (uint8_t *) (aInputBufAL3);
			oPyrOptflSelectGoodFeatures.pImgOut = (uint8_t *) (aOutBufAL3);
			oPyrOptflSelectGoodFeatures.nImageWidth = ADI_IMAGE_WIDTH;
			oPyrOptflSelectGoodFeatures.nImageHeight = ADI_IMAGE_HEIGHT;
			oPyrOptflSelectGoodFeatures.pSmoothImgOut = aImgBufSmoothAL3;
			oPyrOptflSelectGoodFeatures.pGradX = aGradXAL3;
			oPyrOptflSelectGoodFeatures.pGradY = aGradYAL3;
			oPyrOptflSelectGoodFeatures.pScore = aScoreAL3;
			oPyrOptflSelectGoodFeatures.pGoodFeatures = aGoodFeatures;
			oPyrOptflSelectGoodFeatures.nSenstivity = HARRIS_SENSTIVITY;
			oPyrOptflSelectGoodFeatures.nThreshold = HARRIS_THRESHOLD;
			oPyrOptflSelectGoodFeatures.nMinDistance = HARRIS_MINDIST;
			oPyrOptflSelectGoodFeatures.pScratchBuf = aTempBuffL3;
			oPyrOptflSelectGoodFeatures.pScratchBufL1 = (uint8_t *)aL1BuffTemp;
			oPyrOptflSelectGoodFeatures.pFeatureCoord = aL3BuffStoreCoord;

			adi_SelGoodFeatPreProc_i8(&oPyrOptflSelectGoodFeatures,
				aMaskSmooth);

			adi_SelectGoodFeatures_Appl_i8(&oPyrOptflSelectGoodFeatures);

			adi_harrisMarkCorners(&oPyrOptflSelectGoodFeatures);

			oPyrOptflImgAFeatures.pImg = (uint8_t *) (aInputBufAL3);
			oPyrOptflImgAFeatures.nImageWidth = ADI_IMAGE_WIDTH;
			oPyrOptflImgAFeatures.nImageHeight = ADI_IMAGE_HEIGHT;
			oPyrOptflImgAFeatures.pSmoothImg = aImgBufSmoothAL3;
			oPyrOptflImgAFeatures.pImgPyr = aImgPyrAL3;
			oPyrOptflImgAFeatures.pGradXImg = aGradXAL3;
			oPyrOptflImgAFeatures.pGradYImg = aGradYAL3;
			oPyrOptflImgAFeatures.pGradXPyr = aGradXPyrAL3;
			oPyrOptflImgAFeatures.pGradYPyr = aGradYPyrAL3;

			/* Preprocess Image A */
			adi_TrackFeaturesImagePreProcess_i8(&oPyrOptflImgAFeatures,
				aMaskSmooth,
				aMaskHorzDeriv,
				aMaskVertDeriv);


			oPyrOptflImgBFeatures.pImg = (uint8_t *) (aInputBufBL3);
			oPyrOptflImgBFeatures.nImageWidth = ADI_IMAGE_WIDTH;
			oPyrOptflImgBFeatures.nImageHeight = ADI_IMAGE_HEIGHT;
			oPyrOptflImgBFeatures.pSmoothImg = aImgBufSmoothBL3;
			oPyrOptflImgBFeatures.pImgPyr = aImgPyrBL3;
			oPyrOptflImgBFeatures.pGradXImg = aGradXBL3;
			oPyrOptflImgBFeatures.pGradYImg = aGradYBL3;
			oPyrOptflImgBFeatures.pGradXPyr = aGradXPyrBL3;
			oPyrOptflImgBFeatures.pGradYPyr = aGradYPyrBL3;

			/* Preprocess Image B */
			adi_TrackFeaturesImagePreProcess_i8(&oPyrOptflImgBFeatures,
				aMaskSmooth,
				aMaskHorzDeriv,
				aMaskVertDeriv);

			adi_TrackFeatures_i8(&oPyrOptflImgAFeatures,
				&oPyrOptflImgBFeatures,
				oPyrOptflSelectGoodFeatures.nNumGoodFeatures,
				oPyrOptflSelectGoodFeatures.pGoodFeatures,
				WINDOW_SIZE,
				MAX_NUM_ITERATIONS,
				MIN_DISPLACEMENT,
				aL1Buff + 2);  /* +2 for unrolling */

			printf("Previous Coordinates\tNew Coordinates\n");
			for (int j = 0; j < (oPyrOptflSelectGoodFeatures.nNumGoodFeatures); j++)
			{
				/*printf("\n(%d, %d)\t\t\t(%d, %d)",
					aGoodFeatures[j].nCoordX,
					aGoodFeatures[j].nCoordY,
					aGoodFeatures[j].nNewCoordX,
					aGoodFeatures[j].nNewCoordY);*/
				
				cv::circle(temp_image, cv::Point(aGoodFeatures[j].nCoordX, aGoodFeatures[j].nCoordY), 3, cv::Scalar(0, 255, 0));
				cv::circle(temp_image, cv::Point(aGoodFeatures[j].nNewCoordX,aGoodFeatures[j].nNewCoordY), 3, cv::Scalar(0, 0, 255));

				cv::line(temp_image, 
					cv::Point(aGoodFeatures[j].nCoordX, aGoodFeatures[j].nCoordY), 
					cv::Point(aGoodFeatures[j].nNewCoordX,aGoodFeatures[j].nNewCoordY),
					cv::Scalar(255, 255, 255), 1, 8);
			}

			// Calculate the time cost and print
			duration = static_cast<double>(cv::getTickCount()) - duration;
			duration /= cv::getTickFrequency();
			std::cout << duration * 1000 << " ms" << std::endl;
		}

		previous_image = input_image.clone();

		cv::imshow("result", temp_image);
		cv::waitKey(1);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	test_many_images();

	FILE            *pInputFile;
	FILE            *pOutputFile;
	uint32_t        nInputBytes;
	uint32_t        nOutputBytes;
	uint32_t        nInputoffset;
	int32_t         i, nInput_size;
	uint16_t        nInputDataLen;

	int32_t         x;

	int32_t         height;
	int32_t         width;

	enum COMMAND    aCommands[NUM_INPUT_FILES] =
	{
		OPTICAL_FLOW_LUCAS_KANADE_PYRAMID_8
	};

	nInputoffset = 0;

	/* Process all the input files */
	for (i = 0; i < NUM_INPUT_FILES; i++)
	{
		strcpy(aPathToMedia, "C:\\Analog Devices\\SoftwareModules\\Media\\IPTBX\\input\\");
		strcpy(atempFilename, aPathToMedia);
		strcat(atempFilename, aInFilename[i]);
		strcat(atempFilename, ".yuv");
		printf("*******************************************************************\n");
		printf("Input File: %s\n", aInFilename[i]);
		//printf("Input File Full Path: %s\n", atempFilename);
		pInputFile = fopen(atempFilename, "rb");
		if (pInputFile == NULL)
		{
			printf("Error opening %s\n", aInFilename[i]);
		}

		nInput_size = sizeof(uint8_t);

		for (x = 0; x < FRAME_START; x++)
		{
			nInputBytes = fread(aInputBufAL3 + (ADI_IMAGE_WIDTH * 2 * nInput_size),
				1U,
				ADI_IMAGE_SIZE * nInput_size,
				pInputFile);
		}

		nInputBytes = fread(aInputBufAL3 + (ADI_IMAGE_WIDTH * 2 * nInput_size),
			1U,
			ADI_IMAGE_SIZE * nInput_size,
			pInputFile);
		nInputBytes = fread(aInputBufBL3 + (ADI_IMAGE_WIDTH * 2 * nInput_size),
			1U,
			ADI_IMAGE_SIZE * nInput_size,
			pInputFile);

		fclose(pInputFile);

		switch (aCommands[i])
		{
		case OPTICAL_FLOW_LUCAS_KANADE_PYRAMID_8:
			printf("process => opticalflow - Lucas Kanade Pyramid 8bit\n");
			nInput_size = sizeof(int8_t);

			oPyrOptflSelectGoodFeatures.pImgIn = (uint8_t *) (aInputBufAL3);
			oPyrOptflSelectGoodFeatures.pImgOut = (uint8_t *) (aOutBufAL3);
			oPyrOptflSelectGoodFeatures.nImageWidth = ADI_IMAGE_WIDTH;
			oPyrOptflSelectGoodFeatures.nImageHeight = ADI_IMAGE_HEIGHT;
			oPyrOptflSelectGoodFeatures.pSmoothImgOut = aImgBufSmoothAL3;
			oPyrOptflSelectGoodFeatures.pGradX = aGradXAL3;
			oPyrOptflSelectGoodFeatures.pGradY = aGradYAL3;
			oPyrOptflSelectGoodFeatures.pScore = aScoreAL3;
			oPyrOptflSelectGoodFeatures.pGoodFeatures = aGoodFeatures;
			oPyrOptflSelectGoodFeatures.nSenstivity = HARRIS_SENSTIVITY;
			oPyrOptflSelectGoodFeatures.nThreshold = HARRIS_THRESHOLD;
			oPyrOptflSelectGoodFeatures.nMinDistance = HARRIS_MINDIST;
			oPyrOptflSelectGoodFeatures.pScratchBuf = aTempBuffL3;
			oPyrOptflSelectGoodFeatures.pScratchBufL1 = (uint8_t *)aL1BuffTemp;
			oPyrOptflSelectGoodFeatures.pFeatureCoord = aL3BuffStoreCoord;

			adi_SelGoodFeatPreProc_i8(&oPyrOptflSelectGoodFeatures,
				aMaskSmooth);

			adi_SelectGoodFeatures_Appl_i8(&oPyrOptflSelectGoodFeatures);

			adi_harrisMarkCorners(&oPyrOptflSelectGoodFeatures);

			oPyrOptflImgAFeatures.pImg = (uint8_t *) (aInputBufAL3);
			oPyrOptflImgAFeatures.nImageWidth = ADI_IMAGE_WIDTH;
			oPyrOptflImgAFeatures.nImageHeight = ADI_IMAGE_HEIGHT;
			oPyrOptflImgAFeatures.pSmoothImg = aImgBufSmoothAL3;
			oPyrOptflImgAFeatures.pImgPyr = aImgPyrAL3;
			oPyrOptflImgAFeatures.pGradXImg = aGradXAL3;
			oPyrOptflImgAFeatures.pGradYImg = aGradYAL3;
			oPyrOptflImgAFeatures.pGradXPyr = aGradXPyrAL3;
			oPyrOptflImgAFeatures.pGradYPyr = aGradYPyrAL3;

			/* Preprocess Image A */
			adi_TrackFeaturesImagePreProcess_i8(&oPyrOptflImgAFeatures,
				aMaskSmooth,
				aMaskHorzDeriv,
				aMaskVertDeriv);

			oPyrOptflImgBFeatures.pImg = (uint8_t *) (aInputBufBL3);
			oPyrOptflImgBFeatures.nImageWidth = ADI_IMAGE_WIDTH;
			oPyrOptflImgBFeatures.nImageHeight = ADI_IMAGE_HEIGHT;
			oPyrOptflImgBFeatures.pSmoothImg = aImgBufSmoothBL3;
			oPyrOptflImgBFeatures.pImgPyr = aImgPyrBL3;
			oPyrOptflImgBFeatures.pGradXImg = aGradXBL3;
			oPyrOptflImgBFeatures.pGradYImg = aGradYBL3;
			oPyrOptflImgBFeatures.pGradXPyr = aGradXPyrBL3;
			oPyrOptflImgBFeatures.pGradYPyr = aGradYPyrBL3;

			/* Preprocess Image B */
			adi_TrackFeaturesImagePreProcess_i8(&oPyrOptflImgBFeatures,
				aMaskSmooth,
				aMaskHorzDeriv,
				aMaskVertDeriv);

			adi_TrackFeatures_i8(&oPyrOptflImgAFeatures,
				&oPyrOptflImgBFeatures,
				oPyrOptflSelectGoodFeatures.nNumGoodFeatures,
				oPyrOptflSelectGoodFeatures.pGoodFeatures,
				WINDOW_SIZE,
				MAX_NUM_ITERATIONS,
				MIN_DISPLACEMENT,
				aL1Buff + 2);  /* +2 for unrolling */
			break;

		default:
			break;
		}

		strcpy(aOutFilename, aInFilename[i]);

		strcat(aOutFilename, "_tracked_features");
		strcat(aOutFilename, ".txt");

		/* Write to Output File */
		printf("Output File: %s\n", aOutFilename);
		strcpy(aOutFilenameWithPath, aOutputPath);
		strcat(aOutFilenameWithPath, aOutFilename);
		pOutputFile = fopen(aOutFilenameWithPath, "w");
		if (pOutputFile == NULL)
		{
			printf("Error opening %s\n", aOutFilename);
		}

		fprintf(pOutputFile, "\nPrevious Coordinates\tNew Coordinates\n\n");
		for (i = 0; i < (oPyrOptflSelectGoodFeatures.nNumGoodFeatures); i++)
		{
			fprintf(pOutputFile,
				"\n(%d, %d)\t\t\t\t(%d, %d)",
				aGoodFeatures[i].nCoordX,
				aGoodFeatures[i].nCoordY,
				aGoodFeatures[i].nNewCoordX,
				aGoodFeatures[i].nNewCoordY);
		}

		fclose(pOutputFile);

	}

	printf("*******************************************************************\n");
	printf("Done\n");

	return 0;
}

