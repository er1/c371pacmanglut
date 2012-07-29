/*
Copyright (C) 2011 

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

#include "EntLight.h"

EntLight::EntLight() {
	// Light pole texture from: http://www.spiralgraphics.biz/packs/metal_robust/index.htm?3#anchor
	TextureHandler::getInstance()->load("lightpole");
	
	on = true;
}

void EntLight::toggle() {
	on = !on;
}

void EntLight::toggleOn() {
	on = true;
}

void EntLight::toggleOff() {
	on = false;
}

void EntLight::think() {

}

void EntLight::draw() {
	// display logic goes here
	// draw the object at 0,0,0
	// x,y are the gameboard, z is 0 for the level
	// remember Z is up now
	
	//Draw light
	GLUquadric *myQuad = TextureHandler::getInstance()->getQuadric();

	glPushMatrix();
		GLfloat ambient_color[] = {0,0,0,1.0};
		GLfloat diff_color[] = {1.0,1.0,1.0,1.0};
		GLfloat spec_color[] = {0.2,0.2,0.2,1.0};
	
		GLfloat l_position[4] = {0,0,50,1};
		GLfloat l_direction[3] = {1,1,-1};

		std::string name = this->getName();
		int id = atoi(name.substr(name.length() - 1, 1).c_str());
	
		switch(id)
		{
		case 1:
			l_direction[0] = 1;
			l_direction[1] = 1;
			glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diff_color);
			glLightfv(GL_LIGHT0, GL_SPECULAR, spec_color);
			glLightfv(GL_LIGHT0, GL_POSITION, l_position );
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l_direction );
			glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25);
			glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
			break;
		case 2:
			l_direction[0] = 1;
			l_direction[1] = -1;
			glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_color);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, diff_color);
			glLightfv(GL_LIGHT1, GL_SPECULAR, spec_color);
			glLightfv(GL_LIGHT1, GL_POSITION, l_position );
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l_direction );
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);
			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
			break;
		case 3:
			l_direction[0] = -1;
			l_direction[1] = 1;
			glLightfv(GL_LIGHT2, GL_AMBIENT, ambient_color);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, diff_color);
			glLightfv(GL_LIGHT2, GL_SPECULAR, spec_color);
			glLightfv(GL_LIGHT2, GL_POSITION, l_position );
			glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, l_direction );
			glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25);
			glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1);
			break;
		case 4:
			l_direction[0] = -1;
			l_direction[1] = -1;
			glLightfv(GL_LIGHT3, GL_AMBIENT, ambient_color);
			glLightfv(GL_LIGHT3, GL_DIFFUSE, diff_color);
			glLightfv(GL_LIGHT3, GL_SPECULAR, spec_color);
			glLightfv(GL_LIGHT3, GL_POSITION, l_position );
			glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, l_direction );
			glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 25);
			glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1);
			break;
		case 5:
			l_direction[0] = 0;
			l_direction[1] = 0;
			ambient_color[0] = 0.25;
			ambient_color[1] = 0.25;
			ambient_color[2] = 0.25;
			diff_color[0] = 0;
			diff_color[1] = 0;
			diff_color[2] = 0;
			spec_color[0] = 0;
			spec_color[1] = 0;
			spec_color[2] = 0;
			glLightfv(GL_LIGHT4, GL_AMBIENT, ambient_color);
			glLightfv(GL_LIGHT4, GL_DIFFUSE, diff_color);
			glLightfv(GL_LIGHT4, GL_SPECULAR, spec_color);
			break;
		default:
			break;
		}

		//Draw Light
		glPushMatrix();
			if (on)
			{
				glColor3f(1, 1, 0);
				switch(id)
				{
				case 1:
					glEnable(GL_LIGHT0);
					break;
				case 2:
					glEnable(GL_LIGHT1);
					break;
				case 3:
					glEnable(GL_LIGHT2);
					break;
				case 4:
					glEnable(GL_LIGHT3);
					break;
				case 5:
					glEnable(GL_LIGHT4);
					break;
				default:
					break;
				}
			}
			else
			{
				glColor3f(0.25, 0.25, 0);
				switch(id)
				{
				case 1:
					glDisable(GL_LIGHT0);
					break;
				case 2:
					glDisable(GL_LIGHT1);
					break;
				case 3:
					glDisable(GL_LIGHT2);
					break;
				case 4:
					glDisable(GL_LIGHT3);
					break;
				case 5:
					glDisable(GL_LIGHT4);
					break;
				default:
					break;
				}
			}
			glTranslatef(0, 0, 40);
			glDisable(GL_LIGHTING);
			gluSphere(myQuad, 5, 20, 20);
		glEnable(GL_LIGHTING);
		glPopMatrix();

		//Draw post
		// Check if we're adding textures.
		if( TextureHandler::getInstance()->getTexture() )
		{
			glColor3dv(vect::ONE);
			TextureHandler::getInstance()->use("wall");
		}
		else
			glColor3f(0, 0, 1);
	
		gluCylinder(myQuad, 1, 1, 40, 30, 50);
		
	glPopMatrix();

}
