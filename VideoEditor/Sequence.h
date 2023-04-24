#pragma once
#include "VideoClip.h"
#include "IPlayable.h"
#include <wx/msgdlg.h>
struct Settings {
	int length; // the num of frames in the video
	int resolution[2]; // width, height
};


class Sequence : public IPlayable<AVFrame*>
{
private:
	Settings settings;	
	std::vector<VideoClip*> video;
	bool created;
	//std::vector<std::vector<AVFrame>> results;
public:
	static Records<Sequence*> sequences;
	Sequence(std::string);
	Sequence();
	virtual ~Sequence();
	void SaveVideo(std::string&);
	void AddClip(VideoClip*, int);
	void AddClip(VideoClip*);
	inline bool GetCreated();

	int GetSize() override { return 0; };
	SyncObject<AVFrame*>& GetChunk(int at) override { return video.at(0)->GetClip().at(0); };
	void Play() override {};
};
extern Records<Sequence*> sequences;

inline bool Sequence::GetCreated()
{
	return created;
}