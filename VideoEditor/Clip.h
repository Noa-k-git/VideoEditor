#include "SourceClip.h"
#include "Effect.h"

using std::string;

struct Limits {
	int start; // the starting frame of the clip
	int end; // the last frame of the clip
};


class Clip
{
private:
	Limits limits; // struct that contains all the clips configurations
public:
	SourceClip *clipPtr; // the source clip
	
public:
	vector<Effect> effects;

public:
	Clip(SourceClip&);

	vector<Mat>& getClip(); // returns the clip from its starting frame to its ending frame

	void setStartFrame(int); // sets the clip starting frame
	void setEndFrame(int); // sets the clip last frame

};

