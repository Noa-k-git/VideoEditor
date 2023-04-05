#pragma once
#include "globals.h"
#include "ISource.h"
#include "IImg.h"
#include "IPlayable.h"

#include <iostream>
#include <sstream>
#include <vector>
#include<chrono>

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



// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class VideoSource :
    //public ISource<std::vector<cv::Mat>>, public IImg, public IPlayable
    public ISource<std::vector<AVFrame>>, public IImg, public IPlayable
{
public:
    VideoSource(std::string);
    virtual ~VideoSource();
    void Show() override;
    void Play() override;
private:
    void ReadSource(std::string) override;

    cv::Mat Avframe2Cvmat(const AVFrame*);
    //AVFrame* CopyFrame(const AVFrame*);
    //AVFrame* Cvmat2Avframe(cv::Mat* image, AVFrame* frame);
};

