#pragma once
#include "VideoSource.h"
#include "IEffect.h"
// FFmpeg

extern "C" {
#include <libavcodec/avcodec.h> // codec API, used for encodeing and decoding video data
#include <libavformat/avformat.h> // format API, used for reading and writing multimedia continer formats (mp4, avi..)
#include <libswscale/swscale.h> // image scaling and color conversion API, converting pixel formats and resizing images 
}

class VideoClip
{
private:
	VideoSource* videoSource;
	int edges[2];
	std::vector<IEffect> effects;
	std::vector<AVFrame*>* clip;
	bool updated;
public:
	VideoClip(VideoSource*);
	virtual ~VideoClip();
	bool SetStart(int);
	bool SetEnd(int);
	void ApplyEffects();
	vector<AVFrame*>& GetClip();
	std::string ToString() const {
		
	}
	//void AddEffect(IEffect); // changes the updated to false
};

