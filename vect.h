/*
Copyright (C) 2011 Eric Chan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef __VECT__
#define __VECT__
#include <cmath>

class vect {
public:
	double x,y,z;

	inline vect(const double fx, const double fy, const double fz = 0) : x(fx), y(fy), z(fz) {};
	inline vect(const double f = 0) : x(f), y(f), z(f) {};
	inline vect(const vect& f) : x(f.x), y(f.y), z(f.z) {};
	inline vect& operator = (const vect& f) { x = f.x; y = f.y; z = f.z; return *this; };

	inline operator double* () { return &x; };
	inline operator const double* () const { return &x; };
	
	inline vect operator - () const { return vect(-x, -y, -z); };
	
	inline bool operator == (const vect& f) const { return (x == f.x && y == f.y && z == f.z); };
	inline bool operator != (const vect& f) const { return (x != f.x || y != f.y || z != f.z); };

	inline vect operator + (const vect& f) const { return vect(x + f.x, y + f.y, z + f.z); };
	inline vect& operator += (const vect& f) { x += f.x; y += f.y; z += f.z; return *this; };
	inline vect operator - (const vect& f) const { return vect(x - f.x, y - f.y, z - f.z); };
	inline vect& operator -= (const vect& f) { x -= f.x; y -= f.y; z -= f.z; return *this; };
	inline vect operator * (const vect& f) const { return vect(x * f.x, y * f.y, z * f.z); };
	inline vect operator * (const double f) const { return vect(x * f, y * f, z * f); };
	inline vect& operator *= (const vect& f) { x *= f.x; y *= f.y; z *= f.z; return *this; };
	inline vect& operator *= (const double f) { x *= f; y *= f; z *= f; return *this; };
	inline vect operator / (const double f) const { return vect(x / f, y / f, z / f); };
	inline vect& operator /= (const double f) { x /= f; y /= f; z /= f; return *this; };
	
	static const vect ZERO;
	static const vect ONE;
	static const vect UP;
	static const vect DOWN;
	static const vect FORWARD;
	static const vect BACKWARD;
	static const vect LEFT;
	static const vect RIGHT;
	
	inline double length() const { return sqrt(x * x + y * y + z * z); }
	inline double lengthSquared() const { return x * x + y * y + z * z; };
	inline double distance(const vect& f) const { return (*this - f).length(); };
	inline double distanceSquared(const vect& f) const { return (*this - f).lengthSquared(); };
	
	inline vect normalized() const { return *this / this->length(); };
	inline vect& normalize() { return *this /= this->length(); };
	
	inline double dot(const vect& f) const { return x * f.x + y * f.y + z * f.z; };
	inline vect cross(const vect& f) const { return vect(y * f.z - z * f.y, z * f.x - x * f.z, x * f.y - y * f.x); };
	
	inline double angleBetween(const vect& f) const { return acos(this->dot(f) / sqrt(this->lengthSquared() * f.lengthSquared())); };
	inline vect reflect(const vect& f) const { return *this - f * 2 * this->dot(f) / f.lengthSquared(); };
	
	inline vect clamp(const vect& vmin, const vect& vmax) const {
		return vect(
			 (x < vmin.x)?vmin.x:(x > vmax.x)?vmax.x:x,
			 (y < vmin.y)?vmin.y:(y > vmax.y)?vmax.y:y,
			 (z < vmin.z)?vmin.z:(z > vmax.z)?vmax.z:z);
	}

	inline vect lerp(const vect& f, double r) { return *this * (1 - r) + f * r; }
};

#endif
