#include "Bezier.h"

Bezier::Bezier()
{
	curve.fill({ .5f,.5f });
};

Bezier::Bezier(std::array<std::array<float, 2>, 2> newCurve) : Bezier::Bezier()
{
	SetCurve(newCurve);
};

void Bezier::SetCurve(std::array<std::array<float, 2>, 2> newCurve)
{
	if (CurveValuesCheck(newCurve))
		this->curve = newCurve;
};

float Bezier::cubicCurve(float t)
{
	float P0 = this->curve[0][0];
	return pow((1 - t), 3) * P0 + 3 * pow((1 - t), 2) * t * P1 + 3 * (1 - t) * pow(t, 2) * P2 + pow(t, 3) * P3;
};

inline bool Bezier::CurveValuesCheck(std::array<std::array<float, 2>, 2>&)
{
	return true;
};
