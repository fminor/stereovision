#include "../StereoCalibration/stereoCalibration.h"

int main(){ // Task 2: Stereo Calibration
	Mat cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR;
	Mat R, T, E, F;
	char* intrinsicParametersL = "..\\StereoCalibration\\leftParameters.txt";
	char* intrinsicParametersR = "..\\StereoCalibration\\rightParameters.txt";
	char* stereoImages = "..\\Practice\\StereoImages\\Stereo"; int images = 32;
	Size boardSize = Size(10, 7); double squareSize = 2.0;

	// Load intrinsic and distortion paramters calcluated in Task 1
	loadIntrinsicParameters(intrinsicParametersL, cameraMatrixL, distCoeffsL);
	loadIntrinsicParameters(intrinsicParametersR, cameraMatrixR, distCoeffsR);

	// Create chessboard in 3d object coordinates
	vector<Point3f> chessboard3d = makeChessboard(boardSize, squareSize);

	stereoCalibrateFromImages(stereoImages, images, boardSize, chessboard3d, 
		cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR, R, T, E, F);

	cout << "R: " << R << endl;
	cout << "T: " << T << endl;
	cout << "E: " << E << endl;
	cout << "F: " << F << endl;

	// Save F
	saveFundamentalMatrix("F.txt", F);

	cin.get();
	return 0;
}