#pragma once
#include<vector>
#include "Bezier.h"

template <typename T>
struct Keyframe {
		int frame;
		T value;
		Bezier transition; // between prev and current

		Keyframe(int frame, T value);
		Keyframe(int frame, T value, Bezier transition);
	};
//TODO: add func for changing keyframe frame, transition
//TODO: add a func that return the value of the iplimintation number by frame

template <typename T> 
class EffectProperty
{
private:
	T defualt;
	std::vector<Keyframe<T>> keyframes;
public:
	inline EffectProperty(T);
	inline EffectProperty(const EffectProperty<T>&);

	inline T GetDefualt();
	float GetPropValue(int frame);
	inline const std::vector<Keyframe<T>>& getKeyframes() const;

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
	inline void newKeyframe();
	int binarySearchKeyframe(int frame);
};

// --- inline functions ---


