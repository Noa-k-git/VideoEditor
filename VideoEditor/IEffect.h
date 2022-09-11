#include<opencv2/opencv.hpp>
#include <vector>;
#include "Clip.h"

using std::vector;
using cv::Mat;

class IEffect
{
public:
	virtual void applyEffect(vector<Mat>&, SourceClip&, bool) = 0;
};
