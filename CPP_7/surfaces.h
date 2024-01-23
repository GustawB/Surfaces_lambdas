#ifndef SURFACES_H
#define SURFACES_H

#include <iostream>
#include "real.h"

using std::ostream;

class Point
{
private:
	Real point_x;
	Real point_y;
public:
	Point() = delete;
	Point(const Real&, const Real&); // Only constructor;

	~Point() = default;

	friend ostream& operator<<(ostream&, const Point&);
};

inline Point::Point(const Real& a, const Real& b)
{
	point_x = a;
	point_y = b;
}

inline ostream& operator<<(ostream& os, const Point& p)
{
	os << p.point_x << " " << p.point_y;
	return os;
}

#endif

