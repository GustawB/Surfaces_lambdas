#ifndef SURFACES_H
#define SURFACES_H

#include <iostream>
#include <functional>
#include "real.h"

using std::ostream;
using std::invoke;
using std::function;

class Point
{
public:
	Point() = delete;
	Point(const Real&, const Real&); // Only constructor;

	~Point() = default;

	friend ostream& operator<<(ostream&, const Point&);

	Real x;
	Real y;
};

inline Point::Point(const Real& a, const Real& b)
{
	x = a;
	y = b;
}

inline ostream& operator<<(ostream& os, const Point& p)
{
	os << p.x << " " << p.y;
	return os;
}

using Surface = function<Real(const Point&)>;

Surface plain()
{ 
	return [](const Point&) -> Real { return 0.0; };
}

Surface add(Surface f, Real c)
{
	return [c, &f](const Point& p) -> Real { return f(p) + c; };
}

#endif

