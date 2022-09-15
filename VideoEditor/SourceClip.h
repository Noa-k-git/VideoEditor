#include "SourceID.h"
#include "ISourceFile.h"

#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
#include<vector>
#include <sstream>;
#include<chrono>;
#include<thread>;

using std::string;
using std::vector;
using cv::Mat;

struct Info {
	double fps; // clip's frames per seconds
	int framesNum; // number of frames in the source clip
	int resulusion [2]; // width, hight.
};

#pragma once
class SourceClip : public SourceID, public ISourceFile
{
private:
	Info info; // struct that contains all the clips configurations
	vector<cv::Mat> clip; // the source clip

public:
	SourceClip(string);
	void ShowSource() override;

	inline const Info getInfo() const; // returns the info of the video
	inline vector<Mat>& GetSource();
private:
	void ReadClip(cv::VideoCapture&); // helper function to constuctor -> reads the video

};
