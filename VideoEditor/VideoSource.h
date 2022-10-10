#pragma once
#include "ISource.h"
#include "IImg.h"
#include "IPlayable.h"

#include <sstream>
#include <vector>
#include<chrono>

// FFmpeg

extern "C" {

}
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class VideoSource :
    public ISource, public IImg, public IPlayable
{
private:
    std::vector<cv::Mat> clip;
public:
    VideoSource(std::string);
    virtual ~VideoSource();
    void ReadSource(std::string) override;
    void Show() override;
    void Play() override;
    
};

