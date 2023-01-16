#include<opencv2/opencv.hpp>
#include <vector>;
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
using std::vector;
using cv::Mat;

class IEffect
{
public:
	//virtual void applyEffect(vector<Mat>&, SourceClip&, bool) = 0;
};
