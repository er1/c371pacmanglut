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

#ifndef __MAT4__
#define __MAT4__
#include "vect.h"
#include <cmath>

class mat4 {
public:
	double xx, xy, xz, xw, yx, yy, yz, yw, zx, zy, zz, zw, wx, wy, wz, ww;
	inline mat4(
		const double fxx = 0, const double fyx = 0, const double fzx = 0, const double fwx = 0,
		const double fxy = 0, const double fyy = 0, const double fzy = 0, const double fwy = 0,
		const double fxz = 0, const double fyz = 0, const double fzz = 0, const double fwz = 0,
		const double fxw = 0, const double fyw = 0, const double fzw = 0, const double fww = 0
		) : 
		xx(fxx), xy(fxy), xz(fxz), xw(fxw),
		yx(fyx), yy(fyy), yz(fyz), yw(fyw),
		zx(fzx), zy(fzy), zz(fzz), zw(fzw),
		wx(fwx), wy(fwy), wz(fwz), ww(fww) {
	};

	inline mat4(const mat4& f) :
		xx(f.xx), xy(f.xy), xz(f.xz), xw(f.xw),
		yx(f.yx), yy(f.yy), yz(f.yz), yw(f.yw),
		zx(f.zx), zy(f.zy), zz(f.zz), zw(f.zw),
		wx(f.wx), wy(f.wy), wz(f.wz), ww(f.ww) {
	};

	inline mat4& operator = (const mat4& f) {
		xx = f.xx; xy = f.xy; xz = f.xz; xw = f.xw;
		yx = f.yx; yy = f.yy; yz = f.yz; yw = f.yw;
		zx = f.zx; zy = f.zy; zz = f.zz; zw = f.zw;
		wx = f.wx; wy = f.wy; wz = f.wz; ww = f.ww;
		return *this;
	};

	inline operator double* () { return &xx; };
	inline operator const double* () const { return &xx; };

	inline mat4 operator - () const {
		return mat4(
			-xx, -yx, -zx, -wx,
			-xy, -yy, -zy, -wy,
			-xz, -yz, -zz, -wx,
			-xw, -yw, -zw, -ww
		);
	};

	inline bool operator == (const mat4& f) const {
		return (
			xx == f.xx && xy == f.xy && xz == f.xz && xw == f.xw && 
			yx == f.yx && yy == f.yy && yz == f.yz && yw == f.yw && 
			zx == f.zx && zy == f.zy && zz == f.zz && zw == f.zw && 
			wx == f.wx && wy == f.wy && wz == f.wz && ww == f.ww
		);
	};

	inline bool operator != (const mat4& f) const {
		return (
			xx != f.xx || xy != f.xy || xz != f.xz || xw != f.xw || 
			yx != f.yx || yy != f.yy || yz != f.yz || yw != f.yw || 
			zx != f.zx || zy != f.zy || zz != f.zz || zw != f.zw || 
			wx != f.wx || wy != f.wy || wz != f.wz || ww != f.ww
		);
	};

	inline mat4 operator + (const mat4& f) const {
		return mat4(
			xx + f.xx, yx + f.yx, zx + f.zx, wx + f.wx,
			xy + f.xy, yy + f.yy, zy + f.zy, wy + f.wy,
			xz + f.xz, yz + f.yz, zz + f.zz, wx + f.wz,
			xw + f.xw, yw + f.yw, zw + f.zw, ww + f.ww
		);
	};

	inline mat4& operator += (const mat4& f)  {
		xx += f.xx; yx += f.yx; zx += f.zx; wx += f.wx;
		xy += f.xy; yy += f.yy; zy += f.zy; wy += f.wy;
		xz += f.xz; yz += f.yz; zz += f.zz; wx += f.wz;
		xw += f.xw; yw += f.yw; zw += f.zw; ww += f.ww;
		return *this;
	}

	inline mat4 operator - (const mat4& f) const {
		return mat4(
			xx - f.xx, yx - f.yx, zx - f.zx, wx - f.wx,
			xy - f.xy, yy - f.yy, zy - f.zy, wy - f.wy,
			xz - f.xz, yz - f.yz, zz - f.zz, wx - f.wz,
			xw - f.xw, yw - f.yw, zw - f.zw, ww - f.ww
		);
	};

	inline mat4& operator -= (const mat4& f)  {
		xx -= f.xx; yx -= f.yx; zx -= f.zx; wx -= f.wx;
		xy -= f.xy; yy -= f.yy; zy -= f.zy; wy -= f.wy;
		xz -= f.xz; yz -= f.yz; zz -= f.zz; wx -= f.wz;
		xw -= f.xw; yw -= f.yw; zw -= f.zw; ww -= f.ww;
		return *this;
	};

	inline mat4 operator * (const mat4& f) const {
		return mat4(
			xx * f.xx + yx * f.xy + zx * f.xz + wx * f.xw,
			xx * f.yx + yx * f.yy + zx * f.yz + wx * f.yw,
			xx * f.zx + yx * f.zy + zx * f.zz + wx * f.zw,
			xx * f.wx + yx * f.wy + zx * f.wz + wx * f.ww,
			xy * f.xx + yy * f.xy + zy * f.xz + wy * f.xw,
			xy * f.yx + yy * f.yy + zy * f.yz + wy * f.yw,
			xy * f.zx + yy * f.zy + zy * f.zz + wy * f.zw,
			xy * f.wx + yy * f.wy + zy * f.wz + wy * f.ww,
			xz * f.xx + yz * f.xy + zz * f.xz + wz * f.xw,
			xz * f.yx + yz * f.yy + zz * f.yz + wz * f.yw,
			xz * f.zx + yz * f.zy + zz * f.zz + wz * f.zw,
			xz * f.wx + yz * f.wy + zz * f.wz + wz * f.ww,
			xw * f.xx + yw * f.xy + zw * f.xz + ww * f.xw,
			xw * f.yx + yw * f.yy + zw * f.yz + ww * f.yw,
			xw * f.zx + yw * f.zy + zw * f.zz + ww * f.zw,
			xw * f.wx + yw * f.wy + zw * f.wz + ww * f.ww
		);
	};

	inline mat4& operator *= (const mat4& f)  {	return *this = *this * f; };

	inline mat4 operator * (double f) const {
		return mat4(
			xx * f, yx * f, zx * f, wx * f,
			xy * f, yy * f, zy * f, wy * f,
			xz * f, yz * f, zz * f, wx * f,
			xw * f, yw * f, zw * f, ww * f
		);
	};

	inline mat4& operator *= (double f) {
		xx *= f; yx *= f; zx *= f; wx *= f;
		xy *= f; yy *= f; zy *= f; wy *= f;
		xz *= f; yz *= f; zz *= f; wx *= f;
		xw *= f; yw *= f; zw *= f; ww *= f;
		return *this;
	};

	inline mat4 operator / (double f) const { return *this * (1.0 / f); };
	inline mat4 operator /= (double f) { return *this *= (1.0 / f); };

	static const mat4 ZERO;
	static const mat4 IDENTITY;

	inline vect operator * (const vect& f) const {
		return vect(
			xx * f.x + yx * f.y + zx * f.z + wx,
			xy * f.x + yy * f.y + zy * f.z + wy,
			xz * f.x + yz * f.y + zz * f.z + wz
		);
	};
	
	inline mat4& translate(const vect& f) {
		this->wx += f.x;
		this->wy += f.y;
		this->wz += f.z;
		return *this ;
	};

	inline mat4 transpose() {
		return mat4(
			xx, xy, xz, xw,
			yx, yy, yz, yw,
			zx, zy, zz, xw,
			wx, wy, wz, ww
		);
	}

	inline double determinant() {
		return ( 
			wx*zy*yz*xw - zx*wy*yz*xw - wx*yy*zz*xw + yx*wy*zz*xw +
			zx*yy*wz*xw - yx*zy*wz*xw - wx*zy*xz*yw + zx*wy*xz*yw +
			wx*xy*zz*yw - xx*wy*zz*yw - zx*xy*wz*yw + xx*zy*wz*yw +
			wx*yy*xz*zw - yx*wy*xz*zw - wx*xy*yz*zw + xx*wy*yz*zw +
			yx*xy*wz*zw - xx*yy*wz*zw - zx*yy*xz*ww + yx*zy*xz*ww +
			zx*xy*yz*ww - xx*zy*yz*ww - yx*xy*zz*ww + xx*yy*zz*ww
		);
	}

	inline mat4 inverse() {
		const double invDet = 1/((xx * yy - xy * yx) * (zz * ww - zw * wz) - (xx * yz - xz * yx) * (zy * ww - zw * wy) + (xx * yw - xw * yx) * (zy * wz - zz * wy) + (xy * yz - xz * yy) * (zx * ww - zw * wx) - (xy * yw - xw * yy) * (zx * wz - zz * wx) + (xz * yw - xw * yz) * (zx * wy - zy * wx));
		return mat4(
			(yy * (zz * ww - zw * wz) - yz * (zy * ww - zw * wy) + yw * (zy * wz - zz * wy)) * invDet,
			( - yx * (zz * ww - zw * wz) + yz * (zx * ww - zw * wx) - yw * (zx * wz - zz * wx)) * invDet,
			(yx * (zy * ww - zw * wy) - yy * (zx * ww - zw * wx) + yw * (zx * wy - zy * wx)) * invDet,
			( - yx * (zy * wz - zz * wy) + yy * (zx * wz - zz * wx) - yz * (zx * wy - zy * wx)) * invDet,
			( - xy * (zz * ww - zw * wz) + xz * (zy * ww - zw * wy) - xw * (zy * wz - zz * wy)) * invDet,
			(xx * (zz * ww - zw * wz) - xz * (zx * ww - zw * wx) + xw * (zx * wz - zz * wx)) * invDet,
			( - xx * (zy * ww - zw * wy) + xy * (zx * ww - zw * wx) - xw * (zx * wy - zy * wx)) * invDet,
			(xx * (zy * wz - zz * wy) - xy * (zx * wz - zz * wx) + xz * (zx * wy - zy * wx)) * invDet,
			(wy * (xz * yw - xw * yz) - wz * (xy * yw - xw * yy) + ww * (xy * yz - xz * yy)) * invDet,
			( - wx * (xz * yw - xw * yz) + wz * (xx * yw - xw * yx) - ww * (xx * yz - xz * yx)) * invDet,
			(wx * (xy * yw - xw * yy) - wy * (xx * yw - xw * yx) + ww * (xx * yy - xy * yx)) * invDet,
			( - wx * (xy * yz - xz * yy) + wy * (xx * yz - xz * yx) - wz * (xx * yy - xy * yx)) * invDet,
			( - zy * (xz * yw - xw * yz) + zz * (xy * yw - xw * yy) - zw * (xy * yz - xz * yy)) * invDet,
			(zx * (xz * yw - xw * yz) - zz * (xx * yw - xw * yx) + zw * (xx * yz - xz * yx)) * invDet,
			( - zx * (xy * yw - xw * yy) + zy * (xx * yw - xw * yx) - zw * (xx * yy - xy * yx)) * invDet,
			(zx * (xy * yz - xz * yy) - zy * (xx * yz - xz * yx) + zz * (xx * yy - xy * yx)) * invDet
		);
	}

};

#endif
