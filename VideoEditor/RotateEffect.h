#pragma once
#include "IEffect.h"

enum Rotation {
	UP = 0,
	RIGHT = 90,
	UPDOWN = 180,
	LEFT = 270
};

class RotateEffect : public IEffect {
public:
	RotateEffect();
	void RotateLeft();
	void RotateRight();

	void Apply(AVFrame*&) override;
private:
	Rotation rotate;


};