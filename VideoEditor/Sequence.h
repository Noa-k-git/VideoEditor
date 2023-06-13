#pragma once
#include "VideoClip.h"
#include "IPlayable.h"
#include <wx/msgdlg.h>
struct Settings {
	int length; // the num of frames in the video
	int resolution[2]; // width, height
};

/// <summary>
/// A class that represents a sequence
/// </summary>
class Sequence : public IPlayable<AVFrame*>
{
private:
	Settings settings;	
	std::vector<VideoClip*> video;
	bool created;
	//std::vector<std::vector<AVFrame>> results;
public:
	/// <summary>
	/// Creates a black frame/
	/// </summary>
	/// <param name="width">the frame width</param>
	/// <param name="height">the frame height</param>
	/// <param name="">the pixel format</param>
	/// <returns> A black frame</returns>
	static AVFrame* CreateBlackFrame(int width, int height, enum AVPixelFormat pixFmt);
	static Records<Sequence*> sequences;
	Sequence(std::string);
	Sequence();
	Sequence(std::string data, bool load);
	virtual ~Sequence();
	void UpdateCreated();
	/// <summary>
	/// 
	/// </summary>
	/// <returns>A string with the sequence data</returns>
	std::string Write();
	/// <summary>
	/// Exports the video
	/// </summary>
	/// <param name="">the path to the new video</param>
	void SaveVideo(const std::string&);
	/// <summary>
	/// Adding clip to the sequence
	/// </summary>
	/// <param name="">The clip</param>
	/// <param name="">idx</param>
	void AddClip(VideoClip*, int);
	/// <summary>
	/// Adds a clip to the sequence at the end
	/// </summary>
	/// <param name="">The clip</param>
	void AddClip(VideoClip*);
	/// <summary>
	/// Swap between 2 clips
	/// </summary>
	/// <param name="idx1">idx1</param>
	/// <param name="idx2">idx2</param>
	/// <returns></returns>
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
