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

#ifndef __ENT__
#define __ENT__

#include <vector>
#include <map>
#include "Ent.h"

class World;

typedef std::vector<Ent*>::iterator EntIterator;

class EntHandler {
private:
	std::vector<Ent*> list;
	std::map<std::string, Ent*> names;
	World* world;
public:
	EntHandler(World*);
	~EntHandler();
	void add(Ent*);
	void remove(Ent*);

	Ent* find(std::string);

	void thinkAll();
	void drawAll();

	World* getWorld();

	EntIterator getIterator();
	bool more(const EntIterator&);

protected:
	void setName(Ent*, std::string);
	std::string getName(Ent*);
	friend class Ent;
};

#endif
