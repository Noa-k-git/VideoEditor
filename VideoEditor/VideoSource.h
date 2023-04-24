#pragma once
#include "globals.h"
#include "ISource.h"
#include "IImg.h"
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

#include "Map.h"
class VideoSource :
    //public ISource<std::vector<cv::Mat>>, public IImg, public IPlayable
    public ISource<AVFrame*>, public IImg
{
public:
    static Records<VideoSource*> videoSources;
    VideoSource(std::string, std::string);
    VideoSource(std::string);
    virtual ~VideoSource();
    int GetSize() override;
    SyncObject<AVFrame*>& GetChunk(int at) override;
    void Show() override;
    void Play() override;
    inline bool GetCreated();

    const AVFrame* getFirstFrame();
private:
    bool created;
    void ReadSource() override;
    void UpdateCreated();
    cv::Mat Avframe2Cvmat(const AVFrame*);
    //AVFrame* CopyFrame(const AVFrame*);
    //AVFrame* Cvmat2Avframe(cv::Mat* image, AVFrame* frame);
};
extern Records<VideoSource*> videoSources;

inline bool VideoSource::GetCreated()
{
    return created;
}