#pragma once
#include <array>
#include <vector>
#include <cmath>

struct Point {
	float x;
	float y;

	inline Point();
	inline Point(float x, float y);
	Point(std::array<float, 2>);
	~Point() {};
};

class Bezier
{
private:
	std::vector<Point> curve; // Points for bezier curve calculation

public:
	inline Bezier();
	inline Bezier(std::vector<Point>, bool);
	inline void SetCurve(std::vector<Point>);
	inline void SetCurve(std::vector<Point>, int insert);
	inline void CurveValue(float t, Point& pFinal);
private:
	inline void QuadraticCurve(float, Point&);
	inline void CubicCurve(float, Point&);
	//void GenericCurve(float, Point&);
	inline bool CurveValuesCheck(std::vector<Point>&);
};

