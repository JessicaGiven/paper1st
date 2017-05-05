#include <iostream>
#include "opencv2/opencv.hpp"
#include <stdio.h>

#include "CmLib.h" // https://github.com/MingMingCheng/CmCode/wiki

#include <stdio.h>
#include <tchar.h>

using namespace cv;
using namespace std;

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
				f3[fi] = sqrt(f1*f1 * 100 + f2*f2 * 100) / 255;
			}
			temp = 0.2 * f3[0] + 0.8 * f3[1] + 0.8 * f3[2] + 0.2 * f3[3] + 0.1 * f3[4];
			if (f4 < temp) f4 = temp;
			result.at<float>(i, j) = temp;
		}
	}
	result = result.mul(1 / f4);

	return result;
}

int main(int, char**)
{
	VideoCapture cap;
	//cap.open(0);
	cap.open("test1.mp4");

	if (!cap.isOpened())
		return -1;

	Mat prevgray, gray, flow[5], cflow, frame[6], frame_buf;
	//namedWindow("flow", 1);
	for (int i = 0; i < 6; i++)
	{
		cap >> frame_buf;
		cvtColor(frame_buf, frame[i], CV_BGR2GRAY);
	}
	bool first = 1;
	for (;;)
	{
		if (!first)
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

		//imshow("original", frame[2]);

		for (int i = 0; i < 5; i++)
		{
			//calcOpticalFlowFarneback(frame[i], frame[i+1], flow[i], 0.5, 3, 15, 3, 5, 1.2, 0);
		}
		//Mat flow_result = computeSalMap(flow, frame[0]);
		//imshow("Sal_Map_OF", flow_result);
		
		if (waitKey(10) >= 0)
			break;

		string dir_path = "D:/buffer/vot2014/ball/";
		Directory dir;
		vector<string> fileNames = dir.GetListFiles(dir_path, "*.jpg", false);

		for (int i = 0; i < fileNames.size(); i++)
		{
			string fileName = fileNames[i];
			string fileFullName = dir_path + fileName;
			cout << "file name:" << fileName << endl;
			cout << "file paht:" << fileFullName << endl;


			Mat temp = imread(fileFullName);
			temp.convertTo(temp, CV_32F, 1.0 / 255);
			Mat sal = CmSaliencyRC::GetRC(temp);
			imshow("original", temp);
			imshow("Sal_Map_RC", sal );

			if (waitKey(10) >= 0)
				break;
		}

		t = (double)cvGetTickCount() - t;
		cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;

		first = 0;
	}
	return 0;
}