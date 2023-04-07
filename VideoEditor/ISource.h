#pragma once
#include<string>
#include<thread>
#include<mutex>
#include<functional>
#include<wx/msgdlg.h>
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
	static std::vector<std::thread>* readingThreads;
protected:
	std::string path;
	T source_;
	mutable std::mutex mutex_;
	virtual void ReadSource() = 0;

public:
	//Info info; // struct that contains all the clips configurations
	ISource(std::string path) {
		this->path = path;
		//std::thread readData(std::bind(&ISource::ReadSource, this, path));
		//std::thread readData(&ISource::ReadSource, this, path);
		std::thread readData([&]() {
			try {
				// Call the ReadSource function
				this->ReadSource();
			}
			catch (const std::exception& e) {
				// Handle the exception
				wxMessageBox(std::string("Exception caught in readData thread: ") + std::string(e.what()));
			}
			});
		ISource::readingThreads->push_back(std::move(readData));
		// TODO: handle object getting deleted
	}
	virtual ~ISource() {}
	inline const T& GetSource() const {
		return source_;
	}
	inline std::unique_lock<std::mutex> LockSource() {
		return std::unique_lock<std::mutex>(mutex_);
	}
};

// Initializing the static member
template <typename T>
std::vector<std::thread> * ISource<T>::readingThreads = new std::vector<std::thread>();