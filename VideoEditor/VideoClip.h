#pragma once
#include "VideoSource.h"
#include "IPlayable.h"
#include "IEffect.h"
// FFmpeg

extern "C" {
#include <libavcodec/avcodec.h> // codec API, used for encodeing and decoding video data
#include <libavformat/avformat.h> // format API, used for reading and writing multimedia continer formats (mp4, avi..)
#include <libswscale/swscale.h> // image scaling and color conversion API, converting pixel formats and resizing images 
}

class VideoClip : public IPlayable<AVFrame*>
{
private:
	VideoSource* videoSource;
	int edges[2];
	std::vector<IEffect> effects;
	std::vector<SyncObject<AVFrame*>>* clip;
	bool updated;
public:
	VideoClip(VideoSource*);
	virtual ~VideoClip();
	inline int GetStart() { return edges[0]; }
	inline int GetEnd() { return edges[1]; }
	
	bool SetStart(int);
	bool SetEnd(int);
	void ApplyEffects();
	SyncObject<AVFrame*>* GetChunk(int) override;
	int GetSize() override;
	std::string SourceName();
	std::string ToString() const {
		
	}
	//void AddEffect(IEffect); // changes the updated to false
};

