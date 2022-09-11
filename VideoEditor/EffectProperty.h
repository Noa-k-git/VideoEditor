#pragma once
#include "SourceClip.h"

#include <array>

using std::array;
template <typename T>
struct Keyframe {
		int frame;
		T value;
		array<array<float, 2>, 2> bezierCurve;
	};
//TODO: add func for changing keyframe frame, transition

template <typename T> class EffectProperty
{
private:
	T defualt;
	vector<Keyframe<T>> keyframes;
public:
	EffectProperty(T);
	
	inline T getDefualt();
	inline const vector<Keyframe<T>>& getKeyframes() const;

	inline void newKeyframe(Keyframe<T>); // Generate a new keyframe
	inline void newKeyframe(int frame, T value); // Generate a new keyframe

	inline void setKeyframe(T, int loc = -1); // replace an existing keyframe with a new one
	inline void setKeyframe(Keyframe<T>); // replace an existing keyframe with a new one
	
	void setCloseKeyframe(int frame, T oldValue, T value); /* recieves a current frame, oldvalue and new one,
															and changes the close keyframes with the same old value to the new one.*/
	void setAllKeyframes(T oldValue, T value); // changes all keyframes with the oldValue to the new one

	void deleteAllKeyframes(T value);
private:
	inline void sortKeyframes();
	int binarySearchKeyframe(int frame);
	inline void newKeyframe();
};
