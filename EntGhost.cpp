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

#include "EntGhost.h"
#include "EntHandler.h"
#include "World.h"

#define SQRTHALF 0.70710678118654757

// directions defined as quat's
quat EntGhost::DIR_UP = quat(1,0,0,0);
quat EntGhost::DIR_DOWN = quat(0,0,0,1);
quat EntGhost::DIR_LEFT = quat(SQRTHALF,0,0,-SQRTHALF);
quat EntGhost::DIR_RIGHT = quat(SQRTHALF,0,0,SQRTHALF);

EntGhost::EntGhost(vect _color) {
	// set color and animation angle
	color = _color;
	roto = 0;

	// start in normal mode with an unset scatterLocation (Map environment should set it)
	scattering = false;
	scatterLocation = vect::ZERO;
	homeLocation = vect::ZERO;
	scatterTime = 0;
	homebound = false;

	// inform the TextureHandler that we will need to use "ghost" during draw
	// Ghost texture from: http://www.designfreebies.org/design-tutorials/photoshop-tutorials/halloween-graphics-collection-treat-free-vector-cliparts-and-icons-fonts-photoshop-brushes-and-more/
	TextureHandler::getInstance()->load("ghost");

	TextureHandler::getInstance()->load("pellet");

	// ???
	target = scatterLocation;
}

void EntGhost::think() {
	// animate
	roto = (roto + 3) % 360;

	// scatterDetails
	if (--scatterTime <= 0) {
		scattering = false;
	}

	// returning home details
	if (homebound) {
		if ((homeLocation - getPosition()).length() <= 32) {
			homebound = false;
		}
	}

	// if we do not have a map object, bail since the rest of this method relies on it for path finding
	if (!map)
		return;

	// where are we
	MapTile current = map->TileFromVect(getPosition());

	// are we in a different place since last time?
	if (current != lastTile) {
		// if we are but we are in a wall (corner case) turn right around and hope for the best
		if (!(map->getTile(current) & 1)) {
			setOrientation(getOrientation() * quat(0,0,0,1));	
		} else {
			// otherwise plan to wander toward a new tile

			// what are our options
			MapTile up = current.up();
			MapTile left = current.left();
			MapTile down = current.down();
			MapTile right = current.right();
			// where do we want to get to
			MapTile targetTile = map->TileFromVect(target);
			
			// find out the distance each option gets us
			double updist = (map->VectFromTile(up) - map->VectFromTile(targetTile)).length();
			double leftdist = (map->VectFromTile(left) - map->VectFromTile(targetTile)).length();
			double downdist = (map->VectFromTile(down) - map->VectFromTile(targetTile)).length();
			double rightdist = (map->VectFromTile(right) - map->VectFromTile(targetTile)).length();

			// fallback options
			MapTile newTarget = current;
			double newTargetdist = 0;
			
			// whats the worst case? (we will substitute better options as we find them later
			// simple linear search
			if ((rightdist >= newTargetdist) && (map->getTile(right) & 1) && (right != lastTile))
				newTargetdist = rightdist;
			if ((downdist >= newTargetdist) && (map->getTile(down) & 1) && (down != lastTile))
				newTargetdist = downdist;
			if ((leftdist >= newTargetdist) && (map->getTile(left) & 1) && (left != lastTile))
				newTargetdist = leftdist;
			if ((updist >= newTargetdist) && (map->getTile(up) & 1) && (up != lastTile))
				newTargetdist = updist;

			// now do the acctual direction finding (bring us to the best case direction)
			// simple linear search
			if ((rightdist <= newTargetdist) && (map->getTile(right) & 1) && (right != lastTile)) {
				newTarget = right;
				newTargetdist = rightdist;
				orientation = DIR_RIGHT;
			}
			if ((downdist <= newTargetdist) && (map->getTile(down) & 1) && (down != lastTile)) {
				newTarget = down;
				newTargetdist = downdist;
				orientation = DIR_DOWN;
			}
			if ((leftdist <= newTargetdist) && (map->getTile(left) & 1) && (left != lastTile)) {
				newTarget = left;
				newTargetdist = leftdist;
				orientation = DIR_LEFT;
			}
			if ((updist <= newTargetdist) && (map->getTile(up) & 1) && (up != lastTile)) {
				newTarget = up;
				newTargetdist = updist;
				orientation = DIR_UP;
			}
		}
		// now are are somewhere new
		lastTile = current;
	}
	// acctually move us (speed controlled by scatter state)
	setPosition(getPosition() + getOrientation().rotate(vect::FORWARD * (scattering?0.5:2.0)));
}

void EntGhost::draw() {
	// display logic goes here
	// draw the object at 0,0,0
	// x,y are the gameboard, z is 0 for the level
	// remember Z is up now

	//Draw ghosts
	GLUquadric *myQuad = TextureHandler::getInstance()->getQuadric();

	double red = color.x;
	double green = color.y;
	double blue = color.z;
	double alpha = 0.8;

	// color me
	glColor4d(red,green,blue,alpha);
	
	// texture me
	if (TextureHandler::getInstance()->getTexture()) {
		TextureHandler::getInstance()->use("ghost");
		glColor4d(red * 0.5 + 0.5, green * 0.5 + 0.5, blue * 0.5 + 0.5,alpha); // looks better for texturing
	} 

	if (scattering) {
		TextureHandler::getInstance()->use("pellet");
		glColor4d(0.3, 0.3, 1.0, 0.9); // looks better for texturing
	}

	if (!homebound) {
	glPushMatrix();
		// middle
		glTranslatef (0,0, -9);
		gluCylinder(myQuad, 6, 6, 9, 16, 16);
		glTranslatef (0,0, 9);
		
		// top
		glRotatef (90, 1, 0, 0.0);
		gluSphere(myQuad, 6, 16, 16);
		glRotatef (90,1, 0,0);
		glTranslatef (0,0, 9);
		gluDisk(myQuad, 0, 6, 16, 16);

		// bottom
		for (int angle = 0; angle < 360; angle += 40) {
			glPushMatrix();
				glRotatef (angle + roto, 0,0,1); // animate the bottom (:
				glTranslatef (4.5,0,0);
				gluCylinder(myQuad, 1.5,  0, 3, 16, 16);
			glPopMatrix();
		}
	glPopMatrix();
	
	// shadow
	glPushMatrix();
	TextureHandler::getInstance()->use("none");
	glColor4d(0,0,0,0.5);
	glTranslated(0,0,-14);
	gluDisk(myQuad, 0, 6, 16, 16);
	glPopMatrix();
	}

	TextureHandler::getInstance()->use("none");

	// eyes
	glPushMatrix();
		glColor3f(1,1,1);
		glTranslated(4,4,1);
		gluSphere(myQuad, 2, 8, 8);

		glColor3f(0,0,0);
		glTranslated(0,2,0);
		gluSphere(myQuad, 1, 8, 8);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1,1,1);
		glTranslated(-4,4,1);
		gluSphere(myQuad, 2, 8, 8);

		glColor3f(0,0,0);
		glTranslated(0,2,0);
		gluSphere(myQuad, 1, 8, 8);
	glPopMatrix();

	// shadow
	// here is the best example to show how shadows are handled
	// everywhere else they ar assuming the model is a sphere which in most cases is correct

	glPushMatrix();
		TextureHandler::getInstance()->use("none"); // use a white texture
		glColor4d(0,0,0,0.5); // set it to show black with half opacity
		glTranslated(0,0,-14); // move it to our ground plane which we are using -14 for this
		glScaled(1,1,0); // flatten Z (orthographic projection to the ground plane)
		// draw again (same as above code minus color above)
		glPushMatrix();
			glTranslated(4,4,1);
			gluSphere(myQuad, 2, 8, 8);
			glTranslated(0,2,0);
			gluSphere(myQuad, 1, 8, 8);
		glPopMatrix();
		glPushMatrix();
			glTranslated(-4,4,1);
			gluSphere(myQuad, 2, 8, 8);
			glTranslated(0,2,0);
			gluSphere(myQuad, 1, 8, 8);
		glPopMatrix();
	glPopMatrix();

	glColor3d(0.4,1,0);

	vect s( 0.25 ); // scale
	vect d( 0, 0, 8 ); // translation

	// put our number on top of the ghost
	quad( vect(-4,5)  * s + d , vect(4,7)   * s + d );
	quad( vect(-4,-1) * s + d , vect(4,1)   * s + d );
	quad( vect(-4,-7) * s + d , vect(4,-5)  * s + d );
	quad( vect(-6,1)  * s + d , vect(-4,5)  * s + d );
	quad( vect(-6,-5) * s + d , vect(-4,-1) * s + d );
	quad( vect(4,-5)  * s + d , vect(6,-1)  * s + d );

}

// draw a quad bounded by two vectors
void EntGhost::quad(vect l, vect h) {
	glBegin(GL_QUADS);
		glTexCoord2d(0,0);
		glNormal3d(0,0,1);
		glVertex3d(l.x, h.y, (l.z + h.z) / 2);
		glVertex3d(l.x, l.y, (l.z + h.z) / 2);
		glVertex3d(h.x, l.y, (l.z + h.z) / 2);
		glVertex3d(h.x, h.y, (l.z + h.z) / 2);
	glEnd();
}

// set the ghosts location to go to when they are blue
void EntGhost::setScatterLocation(vect sloc) {
	scatterLocation = sloc;
}

void EntGhost::setHomeLocation(vect sloc) {
	homeLocation = sloc;
}

bool EntGhost::Scattering() {
	return scattering;
}

bool EntGhost::Homebound() {
	return homebound;
}

// tell the ghosts to turn blue and wander
void EntGhost::Scatter(int time) {
	scattering = true;
	scatterTime = time;
	lastTile = MapTile(-1, -1);
}

void EntGhost::eat() {
	scattering = false;
	homebound = true;
}

// when i get a container, ask it for its corresponding map
void EntGhost::join() {
	map = dynamic_cast<MapEnvironment*>(handler->getWorld()->getEnvironment());
}

void EntGhost::part() {
	map = 0;
}
