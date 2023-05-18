#include "RotateEffect.h"
extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

RotateEffect::RotateEffect()
{
	rotate = UP;
}

void RotateEffect::RotateLeft()
{
		rotate = static_cast<Rotation>((rotate + 270) % 360);
}

void RotateEffect::RotateRight()
{
	rotate = static_cast<Rotation>((rotate + 90) % 360);
}

void RotateEffect::Apply(AVFrame*& frame)
{
    int width = frame->width;
    int height = frame->height;

    // Create a new AVFrame for the rotated frame
    AVFrame* rotatedFrame = av_frame_alloc();
    rotatedFrame->format = frame->format;
    switch (rotate) {
    case UP: case UPDOWN:
        rotatedFrame->width = width;
        rotatedFrame->height = height;
        break;
    default:
        rotatedFrame->width = height;
        rotatedFrame->height = width;
    }

    // Allocate memory for the rotated frame's data
    int ret = av_frame_get_buffer(rotatedFrame, 0);
    if (ret < 0) {
        // Error handling
        av_frame_free(&rotatedFrame);
        return;
    }

    // Create SwsContext for frame conversion
    SwsContext* swsCtx = sws_getContext(height, width, static_cast<AVPixelFormat>(frame->format),
        rotatedFrame->height, rotatedFrame->width,
        static_cast<AVPixelFormat>(rotatedFrame->format),
        0, nullptr, nullptr, nullptr);
    
    if (!swsCtx) {
        av_frame_free(&rotatedFrame);
        return;
    }
    // Set the rotation angle
    // Perform the frame rotation
    sws_scale(swsCtx, frame->data, frame->linesize, 0, height, rotatedFrame->data, rotatedFrame->linesize);

    // Free the SwsContext
    sws_freeContext(swsCtx);

    // Free the original frame's data
    av_freep(&frame->data[0]);

    // Copy the rotated frame's data to the original frame
    av_image_copy(frame->data, frame->linesize, const_cast<const uint8_t**>(rotatedFrame->data), rotatedFrame->linesize,
        static_cast<AVPixelFormat>(frame->format), frame->width, frame->height);

    // Update the frame properties
    frame->width = rotatedFrame->width;
    frame->height = rotatedFrame->height;
    frame->linesize[0] = rotatedFrame->linesize[0];

    // Free the rotated frame
    av_frame_free(&rotatedFrame);
}
