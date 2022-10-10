#pragma once

#include "SourceID.h"
#include "ISource.h"

#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
#include<vector>
#include <sstream>;
#include<thread>;

using std::string;
using std::vector;
using cv::Mat;



#pragma once
class SourceClip : public SourceID, public ISource
{
private:
	vector<cv::Mat> clip; // the source clip

public:
	SourceClip(string);
	void ShowSource() override;

	inline Info GetInfo() const; // returns the info of the video
	inline vector<Mat>& GetSource();
private:
	void ReadClip(cv::VideoCapture&); // helper function to constuctor -> reads the video

};

inline Info SourceClip::GetInfo() const
{
	return this->info;
};
inline vector<Mat>& SourceClip::GetSource()
{
	return this->clip;
};