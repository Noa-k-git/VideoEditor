#include "EffectProperty.h"

template<typename T>
EffectProperty<T>::EffectProperty(T value)
{
	this->defualt = value;
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

