#pragma once
#include "globals.h"
#include "ISource.h"
#include "IPlayable.h"
#include "Records.h"
#include "SyncObject.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include<chrono>
#include <wx/splitter.h>
// FFmpeg

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h> // contains the utility API, used for memory management, logging, and mathematical operations
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>

#include<libavfilter/avfilter.h>

    // types
#include <inttypes.h>
}

#include<wx/msgdlg.h>

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

/// <summary>
/// A class that represents a video source object
/// </summary>
class VideoSource :
    public ISource<AVFrame*>
{
public:
    static Records<VideoSource*> videoSources;
    VideoSource(std::string path, std::string name);
    VideoSource(std::string, std::string data, bool load = false);
    ~VideoSource() override;
    /// <summary>
    /// Loading a video source from a string
    /// </summary>
    /// <param name="">The project path</param>
    /// <param name="">The string data</param>
    void Load(std::string, std::string);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="">The defult path</param>
    /// <returns>A string that describe the Video source object</returns>
    std::string Write(std::string);
    int GetSize() override;
    SyncObject<AVFrame*>* GetChunk(int at) override;
    //void Play() override;
    inline bool GetCreated();
    inline bool GetRead();
    const AVFrame* getFirstFrame();
private:
    bool created;
    bool read;
    void ReadSource() override;
    void UpdateCreated();

};
extern Records<VideoSource*> videoSources;

inline bool VideoSource::GetCreated()
{
    return created;
}

inline bool VideoSource::GetRead()
{
    return read;
}