#ifndef SURFACES_H
#define SURFACES_H

#include <iostream>
#include <cmath>
#include <functional>
#include "real.h"

using std::ostream;
using std::invoke;
using std::function;
using std::abs;
using std::sin;
using std::cos;

class Point
{
public:
	Point() = delete;
	Point(const Real&, const Real&); // Only constructor;

	~Point() = default;

	friend ostream& operator<<(ostream&, const Point&);

	const Real x;
	const Real y;
};

inline Point::Point(const Real& a, const Real& b) : x(a), y(b)
{}

inline ostream& operator<<(ostream& os, const Point& p)
{
	os << p.x << " " << p.y;
	return os;
}

using Surface = function<Real(const Point&)>;

// Surface described as 0.0;
Surface plain()
{ 
	return [](const Point&) -> Real { return 0.0; };
}

// Surface described as x, where x is the coordinate of the
// Point passed as the lambda param.
Surface slope()
{
	return [](const Point& p) -> Real { return p.x; };
}

// Surface that is represented by steps.
Surface steps(const Real& s = 1.0)
{
	return [&](const Point& p) -> Real
	{
		if (s <= 0.0) { return 0.0; }
		int quotient = abs(p.x) / s;
		quotient = (p.x < 0.0) ? -quotient : quotient;
		Real result = quotient * 1.0;
		return result;
	};
}

// Surface that represents a chessboard.
Surface checker(const Real& s = 1.0)
{
	return [&](const Point& p) -> Real
	{
		if (s <= 0.0) { return 0.0; }
		int x_quotient = abs(p.x) / s;
		int y_quotient = abs(p.y) / s;
		return (x_quotient % 2 == 0) ?
			((y_quotient % 2 == 0) ?
				1.0 : 0.0) :
			((y_quotient % 2 == 0) ?
				0.0 : 1.0);
	};
}

// Surface that represents x*x.
Surface sqr()
{
	return [](const Point& p) -> Real { return p.x * p.x; };
}

// Surface that represents sin(x).
Surface sin_wave()
{
	return [](const Point& p) -> Real { return sin(p.x); };
}

// Surface that represents cos(x).
Surface cos_wave()
{
	return [](const Point& p) -> Real { return cos(p.x); };
}

// Surface that represents rings.
Surface rings(const Real& s = 1.0)
{
	return [&](const Point& p) -> Real 
	{
		if (s <= 0.0) { return 0.0; }
		int x_quotient = abs(p.x) / s;
		int y_quotient = abs(p.y) / s;
		x_quotient = (s * x_quotient == abs(p.y)) ? x_quotient - 1 : x_quotient;
		x_quotient = (p.x < 0.0) ? -x_quotient : x_quotient;
		y_quotient = (s * y_quotient == abs(p.y)) ? y_quotient - 1 : y_quotient;
		y_quotient = (p.x < 0.0) ? -y_quotient : y_quotient;
		return (x_quotient % 2 == y_quotient % 2) ?
			((x_quotient % 2 == 0) ? 1.0 : 0.0) : 0.0;
	};
}

// Surface that represents ellipse.
Surface ellipse(const Real& a = 1.0, const Real& b = 1.0)
{
	return [&](const Point& p) -> Real
	{
		if (a <= 0.0 || b <= 0.0) { return 0.0; }
		Real first = p.x * p.x / a * a;
		Real second = p.y * p.y / b * b;
		return (first + second <= 1.0) ? 1.0 : 0.0;
	};
}

// Surface that represents rectangle.
Surface rectangle(const Real& a = 1.0, const Real& b = 1.0)
{
	return [&](const Point& p) -> Real
	{
		if (a <= 0.0 || b <= 0.0) { return 0.0; }
		return (abs(p.x) <= a && abs(p.y) <= b) ? 1.0 : 0.0;
	};
}

// Surface that represents stripes.
Surface stripes(const Real& s = 1.0)
{
	return [&](const Point& p) -> Real
	{
		if (s <= 0.0) { return 0.0; }
		int quotient = abs(p.x) / s;
		quotient = (quotient * s == abs(p.x)) ? quotient - 1 : quotient;
		return (quotient % 2 == 0) ? 1.0 : 0.0;
	};
}

// Rotate the domain of the given function by the gven angle. 
Surface rotate(Surface&& f, Real deg)
{
	return [deg, &f](const Point& p) -> Real
	{
		Point p_new(p.x * cos(deg) - p.y * sin(deg),
			p.x * sin(deg) + p.y * cos(deg));
		return f(p_new);
	};
}

// Moves the domain of the given surface by the given vector (Point).
Surface translate(Surface&& f, const Point& pa)
{
	return [&pa, &f](const Point& pb) -> Real
	{
		Point p_new(pb.x + pa.x, pb.y + pa.y);
		return f(p_new);
	};
}

// Scales the domain of the given surface by the given vector (Point).
Surface scale(Surface&& f, const Point& pa)
{
	return [&pa, &f](const Point& pb) -> Real 
	{
		Point p_new(pb.x * pa.x, pb.y * pa.y);
		return f(p_new);
	};
}

// Inverts the given surface (swaps x's with y's).
Surface invert(Surface&& f)
{
	return [&f](const Point& p) -> Real 
	{
		Point p_new(p.y, p.x);
		return f(p_new); 
	};
}

// Flips the domain of the Surface (flips x to -x).
Surface flip(Surface&& f)
{
	return [&f](const Point& p) -> Real 
	{
		Point p_new(-p.x, p.y);
		return f(p_new);
	};
}


// Multiplies the given surface by the scalar c.
Surface mul(Surface&& f, Real c)
{
	return [c, &f](const Point& p) -> Real { return f(p) * c; };
}

// Adds a scalar c to the given surface.
Surface add(Surface&& f, Real c)
{
	return [c, &f](const Point& p) -> Real { return f(p) + c; };
}

template <class F, class... Args>
auto evaluate(F&& f, Surface&& a, Args&&... args) -> decltype(auto)
{
	return [&](const Point& p) -> Real
	{
		return f(a(p), a(args)...);
	};
}

auto compose() -> decltype(auto)
{
	return[&](Real r)->Real
	{
		return r;
	};
}

template <class F>
auto compose(F&& f) -> decltype(auto)
{
	return[&](Real r)->Real
	{
		return f(r);
	};
}

template <class F, class... T>
auto compose(F&& f, T&&... t) -> decltype(auto)
{
	return[&](Real r)->Real
	{
		return f(compose(t...)(r));
	};
}

#endif

