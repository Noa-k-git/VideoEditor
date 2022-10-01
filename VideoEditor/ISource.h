#pragma once
#include<string>

using std::string;

struct Info {
	double fps; // clip's frames per seconds
	int framesNum; // number of frames in the source clip
	int resulusion[2]; // width, hight.
	string type;
};

class ISource {
private:
	Info info; // struct that contains all the clips configurations

	virtual ~ISource() {}

	virtual void ShowSource() = 0;

};