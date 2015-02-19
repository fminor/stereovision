#include "../StereoCalibration/stereoCalibration.h"

int main(){
	Mat img[2], rimg[2], cimg[2], R, T, E, F;
	Mat cameraMatrix[2];
	Mat distCoeffs[2];
	Size boardSize = Size(10, 7); double squareSize = 3.88; int pair = 19;
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

	// Create chessboard in 3d object coordinates
	vector<Point3f> chessboard3d = makeChessboard(boardSize, squareSize);


	// Display images (for debugging)
	namedWindow("Left"); imshow("Left", img[0]);
	namedWindow("Right"); imshow("Right", img[1]);
	waitKey(0);

	// Save images
	imwrite("Left.bmp", img[0]);
	imwrite("Right.bmp", img[1]);

	return 0;
}