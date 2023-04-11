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
	inline virtual ~Bezier() {};
	inline void SetCurve(std::vector<Point>);
	inline void SetCurve(std::vector<Point>, int insert);
	inline void GetValue(float t, Point& pFinal);
	template<typename Tvalue, typename std::enable_if<std::is_arithmetic<Tvalue>::value>::type* = nullptr >
	void CalculateTransitionValue(float t, Point& pFinal, Tvalue start, Tvalue end);
	inline void CurveValue(float t, Point& pFinal);

private:
	template<typename Tvalue, typename std::enable_if<std::is_arithmetic<Tvalue>::value>::type* = nullptr >
	inline float static Linear(float, Tvalue, Tvalue); // find the value by linear function between two end values
	inline void Linear(float, Point&); // find the point in linear from the curve
	inline void QuadraticCurve(float, Point&);
	inline void CubicCurve(float, Point&);
	//void GenericCurve(float, Point&);
	inline bool CurveValuesCheck(std::vector<Point>&);
};


