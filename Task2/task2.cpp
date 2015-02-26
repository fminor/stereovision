#include "../StereoCalibration/stereoCalibration.h"
#include <fstream>
int main(){ // Task 2: Stereo Calibration
	Mat cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR;
	Mat R, T, E, F;
	char* intrinsicParametersL = "..\\StereoCalibration\\leftParameters.txt";
	char* intrinsicParametersR = "..\\StereoCalibration\\rightParameters.txt";
	//char* stereoImages = "..\\Practice\\StereoImages\\Stereo"; int images = 32;
	char* stereoImages = "..\\tennis\\stereo\\stereo"; int images = 32;
	Size boardSize = Size(10, 7); double squareSize = 3.88;

	// Load intrinsic and distortion paramters calcluated in Task 1
	loadIntrinsicParameters(intrinsicParametersL, cameraMatrixL, distCoeffsL);
	loadIntrinsicParameters(intrinsicParametersR, cameraMatrixR, distCoeffsR);

	// Create chessboard in 3d object coordinates
	vector<Point3f> chessboard3d = makeChessboard(boardSize, squareSize);

	stereoCalibrateFromImages(stereoImages, images, boardSize, chessboard3d, 
		cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR, R, T, E, F);

	ofstream fout("task5.txt");
	fout << "R: " << R << endl;
	fout << "T: " << T << endl;
	fout << "E: " << E << endl;
	fout << "F: " << F << endl;

	// Save F
	saveFundamentalMatrix("F.txt", F);
	saveStereoCalib("Stereo.txt", R, T, E, F);

	cin.get();
	return 0;
}