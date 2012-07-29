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

#ifndef __QUAT__
#define __QUAT__
#include <cmath>

class vect;
class mat4;

class quat {
public:
	double x,y,z,w;
	inline quat(const double fw = 1, const double fx = 0, const double fy = 0, const double fz = 0) : x(fx), y(fy), z(fz), w(fw) {};
	inline quat(const quat& f) : w(f.w), x(f.x), y(f.y), z(f.z) {};
	
	inline double * ptr() { return &x; };
	inline const double * ptr() const { return &x; };
	inline double& operator [] (const int i) { return *(double*)((&w)+(i%4)); };
	inline double operator [] (const int i) const { return (&x)[i%4]; };
	
	inline bool operator == (const quat& f) const { return (w == f.w && x == f.x && y == f.y && z == f.z); };
	inline bool operator != (const quat& f) const { return (w != f.w || x != f.x || y != f.y || z != f.z); };

	inline quat& operator = (const quat& f) { w = f.w; x = f.x; y = f.y; z = f.z; return *this; };
	
	inline quat operator + (const quat& f) const { return quat(w + f.w, x + f.x, y + f.y); };
	inline quat& operator += (const quat& f) { w += f.w; x += f.x; y += f.y; z += f.z; return *this; };
	inline quat operator - (const quat& f) const { return quat(w - f.w, x - f.x, y - f.y, z - f.z); };
	inline quat& operator -= (const quat& f) { w -= f.w; x -= f.x; y -= f.y; z -= f.z; return *this; };
	inline quat operator / (const double f) const { return quat(w / f, x / f, y / f, z / f); };
	inline quat& operator /= (const double f) { w /= f; x /= f; y /= f; z /= f; return *this; };

	inline quat calculateW() const { return quat(sqrt(std::abs(1.0 - x * x - y * y - z * z)), x, y, z); };
	inline quat inverse() const { return quat(w, -x, -y, -z); };
	inline double length() const { return sqrt(w * w + x * x + y * y + z * z); };
	//inline quat normalize(const quat& f) const { return quat(f.x, f.y, f.z, f.w); };

	inline quat operator * (const quat& f) const {
		return quat(
			w * f.w - x * f.x - y * f.y - z * f.z,
			w * f.x + x * f.w + y * f.z - z * f.y,
			w * f.y - x * f.z + y * f.w + z * f.x,
			w * f.z + x * f.y - y * f.x + z * f.w
		);
	};

	//inline quat& operator *= (const quat& f) { x *= f.x; y *= f.y; z *= f.z; return *this; };
	
	vect rotate(const vect& v) const;
	mat4 toMatrix() const;

	//inline quat operator * (const vect f) const { return vect(x * f, y * f, z * f); };
	//inline quat& operator *= (const vect f) { x *= f; y *= f; z *= f; return *this; };

	static const quat ZERO;
	static const quat IDENTITY;
};

#endif
