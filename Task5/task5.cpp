#include "../StereoCalibration/stereoCalibration.h"
#include <fstream>

int main(){
	Mat img[2], cimg[2], R, T, E, F;
	Mat R_[2], P[2], Q;
	Mat cameraMatrix[2];
	Mat distCoeffs[2];
	Size boardSize = Size(10, 7); double squareSize = 3.88; int pair = 7;
	char* path = "..\\tennis\\stereo\\stereo";

	// Load intrinsic parameters
	char* intrinsicParametersL = "..\\StereoCalibration\\leftParameters.txt";
	char* intrinsicParametersR = "..\\StereoCalibration\\rightParameters.txt";
	char* stereoParameters = "..\\Task2\\Stereo.txt";
	char* stereoRect = "..\\Task4\\stereoRect.txt";
	loadIntrinsicParameters(intrinsicParametersL, cameraMatrix[0], distCoeffs[0]);
	loadIntrinsicParameters(intrinsicParametersR, cameraMatrix[1], distCoeffs[1]);
	loadStereoCalib(stereoParameters, R, T, E, F);
	loadStereoRect(stereoRect, R_[0], R_[1], P[0], P[1], Q);

	// Load images
	char imageLpath[512]; sprintf(imageLpath, "%sL%02d.bmp", path, pair);
	char imageRpath[512]; sprintf(imageRpath, "%sR%02d.bmp", path, pair);
	img[0] = imread(imageLpath, CV_LOAD_IMAGE_GRAYSCALE);
	img[1] = imread(imageRpath, CV_LOAD_IMAGE_GRAYSCALE);
	
	// Select 3d points
	//Mat  disparity; vector<Point3f> img3d;
	//StereoSGBM stereo(4*-16, 4 * 16, 21, 0, 0, 32, 32, 3, 0, 0, false);
	//stereo(img[0], img[1], disparity);

	
	Mat oldPoints[2], newPoints[2]; vector<Point2f> vpoints;
	int x[4] = {490, 290, 231, 354 };
	int y[4] = { 384, 267, 87, 210 };
	for (int k = 0; k < 4; k++){
		Point2i point = Point2i(x[k],y[k]);
		vpoints.push_back(point);
	}
	for (int i = 0; i < 2; i++){
		oldPoints[i] = Mat(vpoints);
		undistortPoints(oldPoints[i], newPoints[i], cameraMatrix[i], distCoeffs[i], R_[i], P[i]);
	}
	Mat old3dPoints[2], new3dPoints[2]; vector<Point3f> v3dpoints[2];
	for (int k = 0; k < 4; k++){
		int x[2], y[2], z;
		Point2f point1 = newPoints[0].at<Point2f>(k);
		Point2f point2 = newPoints[1].at<Point2f>(k);
		x[0] = point1.x; x[0] = point2.x;
		y[0] = point1.y; y[1] = point2.y;
		z = point1.x - point2.x;
		v3dpoints[0].push_back(Point3f(x[0], y[0], z));
		v3dpoints[1].push_back(Point3f(x[1], y[1], z));
	}
	ofstream fout("task5.txt");
	for (int i = 0; i < 2; i++){
		old3dPoints[i] = Mat(v3dpoints[i]);
		perspectiveTransform(old3dPoints[i], new3dPoints[i], Q);
		fout << "new3dPoints" << i << ": \n" << new3dPoints[i] << endl;
	}
	for (int i = 0; i < 2; i++){
		cvtColor(img[i], cimg[i], CV_GRAY2RGB);
		for (int k = 0; k < 4; k++){
			circle(cimg[i], Point(x[k], y[k]), 4, Scalar(0, 255, 0), 1, 8);
		}
	}

	// Save images
	imwrite("Left.bmp", cimg[0]);
	imwrite("Right.bmp", cimg[1]);

	return 0;
}