#include "ScaleEffect.h"
using cv::Size;

ScaleEffect::ScaleEffect() : scale (1)
{
	
}

void ScaleEffect::applyEffect(vector<Mat>& clip, SourceClip& sc, bool defult)
{
	if (clip.empty())
		return;
	auto res = sc.getInfo().resulusion;
	auto def = this->scale.getDefualt();
	if (defult || this->scale.getKeyframes().empty()) {
		for (Mat& frame : clip) {
			if (res[0] * def != clip[0].rows) {
				resize(frame, frame, Size(res[0] * def, res[1] * def));
			}
		}
	}
	else {
		auto kfs = this->scale.getKeyframes();
		for (int frame = 0; frame < kfs[0].frame; frame++) {
			if (clip[0].rows != res[0] * kfs[0].frame) {
				resize(clip[frame], clip[frame], Size(kfs[0].value * res[0], kfs[0].value * res[1]))
			}
		}
	}

}
