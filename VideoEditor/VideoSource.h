#pragma once
#include "ISource.h"
#include "IImg.h"
#include "IPlayable.h"

#include <sstream>
#include <vector>
#include<chrono>

// FFmpeg

extern "C" {
#include <libavformat\avformat.h>
#include <libavcodec\avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class VideoSource :
    public ISource<std::vector<cv::Mat>>, public IImg, public IPlayable
{
public:
    VideoSource(std::string);
    virtual ~VideoSource();
    void Show() override;
    void Play() override;
private:
    void ReadSource(std::string) override;
};

