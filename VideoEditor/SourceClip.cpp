#include "SourceClip.h"

#include<opencv2/opencv.hpp>
#include<iostream>
#include<cmath>
#include<vector>
#include <sstream>;
#include<chrono>;
#include<thread>;

using namespace std;
using namespace std::chrono;
using namespace cv;

SourceClip::SourceClip(string path) {
	VideoCapture vidCapture(path);

	// Print error message if the stream is invalid
	if (!vidCapture.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
	}

	else
	{
		// Obtain fps and frame count by get() method and print
		// You can replace 5 with CAP_PROP_FPS as well, they are enumerations
		info.fps = vidCapture.get(5);
		cout << "Frames per second :" << info.fps << endl;

		// Obtain frame_count using opencv built in frame count reading method
		// You can replace 7 with CAP_PROP_FRAME_COUNT as well, they are enumerations
		info.framesNum = vidCapture.get(7);

		info.resulusion[0] = vidCapture.get(cv::CAP_PROP_FRAME_WIDTH);
		info.resulusion[1] = vidCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
		cout << "  Frame count :" << info.framesNum << endl;
	}

	Mat blackImage(info.resulusion[0], info.resulusion[1], CV_8UC3, cv::Scalar(0, 0, 0));
	clip = vector<Mat>(info.framesNum, blackImage);

	std::thread read_clip(&ReadClip, vidCapture);
}

void SourceClip::ReadClip(VideoCapture& vidCapture) {
	while (vidCapture.isOpened())
	{
		// Initialise frame matrix
		Mat frame;

		// Initialize a boolean to check if frames are there or not
		bool isSuccess = vidCapture.read(frame);


		// Unhandled exception at 0x00007FFA6A4E4FD9 in VideoEditor1.exe: Microsoft C++ exception: cv::Exception at memory location 0x0000004F7E2FE890.

		// cout << cell;
		if (isSuccess) {
			if (clip.size() == clip.max_size()) {
				cout << "Video is too big";
			}
			clip.push_back(frame);
		}
		//cout << delay << endl;

		// If frames are not there, close it
		if (!isSuccess)
		{
			cout << "Video camera is disconnected" << endl;
			break;
		}
	}
	vidCapture.release();
};


void SourceClip::ShowSource() {
	auto start = high_resolution_clock::now();
	// Read the frames to the last frame
	auto sec_delay = milliseconds(long long(float(1 / info.fps) * 1e3));
	for (int frame = 0; frame < clip.size(); frame++)
	{
		//display frames
		imshow("Frame", clip[frame]);
		int key = waitKey(1 / info.fps * 1e3 - 9);
		if (key == 'q')
		{
			cout << "q key is pressed by the user. Stopping the video" << endl;
			destroyAllWindows();
			return;
		}
	}
	cv::destroyAllWindows();
}
inline const Info SourceClip::getInfo() const
{
	return this->info;
}
inline vector<Mat>& SourceClip::GetSource()
{
	return this->clip;
}
;

