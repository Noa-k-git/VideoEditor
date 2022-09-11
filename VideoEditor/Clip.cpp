#include "Clip.h"

#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
#include<vector>
#include <sstream>;
#include<chrono>;
#include<thread>;

using namespace std::chrono;
using namespace cv;

Clip::Clip(SourceClip& clip)
{
	this->clipPtr = &clip;
	this->limits.start = 0;
	this->limits.end = (*this->clipPtr).getInfo().framesNum;
}

vector<Mat>& Clip::getClip()
{
	auto source = (*this->clipPtr).GetSource();/*
	auto sourceStart = source.begin();
	auto first = sourceStart + this->limits.start;
	auto last = sourceStart + this->limits.end;*/
	vector<Mat> sub(&source[this->limits.start], &source[this->limits.end]);
	return sub;
}

void Clip::setStartFrame(int newStart)
{	
	if (newStart >= this->limits.end)
		this->limits.start = this->limits.end;
	else if (newStart <= this->limits.start)
		this->limits.start = 0;
	else
		this->limits.start = newStart;
}

void Clip::setEndFrame(int newEnd)
{
	int length = (*this->clipPtr).getInfo().framesNum;
	if (newEnd <= this->limits.start)
		this->limits.end = this->limits.start;
	else if (newEnd >= this->limits.end)
		this->limits.end = length;
	else
		this->limits.end = newEnd;
}
