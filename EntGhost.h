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

#ifndef __ENTGhost__
#define __ENTGhost__

#include "Ent.h"
#include "MapEnvironment.h"

/*
	Generic Ghost Class
*/

class EntGhost : public Ent {
protected:
	// static orientations
	static quat DIR_UP;
	static quat DIR_DOWN;
	static quat DIR_LEFT;
	static quat DIR_RIGHT;

	vect color;
	vect scatterLocation; // the point where the ghost runs away to
	vect homeLocation;
	bool scattering; // and if he is running away
	bool homebound; // are we heading home
	int scatterTime; // how long we have left to scatter

	vect target; // the current target the ghost is attempting to get to, an overriden think() will set this

	MapEnvironment* map; // the map to plan paths
	MapTile lastTile; // the last Tile the on the Map the ghost was on
private:
	int roto; // animation variable
	void quad(vect, vect); // function to draw a quad facing up
public:
	EntGhost(vect = vect::ONE);
	virtual void think();
	virtual void draw();
	virtual void join();
	virtual void part();

	// gameplay methods
	void setScatterLocation(vect);
	void setHomeLocation(vect);
	void Scatter(int time = 0);
	bool Scattering();
	bool Homebound();
	void eat();
};

#endif
