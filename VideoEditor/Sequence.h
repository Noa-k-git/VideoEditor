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
	static AVFrame* CreateBlackFrame(int width, int height, enum AVPixelFormat pixFmt);
	static Records<Sequence*> sequences;
	Sequence(std::string);
	Sequence();
	virtual ~Sequence();
	void SaveVideo(std::string&);
	void AddClip(VideoClip*, int);
	void AddClip(VideoClip*);
	bool SwapClipsAt(int idx1, int idx2);
	inline bool GetCreated();
	inline int GetLength() { return video.size(); }
	inline VideoClip* GetClipAt(int idx) { return video.at(idx); }

	int GetSize() override { 
		int sum = 0;
		for (int i = 0; i < GetLength(); i++) {
			sum += video.at(i)->GetSize();
		}
		return sum; };
	SyncObject<AVFrame*>* GetChunk(int at) override;
};
extern Records<Sequence*> sequences;

inline bool Sequence::GetCreated()
{
	return created;
}
