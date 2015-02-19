#include "../StereoCalibration/stereoCalibration.h"

int main(){
	Mat img[2], rimg[2], cimg[2], R, T, E, F;
	Mat R1, R2, P1, P2, Q;
	Mat cameraMatrix[2];
	Mat distCoeffs[2];
	Size boardSize = Size(10, 7); int pair = 23;
	char* path = "..\\tennis\\stereo\\stereo";

	// Load intrinsic parameters
	char* intrinsicParametersL = "..\\StereoCalibration\\leftParameters.txt";
	char* intrinsicParametersR = "..\\StereoCalibration\\rightParameters.txt";
	char* stereoParameters = "..\\Task2\\Stereo.txt";
	loadIntrinsicParameters(intrinsicParametersL, cameraMatrix[0], distCoeffs[0]);
	loadIntrinsicParameters(intrinsicParametersR, cameraMatrix[1], distCoeffs[1]);
	loadStereoCalib(stereoParameters, R, T, E, F);


	// Load images
	char imageLpath[512]; sprintf(imageLpath, "%sL%02d.bmp", path, pair);
	char imageRpath[512]; sprintf(imageRpath, "%sR%02d.bmp", path, pair);
	img[0] = imread(imageLpath, CV_LOAD_IMAGE_GRAYSCALE);
	img[1] = imread(imageRpath, CV_LOAD_IMAGE_GRAYSCALE);

	Mat rmap[2][2]; Rect validRoi[2];
	stereoRectify(cameraMatrix[0], distCoeffs[0], cameraMatrix[1], distCoeffs[1],
		boardSize, R, T, R1, R2, P1, P2, Q);// ,		CALIB_ZERO_DISPARITY, 1, img[0].size(),&validRoi[0],&validRoi[1]);
	initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], 
		R1, P1, img[0].size(), CV_16SC2, rmap[0][0], rmap[0][1]);
	initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1],
		R2, P2, img[1].size(), CV_16SC2, rmap[1][0], rmap[1][1]);
	for (int i = 0; i < 2; i++){
		remap(img[i], rimg[i], rmap[i][0], rmap[i][1], CV_INTER_LINEAR);
		cvtColor(rimg[i], cimg[i], CV_GRAY2BGR);
		for (int k = 1; k < 10 + 1; k++){
			Point2i point[2];
			int y = k * img[i].rows / 10;
			point[0] = Point(0, y);
			point[1] = Point(640, y);
			line(cimg[i], point[0], point[1], Scalar(0, 0, 255));
		}
		
	}

	// Display images (for debugging)
	namedWindow("Left"); imshow("Left", cimg[0]);
	namedWindow("Right"); imshow("Right", cimg[1]);
	waitKey(0);

	// Save images
	imwrite("Left.bmp", cimg[0]);
	imwrite("Right.bmp", cimg[1]);

	return 0;
}