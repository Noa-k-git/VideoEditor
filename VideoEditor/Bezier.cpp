#include "Bezier.h"

inline Point::Point() {
	this->x = 0;
	this->y = 0;
}

inline Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}


inline Point::Point(std::array<float, 2> point) {
	this->x = point[0];
	this->y = point[1];
}

inline Bezier::Bezier()
{
	curve.push_back({ 0,0 });
	curve.push_back({ 1,1 });
};

inline Bezier::Bezier(std::vector<Point> newCurve, bool insert) : Bezier::Bezier()
{
	if (insert)
		SetCurve(newCurve, 1);
	else
		SetCurve(newCurve);
};

inline void Bezier::SetCurve(std::vector<Point> newCurve)
{
	if (CurveValuesCheck(newCurve))
			this->curve = newCurve;
};

inline void Bezier::SetCurve(std::vector<Point> values, int insert)
{
	if (CurveValuesCheck(values))
		this->curve.insert(curve.begin() + insert, values.begin(), values.end());
}
inline void Bezier::GetValue(float t, Point& pFinal)
{
	// Add multiplation of curve value and linear value
	
}
;

inline void Bezier::CurveValue(float t, Point& pFinal)
{
	switch (curve.size()) {
	case 2:
		Linear(t, pFinal);
		break;
	case 3:
		QuadraticCurve(t, pFinal);
		break;
	case 4:
		CubicCurve(t, pFinal);
		break;
	default:
		break;
	}

};

inline void Bezier::Linear(float t, Point& pFinal)
{
	pFinal.x = abs(this->curve[0].x - this->curve[1].x) * t;
	pFinal.y = abs(this->curve[0].y - this->curve[1].y) * t;
}

inline void Bezier::QuadraticCurve(float t, Point& pFinal)
{
	float s = 1 - t;
	// By the formula
	pFinal.x = pFinal.x * // the linear base
		pow(s, 2) * curve[0].x +
		s * 2 * t * curve[1].x +
		t * t * curve[2].x;
	pFinal.y = pFinal.y * // the linear base
		pow(s, 2) * curve[0].y +
		s * 2 * t * curve[1].y +
		t * t * curve[2].y;
}

inline void Bezier::CubicCurve(float t, Point& pFinal)
{
	float s = 1 - t;
	// By Bezier curve formula
	pFinal.x = pow(s, 3) * curve[0].x + 
		3 * pow(s, 2) * t * curve[1].x + 
		3 * s * pow(t, 2) * curve[2].x + 
		pow(t, 3) * curve[3].x;

	pFinal.x = pow((1 - t), 3) * curve[0].y +
		3 * pow((1 - t), 2) * t * curve[1].y + 
		3 * (1 - t) * pow(t, 2) * curve[2].y + 
		pow(t, 3) * curve[3].y;
}
/*
void Bezier::GenericCurve(float, Point&)
{
	Point p0, p1, mid;
	std::vector<Point> ;
	for (int i = 1; i < curve.size() - 2; i++) {
		p0 = curve[i];
		p1 = curve[i + 1];
		mid.x = (p0.x + p1.x) / 2;
		mid.y = (p0.y + p1.y) / 2;
	}
};*/

inline bool Bezier::CurveValuesCheck(std::vector<Point>&)
{
	return true;
};
