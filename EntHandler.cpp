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

#include "mat4.h"
#include "EntHandler.h"
#include <algorithm>

EntHandler::EntHandler(World* w) {
	list.clear();
	world = w;
}

EntHandler::~EntHandler() {
	while (list.size()>1) {
		remove(list[0]);
	}
}

void EntHandler::add(Ent* _e) {
	// assert(_e->handler == NULL);
	list.push_back(_e);
	_e->handler = this;
	_e->join();
}

void EntHandler::remove(Ent* _e) {
	_e->part();
	list.erase(std::find(list.begin(), list.end(), _e));
	delete _e;
}

void EntHandler::setName(Ent* e, std::string n) {
	names.erase(n);
	names.insert(std::pair<std::string, Ent*>(n,e));
}

std::string EntHandler::getName(Ent* e) {
	std::map<std::string, Ent*>::iterator it;
	for (it = names.begin(); it != names.end(); ++it) {
		if ((*it).second == e)
			return (*it).first;
	}
	return ""; // not found
}

Ent* EntHandler::find(std::string name) {
	std::map<std::string, Ent*>::iterator it;
	it = names.find(name);
	if (it == names.end()) {
		return 0; // NULL
	}
	return (*it).second;
}

void EntHandler::thinkAll() {
	for (std::vector<Ent*>::iterator it = list.begin(); it != list.end(); ++it)
		(*it)->think();
}

void EntHandler::drawAll() {
	for (std::vector<Ent*>::iterator it = list.begin(); it != list.end(); ++it) {
		glPushMatrix();
		vect pos = (*it)->getPosition();
		glTranslated(pos.x, pos.y, pos.z);
		glMultMatrixd((*it)->getOrientation().toMatrix());
		(*it)->draw();
		glPopMatrix();
	}
}

World* EntHandler::getWorld() {
	return world;
}

EntIterator EntHandler::getIterator() {
	return list.begin();
}

bool EntHandler::more(const EntIterator& ei) {
	return (ei != list.end());
}

