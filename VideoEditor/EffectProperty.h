#pragma once
#include <array>
#include<vector>
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
template<typename T>
inline T EffectProperty<T>::getDefualt()
{
	return this->defualt;
}

template<typename T>
inline const std::vector<Keyframe<T>>& EffectProperty<T>::getKeyframes() const
{
	return this->keyframes;
}

template<typename T>
inline void EffectProperty<T>::newKeyframe()
{
	this->keyframes.back().bezierCurve = { {0, 0}, {1, 1} };
	this->sortKeyframes();
}


template<typename T>
inline void EffectProperty<T>::newKeyframe(Keyframe<T> kf)
{
	this->keyframes.push_back(kf);
	this->newKeyframe();
}

template<typename T>
inline void EffectProperty<T>::newKeyframe(int frame, T value)
{
	this->keyframes.push_back({ frame, value });
	this->newKeyframe();
}

template<typename T>
inline void EffectProperty<T>::setKeyframe(T value, int loc)
{
	if (loc >= 0 && loc < this->keyframes.size()) {
		this->keyframes[loc].value = value;
	}
}

template<typename T>
inline void EffectProperty<T>::setKeyframe(Keyframe<T> kf)
{
	int loc = binarySearchKeyframe(kf.frame);
	if (loc >= 0 && loc < this->keyframes.size()) {
		this->keyframes[loc] = kf;
	}
}

template<typename T>
inline void EffectProperty<T>::sortKeyframes()
{
	sort(this->keyframes.begin(), this->keyframes.end(), [](Keyframe a, Keyframe b) {
		return a.frame < b.frame;
		});
}