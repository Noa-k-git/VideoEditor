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

class IEffect
{
public:
	virtual void Apply(AVFrame*&) = 0;
};
