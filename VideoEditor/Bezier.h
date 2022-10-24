#include <map>
#include <string>
#include <array>
#include <cmath>

using std::map;
using std::string;
#pragma once
class Bezier
{
private:
	std::array<std::array<float, 2>, 2> curve;
public:
	Bezier();
	Bezier(std::array<std::array<float, 2>, 2>);
	void SetCurve(std::array<std::array<float, 2>, 2> curve);
	float static cubicCurve(float);
private:
	inline bool CurveValuesCheck(std::array<std::array<float, 2>, 2>&);
};

