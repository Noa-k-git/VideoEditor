#pragma once
#include<string>
#include<thread>
/*
struct Info {
	double fps; // clip's frames per seconds
	int framesNum; // number of frames in the source clip
	int resulusion[2]; // width, hight.
	std::string type;
};*/
template <typename T>
class ISource {
public:
	//Info info; // struct that contains all the clips configurations
	std::string path;
	T source;
	ISource(std::string path) {
		this->path = path;/*
		std::thread readData(&ISource::ReadSource, this, path);
		// TODO: handle object getting deleted
		readData.detach();*/
	}
	virtual ~ISource() {}
private:
	virtual void ReadSource(std::string) = 0;

};