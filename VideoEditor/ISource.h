#pragma once
#include<string>
#include<thread>
#include<functional>
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
		this->path = path;
		std::thread readData(std::bind(&ISource::ReadSource, this, path));
		readData.detach();
		// TODO: handle object getting deleted
		/*
		if (readData.joinable())
			try {
			readData.join();
			}
			catch (const std::exception& e) {
				std::string z = e.what();
				//std::cout << "Error: " << e.what() << std::endl;
				auto a = 1;
			}
			//readData.join();
			//readData.detach();
	*/
	}

	virtual ~ISource() {}
private:
	virtual void ReadSource(std::string) = 0;

};