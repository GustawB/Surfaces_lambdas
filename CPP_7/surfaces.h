#ifndef SURFACES_H
#define SURFACES_H

#include <iostream>
#include <cmath>
#include <functional>
#include <numbers> // std::numbers::pi
#include "real.h"

using std::ostream;
using std::invoke;
using std::function;
using std::abs;
using std::sin;
using std::cos;
using std::sqrt;
using PI = std::numbers::pi;

class Point
{
public:
	Point() = delete;
	Point(const Real&, const Real&); // Only constructor;

	~Point() = default;

	// Ostream operator used to print our attributes.
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
inline Surface plain()
{ 
	return [](const Point&) -> Real { return 0.0; };
}

// Surface described as x, where x is the coordinate of the
// Point passed as the lambda param.
inline Surface slope()
{
	return [](const Point& p) -> Real { return p.x; };
}

// Surface that is represented by steps.
inline Surface steps(Real s = 1.0)
{
	return [=](const Point& p) -> Real
	{
		if (s <= 0.0) { return 0.0; }
		int quotient = (p.x / s);
		quotient = (p.x < 0 && quotient * s != p.x) ? 
			quotient - 1 : quotient;
		return quotient * 1.0; // casting to Real.
	};
}

// Surface that represents a chessboard.
inline Surface checker(Real s = 1.0)
{
	return [=](const Point& p) -> Real
	{
		if (s <= 0.0) { return 0.0; }
		int x_quotient = p.x / s;
		int y_quotient = p.y / s;
		x_quotient = (p.x < 0 && x_quotient * s != p.x) ? 
			x_quotient - 1 : x_quotient;
		y_quotient = (p.y < 0 && y_quotient * s != p.y) ? 
			y_quotient - 1 : y_quotient;
		return (x_quotient % 2 == 0) ?
			((y_quotient % 2 == 0) ?
				1.0 : 0.0) :
			((y_quotient % 2 == 0) ?
				0.0 : 1.0);
	};
}

// Surface that represents x*x.
inline Surface sqr()
{
	return [](const Point& p) -> Real { return p.x * p.x; };
}

// Surface that represents sin(x).
inline Surface sin_wave()
{
	return [](const Point& p) -> Real { return sin(p.x); };
}

// Surface that represents cos(x).
inline Surface cos_wave()
{
	return [](const Point& p) -> Real { return cos(p.x); };
}

// Surface that represents rings.
inline Surface rings(Real s = 1.0)
{
	return [=](const Point& p) -> Real 
	{
		if (s <= 0.0) { return 0.0; }
		Real dist = sqrt(p.x * p.x + p.y * p.y);
		int quotient = dist / s;
		return (quotient % 2 == 0) ? 1.0 : 0.0;
	};
}

// Surface that represents ellipse.
inline Surface ellipse(Real a = 1.0, Real b = 1.0)
{
	return [=](const Point& p) -> Real
	{
		if (a <= 0.0 || b <= 0.0) { return 0.0; }
		Real first = (p.x * p.x) / (a * a);
		Real second = (p.y * p.y) / (b * b);
		return (first + second <= 1.0) ? 1.0 : 0.0;
	};
}

// Surface that represents rectangle.
inline Surface rectangle(Real a = 1.0, Real b = 1.0)
{
	return [=](const Point& p) -> Real
	{
		if (a <= 0.0 || b <= 0.0) { return 0.0; }
		return (abs(p.x) <= a && abs(p.y) <= b) ? 1.0 : 0.0;
	};
}

// Surface that represents stripes.
inline Surface stripes(Real s = 1.0)
{
	return [=](const Point& p) -> Real
	{
		if (s <= 0.0) { return 0.0; }
		int quotient = p.x / s;
		quotient = (p.x >= 0 && quotient * s != p.x) ? 
			quotient - 1 : quotient
		return (quotient % 2 == 0) ? 0.0 : 1.0;
	};
}

// Rotate the domain of the given function by the gven angle. 
inline Surface rotate(const Surface& f, Real deg)
{
	return [deg, &f](const Point& p) -> Real
	{
		Real r = Real(sqrt(p.x * p.x + p.y * p.y));
		Real cosAlpha = p.x / r;
		Real sinAlpha = p.y / r;
		Real cosBeta = cos(-deg * PI / 180.0);
		Real sinBeta = sin(-deg * PI / 180.0);
		Real sinS = sinAlpha * cosBeta + sinBeta * cosAlpha;
		Real cosS = cosAlpha * cosBeta - sinAlpha * sinBeta;
		Point p_new(r * cosS, r * sinS);
		return f(p_new);
	};
}

// Moves the domain of the given surface by the given vector (Point).
inline Surface translate(const Surface& f, const Point& pa)
{
	return [&pa, &f](const Point& pb) -> Real
	{
		Point p_new(pb.x - pa.x, pb.y - pa.y);
		return f(p_new);
	};
}

// Scales the domain of the given surface by the given vector (Point).
inline Surface scale(const Surface& f, const Point& pa)
{
	return [&pa, &f](const Point& pb) -> Real
	{
		Point p_new(pb.x / pa.x, pb.y / pa.y);
		return f(p_new);
	};
}

// Inverts the given surface (swaps x's with y's).
inline Surface invert(const Surface& f)
{
	return [&f](const Point& p) -> Real
	{
		Point p_new(p.y, p.x);
		return f(p_new); 
	};
}

// Flips the domain of the Surface (flips x to -x).
inline Surface flip(const Surface& f)
{
	return [&f](const Point& p) -> Real
	{
		Point p_new(-p.x, p.y);
		return f(p_new);
	};
}


// Multiplies the given surface by the scalar c.
inline Surface mul(const Surface& f, Real c)
{
	return [c, &f](const Point& p) -> Real 
	{ 
		return f(p) * c;
	};
}

// Adds a scalar c to the given surface.
inline Surface add(const Surface& f, Real c)
{
	return [c, &f](const Point& p) -> Real 
	{
		return f(p) + c;
	};
}

// Calls f with results of t and args function calls.
template <class F, class T, class... Args>
auto evaluate(const F& f, const T& t, const Args&... args) -> decltype(auto)
{
	return [&](const Point& p) -> Real
	{
		return f(t(p), args(p)...);
	};
}

// Non-templated overload of the compose function used
// when we received no parameter.
inline auto compose() -> decltype(auto)
{
	return[](Real r)->Real
	{
		return r;
	};
}

// Compose function specialization for one parameter.
template <class F>
auto compose(const F& f) -> decltype(auto)
{
	return[&](Real r)->Real
	{
		return f(r);
	};
}

// Function that generates function composition t_n(t_n-1...(f_2(f_1))...).
template <class F, class... T>
auto compose(const F& f, const T&... t) -> decltype(auto)
{
	return[&]<typename K>(const K& k)->Real
	{
		return compose(t...)(f(k));
	};
}

#endif

