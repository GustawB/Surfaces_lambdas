#ifndef SURFACES_H
#define SURFACES_H

#include <iostream>
#include "real.h"

using std::ostream;

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

class Surface
{
public:
	Surface();
	Surface(const Real&, const Real&);

protected:
	Real point_x;
	Real point_y;
};

inline Surface::Surface()
{
	point_x = 0.0;
	point_y = 0.0;
}

inline Surface::Surface(const Real& x, const Real& y)
{
	point_x = x;
	point_y = y;
}

class plain : public Surface
{
public:
	plain();
	plain(const Real&, const Real&);
	Real operator()(const Point& x)
	{
		return 0.0;
	}
};

inline plain::plain() : Surface()
{}

inline plain::plain(const Real& x, const Real& y) : Surface(x, y)
{}

#endif

