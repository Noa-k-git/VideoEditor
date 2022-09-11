#include "Clip.h";

#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
#include<vector>
#include <sstream>;
//#include<chrono>;
//#include<thread>;

//using namespace std::chrono;
//using namespace cv;

using cv::Scalar;

#pragma once
struct Settings {
	int length; // the num of frames in the video
	int resulusion[2]; // width, height
	Scalar colorRGB; // r, g, b

};


class Sequence
{
private:
	Settings settings;	
	vector<Mat> bg;

};

