#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

// Task 1
vector<Point3f> makeChessboard(Size boardSize, double squareSize);
void calibrateFromImages(char* path, int images, Size boardSize, vector<Point3f> chessboard3d, Mat &cameraMatrix, Mat &distCoeffs);
void saveIntrinsicParameters(char* output, Mat &cameraMatrix, Mat &distCoeffs);

// Task 2
void loadIntrinsicParameters(char* input, Mat &cameraMatrix, Mat &distCoeffs);
double stereoCalibrateFromImages(char* path, int images, Size boardSize, vector<Point3f> chessboard3d,
	Mat& cameraMatrix1, Mat& distCoeffs1, Mat& cameraMatrix2, Mat& distCoeffs2,
	Mat& R, Mat& T, Mat& E, Mat& F);
void saveFundamentalMatrix(char* output, Mat& F);

// Task 3
void loadFundamentalMatrix(char* input, Mat& F);
vector<Point2f> cornersFromImage(char* path, char* pattern, int i, Size boardSize, bool& patternFound);
void drawEpiLine(Mat& img, Point3f abc, Scalar Color);

// Task 4
void saveStereoCalib(char* file, Mat& R, Mat& T, Mat& E, Mat& F);
void loadStereoCalib(char* file, Mat& R, Mat& T, Mat& E, Mat& F);
void saveStereoRect(char* file, Mat& R1, Mat& R2, Mat& P1, Mat& P2, Mat& Q);
void loadStereoRect(char* file, Mat& R1, Mat& R2, Mat& P1, Mat& P2, Mat& Q);