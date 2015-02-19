#include "stereoCalibration.h"

// Makes chessboard of *boardSize* where each square is *squareSize* big
// Returns chessboard3d - The chessboard in the object coordinate system
vector<Point3f> makeChessboard(Size boardSize, double squareSize){
	vector<Point3f> chessboard3d;
	double size = 1.0; // ! change size here
	cout << "Making chessboard3d" << endl;
	for (int i = 0; i < boardSize.height; i++){
		for (int j = 0; j < boardSize.width; j++){
			chessboard3d.push_back(Point3f(i*squareSize, j*squareSize, 0));
		}
	}
	//cout << "Chessboard3d:\n" << chessboard3d << endl;
	return chessboard3d;
}

// Saves Intrinsic Parameters
// INPUTS
// output - path to write file
// cameraMatrix - (3x1) Intrinsic parameters
// distCoeffs - (5x1) Distortion parameters
void saveIntrinsicParameters(char* output, Mat &cameraMatrix, Mat &distCoeffs){
	cout << "Saving cameraMatrix and distCoeffs" << endl;
	cout << "cameraMatrix:\n" << cameraMatrix << endl;
	cout << "distCoeffs:\n" << distCoeffs << endl;
	FileStorage fs(output, FileStorage::WRITE);
	fs << "cameraMatrix" << cameraMatrix;
	fs << "distCoeffs" << distCoeffs;
}

// Load Intrinsic Parameters
// INPUT
// input - path to file where paramters have been saved
// OUTPUTS
// cameraMatrix - (3x3) Intrinsic parameters
// distCoeffs - (5x1) Distortion parameters
void loadIntrinsicParameters(char* input, Mat &cameraMatrix, Mat &distCoeffs){
	cout << "Load cameraMatrix and distCoeffs" << endl;
	FileStorage fs(input, FileStorage::READ);
	fs["cameraMatrix"] >> cameraMatrix;
	fs["distCoeffs"] >> distCoeffs;
	cout << "cameraMatrix:\n" << cameraMatrix << endl;
	cout << "distCoeffs:\n " << distCoeffs << endl;
}

// Finds corners from an image
// INPUTS
// path - To directory with images
// pattern - String pattern used to create filename
// image - int used to complete filename
// boardSize - size of board
// Returns corners - the objects found in image
// patternFound is a boolean the indicates weather or not the pattern was found and is changed =)
vector<Point2f> cornersFromImage(char* path, char* pattern, int i, Size boardSize, bool& patternFound){
	Mat image;
	vector<Point2f> corners;
	// Read in image (in grayscale)
	char FileName[512]; sprintf(FileName, pattern, path, i);
	cout << FileName << endl;
	image = imread(FileName, CV_LOAD_IMAGE_GRAYSCALE);

	// Find Corners
	patternFound = findChessboardCorners(image, boardSize, corners, CV_CALIB_CB_ADAPTIVE_THRESH + CV_CALIB_CB_NORMALIZE_IMAGE);
	cornerSubPix(image, corners, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS, 30, 0.01));
	return corners;
}

// Calibrates a single camera from images
// INPUTS
// path - Path to directory where images are
// images - number of images in directory 
// boardSize - size of chessboard
// chessboard3d - chessboard in 3d object coordinate system
// OUTPUTS
// cameraMatrix - (3x3) Intrinsic parameters
// distCoeffs - (5x1) Distortion parameters
void calibrateFromImages(char* path, int images, Size boardSize, vector<Point3f> chessboard3d, Mat &cameraMatrix, Mat &distCoeffs){
	vector<Mat> rvecs; vector<Mat> tvecs; Mat image; vector<vector<Point3f>> objectPoints; vector<vector<Point2f>> imagePoints;
	cout << "Gathering imagePoints" << endl;
	// For each picture
	for (int i = 0; i < images; i++){
		bool patternFound = false;
		vector<Point2f> corners = cornersFromImage(path, "%s%02d.bmp", i, boardSize, patternFound);
		if (patternFound){
			imagePoints.push_back(corners); // Save corners for this image
			objectPoints.push_back(chessboard3d); // Object is same in all pictures
		}
	}

	cout << "Calibrating Camera: This may take a while" << endl;
	cameraMatrix = Mat::eye(3, 3, CV_64F);
	distCoeffs = Mat(5, 1, CV_64F);
	TermCriteria criteria = TermCriteria(CV_CALIB_USE_INTRINSIC_GUESS, 70, 1e-5);
	calibrateCamera(objectPoints, imagePoints, boardSize, cameraMatrix, distCoeffs, rvecs, tvecs, 0, criteria);
}

// Stereo Calibration from images
// INPUTS
// path - Path to directory where images are
// images - number of images in directory
// boardSize - size of chessboard
// chessboard3d - chessboard in 3d object coordinate system
// cameraMatrix1 - (3x3) Intrinsic parameters for Camera 1
// distCoeffs1 - (5x1) Distortion parameters for Camera 1
// cameraMatrix2 - (3x3) Intrinsic parameters for Camera 2
// distCoeffs2 - (5x1) Distortion parameters for Camera 2
// OUTPUTS
// R - Output rotation matrix between the 1st and 2nd coordinate systems
// T - Output translation vector
// E - Output essential matrix
// F - Output fundamental matrix
// Returns the exit code from stereoCalibrate
double stereoCalibrateFromImages(char* path, int images, Size boardSize, vector<Point3f> chessboard3d, 
	Mat& cameraMatrix1, Mat& distCoeffs1, Mat& cameraMatrix2, Mat& distCoeffs2,
	Mat& R, Mat& T, Mat& E, Mat& F){
	vector<vector<Point3f>> objectPoints; 
	vector<vector<Point2f>> imagePoints1;
	vector<vector<Point2f>> imagePoints2;

	for (int i = 0; i < images; i++){
		bool patternFound[2] = { false, false };
		vector<Point2f> corners[2];
		corners[0] = cornersFromImage(path, "%sL%02d.bmp", i, boardSize, patternFound[0]);
		corners[1] = cornersFromImage(path, "%sR%02d.bmp", i, boardSize, patternFound[1]);
		if (patternFound[0] && patternFound[1]) {
			imagePoints1.push_back(corners[0]);
			imagePoints2.push_back(corners[1]);
			objectPoints.push_back(chessboard3d);
		}
		int x = 0;
	}

	cout << "Performing stereo calibration" << endl;
	TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.1);
	int flags = CV_CALIB_FIX_INTRINSIC;
	return stereoCalibrate(objectPoints, imagePoints1, imagePoints2,
		cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, 
		boardSize, R, T, E, F, criteria, flags);
}

// Save Fundamental Matrix
// output - input path to save file
// F - input fundamental matrix
void saveFundamentalMatrix(char* output, Mat &F){
	FileStorage fs(output, FileStorage::WRITE);
	fs << "F" << F;
}

// Load Fundamental Matrix
// input - input path to load file
// F - output fundamental matrix
void loadFundamentalMatrix(char* output, Mat &F){
	FileStorage fs(output, FileStorage::READ);
	fs["F"] >> F;
}

// Save/load stereo calibration
// input file
// input/output R, T, E, F
void saveStereoCalib(char* file, Mat& R, Mat& T, Mat& E, Mat& F){
	FileStorage fs(file, FileStorage::WRITE);
	fs << "R" << R;
	fs << "T" << T;
	fs << "E" << E;
	fs << "F" << F;
}
void loadStereoCalib(char* file, Mat& R, Mat& T, Mat& E, Mat& F){
	FileStorage fs(file, FileStorage::READ);
	fs["R"] >> R;
	fs["T"] >> T;
	fs["E"] >> E;
	fs["F"] >> F;
}

// Draw Epi Line
// input/output img - image we draw on 
// input abc - line defined like so: ax + by + c
// input color - color of line
void drawEpiLine(Mat& img, Point3f abc, Scalar color){
	Point2f point1 = Point(0, -abc.z / abc.y);
	Point2f point2 = Point(img.cols, -(abc.z + abc.x*img.cols) / abc.y);
	line(img,
		point1,
		point2,
		color);
}
