#include "VideoClip.h"

VideoClip::VideoClip(VideoSource* videoSource)
{
	this->videoSource = videoSource;
	this->edges[0] = 0;
	this->edges[0] = videoSource->source.size() - 1;
	updated = true;
}

void VideoClip::ApplyEffects(std::vector<AVFrame>* result)
{
	this->updated = true;
	*result = videoSource->source;
}
