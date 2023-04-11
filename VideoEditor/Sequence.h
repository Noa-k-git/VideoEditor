#pragma once
#include "VideoClip.h"
#include <wx/msgdlg.h>
struct Settings {
	int length; // the num of frames in the video
	int resolution[2]; // width, height
};


class Sequence
{
private:
	Settings settings;	
	std::vector<VideoClip*> video;
	//std::vector<std::vector<AVFrame>> results;
public:
	static Map<Sequence*> sequences;
	Sequence(std::string);
	Sequence();
	virtual ~Sequence();
	void SaveVideo(std::string&);
	void AddClip(VideoClip*, int);
	void AddClip(VideoClip*);
};
extern Map<Sequence*> sequences;
