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

#ifndef __ENTHANDLER__
#define __ENTHANDLER__

#include <string>
#include "gl.h"
#include "vect.h"
#include "quat.h"

// the container class for all of the objects
class EntHandler;

// The base object from which all objects are based
class Ent {
protected:
	EntHandler* handler;
	~Ent();
	// position and orientation of the object
	vect position;
	quat orientation;

	// override: called when added to or removed from its container Handler
	virtual void join();
	virtual void part();
public:
	Ent();
	EntHandler* getHandler();
	// override: called once per fixed time interval
	virtual void think() = 0;
	// override: called when a frame needs to get drawn to draw the object
	virtual void draw() = 0;

	// getters and setters
	void setName(std::string);
	std::string getName();

	const vect& getPosition() const;
	void setPosition(const vect&);
	const quat& getOrientation() const;
	void setOrientation(const quat&);

	// our container needs access to certain protected variables
	friend class EntHandler;
};

#endif
