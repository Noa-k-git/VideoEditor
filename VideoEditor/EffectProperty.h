#pragma once
#include<vector>
#include "Bezier.h"

template <typename Tvalue>
struct Keyframe {
		int frame;
		Tvalue value;
		Bezier transition; // between prev and current

		Keyframe(int frame, Tvalue value);
		Keyframe(int frame, Tvalue value, Bezier transition);
	};
//TODO: add func for changing keyframe frame, transition
//TODO: add a func that return the value of the iplimintation number by frame

template <typename Tvalue> 
class EffectProperty
{
private:
	Tvalue defualt;
	std::vector<Keyframe<Tvalue>> keyframes;
public:
	inline EffectProperty(Tvalue);
	inline EffectProperty(const EffectProperty<Tvalue>&);

	inline Tvalue GetDefualt();
	float GetPropValue(int frame);
	inline const std::vector<Keyframe<Tvalue>>& getKeyframes() const;

	inline void newKeyframe(Keyframe<Tvalue>); // Generate a new keyframe
	inline void newKeyframe(int frame, Tvalue value); // Generate a new keyframe

	inline void setKeyframe(Tvalue, int loc = -1); // replace an existing keyframe with a new one
	inline void setKeyframe(Keyframe<Tvalue>); // replace an existing keyframe with a new one
	
	void setCloseKeyframe(int frame, Tvalue oldValue, Tvalue value); /* recieves a current frame, oldvalue and new one,
															and changes the close keyframes with the same old value to the new one.*/
	void setAllKeyframes(Tvalue oldValue, Tvalue value); // changes all keyframes with the oldValue to the new one

	void deleteAllKeyframes(Tvalue value);
	
private:
	inline void sortKeyframes();
	inline void newKeyframe();
	int binarySearchKeyframe(int frame);
};

// --- inline functions ---


