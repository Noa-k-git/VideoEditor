#pragma once
#include "IEffect.h"
#include "EffectProperty.h"
// FFmpeg

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>

#include<libavfilter/avfilter.h>
    // types
#include <inttypes.h>
}
class ScaleEffect :
    public IEffect
{
private:
    EffectProperty<double> scale;
public:
    void ApplyEffect(AVFrame& src, AVFrame& otp, int frame); //override;
};
