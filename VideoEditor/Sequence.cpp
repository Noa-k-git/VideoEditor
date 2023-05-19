#pragma once
#include "Sequence.h"
#include "string_utils.h"
#include <algorithm>

using namespace string_utils;
#undef av_err2str
//extern char x[AV_ERROR_MAX_STRING_SIZE];
#define av_err2str(errnum) \
    av_make_error_string(x, AV_ERROR_MAX_STRING_SIZE, errnum)

AVFrame* Sequence::CreateBlackFrame(int width, int height, enum AVPixelFormat pixFmt) {
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        return NULL;
    }

    frame->format = pixFmt;
    frame->width = width;
    frame->height = height;

    int ret = av_frame_get_buffer(frame, 32); // 32-byte alignment
    if (ret < 0) {
        av_frame_free(&frame);
        return NULL;
    }

    // Set all pixel values to zero
    av_frame_make_writable(frame);
    memset(frame->data[0], 0, frame->linesize[0] * frame->height);

    return frame;
}

Records<Sequence*> Sequence::sequences;

Sequence::Sequence(std::string name) : IPlayable<AVFrame*>(name)
{
    UpdateCreated();
}

Sequence::Sequence() : Sequence("My Sequence")
{
}

Sequence::Sequence(std::string data, bool load) : IPlayable<AVFrame*>("this is a dummy name")
{
    std::vector<std::string> args = SplitString(data, ',');
    this->SetName(args.at(0));
    UpdateCreated();
    if (!created) return;
    args.erase(args.begin());
    for (std::string& clipData : args) {
        std::vector<std::string> clipArgs = SplitString(clipData, '=');
        if (clipArgs.size() == 3) {
            auto res = VideoSource::videoSources.Contains(clipArgs.at(0));
            if (res.second) {
                VideoClip* newClip = new VideoClip(*res.first);
                newClip->SetStart(std::stoi(clipArgs.at(1)));
                newClip->SetEnd(std::stoi(clipArgs.at(2)));
                this->AddClip(newClip);
            }
        }
    }
}

Sequence::~Sequence()
{
    for (auto& clip : video) {
        delete clip;
    }
}

void Sequence::UpdateCreated()
{
    if (Sequence::sequences.AddRecord(this).second)
        created = true;
    else
        created = false;
}

std::string Sequence::Write()
{
    std::string res = GetName();
    for (VideoClip*& oneClip : video) {
        res += ',' + oneClip->GetName() + '=' + std::to_string(oneClip->GetStart()) + '=' + std::to_string(oneClip->GetEnd());
    }
    return res;
}

void Sequence::SaveVideo(std::string& output_filename)
{
    // Open the output file context
    AVFormatContext* format_ctx = nullptr;
    int ret = avformat_alloc_output_context2(&format_ctx, nullptr, nullptr, output_filename.c_str());
    if (ret < 0) {
        wxMessageBox("Error creating output context: ");
        wxMessageBox(av_err2str(ret));
        return;
    }

    // Open the output file
    ret = avio_open(&format_ctx->pb, output_filename.c_str(), AVIO_FLAG_WRITE);
    if (ret < 0) {
        std::cerr << "Error opening output file: " << av_err2str(ret) << std::endl;
        avformat_free_context(format_ctx);
        return;
    }

    // Create the video stream
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "Error finding H.264 encoder" << std::endl;
        avformat_free_context(format_ctx);
        return;
    }

    AVStream* stream = avformat_new_stream(format_ctx, codec);
    if (!stream) {
        std::cerr << "Error creating output stream" << std::endl;
        avformat_free_context(format_ctx);
        return;
    }
    settings.length = video.front()->GetSize();
    settings.resolution[0] = video.front()->GetChunk(0)->GetObject()->width;
    settings.resolution[1] = video.front()->GetChunk(0)->GetObject()->height;
    //// Set the stream parameters
    //stream->codecpar->codec_id = AV_CODEC_ID_H264;
    //stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    //stream->codecpar->width = settings.resolution[0]; // 270
    //stream->codecpar->height = settings.resolution[1]; // 480
    //stream->codecpar->format = AV_PIX_FMT_YUV420P;
    //stream->codecpar->bit_rate = 16000;
    //AVRational framerate = { 1, 30};
    //stream->time_base = av_inv_q(framerate);

    // Open the codec context
    AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
    //codec_ctx->codec_tag = 0;
    //codec_ctx->time_base = stream->time_base;
    //codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    //if (!codec_ctx) {
    //    std::cout << "Error allocating codec context" << std::endl;
    //    avformat_free_context(format_ctx);
    //    return;
    //}

    //ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
    //if (ret < 0) {
    //    std::cout << "Error setting codec context parameters: " << av_err2str(ret) << std::endl;
    //    avcodec_free_context(&codec_ctx);
    //    avformat_free_context(format_ctx);
    //    return;
    //}
    codec_ctx->codec_id = AV_CODEC_ID_H264;
    codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    codec_ctx->width = settings.resolution[0];
    codec_ctx->height = settings.resolution[1];
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    codec_ctx->bit_rate = 400000;
    AVRational framerate = { 30, 1 };
    //AVRational framerate = { 1, 30 };
    codec_ctx->time_base = av_inv_q(framerate);
    codec_ctx->codec_tag = 0;
    codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    stream->time_base = codec_ctx->time_base;

    AVDictionary* opt = NULL;
    ret = avcodec_open2(codec_ctx, codec, &opt);
    if (ret < 0) {
        wxMessageBox("Error opening codec: ");
        wxMessageBox(av_err2str(ret));
        avcodec_free_context(&codec_ctx);
        avformat_free_context(format_ctx);
        return;
    }
    avcodec_parameters_from_context(stream->codecpar, codec_ctx);
    stream->time_base = codec_ctx->time_base;
    // Allocate a buffer for the frame data
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        std::cerr << "Error allocating frame" << std::endl;
        avcodec_free_context(&codec_ctx);
        avformat_free_context(format_ctx);
        return;
    }

    frame->format = codec_ctx->pix_fmt;
    frame->width = codec_ctx->width;
    frame->height = codec_ctx->height;

    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) {
        std::cerr << "Error allocating frame buffer: " << av_err2str(ret) << std::endl;
        av_frame_free(&frame);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(format_ctx);
        return;
    }

    // Allocate a buffer for the converted frame data
    AVFrame* converted_frame = av_frame_alloc();
    if (!converted_frame) {
        std::cerr << "Error allocating converted frame" << std::endl;
        av_frame_free(&frame);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(format_ctx);
        return;
    }

    converted_frame->format = AV_PIX_FMT_YUV420P;
    converted_frame->width = codec_ctx->width;
    converted_frame->height = codec_ctx->height;

    ret = av_frame_get_buffer(converted_frame, 0);
    if (ret < 0) {
        std::cerr << "Error allocating converted frame buffer: " << av_err2str(ret) << std::endl;
        av_frame_free(&frame);
        av_frame_free(&converted_frame);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(format_ctx);
        return;
    }

    // Initialize the converter
    SwsContext* converter = sws_getContext(
        codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
        codec_ctx->width, codec_ctx->height, AV_PIX_FMT_YUV420P,
        SWS_BICUBIC, nullptr, nullptr, nullptr
    );
    if (!converter) {
        std::cerr << "Error initializing converter" << std::endl;
        av_frame_free(&frame);
        av_frame_free(&converted_frame);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(format_ctx);
        return;
    }

    // Write the header to the output file
    ret = avformat_write_header(format_ctx, nullptr);
    //stream->time_base = codec_ctx->time_base;

    if (ret < 0) {
        std::cerr << "Error writing header to output file: " << av_err2str(ret) << std::endl;
        av_frame_free(&frame);
        av_frame_free(&converted_frame);
        sws_freeContext(converter);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(format_ctx);
        return;
    }

    // Iterate over the frames and write them to the output file
    int frame_count = 0;
    double pts = 0;
    for (auto& clip : video) {
        for (int i = 0; i < clip->GetSize(); i ++) {
            auto srcFrame = clip->GetChunk(i)->GetObject();
            // Convert the frame to the output format
            sws_scale(converter,
                srcFrame->data, srcFrame->linesize, 0, srcFrame->height,
                converted_frame->data, converted_frame->linesize
            );

            // Set the frame properties
            //converted_frame->pts = av_rescale_q(frame_count, stream->time_base, codec_ctx->time_base);
            converted_frame->pts = pts;
            pts += av_rescale_q(1, codec_ctx->time_base, stream->time_base);
            frame_count++;
            //converted_frame->time_base.den = codec_ctx->time_base.den;
            //converted_frame->time_base.num = codec_ctx->time_base.num;
            // Encode the frame and write it to the output
            ret = avcodec_send_frame(codec_ctx, converted_frame);
            if (ret < 0) {
                std::cerr << "Error sending frame for encoding: " << av_err2str(ret) << std::endl;
                av_frame_free(&frame);
                av_frame_free(&converted_frame);
                sws_freeContext(converter);
                avcodec_free_context(&codec_ctx);
                avformat_free_context(format_ctx);
                return;
            }
            AVPacket* pkt = av_packet_alloc();
            if (!pkt) {
                std::cerr << "Error allocating packet" << std::endl;
                return;
            }
            while (ret >= 0) {
                ret = avcodec_receive_packet(codec_ctx, pkt);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    std::string a = av_err2str(ret);
                    av_packet_unref(pkt);
                    break;
                }
                else if (ret < 0) {
                    wxMessageBox("Error during encoding");
                    wxMessageBox(av_err2str(ret));
                    av_packet_unref(pkt);
                    av_frame_free(&frame);
                    av_frame_free(&converted_frame);
                    sws_freeContext(converter);
                    avcodec_free_context(&codec_ctx);
                    avformat_free_context(format_ctx);
                    return;
                }

                // Write the packet to the output file
                //av_packet_rescale_ts(pkt, codec_ctx->time_base, stream->time_base);
                pkt->stream_index = stream->index;
                
                pkt->duration = av_rescale_q(1, codec_ctx->time_base, stream->time_base);
                ret = av_interleaved_write_frame(format_ctx, pkt);
                if (ret < 0) {
                    std::cerr << "Error writing packet to output file: " << av_err2str(ret) << std::endl;
                    auto a = av_err2str(ret);
                    av_frame_free(&frame);
                    av_frame_free(&converted_frame);
                    sws_freeContext(converter);
                    avcodec_free_context(&codec_ctx);
                    avformat_free_context(format_ctx);
                    return;
                }
                av_packet_unref(pkt);
            }
        }
    }

    // Flush the encoder
    ret = avcodec_send_frame(codec_ctx, nullptr);
    if (ret < 0) {
        std::cerr << "Error flushing encoder: " << av_err2str(ret) << std::endl;
        av_frame_free(&frame);
        av_frame_free(&converted_frame);
        sws_freeContext(converter);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(format_ctx);
        return;
    }

    while (ret >= 0) {
        AVPacket* pkt = av_packet_alloc();
        if (!pkt) {
            std::cerr << "Error allocating packet" << std::endl;
            return;
        }
        ret = avcodec_receive_packet(codec_ctx, pkt);
        //if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
        //    wxMessageBox("Error recieving packet");
        //    wxMessageBox(av_err2str(ret));
        //    break;
        //}
        //else if (ret < 0) {
        //    std::cerr << "Error during encoding: " << av_err2str(ret) << std::endl;
        //    av_packet_unref(pkt);
        //    av_frame_free(&frame);
        //    av_frame_free(&converted_frame);
        //    sws_freeContext(converter);
        //    avcodec_free_context(&codec_ctx);
        //    avformat_free_context(format_ctx);
        //    return;
        //}

        // Write the packet to the output file
        //av_packet_rescale_ts(pkt, codec_ctx->time_base, stream->time_base);
        if (ret == 0)
        {
            pkt->stream_index = stream->index;
            pkt->duration = av_rescale_q(1, codec_ctx->time_base, stream->time_base);
            ret = av_interleaved_write_frame(format_ctx, pkt);
            av_packet_unref(pkt);
            if (ret < 0) {
                std::cerr << "Error writing packet to output file: " << av_err2str(ret) << std::endl;
                av_frame_free(&frame);
                av_frame_free(&converted_frame);
                sws_freeContext(converter);
                avcodec_free_context(&codec_ctx);
                avformat_free_context(format_ctx);
                return;
            }
        }
    }

    // Write the trailer to the output file
    ret = av_write_trailer(format_ctx);
    if (ret < 0) {
        std::cerr << "Error writing trailer to output file: " << av_err2str(ret) << std::endl;
    }

    // Free all resources
    av_frame_free(&frame);
    av_frame_free(&converted_frame);
    sws_freeContext(converter);
    avcodec_free_context(&codec_ctx);
    avformat_free_context(format_ctx);
}


SyncObject<AVFrame*>* Sequence::GetChunk(int idx)
{
   // [[1, 2, 3], [4, 5, 6]]; at = 4;
   // "idx = 1"
    // sum = 6
    // prevsum = 3
    // 6-3
    // 4 - (6-3) = 1 V
    int sum = 0;
    int currIdx = 0;
    for (int i = 0; i < GetLength(); i++)
    {
        sum += video.at(i)->GetSize();
        if (sum > idx) {
            return video.at(i)->GetChunk(idx-currIdx);
        }
        currIdx = sum;
    }
    return nullptr;
}

void Sequence::AddClip(VideoClip* videoClip, int idx)
{
    if (idx < 0 || idx > video.size()) {
        idx = video.size();
    }
    video.insert(video.begin() + idx, videoClip);

}

void Sequence::AddClip(VideoClip* videoClip)
{
    AddClip(videoClip, -1);
}

bool Sequence::SwapClipsAt(int idx1, int idx2) {
    if (idx1 > -1 && idx2 > -1 && idx1 < video.size() && idx2 < video.size())
    {
        std::iter_swap(video.begin() + idx1, video.begin() + idx2);
        return true;
    }
    return false;
}
