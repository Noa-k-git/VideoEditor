#include "VideoSource.h"
#include "string_utils.h"
using namespace string_utils;
#undef av_err2str
char x[AV_ERROR_MAX_STRING_SIZE];
#define av_err2str(errnum) \
    av_make_error_string(x, AV_ERROR_MAX_STRING_SIZE, errnum)

Records<VideoSource*> VideoSource::videoSources;

VideoSource::VideoSource(std::string path, std::string name) : ISource(path, name), IImg()
{
	UpdateCreated();
}

VideoSource::VideoSource(std::string sourcePath, std::string data, bool load) : ISource(data), IImg()
{
	if (load)
		Load(sourcePath, data);
	else path = sourcePath + data;
	UpdateCreated();
}

VideoSource::~VideoSource()
{
	if (created) {
		VideoSource::videoSources.RemoveRecord(GetName());
		for (auto& element : source_) {
			auto syncPair = element.GetObjectForUpdate();
			av_frame_free(&syncPair.second);
			av_frame_unref(syncPair.second);
			//av_freep(element);
		}
		this->source_.clear();
	}
	//std::vector<cv::Mat>().swap(source);
}

void VideoSource::Load(std::string sourcePath, std::string data)
{
	std::vector<std::string> args = SplitString(data, '`');
	this->SetName(args.at(0));
	this->path = sourcePath + args.at(1);
}

std::string VideoSource::Write(std::string defPath)
{
	return GetName() + '`' + FindDifference(defPath, path);
}

void VideoSource::UpdateCreated() {
	if (!VideoSource::videoSources.AddRecord(this).second) {
		created = false;
	}
	else {
		created = true;
		VideoSource::ReadSource();
		//std::thread readData(&VideoSource::ReadSource, this);
		//readingThreads->push_back(std::move(readData));
	}
}
int VideoSource::GetSize()
{
	return source_.size();
}

void VideoSource::ReadSource()
{
	auto lock = this->LockSource();
	std::vector<SyncObject<AVFrame*>> newSource;

	//cv::VideoCapture vidCapture(path);
	//// prints error message if the stream is invalid
	//if (!vidCapture.isOpened())
	//{
	//	wxMessageBox("Error openning video stream of file with OpenCV");
	//	return;
	//}
	//else
	//{
	//	// Obtain fps and frame count by get() method and print
	//	// You can replace 5 with CAP_PROP_FPS as well, they are enumerations
	//	this->fps = vidCapture.get(5);
	//	std::cout << "Frames per second :" << this->fps << std::endl;

	//	// Obtain frame_count using opencv built in frame count reading method
	//	// You can replace 7 with CAP_PROP_FRAME_COUNT as well, they are enumerations
	//	this->length = vidCapture.get(7); // number of frames

	//	this->resolution[0] = vidCapture.get(cv::CAP_PROP_FRAME_WIDTH);
	//	this->resolution[1] = vidCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
	//	std::cout << "  Frame count :" << this->length << std::endl;
	//}

	//vidCapture.release(); // closes the file

	// Open the file using libavformat
	AVFormatContext* av_format_ctx = avformat_alloc_context();
	if (!av_format_ctx) {
		//wxMessageBox("Couldn't create AVFormatContext\n");
		created = false;
		return;
	}
	if (avformat_open_input(&av_format_ctx, path.c_str(), NULL, NULL) != 0) {
		//wxMessageBox("Couldn't open video file\n");
		created = false;
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
		//wxMessageBox("Couldn't find valid video stream inside file\n");
		read_error = false;
		return;
	}

	// Set up a codec context for the decoder
	AVCodecContext* av_codec_ctx = avcodec_alloc_context3(av_codec);
	if (!av_codec_ctx) {
		//wxMessageBox("Couldn't create AVCpdecContext\n");
		read_error = false;
		return;
	}

	if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0)
	{
		//wxMessageBox("Couldn't initialize AVCodecContext\n");
		read_error = false;
		return;
	}
	if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0) {
		//wxMessageBox("Couldn't open codec\n");

		read_error = false;
		return;
	}

	AVFrame* av_frame = av_frame_alloc();
	if (!av_frame) {
		//wxMessageBox("Couldn't allocate AVFrame\n");

		read_error = false;
		return;
	}
	AVPacket* av_packet = av_packet_alloc();
	if (!av_packet) {
		//wxMessageBox("Couldn't allocate AVPacket\n");

		read_error = false;
		return;
	}
	int response;
	int counter = 0;
	while (av_read_frame(av_format_ctx, av_packet) >= 0 && counter < 100000) {
		if (av_packet->stream_index != video_stream_index) {
			av_packet_unref(av_packet);
			continue;
		}
		response = avcodec_send_packet(av_codec_ctx, av_packet);
		if (response < 0) {
			//wxMessageBox("Failed to decode packet: %s\n", av_err2str(response));

			read_error = false;
			return;
		}
		response = avcodec_receive_frame(av_codec_ctx, av_frame);
		if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
			av_packet_unref(av_packet);
			continue;
		}
		else if (response < 0) {
			//wxMessageBox("Failed to decode frame: %s\n", av_err2str(response));

			read_error = false;
			return;
		}
		counter++;
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
		newSource.push_back(SyncObject<AVFrame*>(av_frame_clone(av_frame)));

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
	//this->LockSource();
	source_.swap(newSource);
}

const AVFrame* VideoSource::getFirstFrame(){
	auto lock = this->LockSource();
	return source_.front().GetObject();
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

SyncObject<AVFrame*>* VideoSource::GetChunk(int idx)
{
	if (GetSize() == 0) return nullptr;
	if (idx < GetSize())
		return &source_.at(idx);
	return &source_.at(GetSize() - 1);
}

void VideoSource::Show()
{


}

//void VideoSource::Play()
//{
//	//auto start = std::chrono::high_resolution_clock::now();
//	//// Read the frames to the last frame
//	//auto sec_delay = std::chrono::milliseconds(long long(float(1 / this->fps) * 1e3));
//	//for (int frame = 0; frame < source_.size(); frame++)
//	//{
//	//	//display frames
//	//	//cv::imshow("Frame", source[frame]);
//	//	int key = cv::waitKey(1 / this->fps * 1e3 - 9);
//	//	if (key == 'q')
//	//	{
//	//		std::cout << "q key is pressed by the user. Stopping the video" << std::endl;
//	//		cv::destroyAllWindows();
//	//		return;
//	//	}
//	//}
//	//cv::destroyAllWindows();
//}
