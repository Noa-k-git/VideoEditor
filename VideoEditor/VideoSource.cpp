#include "VideoSource.h"
#undef av_err2str(errnum)
char x[AV_ERROR_MAX_STRING_SIZE];
#define av_err2str(errnum) \
    av_make_error_string(x, AV_ERROR_MAX_STRING_SIZE, errnum)

VideoSource::VideoSource(std::string path) : ISource(path), IImg(), IPlayable()
{
}

VideoSource::~VideoSource()
{
	this->source.clear();
	//std::vector<cv::Mat>().swap(source);
}

void VideoSource::ReadSource(std::string path)
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

	vidCapture.release(); // closes the file

	// Open the file using libavformat
	AVFormatContext* av_format_ctx = avformat_alloc_context();
	if (!av_format_ctx) {
		printf("Couldn't create AVFormatContext\n");
		return; 
	}
	if (avformat_open_input(&av_format_ctx, path.c_str(), NULL, NULL) != 0) {
		printf("Couldn't open video file\n");
		return;
	}

	// Find the first valid video stream inside the file
	int video_stream_index = -1;
	AVCodecParameters* av_codec_params = NULL;
	const AVCodec* av_codec = NULL;
	for (int i = 0; i < av_format_ctx->nb_streams; i)
	{
		av_codec_params = av_format_ctx->streams[i]->codecpar;
		av_codec = avcodec_find_decoder(av_codec_params->codec_id);

		if (!av_codec) {
			continue;
		}
		if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
			video_stream_index = i;
			break;
		}
	}

	if (video_stream_index == -1) {
		printf("Couldn't find valid video stream inside file\n");
		return;
	}

	// Set up a codec context for the decoder
	AVCodecContext* av_codec_ctx = avcodec_alloc_context3(av_codec);
	if (!av_codec_ctx) {
		printf("Couldn't create AVCpdecContext\n");
		return;
	}

	if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0)
	{
		printf("Couldn't initialize AVCodecContext\n");
		return;
	}
	if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0) {
		printf("Couldn't open codec\n");
		return;
	}

	AVFrame* av_frame = av_frame_alloc();
	if (!av_frame) {
		printf("Couldn't allocate AVFrame\n");
		return;
	}
	AVPacket* av_packet = av_packet_alloc();
	if (!av_packet) {
		printf("Couldn't allocate AVPacket\n");
		return;
	}
	int response;

	while (av_read_frame(av_format_ctx, av_packet) >= 0) {
		if (av_packet->stream_index != video_stream_index) {
			av_packet_unref(av_packet);
			continue;
		}
		response = avcodec_send_packet(av_codec_ctx, av_packet);
		if (response < 0) {
			printf("Failed to decode packet: %s\n", av_err2str(response));
			return;
		}
		response = avcodec_receive_frame(av_codec_ctx, av_frame);
		if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
			av_packet_unref(av_packet);
			continue;
		}
		else if (response < 0) {
			printf("Failed to decode frame: %s\n", av_err2str(response));
			return;
		}
		av_packet_unref(av_packet);

		av_packet = av_packet_alloc();

		response = avcodec_send_frame(av_codec_ctx, av_frame);
		std::string tmp = av_err2str(response);
		//source.push_back(*av_frame);
		//auto mat_frame = Avframe2Cvmat(av_frame);

		//source.push_back(im);
		//bool isEqual = (cv::sum(Avframe2Cvmat(av_frame) != Avframe2Cvmat(&source[0])) == cv::Scalar(0, 0, 0, 0));
		//bool isEqual = (cv::sum(im != source[0]) == cv::Scalar(0, 0, 0, 0));
		//im.release();

		source.push_back(*new AVFrame);
		source.back() = *av_frame_clone(av_frame);
		/*
		if (int iRet = av_frame_copy(&source.back(), av_frame) == 0) {
			av_log(NULL, AV_LOG_INFO, "Ok");
		}
		else {
			av_log(NULL, AV_LOG_INFO, "Error: %s\n", av_err2str(iRet));
		}*/
		av_frame_unref(av_frame);
	}
	

	avformat_close_input(&av_format_ctx);
	avformat_free_context(av_format_ctx);
	av_frame_free(&av_frame);
	av_packet_free(&av_packet);
	avcodec_free_context(&av_codec_ctx);
}

cv::Mat VideoSource::Avframe2Cvmat(const AVFrame* av_frame)
{
	int width = av_frame->width;
	int height = av_frame->height;
	cv::Mat image(height, width, CV_8UC3);
	int cvLinesizes[1];
	cvLinesizes[0] = image.step1();
	SwsContext* conversion = sws_getContext(
		width, height, (AVPixelFormat)av_frame->format, width, height,
		AVPixelFormat::AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	sws_scale(conversion, av_frame->data, av_frame->linesize, 0, height, &image.data,
		cvLinesizes);
	sws_freeContext(conversion);
	return image;
}

/*
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
}*/

void VideoSource::Show()
{
    // TODO: add exeption handling for not having anything inside source
	//cv::imshow("Frame", source[0]);
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
		//cv::imshow("Frame", source[frame]);
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
