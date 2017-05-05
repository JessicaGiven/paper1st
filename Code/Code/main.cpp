#include <iostream>
#include "opencv2/opencv.hpp"
#include <stdio.h>

#include "CmLib.h" // https://github.com/MingMingCheng/CmCode/wiki

#include <stdio.h>
#include <tchar.h>

using namespace cv;
using namespace std;

// 光流显著图计算函数
Mat computeSalMap(Mat *flow, Mat gray)
{
	Mat result(gray.size(), CV_32FC1);
	float f1;
	float f2;
	float f3[5];
	float f4 = 0;
	float temp = 0;
	 
	for (int i = 0; i < flow[0].rows; i++)
	{
		for (int j = 0; j < flow[0].cols; j++)
		{
			for (int fi = 0; fi < 5; fi++)
			{
				f1 = flow[fi].ptr<float>(i, j)[0];
				f2 = flow[fi].ptr<float>(i, j)[1];
				f3[fi] = sqrt(f1*f1 * 100 + f2*f2 * 100) / 255; // 100 和 255 都可以修改，影响显著图的整体显著度
			}
			temp = 0.2 * f3[0] + 0.8 * f3[1] + 0.8 * f3[2] + 0.2 * f3[3] + 0.1 * f3[4]; // 关键平方加权和公式
			if (f4 < temp) f4 = temp;
			result.at<float>(i, j) = temp;
		}
	}
	result = result.mul(1 / f4);

	return result;
}

int main()
{
	cout << "Please choose mode: (0)OP (1)RC (2)both  " << endl;	// 模式选择 (0)光流 (1)RC (2)前两种方法结合
	int mode = 0;
	cin >> mode;
	if (mode == 0) // 光流方法
	{
		VideoCapture cap;
		//cap.open(0);
		cap.open("test1.mp4");	// 光流方法输入视频

		if (!cap.isOpened())
			return -1;

		Mat prevgray, gray, flow[5], cflow, frame[6], frame_buf; 
		//namedWindow("flow", 1);
		for (int i = 0; i < 6; i++)
		{
			cap >> frame_buf;
			cvtColor(frame_buf, frame[i], CV_BGR2GRAY);	// 转换成灰度图像
		}
		bool first = 1;
		for (;;)
		{
			if (!first) // 当不是第一帧的时候，往待处理图像序列（6帧）中移入新的一帧，图像序列整体后移一帧，末尾一帧抛弃
			{
				frame[0] = frame[1].clone();
				frame[1] = frame[2].clone();
				frame[2] = frame[3].clone();
				frame[3] = frame[4].clone();
				frame[4] = frame[5].clone();
				cap >> frame_buf;
				cvtColor(frame_buf, frame[5], CV_BGR2GRAY);
			}

			double t = (double)cvGetTickCount();

			imshow("original", frame[2]);

			for (int i = 0; i < 5; i++)
			{
				calcOpticalFlowFarneback(frame[i], frame[i+1], flow[i], 0.5, 3, 15, 3, 5, 1.2, 0); // 计算两帧之间光流
			}
			Mat flow_result = computeSalMap(flow, frame[0]); // 计算6帧之间的显著图
			imshow("Sal_Map_OF", flow_result);

			t = (double)cvGetTickCount() - t;
			cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl; // 计算时间消耗

			if (waitKey(10) >= 0)
				break;
			first = 0;
		}
	} else if (mode == 1) // RC方法
	{

		string dir_path = "D:/buffer/vot2014/ball/"; // RC方法输入图像文件夹路径
		Directory dir;
		vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);

		for (int i = 0; i < fileNames.size(); i++)
		{
			string fileName = fileNames[i];
			string fileFullName = dir_path + fileName;
			cout << "file name:" << fileName << endl;
			cout << "file paht:" << fileFullName << endl; // 遍历整个文件夹里的图像

			double t = (double)cvGetTickCount();
			Mat temp = imread(fileFullName);
			temp.convertTo(temp, CV_32F, 1.0 / 255);
			Mat sal = CmSaliencyRC::GetRC(temp); // 获得显著图
			imshow("original", temp);
			imshow("Sal_Map_RC", sal );

			t = (double)cvGetTickCount() - t;
			cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;

			if (waitKey(10) >= 0)
				break;
		}
	}
	else if (mode == 2) // 联合方法（空缺）
	{
		
	}
	return 0;
}