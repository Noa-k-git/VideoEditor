#include "EffectProperty.h"

template<typename Tvalue>
Keyframe<Tvalue>::Keyframe(int frame, Tvalue value) {
	this->frame = frame;
	this->value = value;
}

template<typename Tvalue>
Keyframe<Tvalue>::Keyframe(int frame, Tvalue value, Bezier transition) : Keyframe<Tvalue>::Keyframe(frame, value) {
	this->transition = transition;
}


template<typename Tvalue>
inline EffectProperty<Tvalue>::EffectProperty(Tvalue value)
{
	this->defualt = value;
}

template<typename Tvalue>
inline EffectProperty<Tvalue>::EffectProperty(const EffectProperty<Tvalue>& origin) {
	this->defualt = origin.defualt;
	this->keyframes = origin.keyframes;
}

template<typename Tvalue>
float EffectProperty<Tvalue>::GetPropValue(int frame)
{
	int prev = binarySearchKeyframe(frame);
	int next;
	if (prev == this->keyframes.size() - 1)
		next = prev;
	else
		next = prev + 1;

	float transitionState = next; // float value of y between two frames

	return 0.0f;
}
template<typename Tvalue>
void EffectProperty<Tvalue>::setCloseKeyframe(int currFrame, Tvalue oldValue, Tvalue value)
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

template<typename Tvalue>
void EffectProperty<Tvalue>::setAllKeyframes(Tvalue oldValue, Tvalue value)
{
	int loc = 0;
	while (loc < this->keyframes.size())
	{
		if (this->keyframes[loc].value == oldValue)
			setKeyframe(value, loc);
	}
}

template<typename Tvalue>
void EffectProperty<Tvalue>::deleteAllKeyframes(Tvalue value)
{
	this->defualt = value;
	this->keyframes.clear();
}

template<typename Tvalue>
int EffectProperty<Tvalue>::binarySearchKeyframe(int frame)
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

template<typename Tvalue>
inline Tvalue EffectProperty<Tvalue>::GetDefualt()
{
	return this->defualt;
}


template<typename Tvalue>
inline const std::vector<Keyframe<Tvalue>>& EffectProperty<Tvalue>::getKeyframes() const
{
	return this->keyframes;
}

template<typename Tvalue>
inline void EffectProperty<Tvalue>::newKeyframe()
{
	this->keyframes.back().transition = { {0, 0}, {1, 1} };
	this->sortKeyframes();
}


template<typename Tvalue>
inline void EffectProperty<Tvalue>::newKeyframe(Keyframe<Tvalue> kf)
{
	this->keyframes.push_back(kf);
	this->newKeyframe();
}

template<typename Tvalue>
inline void EffectProperty<Tvalue>::newKeyframe(int frame, Tvalue value)
{
	this->keyframes.push_back({ frame, value });
	this->newKeyframe();
}

template<typename Tvalue>
inline void EffectProperty<Tvalue>::setKeyframe(Tvalue value, int loc)
{
	if (loc >= 0 && loc < this->keyframes.size()) {
		this->keyframes[loc].value = value;
	}
}

template<typename Tvalue>
inline void EffectProperty<Tvalue>::setKeyframe(Keyframe<Tvalue> kf)
{
	int loc = binarySearchKeyframe(kf.frame);
	if (loc >= 0 && loc < this->keyframes.size()) {
		this->keyframes[loc] = kf;
	}
}

template<typename Tvalue>
inline void EffectProperty<Tvalue>::sortKeyframes()
{
	sort(this->keyframes.begin(), this->keyframes.end(), [](Keyframe a, Keyframe b) {
		return a.frame < b.frame;
		});
}