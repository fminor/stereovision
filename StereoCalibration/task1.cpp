
#include "..\\StereoCalibration\\stereoCalibration.h"

int main() { // Task 1: Camera Calibration
	Mat cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR;

	// Make changes here!
	Size boardSize = Size(10, 7); double squareSize = 2.0;
	char* pathL = "..\\Practice\\LeftCameraImages\\CameraL"; int imagesL = 32;
	char* pathR = "..\\Practice\\RightCameraImages\\CameraR"; int imagesR = 32;

	// Create chessboard in 3d object coordinates
	vector<Point3f> chessboard3d = makeChessboard(boardSize, squareSize);

	// LEFT CAMERA
	cout << "Left Camera" << endl;
	calibrateFromImages(pathL, imagesL, boardSize, chessboard3d, cameraMatrixL, distCoeffsL);
	saveIntrinsicParameters("leftParameters.txt", cameraMatrixL, distCoeffsL);

	// RIGHT CAMERA
	cout << "Right Camera" << endl;
	calibrateFromImages(pathR, imagesR, boardSize, chessboard3d, cameraMatrixR, distCoeffsR);
	saveIntrinsicParameters("rightParameters.txt", cameraMatrixR, distCoeffsR);

	cout << "Done. Hit any key to end program." << endl;
	cin.get();
	return 0;
}