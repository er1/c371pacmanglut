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

#ifndef __GLUTAPP__
#define __GLUTAPP__

#include "gl.h"

class GlutApp {
private:
	static GlutApp* instance;
	static bool GlutAppKmap[];

protected:
	virtual void display();
	virtual void keyboard(const unsigned char);
	virtual void mouse(const int, const int, const int, const int);
	virtual void idle();
	virtual void timer();
	virtual void init();

	bool keyPressed(const unsigned char);
	double aspectRatio;

	bool specialPressed(const unsigned char, const int);

public:
	GlutApp();

	void run(int, char**);

	friend void GlutAppResize(const int, const int);
	friend void GlutAppKeyboard(const unsigned char, const int, const int);
	friend void GlutAppKeyboardUp(const unsigned char, const int, const int);
	friend void GlutAppSpecial(const int, const int, const int);
	friend void GlutAppMouse(const int, const int, const int, const int);
	friend void GlutAppMotion(const int, const int);
	friend void GlutAppPassiveMotion(const int, const int);
	friend void GlutAppTimer(const int);
	friend void GlutAppDisplay();
	friend void GlutAppIdle();
};

#endif
