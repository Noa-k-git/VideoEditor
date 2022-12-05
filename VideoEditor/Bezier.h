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
	/* 
	each cubic curve has 4 points, 
	only two of them can be set by the user, the other are:
	(0,0) and (1,1).
	
	the array contains 2 points
	
	https://math.stackexchange.com/questions/4097105/find-value-y-on-cubic-b%c3%a9zier-given-value-x
	
	https://math.stackexchange.com/questions/26846/is-there-an-explicit-form-for-cubic-b%C3%A9zier-curves?noredirect=1&lq=1
	https://math.stackexchange.com/questions/2571471/understanding-of-cubic-b%C3%A9zier-curves-in-one-dimension
	https://math.stackexchange.com/questions/2127059/finding-y-coordinate-given-x-coordinate-of-cubic-bezier-curve-under-restrict?rq=1
	*/
	std::array<std::array<float, 2>, 2> curve;
public:
	Bezier();
	Bezier(std::array<std::array<float, 2>, 2>);
	void SetCurve(std::array<std::array<float, 2>, 2> curve);
	float cubicCurve(float);
private:
	inline bool CurveValuesCheck(std::array<std::array<float, 2>, 2>&);
};

