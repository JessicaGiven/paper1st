/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#include <precomp.hpp>

#include "opencv2/legacy/legacy.hpp"

#include "opencv2/core/internal.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/video/background_segm.hpp"

#include "opencv2/legacy/blobtrack.hpp"
#include "opencv2/legacy/compat.hpp"

#define CONV( A, B, C)  ( (float)( A +  (B<<1)  + C ) )

typedef struct
{
    float xx;
    float xy;
    float yy;
    float xt;
    float yt;
    float alpha;                /* alpha = 1 / ( 1/lambda + xx + yy ) */
}
icvDerProductEx;

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name: icvCalcOpticalFlowHS_8u32fR (Horn & Schunck method )
//    Purpose: calculate Optical flow for 2 images using Horn & Schunck algorithm
//    Context:
//    Parameters:
//            imgA          -  pointer to first frame ROI 指向第一帧图像ROI的指针
//            imgB          -  pointer to second frame ROI 指向第二帧图像ROI的指针
//            imgStep       -  width of single row of source images in bytes 原图像一行的像素数
//            imgSize       -  size of the source image ROI 原图像尺寸
//            usePrevious   -  use previous (input) velocity field. 是否使用上一帧的速度向量
//            velocityX     -  pointer to horizontal and 
//            velocityY     -  vertical components of optical flow ROI X、Y方向的速度向量帧
//            velStep       -  width of single row of velocity frames in bytes 速度向量帧行宽
//            lambda        -  Lagrangian multiplier 拉格朗日乘子？
//            criteria      -  criteria of termination processmaximum number of iterations 循环执行规则
//
//    Returns: CV_OK         - all ok 成功标志位
//             CV_OUTOFMEM_ERR  - insufficient memory for function work
//             CV_NULLPTR_ERR - if one of input pointers is NULL
//             CV_BADSIZE_ERR   - wrong input sizes interrelation 一系列错误返回值
//
//    Notes:  1.Optical flow to be computed for every pixel in ROI 针对ROI中的所有像素计算光流
//            2.For calculating spatial derivatives we use 3x3 Sobel operator. 用3x3的Sobel算子计算空间导数
//            3.We use the following border mode.
//              The last row or column is replicated for the border 复制最后一行或最后一列作为边界
//              ( IPL_BORDER_REPLICATE in IPL ).
//
//
//F*/
static CvStatus CV_STDCALL
icvCalcOpticalFlowHS_8u32fR( uchar*  imgA,
                             uchar*  imgB,
							 uchar*  imgC,
							 uchar*  imgD,
							 uchar*  imgE,
                             int     imgStep,
                             CvSize imgSize,
                             int     usePrevious,
                             float*  velocityX,
                             float*  velocityY,
                             int     velStep,
                             float   lambda,
							 float   alpha,
                             CvTermCriteria criteria )
{
    /* Loops indexes 循环索引 */ 
    int i, j, k, address;

    /* Buffers for Sobel calculations Sobel滤波的缓冲区 */
    float *MemX[2];
    float *MemY[2];

    float ConvX, ConvY;
    float GradX, GradY, GradT;

    int imageWidth = imgSize.width;
    int imageHeight = imgSize.height;

    int ConvLine;
    int LastLine;

    int BufferSize;

    float Ilambda = 1 / lambda;
    int iter = 0;
    int Stop;

    /* buffers derivatives product 导数运算结果的缓冲区 */
    icvDerProductEx *II[10];

    float *VelBufX[2];
    float *VelBufY[2];

    /* variables for storing number of first pixel of image line 用于存储图像头一个像素的变量 */
    int Line1;
    int Line2;
    int Line3;

    int pixNumber;
	 
    /* auxiliary 辅助变量*/
    int NoMem = 0;

    /* Checking bad arguments 检查错误 */
    if( imgA == NULL )
        return CV_NULLPTR_ERR;
    if( imgB == NULL )
        return CV_NULLPTR_ERR;

    if( imgSize.width <= 0 )
        return CV_BADSIZE_ERR;
    if( imgSize.height <= 0 )
        return CV_BADSIZE_ERR;
    if( imgSize.width > imgStep )
        return CV_BADSIZE_ERR;

    if( (velStep & 3) != 0 )
        return CV_BADSIZE_ERR;

    velStep /= 4;

    /****************************************************************************************/
    /* Allocating memory for all buffers 为所有缓冲区分配内存                                 */
    /****************************************************************************************/
    for( k = 0; k < 2; k++ )
    {
        MemX[k] = (float *) cvAlloc( (imgSize.height) * sizeof( float ));

        if( MemX[k] == NULL )
            NoMem = 1;
        MemY[k] = (float *) cvAlloc( (imgSize.width) * sizeof( float ));

        if( MemY[k] == NULL )
            NoMem = 1;

        VelBufX[k] = (float *) cvAlloc( imageWidth * sizeof( float ));

        if( VelBufX[k] == NULL )
            NoMem = 1;
        VelBufY[k] = (float *) cvAlloc( imageWidth * sizeof( float ));

        if( VelBufY[k] == NULL )
            NoMem = 1;
    }

    BufferSize = imageHeight * imageWidth;

    II = (icvDerProductEx *) cvAlloc( BufferSize * sizeof( icvDerProductEx ));
    if( II == NULL )
        NoMem = 1;

    if( NoMem )
    {
        for( k = 0; k < 2; k++ )
        {
            if( MemX[k] )
                cvFree( &MemX[k] );

            if( MemY[k] )
                cvFree( &MemY[k] );

            if( VelBufX[k] )
                cvFree( &VelBufX[k] );

            if( VelBufY[k] )
                cvFree( &VelBufY[k] );
        }
        if( II )
            cvFree( &II );
        return CV_OUTOFMEM_ERR;
    }
/****************************************************************************************\
*         Calculate first line of memX and memY 计算 memX 和 memY 的第一行                *
\****************************************************************************************/
    MemY[0][0] = MemY[1][0] = CONV( imgA[0], imgA[0], imgA[1] );
    MemX[0][0] = MemX[1][0] = CONV( imgA[0], imgA[0], imgA[imgStep] );

    for( j = 1; j < imageWidth - 1; j++ )
    {
        MemY[0][j] = MemY[1][j] = CONV( imgA[j - 1], imgA[j], imgA[j + 1] );
    }

    pixNumber = imgStep;
    for( i = 1; i < imageHeight - 1; i++ )
    {
        MemX[0][i] = MemX[1][i] = CONV( imgA[pixNumber - imgStep],
                                        imgA[pixNumber], imgA[pixNumber + imgStep] );
        pixNumber += imgStep;
    }

    MemY[0][imageWidth - 1] =
        MemY[1][imageWidth - 1] = CONV( imgA[imageWidth - 2],
                                        imgA[imageWidth - 1], imgA[imageWidth - 1] );

    MemX[0][imageHeight - 1] =
        MemX[1][imageHeight - 1] = CONV( imgA[pixNumber - imgStep],
                                         imgA[pixNumber], imgA[pixNumber] );


/****************************************************************************************\
*     begin scan image, calc derivatives 开始遍历图像，计算导数                            *
\****************************************************************************************/

    ConvLine = 0;
    Line2 = -imgStep;
    address = 0;
    LastLine = imgStep * (imageHeight - 1);
    while( ConvLine < imageHeight )
    {
        /*Here we calculate derivatives for line of image */
        int memYline = (ConvLine + 1) & 1;

        Line2 += imgStep;
        Line1 = Line2 - ((Line2 == 0) ? 0 : imgStep);
        Line3 = Line2 + ((Line2 == LastLine) ? 0 : imgStep);

        /* Process first pixel */
        ConvX = CONV( imgA[Line1 + 1], imgA[Line2 + 1], imgA[Line3 + 1] );
        ConvY = CONV( imgA[Line3], imgA[Line3], imgA[Line3 + 1] );

        GradY = (ConvY - MemY[memYline][0]) * 0.125f;
        GradX = (ConvX - MemX[1][ConvLine]) * 0.125f;

        MemY[memYline][0] = ConvY;
        MemX[1][ConvLine] = ConvX;

        GradT = (float) (imgB[Line2] - imgA[Line2]);

        II[address].xx = GradX * GradX;
        II[address].xy = GradX * GradY;
        II[address].yy = GradY * GradY;
        II[address].xt = GradX * GradT;
        II[address].yt = GradY * GradT;

        II[address].alpha = 1 / (Ilambda + II[address].xx + II[address].yy);
        address++;

        /* Process middle of line */
        for( j = 1; j < imageWidth - 1; j++ )
        {
            ConvX = CONV( imgA[Line1 + j + 1], imgA[Line2 + j + 1], imgA[Line3 + j + 1] );
            ConvY = CONV( imgA[Line3 + j - 1], imgA[Line3 + j], imgA[Line3 + j + 1] );

            GradY = (ConvY - MemY[memYline][j]) * 0.125f;
            GradX = (ConvX - MemX[(j - 1) & 1][ConvLine]) * 0.125f;

            MemY[memYline][j] = ConvY;
            MemX[(j - 1) & 1][ConvLine] = ConvX;

            GradT = (float) (imgB[Line2 + j] - imgA[Line2 + j]);

            II[address].xx = GradX * GradX;
            II[address].xy = GradX * GradY;
            II[address].yy = GradY * GradY;
            II[address].xt = GradX * GradT;
            II[address].yt = GradY * GradT;

            II[address].alpha = 1 / (Ilambda + II[address].xx + II[address].yy);
            address++;
        }
        /* Process last pixel of line */
        ConvX = CONV( imgA[Line1 + imageWidth - 1], imgA[Line2 + imageWidth - 1],
                      imgA[Line3 + imageWidth - 1] );

        ConvY = CONV( imgA[Line3 + imageWidth - 2], imgA[Line3 + imageWidth - 1],
                      imgA[Line3 + imageWidth - 1] );


        GradY = (ConvY - MemY[memYline][imageWidth - 1]) * 0.125f;
        GradX = (ConvX - MemX[(imageWidth - 2) & 1][ConvLine]) * 0.125f;

        MemY[memYline][imageWidth - 1] = ConvY;

        GradT = (float) (imgB[Line2 + imageWidth - 1] - imgA[Line2 + imageWidth - 1]);

        II[address].xx = GradX * GradX;
        II[address].xy = GradX * GradY;
        II[address].yy = GradY * GradY;
        II[address].xt = GradX * GradT;
        II[address].yt = GradY * GradT;

        II[address].alpha = 1 / (Ilambda + II[address].xx + II[address].yy);
        address++;

        ConvLine++;
    }
/****************************************************************************************\
*      Prepare initial approximation  准备初始的近似值                                    *
\****************************************************************************************/
    if( !usePrevious )
    {
        float *vx = velocityX;
        float *vy = velocityY;

        for( i = 0; i < imageHeight; i++ )
        {
            memset( vx, 0, imageWidth * sizeof( float ));
            memset( vy, 0, imageWidth * sizeof( float ));

            vx += velStep;
            vy += velStep;
        }
    }
/****************************************************************************************\
*      Perform iterations 处理循环                                                        *
\****************************************************************************************/
    iter = 0;
    Stop = 0;
    LastLine = velStep * (imageHeight - 1);
    while( !Stop )
    {
        float Eps = 0;
        address = 0;

        iter++;
/****************************************************************************************\
*     begin scan velocity and update it 计算速度并更新                                    *
\****************************************************************************************/
        Line2 = -velStep;
        for( i = 0; i < imageHeight; i++ )
        {
            /* Here average velocity */

            float averageX;
            float averageY;
            float tmp;

            Line2 += velStep;
            Line1 = Line2 - ((Line2 == 0) ? 0 : velStep);
            Line3 = Line2 + ((Line2 == LastLine) ? 0 : velStep);
            /* Process first pixel */
            averageX = (velocityX[Line2] +
                        velocityX[Line2 + 1] + velocityX[Line1] + velocityX[Line3]) / 4;

            averageY = (velocityY[Line2] +
                        velocityY[Line2 + 1] + velocityY[Line1] + velocityY[Line3]) / 4;

            VelBufX[i & 1][0] = averageX -
                (II[address].xx * averageX +
                 II[address].xy * averageY + II[address].xt) * II[address].alpha;

            VelBufY[i & 1][0] = averageY -
                (II[address].xy * averageX +
                 II[address].yy * averageY + II[address].yt) * II[address].alpha;

            /* update Epsilon */
            if( criteria.type & CV_TERMCRIT_EPS )
            {
                tmp = (float)fabs(velocityX[Line2] - VelBufX[i & 1][0]);
                Eps = MAX( tmp, Eps );
                tmp = (float)fabs(velocityY[Line2] - VelBufY[i & 1][0]);
                Eps = MAX( tmp, Eps );
            }
            address++;
            /* Process middle of line */
            for( j = 1; j < imageWidth - 1; j++ )
            {
                averageX = (velocityX[Line2 + j - 1] +
                            velocityX[Line2 + j + 1] +
                            velocityX[Line1 + j] + velocityX[Line3 + j]) / 4;
                averageY = (velocityY[Line2 + j - 1] +
                            velocityY[Line2 + j + 1] +
                            velocityY[Line1 + j] + velocityY[Line3 + j]) / 4;

                VelBufX[i & 1][j] = averageX -
                    (II[address].xx * averageX +
                     II[address].xy * averageY + II[address].xt) * II[address].alpha;

                VelBufY[i & 1][j] = averageY -
                    (II[address].xy * averageX +
                     II[address].yy * averageY + II[address].yt) * II[address].alpha;
                /* update Epsilon */
                if( criteria.type & CV_TERMCRIT_EPS )
                {
                    tmp = (float)fabs(velocityX[Line2 + j] - VelBufX[i & 1][j]);
                    Eps = MAX( tmp, Eps );
                    tmp = (float)fabs(velocityY[Line2 + j] - VelBufY[i & 1][j]);
                    Eps = MAX( tmp, Eps );
                }
                address++;
            }
            /* Process last pixel of line */
            averageX = (velocityX[Line2 + imageWidth - 2] +
                        velocityX[Line2 + imageWidth - 1] +
                        velocityX[Line1 + imageWidth - 1] +
                        velocityX[Line3 + imageWidth - 1]) / 4;

            averageY = (velocityY[Line2 + imageWidth - 2] +
                        velocityY[Line2 + imageWidth - 1] +
                        velocityY[Line1 + imageWidth - 1] +
                        velocityY[Line3 + imageWidth - 1]) / 4;


            VelBufX[i & 1][imageWidth - 1] = averageX -
                (II[address].xx * averageX +
                 II[address].xy * averageY + II[address].xt) * II[address].alpha;

            VelBufY[i & 1][imageWidth - 1] = averageY -
                (II[address].xy * averageX +
                 II[address].yy * averageY + II[address].yt) * II[address].alpha;

            /* update Epsilon */
            if( criteria.type & CV_TERMCRIT_EPS )
            {
                tmp = (float)fabs(velocityX[Line2 + imageWidth - 1] -
                                  VelBufX[i & 1][imageWidth - 1]);
                Eps = MAX( tmp, Eps );
                tmp = (float)fabs(velocityY[Line2 + imageWidth - 1] -
                                  VelBufY[i & 1][imageWidth - 1]);
                Eps = MAX( tmp, Eps );
            }
            address++;

            /* store new velocity from old buffer to velocity frame */
            if( i > 0 )
            {
                memcpy( &velocityX[Line1], VelBufX[(i - 1) & 1], imageWidth * sizeof( float ));
                memcpy( &velocityY[Line1], VelBufY[(i - 1) & 1], imageWidth * sizeof( float ));
            }
        }                       /*for */
        /* store new velocity from old buffer to velocity frame */
        memcpy( &velocityX[imageWidth * (imageHeight - 1)],
                VelBufX[(imageHeight - 1) & 1], imageWidth * sizeof( float ));

        memcpy( &velocityY[imageWidth * (imageHeight - 1)],
                VelBufY[(imageHeight - 1) & 1], imageWidth * sizeof( float ));

        if( (criteria.type & CV_TERMCRIT_ITER) && (iter == criteria.max_iter) )
            Stop = 1;
        if( (criteria.type & CV_TERMCRIT_EPS) && (Eps < criteria.epsilon) )
            Stop = 1;
    }
    /* Free memory */
    for( k = 0; k < 2; k++ )
    {
        cvFree( &MemX[k] );
        cvFree( &MemY[k] );
        cvFree( &VelBufX[k] );
        cvFree( &VelBufY[k] );
    }
    cvFree( &II );

    return CV_OK;
} /*icvCalcOpticalFlowHS_8u32fR*/


/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    cvCalcOpticalFlowHS
//    Purpose: Optical flow implementation
//    Context:
//    Parameters:
//             srcA, srcB - source image
//             velx, vely - destination image
//    Returns:
//
//    Notes:
//F*/
CV_IMPL void
adHS(const void* srcarrA, const void* srcarrB, const void* srcarrC, const void* srcarrD, const void* srcarrE, int usePrevious,
                     void* velarrx, void* velarry,
					 float alpha, 
                     double lambda, CvTermCriteria criteria )
{
    CvMat stubA, *srcA = cvGetMat( srcarrA, &stubA );
    CvMat stubB, *srcB = cvGetMat( srcarrB, &stubB );
	CvMat stubC, *srcC = cvGetMat( srcarrC, &stubC );
	CvMat stubD, *srcD = cvGetMat( srcarrD, &stubD );
	CvMat stubE, *srcE = cvGetMat( srcarrE, &stubE );
    CvMat stubx, *velx = cvGetMat( velarrx, &stubx );
    CvMat stuby, *vely = cvGetMat( velarry, &stuby );	//转化输入格式CvArr到CvMat

    /*if( !CV_ARE_TYPES_EQ( srcA, srcB ))
        CV_Error( CV_StsUnmatchedFormats, "Source images have different formats" );

    if( !CV_ARE_TYPES_EQ( velx, vely ))
        CV_Error( CV_StsUnmatchedFormats, "Destination images have different formats" );

    if( !CV_ARE_SIZES_EQ( srcA, srcB ) ||
        !CV_ARE_SIZES_EQ( velx, vely ) ||
        !CV_ARE_SIZES_EQ( srcA, velx ))
        CV_Error( CV_StsUnmatchedSizes, "" );

    if( CV_MAT_TYPE( srcA->type ) != CV_8UC1 ||
        CV_MAT_TYPE( velx->type ) != CV_32FC1 )
        CV_Error( CV_StsUnsupportedFormat, "Source images must have 8uC1 type and "
                                           "destination images must have 32fC1 type" );

    if( srcA->step != srcB->step || velx->step != vely->step )
        CV_Error( CV_BadStep, "source and destination images have different step" );*/

	IPPI_CALL(icvCalcOpticalFlowHS_8u32fR((uchar*)srcA->data.ptr, (uchar*)srcB->data.ptr, (uchar*)srcC->data.ptr, (uchar*)srcD->data.ptr, (uchar*)srcE->data.ptr,
                                            srcA->step, cvGetMatSize( srcA ), usePrevious,
                                            velx->data.fl, vely->data.fl,
                                            velx->step, (float)lambda, alpha, criteria ));
}

/* End of file. */
