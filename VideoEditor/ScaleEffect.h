#pragma once
#include "IEffect.h"
#include "EffectProperty.h"

class ScaleEffect :
    public IEffect
{
private:
    EffectProperty<double> scale;
public:
    void ApplyEffect(AVFrame& src, AVFrame& otp, int frame); //override;
};
