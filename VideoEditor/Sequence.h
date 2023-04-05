#pragma once
#include "VideoClip.h"

struct Settings {
	int length; // the num of frames in the video
	int resolution[2]; // width, height
};


class Sequence
{
private:
	Settings settings;	
	std::vector<VideoClip*> video;
	std::vector<std::vector<AVFrame>> results;
public:
	void SaveVideo(std::string&);
};
