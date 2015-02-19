#include "..\\StereoCalibration\\stereoCalibration.h"

int main() { // Task 3: Epipolar Lines
	Mat imageL1, imageL2, imageR1, imageR2, F;
	Mat cameraMatrixL, cameraMatrixR, distCoeffsL, distCoeffsR;
	Size boardSize = Size(10, 7); int pair = 27;
	char* path = "..\\tennis\\stereo\\stereo";

	// Load intrinsic and distortion parameters
	char* intrinsicParametersL = "..\\StereoCalibration\\leftParameters.txt";
	char* intrinsicParametersR = "..\\StereoCalibration\\rightParameters.txt";
	loadIntrinsicParameters(intrinsicParametersL, cameraMatrixL, distCoeffsL);
	loadIntrinsicParameters(intrinsicParametersR, cameraMatrixR, distCoeffsR);

	// Load images
	char imageLpath[512]; sprintf(imageLpath, "%sL%02d.bmp", path, pair);
	char imageRpath[512]; sprintf(imageRpath, "%sR%02d.bmp", path, pair);
	imageL1 = imread(imageLpath, CV_LOAD_IMAGE_GRAYSCALE);
	imageR1 = imread(imageRpath, CV_LOAD_IMAGE_GRAYSCALE);

	// Undistort images
	undistort(imageL1, imageL2, cameraMatrixL, distCoeffsL);
	undistort(imageR1, imageR2, cameraMatrixR, distCoeffsR);

	// Find corners
	bool patternFound = false;
	vector<Point2f> cornersL = cornersFromImage(path, "%sL%02d.bmp", pair, boardSize, patternFound);
	vector<Point2f> cornersR = cornersFromImage(path, "%sR%02d.bmp", pair, boardSize, patternFound);

	// Select points
	cvtColor(imageL2, imageL2, CV_GRAY2RGB);
	cvtColor(imageR2, imageR2, CV_GRAY2RGB);
	vector<Point2f> pointsL, pointsR; int points[6] = { 17, 42, 67, 9, 20, 54 };
	for (int i = 0; i < 3; i++){
		pointsL.push_back(cornersL[points[i]]);
		pointsR.push_back(cornersR[points[3 + i]]);

	}

	vector<Point3f> linesL, linesR;
	loadFundamentalMatrix("..\\Task2\\F.txt", F);
	// Compute epipolor lines for selected points
	computeCorrespondEpilines(pointsL, 1, F, linesR);
	computeCorrespondEpilines(pointsR, 2, F, linesL);
	

	// Draw epipolar lines
	for (int i = 0; i < 3; i++){
		circle(imageL2, pointsL[i], 3, Scalar(0, 255, 0), -1, 8);
		circle(imageR2, pointsR[i], 3, Scalar(0, 255, 0), -1, 8);
		drawEpiLine(imageL2, linesL[i], Scalar(255,0,0));
		drawEpiLine(imageR2, linesR[i], Scalar(0, 0, 255));
	}

	// Display images (for debugging)
	namedWindow("Left"); imshow("Left", imageL2);
	namedWindow("Right"); imshow("Right", imageR2);
	waitKey(0);

	// Save images
	imwrite("Left.bmp", imageL2);
	imwrite("Right.bmp", imageR2);

	return 0;
}