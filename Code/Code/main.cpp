#include <iostream>
#include "opencv2/opencv.hpp"
#include <stdio.h>

#include "CmLib.h" // https://github.com/MingMingCheng/CmCode/wiki

#include <stdio.h>
#include <tchar.h>

#include <sstream>

using namespace cv;
using namespace std;

// 光流显著图计算函数
Mat computeSalMapOF(Mat *flow, Mat gray, float *max, float *mean)
{
	Mat result(gray.size(), CV_32FC1);
	float f1;
	float f2;
	float f3[4];
	float temp = 0;
	 
	for (int i = 0; i < flow[0].rows; i++)
	{
		for (int j = 0; j < flow[0].cols; j++)
		{
			for (int fi = 0; fi < 4; fi++)
			{
				f1 = flow[fi].ptr<float>(i, j)[0];
				f2 = flow[fi].ptr<float>(i, j)[1];
				f3[fi] = sqrt(f1*f1 * 100 + f2*f2 * 100) / 255; // 100 和 255 都可以修改，影响显著图的整体显著度
			}
			temp = 0.8 * f3[0] + 0.6 * f3[1] + 0.4 * f3[2] + 0.2 * f3[3]; // 关键平方加权和公式
			if (*max < temp) *max = temp;
			result.at<float>(i, j) = temp;
			*mean += temp;
		}
	}
	*mean /= (flow[0].rows * flow[0].cols);
	result = result.mul(1 / *max);

	return result;
}

void methodOF(string dir_path)
{
	Directory dir;
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);

	Mat prevgray, gray, flow[4], cflow, frame[5], frame_buf;
	//namedWindow("flow", 1);
	for (int i = 0; i < 5; i++)
	{
		string fileName = fileNames[i];
		string fileFullName = dir_path + fileName;
		cout << "file name:" << fileName << endl;
		cout << "file paht:" << fileFullName << endl; // 遍历整个文件夹里的图像
		frame_buf = imread(fileFullName);;
		cvtColor(frame_buf, frame[i], CV_BGR2GRAY);	// 转换成灰度图像
	}
	bool first = 1;
	for (int i = 5; i < fileNames.size(); i++)
	{
		if (!first) // 当不是第一帧的时候，往待处理图像序列（5帧）中移入新的一帧，图像序列整体后移一帧，末尾一帧抛弃
		{
			frame[0] = frame[1].clone();
			frame[1] = frame[2].clone();
			frame[2] = frame[3].clone();
			frame[3] = frame[4].clone();
			string fileName = fileNames[i];
			string fileFullName = dir_path + fileName;
			cout << "file name:" << fileName << endl;
			cout << "file paht:" << fileFullName << endl; // 遍历整个文件夹里的图像
			frame_buf = imread(fileFullName);;
			cvtColor(frame_buf, frame[4], CV_BGR2GRAY);
		}

		double t = (double)cvGetTickCount();

		imshow("original", frame[0]);

		for (int i = 0; i < 4; i++)
		{
			calcOpticalFlowFarneback(frame[i], frame[i + 1], flow[i], 0.5, 3, 15, 3, 5, 1.2, 0); // 计算两帧之间光流
		}
		float max = 0;
		float mean = 0;
		Mat flow_result = computeSalMapOF(flow, frame[0], &max, &mean); // 计算5帧之间的显著图
		imshow("Sal_Map_OF", flow_result);

		t = (double)cvGetTickCount() - t;
		cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl; // 计算时间消耗

		if (waitKey(10) >= 0)
			break;
		first = 0;
	}
}

void methodRC(string dir_path)
{
	Directory dir;
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.png", false);

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
		imshow("Sal_Map_RC", sal);

		t = (double)cvGetTickCount() - t;
		cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;

		if (waitKey(10) >= 0)
			break;
	}
}

void methodFU(string dir_path)
{
	Directory dir;
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);

	Mat prevgray, gray, flow[4], cflow, frame[5], frame_buf;
	//namedWindow("flow", 1);
	for (int i = 0; i < 5; i++)
	{
		string fileName = fileNames[i];
		string fileFullName = dir_path + fileName;
		cout << "file name:" << fileName << endl;
		cout << "file paht:" << fileFullName << endl; // 遍历整个文件夹里的图像
		frame_buf = imread(fileFullName);;
		cvtColor(frame_buf, frame[i], CV_BGR2GRAY);	// 转换成灰度图像
	}
	bool first = 1;
	for (int i = 5; i < fileNames.size(); i++)
	{
		if (!first) // 当不是第一帧的时候，往待处理图像序列（5帧）中移入新的一帧，图像序列整体后移一帧，末尾一帧抛弃
		{
			frame[0] = frame[1].clone();
			frame[1] = frame[2].clone();
			frame[2] = frame[3].clone();
			frame[3] = frame[4].clone();
			string fileName = fileNames[i];
			string fileFullName = dir_path + fileName;
			cout << "file name:" << fileName << endl;
			cout << "file paht:" << fileFullName << endl; // 遍历整个文件夹里的图像
			frame_buf = imread(fileFullName);;
			cvtColor(frame_buf, frame[4], CV_BGR2GRAY);
		}

		double t = (double)cvGetTickCount();

		for (int ii = 0; ii < 4; ii++)
		{
			calcOpticalFlowFarneback(frame[ii], frame[ii + 1], flow[ii], 0.5, 3, 15, 3, 5, 1.2, 0); // 计算两帧之间光流
		}
		float max = 0; // 存储光流显著图最大值
		float mean = 0; // 存储光流显著图均值
		Mat salOF = computeSalMapOF(flow, frame[0], &max, &mean); // 计算5帧之间的光流显著图

		string fileFullName = dir_path + fileNames[i - 5];
		Mat temp = imread(fileFullName);
		temp.convertTo(temp, CV_32F, 1.0 / 255);
		Mat salRC = CmSaliencyRC::GetRC(temp); // 获得RC显著图

		Mat sal_all = (mean / max) * salRC + (1 - (mean / max)) * salOF; // 动态加权融合

		string str;
		stringstream stream;
		stream << i - 5;
		stream >> str;

		string sal_dir = "F:/git/paper1st/SalBenchmark-master/Data/DataSet3/";
		imwrite(sal_dir + "Saliency/0000000" + str + "_OURS.png", sal_all * 255);

		imshow("original", temp);
		imshow("Sal_Map_OF", salOF);
		imshow("Sal_Map_RC", salRC);
		imshow("Sal_Map_Total", sal_all);
		
		t = (double)cvGetTickCount() - t;
		cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl; // 计算时间消耗

		if (waitKey(10) >= 0)
			break;
		first = 0;
	}
}

int main(int argc, char *argv[])
{
	cout << "Please choose mode: (0)OF (1)RC (2)Fusion  " << endl;	// 模式选择 (0)光流 (1)RC (2)前两种方法结合
	int mode = 0;
	cin >> mode;
	string db_dir = "D:/buffer/SegTrackv2/JPEGImages/hummingbird/";
	switch (mode)
	{
	case 0:
		methodOF(db_dir);
		break;
	case 1:
		methodRC(db_dir);
		break;
	case 2:
		methodFU(db_dir);
		break;
	default:
		break;
	}
	return 0;
}