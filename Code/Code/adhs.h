#ifndef ADHS
#define ADHS

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"


CVAPI(void)  adHS(const CvArr* frame1, const CvArr* frame2, const CvArr* frame3, const CvArr* frame4, const CvArr* frame5,
	int use_previous, CvArr* velx, CvArr* vely,
	float w,
	double lambda, CvTermCriteria criteria);


#endif