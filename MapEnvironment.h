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

#ifndef __MAPENVIRONMENT__
#define __MAPENVIRONMENT__

#include "Environment.h"
#include "vect.h"
#include <string>
#include <vector>

class MapTile {
public:
	int x;
	int y;
	inline MapTile(int _x = 0, int _y = 0) : x(_x), y(_y) {};
	inline MapTile up() { return MapTile(x, y-1); }
	inline MapTile down() { return MapTile(x, y+1); }
	inline MapTile left() { return MapTile(x-1, y); }
	inline MapTile right() { return MapTile(x+1, y); }
	inline bool operator==(const MapTile& f) const { return ((x == f.x) && (y == f.y)); }
	inline bool operator!=(const MapTile& f) const { return ((x != f.x) || (y != f.y)); }
};

class MapEnvironment : public Environment {
private:
	std::vector<std::vector<char> > gamemap;

	void drawTile();
	void drawPellet(double size);
	void scube(vect, vect, int = 0xff);

	bool mapListCreated;
	int mapListState;
	GLuint mapList;
	void drawList();

public:
	void loadMap(const std::string&);
	MapEnvironment(const std::string&);
	virtual void think();
	virtual void draw();
	void initObjects();

	virtual MapTile TileFromVect(const vect&);
	virtual vect VectFromTile(const MapTile&);
	virtual int getTile(const MapTile&);
};

#endif
