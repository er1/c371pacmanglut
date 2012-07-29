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

#include <cmath>
#include "EntPacman.h"
#include "EntHandler.h"
#include "EntGhost.h"
#include "EntPellet.h"
#include "EntPowerup.h"
#include "World.h"

#include <iostream>

#define PI (double)3.141592654

EntPacman::EntPacman() {
	mouthangle = 0;
	mouthdirection = 5;
	deathInProgress = false;

	TextureHandler::getInstance()->load("pacman");
	TextureHandler::getInstance()->load("pacman2");

	altTexture = false;
	homeLocation = vect::ZERO;

	score = 0;
}

void EntPacman::think() {
	// animate
	mouthangle+=mouthdirection;

	if ((mouthangle > 45) && !deathInProgress)
		mouthdirection = -5;
	if (mouthangle <= 0)
		mouthdirection = 5;

	if (mouthangle >= 180) {
		setPosition(homeLocation);
		deathInProgress = false;
		setOrientation(quat(0.70710678118654757, 0, 0, -0.70710678118654757));
		mouthangle = 0;
		std::cout << "Death!" << std::endl;
	}
	
	// have I hit a pellet or a powerup?

	EntIterator it = handler->getIterator();

	std::vector<Ent*> removeList;
	removeList.clear();

	while (handler->more(it)) {

		EntPellet* pellet = dynamic_cast<EntPellet*>(*it);
		if (pellet) {
			if ((pellet->getPosition() - getPosition()).length() <= 10) {
				pellet->collect(this);
				std::cout << "Score = " << score << std::endl;
				setPosition(getPosition() + getOrientation().rotate(vect::BACKWARD * 0.4));
				removeList.push_back(pellet);
			}
		}

		EntPowerup* powerup  = dynamic_cast<EntPowerup*>(*it);
		if (powerup) {
			if ((powerup->getPosition() - getPosition()).length() <= 16) {
				powerup->collect(this);
				std::cout << "Score = " << score << std::endl;
				setPosition(getPosition() + getOrientation().rotate(vect::BACKWARD * 0.4));
				removeList.push_back(powerup);
			}
		}

		EntGhost* ghost  = dynamic_cast<EntGhost*>(*it);
		if (ghost) {
			if ((ghost->getPosition() - getPosition()).length() <= 10) {
				if (ghost->Scattering()) {
					ghost->eat();
					score += 200;
					std::cout << "Score = " << score << std::endl;
				}
				if (!ghost->Scattering() && !ghost->Homebound()) {
					deathInProgress = true;
				}
			}
		}

		++it;
	}

	while (removeList.size() > 0) {
		handler->remove(removeList.back());
		removeList.pop_back();
	}

	if (!deathInProgress) {
		vect newPos = getPosition() + getOrientation().rotate(vect::FORWARD * 2.2);
		if (map && (map->getTile(map->TileFromVect(newPos)) & 1)) {
			setPosition(newPos);
		}
	} else {
		setOrientation(quat(0.70710678118654757, 0, 0.70710678118654757, 0));
	}
}

void EntPacman::draw() {
	// display logic goes here
	// draw the object at 0,0,0
	// x,y are the gameboard, z is 0 for the level
	// remember Z is up now

	if (TextureHandler::getInstance()->getTexture()) {
		if (altTexture)
			TextureHandler::getInstance()->use("pacman2");
		else
			TextureHandler::getInstance()->use("pacman");

		glColor3dv(vect::ONE);
	} else {
		glColor3dv(vect(1,1,0));
	}

	glPushMatrix();
	//glRotated(180.0, 0, 0, 1.0);

	glBegin(GL_TRIANGLES);

	const int anglestep = 5;

	for (int phi = -90; phi < 90; phi += anglestep) {
		double sinphi = std::sin(phi * PI / 180.0);
		double cosphi = std::cos(phi * PI / 180.0);
		double sinphi2 = std::sin((phi + anglestep) * PI / 180.0);
		double cosphi2 = std::cos((phi + anglestep) * PI / 180.0);

		double sinthetam = std::sin(mouthangle * PI / 180.0);
		double costhetam = std::cos(mouthangle * PI / 180.0);

		//vect M1 = vect(sinphi, sinthetam * cosphi, costhetam * cosphi) * 8;
		//vect M2 = vect(sinphi2, sinthetam * cosphi2, costhetam * cosphi2) * 8;
		vect M1 = vect(sinphi, costhetam * cosphi, -sinthetam * cosphi) * 8;
		vect M2 = vect(sinphi2, costhetam * cosphi2, -sinthetam * cosphi2) * 8;

		vect T1 = vect(cosphi, sinphi) * 0.5 + vect(0.5);
		vect T2 = vect(cosphi2, sinphi2) * 0.5 + vect(0.5);

		vect normal = M2.cross(M1).normalized();
		glNormal3dv(normal);
		glTexCoord2dv(vect::ONE * 0.5); glVertex3dv(vect::ZERO);
		glTexCoord2dv(T2); glVertex3dv(M2);
		glTexCoord2dv(T1); glVertex3dv(M1);
		M1 *= vect(1,1,-1);
		M2 *= vect(1,1,-1);
		normal = M1.cross(M2).normalized();
		glNormal3dv(normal);
		glTexCoord2dv(vect::ONE * 0.5); glVertex3dv(vect::ZERO);
		glTexCoord2dv(T1); glVertex3dv(M1);
		glTexCoord2dv(T2); glVertex3dv(M2);

		for (int theta = mouthangle; theta < 360 - mouthangle; theta += anglestep) {
			double sintheta = std::sin(theta * PI / 180.0);
			double costheta = std::cos(theta * PI / 180.0);
			double sintheta2 = std::sin((theta + anglestep) * PI / 180.0);
			double costheta2 = std::cos((theta + anglestep) * PI / 180.0);

			vect An = vect(sinphi  , costheta  * cosphi , -sintheta  * cosphi );
			vect Bn = vect(sinphi  , costheta2 * cosphi , -sintheta2 * cosphi );
			vect Cn = vect(sinphi2 , costheta  * cosphi2, -sintheta  * cosphi2);
			vect Dn = vect(sinphi2 , costheta2 * cosphi2, -sintheta2 * cosphi2);
			vect A = An * 8;
			vect B = Bn * 8;
			vect C = Cn * 8;
			vect D = Dn * 8;
			vect At = vect(phi / 360.0 + 0.25, theta / 360.0, 0);
			vect Bt = At + vect(0, anglestep / 360.0, 0);
			vect Ct = At + vect(anglestep / 360.0, 0, 0);
			vect Dt = At + vect(anglestep / 360.0, anglestep / 360.0, 0);

			glNormal3dv(An); glTexCoord2dv(At); glVertex3dv(A);
			glNormal3dv(Dn); glTexCoord2dv(Dt); glVertex3dv(D);
			glNormal3dv(Bn); glTexCoord2dv(Bt); glVertex3dv(B);
			glNormal3dv(An); glTexCoord2dv(At); glVertex3dv(A);
			glNormal3dv(Cn); glTexCoord2dv(Ct); glVertex3dv(C);
			glNormal3dv(Dn); glTexCoord2dv(Dt); glVertex3dv(D);
		}
	}

	glEnd();

	glPopMatrix();

        // shadow
        glPushMatrix();
        TextureHandler::getInstance()->use("none");
        glColor4d(0,0,0,0.5);
	if (deathInProgress) glRotated(90, 0, 1, 0);
        glTranslated(0,0,-14);
        gluDisk(TextureHandler::getInstance()->getQuadric(), 0, 8, 16, 16);
        glPopMatrix();

	glColor3d(0.4,1,0);

	vect s( 0.25 );
	vect d( 0, 0, 8 );

	quad( vect(-4,5)  * s + d , vect(4,7)   * s + d );
	quad( vect(-4,-1) * s + d , vect(4,1)   * s + d );
	quad( vect(-4,-7) * s + d , vect(4,-5)  * s + d );
	quad( vect(-6,1)  * s + d , vect(-4,5)  * s + d );
	quad( vect(-6,-5) * s + d , vect(-4,-1) * s + d );
	quad( vect(4,-5)  * s + d , vect(6,-1)  * s + d );

	glPolygonMode( GL_FRONT, GL_FILL );

}

void EntPacman::quad(vect l, vect h) {
	glBegin(GL_QUADS);
		glVertex3d(l.x, h.y, (l.z + h.z) / 2);
		glVertex3d(l.x, l.y, (l.z + h.z) / 2);
		glVertex3d(h.x, l.y, (l.z + h.z) / 2);
		glVertex3d(h.x, h.y, (l.z + h.z) / 2);
	glEnd();
}

void EntPacman::nextTexture() {
	altTexture = !altTexture;
}

void EntPacman::setHomeLocation(vect home) {
	homeLocation = home;
}

// when i get a container, ask it for its corresponding map
void EntPacman::join() {
        map = dynamic_cast<MapEnvironment*>(handler->getWorld()->getEnvironment());
}

void EntPacman::part() {
        map = 0;
}
