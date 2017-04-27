#ifndef ADHS
#define ADHS

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"


CVAPI(void)  adHS(const CvArr* prev, const CvArr* curr,
	int use_previous, CvArr* velx, CvArr* vely,
	double lambda, CvTermCriteria criteria);


#endif