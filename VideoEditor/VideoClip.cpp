#pragma once
#include "VideoClip.h"
#include<algorithm>
VideoClip::VideoClip(VideoSource* videoSource)
{
	this->videoSource = videoSource;
	this->edges[0] = 0;
	this->edges[1] = videoSource->GetSource().size() - 1;
	updated = false;
	this->clip = new std::vector<SyncObject<AVFrame*>>();
}

VideoClip::~VideoClip()
{
	//if (clip != nullptr) {
	//	for (auto& element : *clip) {
	//		if (element != nullptr) {
	//			av_frame_free(&element);
	//			//av_frame_unref(element);
	//			//av_freep(element);
	//		}
	//	}
	//}
	clip->clear();
}

bool VideoClip::SetStart(int start)
{
	if (start < edges[1]) {
		edges[0] = std::max(start, 0);
		return true;
	}
	return false;
}

bool VideoClip::SetEnd(int end)
{
	if (end > edges[0]) {
		edges[1] = std::min(end, (int)(videoSource->GetSource().size() - 1));
		return true;
	}
	return false;
}

void VideoClip::ApplyEffects()
{
	if (updated)
		return;
	clip->clear();
	auto lock = videoSource->LockSource();
	std::vector<SyncObject<AVFrame*>> sourceFrames = videoSource->GetSource();
	for (auto i = sourceFrames.begin() + edges[0]; i < sourceFrames.begin() + edges[1]; i++) {
		clip->push_back(SyncObject<AVFrame*>(av_frame_clone(i->GetObject())));
	}
	//std::vector<AVFrame*> framesCopy(sourceFrames.begin()+edges[0], sourceFrames.begin()+edges[1]);
	// Apply effects to framesCopy
	//for (auto& effect : effects) {
	//	effect.Apply(clip);
	//}
	updated = true;
	//// Copy the modified frames to the result
	//std::vector<AVFrame*>& clipRef = *clip;
	//clipRef.swap(framesCopy);
	//this->updated = true;
}

vector<SyncObject<AVFrame*>>& VideoClip::GetClip()
{
	this->ApplyEffects();
	return *clip;
}
