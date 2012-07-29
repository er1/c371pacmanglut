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

#include "Ent.h"
#include "EntHandler.h"

Ent::Ent() {
	// set defaults
	handler = 0;
	position = vect::ZERO;
	orientation = quat::IDENTITY;
}

Ent::~Ent() {

}

// control object name so that we can find it by name from the Handler

void Ent::setName(std::string n) {
	handler->setName(this, n);
}

std::string Ent::getName() {
	return handler->getName(this);
}

// traverse upward up the object hierarchy

EntHandler* Ent::getHandler() {
	return handler;
}

// control position and orientation

const vect& Ent::getPosition() const {
	return position;
}

void Ent::setPosition(const vect& p) {
	position = p;
}

const quat& Ent::getOrientation() const {
	return orientation;
}

void Ent::setOrientation(const quat& d) {
	orientation = d;
}

// called when the object joins or parts from an EntHandler

void Ent::join() {
}

void Ent::part() {
}
