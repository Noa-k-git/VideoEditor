#pragma once
#include <wx/image.h>
extern "C" {
#include <libavutil/frame.h>
}
namespace image_helpers {
	wxImage* ConvertAVFrame2WxImg(AVFrame* frame);
	AVFrame* ConvertWxImg2AVFrame(wxImage img);

}