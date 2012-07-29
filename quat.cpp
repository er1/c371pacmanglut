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

#include "quat.h"
#include "vect.h"
#include "mat4.h"

const quat quat::ZERO = quat(0, 0, 0, 0);
const quat quat::IDENTITY = quat(1, 0, 0, 0);

vect quat::rotate(const vect& v) const {
	return toMatrix() * v;
}

mat4 quat::toMatrix() const {
	return mat4(
		w*w+x*x-y*y-z*z, 2*x*y+2*w*z, 2*x*z-2*w*y, 0,
		2*x*y-2*w*z, w*w-x*x+y*y-z*z, 2*y*z+2*w*x, 0,
		2*x*z+2*w*y, 2*y*z-2*w*x, w*w-x*x-y*y+z*z, 0,
		0,	0,	0,	1
	);
}
