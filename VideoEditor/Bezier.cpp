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

float Bezier::cubicCurve(float x)
{
	// p0 = (0,0), p3 = (1,1)
	// pow((1 - x), 3) * P0 + 3 * pow((1 - x), 2) * x * P1 + 3 * (1 - x)  * pow(x, 2) * P2 + pow(x, 3) * P3;
	// 3 * pow((1 - x), 2) * x * P1 + 3 * (1 - x) * pow(x, 2) * P2 + pow(x, 3);
	

	// https://math.stackexchange.com/questions/1178232/calculating-values-for-cubic-bezier-curve
	// https://sciencing.com/solve-cubic-equations-8136094.html

	float x0, x1, x2, x3;
	float y0, y1, y2, y3;
	x0, x1, x2, x3 = 0, curve[0][0], curve[1][0], 1;
	y0, y1, y2, y3 = 0, curve[0][1], curve[1][0], 1;
	
	float a = x0 + 3 * x1 - 3 * x2 + x3;
	float b = -3 * x0 - 6 * x1 + 3 * x3;
	float c = 3 * x0 - 3 * x1;
	float d = (-1) * x0;

	float p = (-1 * b) / (3 * a);
	float q = pow(p, 3) + (b * c - 3 * a * d) / (6 * a * a);
	float r = c / (3 * a);

	float t = pow(q + pow(q * q + pow(r - p * p, 3), 1 / 2), 1 / 3) +
		pow(q - pow(q * q + pow(r - p * p, 3), 1 / 2), 1 / 3) + p;
	
	return pow((1 - t), 3) * y0 + 3 * pow((1 - t), 2) * t * y1 + 3 * (1 - t) * pow(t, 2) * y2 + pow(t, 3) * y3;
};

inline bool Bezier::CurveValuesCheck(std::array<std::array<float, 2>, 2>&)
{
	return true;
};
