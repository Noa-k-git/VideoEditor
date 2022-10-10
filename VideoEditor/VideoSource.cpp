#include "VideoSource.h"

VideoSource::VideoSource(std::string path) : ISource(path), IImg(), IPlayable()
{
	cv::VideoCapture vidCapture(path);
	// prints error message if the stream is invalid
	if (!vidCapture.isOpened())
		std::cout << "Error opening video stream of file" << std::endl;
	else
	{
		// Obtain fps and frame count by get() method and print
		// You can replace 5 with CAP_PROP_FPS as well, they are enumerations
		this->fps = vidCapture.get(5);
		std::cout << "Frames per second :" << this->fps << std::endl;

		// Obtain frame_count using opencv built in frame count reading method
		// You can replace 7 with CAP_PROP_FRAME_COUNT as well, they are enumerations
		this->length = vidCapture.get(7); // number of frames

		this->resolution[0] = vidCapture.get(cv::CAP_PROP_FRAME_WIDTH);
		this->resolution[1] = vidCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
		std::cout << "  Frame count :" << this->length << std::endl;
	}

	cv::Mat blackImage(this->resolution[0], this->resolution[1], CV_8UC3, cv::Scalar(0, 0, 0));
	source = std::vector<cv::Mat>(this->length, blackImage);
	vidCapture.release(); // closes the file
	//delete& vidCapture; // deleting object from memory
    
	//std::thread readData(&VideoSource::ReadSource, this, path);
	//readData.join(); - change to detach
}

VideoSource::~VideoSource()
{
	//this->source.clear();
	//std::vector<cv::Mat>().swap(source);
}
/*
void VideoSource::ReadSource(std::string path)
{
    {
        if (argc < 2) {
            std::cout << "Usage: ff2cv <infile>" << std::endl;
            return 1;
        }
        const char* infile = argv[1];

        // initialize FFmpeg library
        av_register_all();
        //  av_log_set_level(AV_LOG_DEBUG);
        int ret;

        // open input file context
        AVFormatContext* inctx = nullptr;
        ret = avformat_open_input(&inctx, infile, nullptr, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avforamt_open_input(\"" << infile << "\"): ret=" << ret;
            return 2;
        }
        // retrive input stream information
        ret = avformat_find_stream_info(inctx, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avformat_find_stream_info: ret=" << ret;
            return 2;
        }

        // find primary video stream
        AVCodec* vcodec = nullptr;
        ret = av_find_best_stream(inctx, AVMEDIA_TYPE_VIDEO, -1, -1, &vcodec, 0);
        if (ret < 0) {
            std::cerr << "fail to av_find_best_stream: ret=" << ret;
            return 2;
        }
        const int vstrm_idx = ret;
        AVStream* vstrm = inctx->streams[vstrm_idx];

        // open video decoder context
        ret = avcodec_open2(vstrm->codec, vcodec, nullptr);
        if (ret < 0) {
            std::cerr << "fail to avcodec_open2: ret=" << ret;
            return 2;
        }

        // print input video stream informataion
        std::cout
            << "infile: " << infile << "\n"
            << "format: " << inctx->iformat->name << "\n"
            << "vcodec: " << vcodec->name << "\n"
            << "size:   " << vstrm->codec->width << 'x' << vstrm->codec->height << "\n"
            << "fps:    " << av_q2d(vstrm->codec->framerate) << " [fps]\n"
            << "length: " << av_rescale_q(vstrm->duration, vstrm->time_base, { 1,1000 }) / 1000. << " [sec]\n"
            << "pixfmt: " << av_get_pix_fmt_name(vstrm->codec->pix_fmt) << "\n"
            << "frame:  " << vstrm->nb_frames << "\n"
            << std::flush;

        // initialize sample scaler
        const int dst_width = vstrm->codec->width;
        const int dst_height = vstrm->codec->height;
        const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_BGR24;
        SwsContext* swsctx = sws_getCachedContext(
            nullptr, vstrm->codec->width, vstrm->codec->height, vstrm->codec->pix_fmt,
            dst_width, dst_height, dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
        if (!swsctx) {
            std::cerr << "fail to sws_getCachedContext";
            return 2;
        }
        std::cout << "output: " << dst_width << 'x' << dst_height << ',' << av_get_pix_fmt_name(dst_pix_fmt) << std::endl;

        // allocate frame buffer for output
        AVFrame* frame = av_frame_alloc();
        std::vector<uint8_t> framebuf(avpicture_get_size(dst_pix_fmt, dst_width, dst_height));
        avpicture_fill(reinterpret_cast<AVPicture*>(frame), framebuf.data(), dst_pix_fmt, dst_width, dst_height);

        // decoding loop
        AVFrame* decframe = av_frame_alloc();
        unsigned nb_frames = 0;
        bool end_of_stream = false;
        int got_pic = 0;
        AVPacket pkt;
        do {
            if (!end_of_stream) {
                // read packet from input file
                ret = av_read_frame(inctx, &pkt);
                if (ret < 0 && ret != AVERROR_EOF) {
                    std::cerr << "fail to av_read_frame: ret=" << ret;
                    return 2;
                }
                if (ret == 0 && pkt.stream_index != vstrm_idx)
                    goto next_packet;
                end_of_stream = (ret == AVERROR_EOF);
            }
            if (end_of_stream) {
                // null packet for bumping process
                av_init_packet(&pkt);
                pkt.data = nullptr;
                pkt.size = 0;
            }
            // decode video frame
            avcodec_decode_video2(vstrm->codec, decframe, &got_pic, &pkt);
            if (!got_pic)
                goto next_packet;
            // convert frame to OpenCV matrix
            sws_scale(swsctx, decframe->data, decframe->linesize, 0, decframe->height, frame->data, frame->linesize);
            {
                cv::Mat image(dst_height, dst_width, CV_8UC3, framebuf.data(), frame->linesize[0]);
                cv::imshow("press ESC to exit", image);
                if (cv::waitKey(1) == 0x1b)
                    break;
            }
            std::cout << nb_frames << '\r' << std::flush;  // dump progress
            ++nb_frames;
        next_packet:
            av_free_packet(&pkt);
        } while (!end_of_stream || got_pic);
        std::cout << nb_frames << " frames decoded" << std::endl;

        av_frame_free(&decframe);
        av_frame_free(&frame);
        avcodec_close(vstrm->codec);
        avformat_close_input(&inctx);
        return 0;
    }
}*/
void VideoSource::ReadSource(std::string path) 
{
	cv::VideoCapture vidCapture(path, cv::CAP_FFMPEG);
	std::vector<cv::Mat> imgs;
	while (vidCapture.isOpened())
	{
		// Initialise frame matrix
		cv::Mat frame;
		// Initialize a boolean to check if frames are there or not
		bool isSuccess = vidCapture.read(frame);

		// Unhandled exception at 0x00007FFA6A4E4FD9 in VideoEditor1.exe: Microsoft C++ exception: cv::Exception at memory location 0x0000004F7E2FE890.

		// cout << cell;
		if (isSuccess) {
			if (imgs.size() == imgs.max_size()) {
				std::cout << "Video is too big";
			}
			else
				imgs.push_back(frame);

			//cout << delay << endl;

			//frame.release();
			//frame.release();
		}
		// If frames are not there, close it
		
		else
		{
			std::cout << "Video camera is disconnected" << std::endl;
			break;
		}

	}
	source.swap(imgs);
	vidCapture.release();
}

void VideoSource::Show()
{
    // TODO: add exeption handling for not having anything inside source
	cv::imshow("Frame", source[0]);
	int key = cv::waitKey(1);
	cv::destroyAllWindows();


}

void VideoSource::Play()
{
	auto start = std::chrono::high_resolution_clock::now();
	// Read the frames to the last frame
	auto sec_delay = std::chrono::milliseconds(long long(float(1 / this->fps) * 1e3));
	for (int frame = 0; frame < source.size(); frame++)
	{
		//display frames
		cv::imshow("Frame", source[frame]);
		int key = cv::waitKey(1 / this->fps * 1e3 - 9);
		if (key == 'q')
		{
			std::cout << "q key is pressed by the user. Stopping the video" << std::endl;
			cv::destroyAllWindows();
			return;
		}
	}
	cv::destroyAllWindows();
}
