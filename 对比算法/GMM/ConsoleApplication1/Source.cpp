/***********************高斯混合模型*******************************/
/********************来自CSDN**************************************/

/************************方法一************好使***************************/
//目前的缺点，第一帧中运动个目标的位置一直都存在，不会消除
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(){
	VideoCapture video("test.mp4");
	//string first_file = "C:\\master\\dataset\\saliency\\Toy\\Toy\\img//%04d.jpg";
	//VideoCapture sequence(first_file);

	Mat frame, mask, thresholdImage, output;
	video/*sequence*/ >> frame;
	int frameNum = 0;//自己定义的
	BackgroundSubtractorMOG bgSubtractor(20, 10, 0.5, false);//bgSubtractor:构造函数   作用：用来在创建对象时完成对对象属性的一些初始化等操作
	while (true){
		video/*sequence*/ >> frame;
		++frameNum;//为什么要++？
		bgSubtractor(frame, mask, 0.001);
		imshow("mask", mask);
		waitKey(10);
	}
	return 0;
}


/**************方法二**************好使，相比于方法一噪声明显*************/
//int main(){
//	VideoCapture video("test.mp4");
//	Mat frame, mask, thresholdImage, output;
//	//video>>frame;
//	int frameNum = 0;
//	
//	BackgroundSubtractorMOG2 bgSubtractor(20, 16, true);
//	while (true){
//		video >> frame;
//		++frameNum;
//		bgSubtractor(frame, mask, 0.001);
//		cout << frameNum << endl;
//		imshow("mask",mask);  
//		waitKey(10);  
//	}
//	return 0;
//}


/******************改进的高斯混合背景模型的实现**********不好使*******/
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2\opencv.hpp>
//#include <vector>
//using namespace cv;
//using namespace std;
//
//
//int main()
//{
//	CvCapture *capture = cvCreateFileCapture("test.mp4");
//	IplImage *img, *img_gray, *img_cny, *back, *fore;//原图像、灰度图像、边缘检测得到的图像、背景图像、前景图像
//	int width, height;//图像的长、宽
//	int cnt = 0; //视频帧数
//	img = cvQueryFrame(capture);
//	cnt++;
//	width = img->width; height = img->height;
//	vector< vector<double> > mean(width*height), sd(width*height), w(width*height);
//	img_gray = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
//	img_cny = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
//	back = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
//	fore = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
//	cvCvtColor(img, img_gray, CV_BGR2GRAY);
//	cvCopy(img_gray, back);
//	//初始化混合高斯背景模型
//	int K = 5, sd_init = 16, T = 100, match = 0;//高斯分布的个数、标准差的初始值 帧数阈值 匹配参数
//	double w_init = 0.05, D = 2.5, alph, p, threold = 0.7;;//权重的初始值、置信参数、学习速率、参数更新速率、预定阈值（用于选取背景模型）
//	for (int i = 0; i < height; i++)
//	{
//		for (int j = 0; j < width; j++)
//		{
//			mean[i*width + j].push_back((uchar)img_gray->imageData[i*width + j]);
//			sd[i*width + j].push_back(sd_init);
//			w[i*width + j].push_back(1);
//		}
//	}
//	while (1)
//	{
//		img = cvQueryFrame(capture);
//		if (!img) break;
//		cvCvtColor(img, img_gray, CV_BGR2GRAY);
//		cnt++;
//
//		//计算学习速率
//		if (cnt < T) alph = (double)1 / (2 * cnt);
//		else alph = (double)1 / (2 * T);
//
//		for (int i = 0; i < height; i++)
//		{
//			for (int j = 0; j < width; j++)
//			{
//				match = 0;
//				int len = mean[i*width + j].size();
//				for (int k = 0; k < len; k++)
//				{
//					//判断一个像素是否与背景模型匹配
//					if (fabs((uchar)img_gray->imageData[i*width + j] - mean[i*width + j][k]) < D*sd[i*width + j][k])
//					{
//						match = 1;
//						//更新权重、均值、标准差
//						w[i*width + j][k] = (1 - alph)*w[i*width + j][k] + alph;
//						p = alph / w[i*width + j][k];
//						mean[i*width + j][k] = (1 - p)*mean[i*width + j][k] + p*(uchar)img_gray->imageData[i*width + j];
//						sd[i*width + j][k] = sqrt((1 - p)*sd[i*width + j][k] * sd[i*width + j][k] + p*pow((uchar)img_gray->imageData[i*width + j] - mean[i*width + j][k], 2));
//					}
//					else
//					{
//						w[i*width + j][k] = (1 - alph)*w[i*width + j][k];
//					}
//				}
//				if (match == 0)
//				{
//					//增加新的背景模型
//					if (len < K)
//					{
//						mean[i*width + j].push_back((uchar)img_gray->imageData[i*width + j]);
//						sd[i*width + j].push_back(sd_init);
//						w[i*width + j].push_back(w_init);
//					}
//					//更改权重最小的背景模型
//					else
//					{
//						int min = 0;
//						double tmp = w[i*width + j][0];
//						for (int k = 1; k < len; k++)
//						{
//							if (tmp >  w[i*width + j][k])
//							{
//								min = k;
//								tmp = w[i*width + j][k];
//							}
//						}
//						mean[i*width + j][min] = (uchar)img_gray->imageData[i*width + j];
//						sd[i*width + j][min] = sd_init;
//						w[i*width + j][min] = w_init;
//					}
//				}
//				//每隔50帧 删除权重最小的高斯分布	
//				if (cnt % 50 == 0)
//				{
//					vector<double>::iterator it_w, it_sd, it_mean;
//					it_w = w[i*width + j].begin();
//					it_sd = sd[i*width + j].begin();
//					it_mean = mean[i*width + j].begin();
//					for (; it_w != w[i*width + j].end() && it_sd != sd[i*width + j].end() && it_mean != mean[i*width + j].end();)
//					{
//						if (*it_w < w_init && (*it_w / *it_sd) < (w_init / sd_init))
//						{
//							it_w = w[i*width + j].erase(it_w);
//							it_sd = sd[i*width + j].erase(it_sd);
//							it_mean = mean[i*width + j].erase(it_mean);
//						}
//						else
//						{
//							it_w++;
//							it_sd++;
//							it_mean++;
//						}
//					}
//				}
//				//权重归一化
//				len = mean[i*width + j].size();
//				double tmp = 0;
//				for (int k = 0; k < len; k++)
//				{
//					tmp += w[i*width + j][k];
//				}
//				for (int k = 0; k < len; k++)
//				{
//					w[i*width + j][k] /= tmp;
//				}
//				//计算各背景模型的优先级，并以从大到小的顺序排序
//				double *rand;//优先级
//				int *rand_ind;//排序后的优先级索引
//				rand = (double *)malloc(sizeof(double)*len);
//				rand_ind = (int *)malloc(sizeof(int)*len);
//				for (int k = 0; k < len; k++)
//				{
//					rand[k] = w[i*width + j][k] / sd[i*width + j][k];
//					rand_ind[k] = k;
//				}
//				for (int k = 1; k< len; k++)
//				{
//					for (int l = 0; l < k; l++)
//					{
//						if (rand[k] > rand[l])
//						{
//							double rand_tmp = rand[k];
//							rand[k] = rand[l];
//							rand[l] = rand_tmp;
//
//							double rand_ind_tmp = rand_ind[k];
//							rand_ind[k] = rand_ind[l];
//							rand_ind[l] = rand_ind_tmp;
//
//						}
//					}
//				}
//				double sum = 0;
//				int B;
//				for (int k = 0; k < len; k++)
//				{
//					int temp = rand_ind[k];
//					sum += w[i*width + j][temp];
//					if (sum>threold)
//					{
//						B = k;
//						break;
//					}
//				}
//				match = 0;
//				back->imageData[i*width + j] = 0;
//				//求背景模型
//				for (int k = 0; k <= B; k++)
//				{
//					int temp = rand_ind[k];
//					back->imageData[i*width + j] += w[i*width + j][temp] * mean[i*width + j][temp];
//				}
//				/*
//				//求前景图像
//				for (int k = 0; k <= B; k++)
//				{
//				int temp = rand_ind[k];
//				if (fabs((uchar)img_gray->imageData[i*width + j] - mean[i*width + j][temp]) < D*sd[i*width + j][temp])
//				{
//				match = 1;
//				break;
//				}
//				}
//				if (match == 1) fore->imageData[i*width + j] = 0;
//				else fore->imageData[i*width + j] = (uchar)img_gray->imageData[i*width + j];
//				*/
//				free(rand);
//				free(rand_ind);
//			}
//		}
//
//		cvAbsDiff(img_gray, back, fore);
//		cvCanny(img_gray, img_cny, 1, 100);
//		cvShowImage("canny", img_cny);
//		cvAnd(img_cny, fore, fore);
//		cvShowImage("fore", fore);
//		cvShowImage("back", back);
//		cvShowImage("src", img);
//		char s = cvWaitKey(33);
//		if (s == 27) break;
//	}
//	cvDestroyAllWindows();
//	cvReleaseCapture(&capture);
//	cvReleaseImage(&img_gray);
//	cvReleaseImage(&fore);
//	cvReleaseImage(&back);
//	cvReleaseImage(&img_cny);
//	return 0;
//}