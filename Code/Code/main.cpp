#include <cv.h>
#include <highgui.h>
#include "opencv2/legacy/legacy.hpp"
#include <math.h>
#include <adhs.h>
//#include "opencv2/core/core_c.h"
//#include "opencv2/core/core.hpp"
//#define _CRT_SECURE_NO_WARNINGS 1
//static const double pi = 3.14159265358979323846;
using namespace cv;
using namespace std;

/*
inline static void allocateOnDemand( IplImage **img, CvSize size, int depth, int channels)   //分配需求
{
if ( *img != NULL ) return;
*img = cvCreateImage( size, depth, channels );
if ( *img == NULL )
{
fprintf(stderr, "Error: Couldn't allocate image. Out of memory?\n");
exit(-1);
}
}*/

int main()
{
	IplImage * frame = NULL;
	IplImage * frame_buf[5] = { NULL };
	int i, j, rows, cols;
	float dx, dy, f;
	IplImage *src_img1 = NULL, *src_img2 = NULL, *dst_img1 = NULL, *dst_img2 = NULL;
	CvMat *velx, *vely, *sal_map;
	CvTermCriteria criteria;

	CvCapture *input_video = cvCreateFileCapture("test.mp4");//cvCaptureFromFile SampleVideo.avi demo-cut.avi kunshan.avi
	if (input_video == NULL)
	{

		/* Either the video didn't exist OR it uses a codec OpenCV
		* doesn't support.
		*/
		fprintf(stderr, "Error: Can't open video.\n");
		return -1; //就是函数结束后，返回个-1，经常用于错误返回 
	}
	cvQueryFrame(input_video);
	/* Read the video's frame size out of the AVI. */ //读取视频帧大小
	CvSize frame_size;
	frame_size.height = (int)cvGetCaptureProperty(input_video, CV_CAP_PROP_FRAME_HEIGHT); //获得视频获取结构的属性
	frame_size.width = (int)cvGetCaptureProperty(input_video, CV_CAP_PROP_FRAME_WIDTH);

	long number_of_frames;
	/* Go to the end of the AVI (ie: the fraction is "1") *///设置视频获取属性
	cvSetCaptureProperty(input_video, CV_CAP_PROP_POS_AVI_RATIO, 1.);
	/* Now that we're at the end, read the AVI position in frames */
	number_of_frames = (int)cvGetCaptureProperty(input_video, CV_CAP_PROP_POS_FRAMES);
	/* Return to the beginning *///设置视频获取属性
	cvSetCaptureProperty(input_video, CV_CAP_PROP_POS_FRAMES, 0.); //单位为帧数的位置（只对视频文件有效

	long current_frame = 0;

	cvSetCaptureProperty(input_video, CV_CAP_PROP_POS_FRAMES, current_frame);

	for (i = 0; i < 5; i++)
	{
		frame = cvQueryFrame(input_video);
		frame_buf[i] = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, frame_buf[i], CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
	}

	//进入循环
	for (int frame_index = 0; frame_index < number_of_frames; frame_index++)
	{
		if (frame_index > 0)
		{
			cvCopy(frame_buf[1], frame_buf[0]);
			cvCopy(frame_buf[2], frame_buf[1]);
			cvCopy(frame_buf[3], frame_buf[2]);
			cvCopy(frame_buf[4], frame_buf[3]);
			/*frame_buf[0] = frame_buf[1];
			frame_buf[1] = frame_buf[2];
			frame_buf[2] = frame_buf[3];
			frame_buf[3] = frame_buf[4];
			frame = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);*/
			frame = cvQueryFrame(input_video);
			cvCvtColor(frame, frame_buf[4], CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
		}
		dst_img2 = (IplImage *)cvClone(frame_buf[2]);
		cols = frame_buf[0]->width;
		rows = frame_buf[0]->height;
		velx = cvCreateMat(rows, cols, CV_32FC1);
		vely = cvCreateMat(rows, cols, CV_32FC1);
		sal_map = cvCreateMat(rows, cols, CV_32FC1);
		cvSetZero(velx);
		cvSetZero(vely);
		criteria = cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1);

		// (2)计算（HS）
		adHS(frame_buf[0], frame_buf[1], frame_buf[2], frame_buf[3], frame_buf[4], 0, velx, vely, 0.9, 100, criteria);
		//（3） 画图

		for (i = 0; i < cols; i++) {
			for (j = 0; j < rows; j++) {
				dx = (float)cvGetReal2D(velx, j, i);
				dy = (float)cvGetReal2D(vely, j, i);
				f = sqrt(dx*dx + dy*dy) /100;
				cvSet2D(sal_map, j, i, cvScalar(f));
				/*if ((5<dx<10) && (5<dy<10))
					cvLine(dst_img2, cvPoint(i, j), cvPoint(i + dx, j + dy), CV_RGB(255, 0, 0), 1, CV_AA, 0);*/
			}
		}

		cvNamedWindow("Image", 1);
		cvShowImage("Image", frame);

		cvNamedWindow("Image_test1", 1);
		cvShowImage("Image_test1", frame_buf[3]);

		cvNamedWindow("Image_test2", 1);
		cvShowImage("Image_test2", frame_buf[4]);

		cvNamedWindow("Sal_Map", 1);
		cvShowImage("Sal_Map", sal_map);

		int key_pressed;
		key_pressed = cvWaitKey(30);
		if (key_pressed == 'b' || key_pressed == 'B') break;
		else
			current_frame = current_frame + 2;
		/* Don't run past the front/end of the AVI. */
		//if (current_frame < 0) current_frame = 0;
		if (current_frame >= number_of_frames - 1) current_frame = number_of_frames - 2;
	}

	


	cvDestroyWindow("ImageHS");
	//    cvDestroyWindow ("ImageLK");
	cvReleaseImage(&src_img1);
	cvReleaseImage(&src_img2);
	cvReleaseImage(&dst_img1);
	cvReleaseImage(&dst_img2);
	cvReleaseMat(&velx);
	cvReleaseMat(&vely);
}