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

#include "EntPellet.h"

EntPellet::EntPellet(const vect& pos) {
	position = pos;
	TextureHandler::getInstance()->load("pellet");
}

void EntPellet::draw() {
	TextureHandler* texh = TextureHandler::getInstance();

	glColor3d(1,1,1);
	if (texh->getTexture()) {
		texh->use("pellet");
	}

        GLfloat specular1[4] = { 1, 1, 1, 1 };
        GLfloat specular0[4] = { 0, 0, 0, 1 };
        const double PI = 3.141592654;

        glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);

        GLUquadric* q = TextureHandler::getInstance()->getQuadric();
        gluSphere(q, 2, 8, 8);

        glMaterialfv(GL_FRONT, GL_SPECULAR, specular0);

        // shadow
        glPushMatrix();
        TextureHandler::getInstance()->use("none");
        glColor4d(0,0,0,0.5);
        glTranslated(0,0,-14);
        gluDisk(TextureHandler::getInstance()->getQuadric(), 0, 2, 16, 1);
        glPopMatrix();


}

void EntPellet::think() {

}

void EntPellet::collect(EntPacman* pacman) {
	pacman->score += 10;
}

GLfloat EntPellet::specular0[] = { 0, 0, 0, 1 };
GLfloat EntPellet::specular1[] = { 1, 1, 1, 1 };

