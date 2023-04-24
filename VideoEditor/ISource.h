#pragma once
#include<string>
#include<thread>
#include<mutex>
#include<functional>
#include<wx/msgdlg.h>
#include"IPLayable.h"
#include "SyncObject.h"
/*
struct Info {
	double fps; // clip's frames per seconds
	int framesNum; // number of frames in the source clip
	int resulusion[2]; // width, hight.
	std::string type;
};*/
template <typename T>
class ISource : public IPlayable<T>{
public:
	//static std::vector<std::thread>* readingThreads;

private:
	std::string ExtractName(std::string path);
protected:
	std::string path;
	std::vector<SyncObject<T>> source_;
	mutable std::mutex mutex_;
	virtual void ReadSource() = 0;

public:
	//Info info; // struct that contains all the clips configurations
	ISource(std::string, std::string);
	ISource(std::string);
	virtual ~ISource() {}
	inline std::vector<SyncObject<T>>& GetSource() {
		return source_;
	}
	inline std::unique_lock<std::mutex> LockSource() {
		return std::unique_lock<std::mutex>(mutex_);
	}
};

// Initializing the static member
//template <typename T>
//std::vector<std::thread> * ISource<T>::readingThreads = new std::vector<std::thread>();

// Implementation of none pure virtual mehtods

template <typename T>
ISource<T>::ISource(std::string path, std::string name) : IPlayable<T>(name) {
	this->path = path;
	//std::thread readData(std::bind(&ISource::ReadSource, this, path));
	//std::thread readData(&ISource::ReadSource, this, path);

	// TODO: handle object getting deleted
}
template <typename T>
ISource<T>::ISource(std::string path) : ISource(path, ExtractName(path)) {

}


template<typename T>
inline std::string ISource<T>::ExtractName(std::string path)
{
	std::vector<std::string> filePath;
	std::istringstream ss(path);
	std::string part;

	while (std::getline(ss, part, '\\')) {
		filePath.push_back(part);
	}
	return filePath.back();
}