#include "EffectProperty.h"

template<typename T>
Keyframe<T>::Keyframe(int frame, T value) {
	this->frame = frame;
	this->value = value;
}

template<typename T>
Keyframe<T>::Keyframe(int frame, T value, Bezier transition) : Keyframe<T>::Keyframe(frame, value) {
	this->transition = transition;
}


template<typename T>
inline EffectProperty<T>::EffectProperty(T value)
{
	this->defualt = value;
}

template<typename T>
inline EffectProperty<T>::EffectProperty(const EffectProperty<T>& origin) {
	this->defualt = origin.defualt;
	this->keyframes = origin.keyframes;
}

template<typename T>
float EffectProperty<T>::GetPropValue(int frame)
{
	int prev = binarySearchKeyframe(frame);
	int next;
	if (prev == this->keyframes.size() - 1)
		next = prev;
	else
		next = prev + 1;

	return 0.0f;
}
template<typename T>
void EffectProperty<T>::setCloseKeyframe(int currFrame, T oldValue, T value)
{
	int loc = binarySearchKeyframe(currFrame);
	int i = loc;
	while (i >= 0 && i < this->keyframes.size() && this->keyframes[i].value == oldValue) {
		this->setKeyframe(value, i);
		i--;
	}
	while (loc < this->keyframes.size() && this->keyframes[loc].value == oldValue) {
		loc++;
		this->setKeyframe(value, i);
	}
}

template<typename T>
void EffectProperty<T>::setAllKeyframes(T oldValue, T value)
{
	int loc = 0;
	while (loc < this->keyframes.size())
	{
		if (this->keyframes[loc].value == oldValue)
			setKeyframe(value, loc);
	}
}

template<typename T>
void EffectProperty<T>::deleteAllKeyframes(T value)
{
	this->defualt = value;
	this->keyframes.clear();
}

template<typename T>
int EffectProperty<T>::binarySearchKeyframe(int frame)
{ // if the frame doesnt exist, returns the prev one.
	// if returns -1, there is no previous one.
	int start = 0;
	int end = this->keyframes.size();

	while (start < end) {
		int mid = (start + end) / 2;

		if (this->keyframes[mid].frame == frame) return mid;
		else if (this->keyframes[mid].frame > frame) end = mid - 1;
		else  end = mid + 1;
	}
	return end;
}

// inline

template<typename T>
inline T EffectProperty<T>::GetDefualt()
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
	this->keyframes.back().transition = { {0, 0}, {1, 1} };
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