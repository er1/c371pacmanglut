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

#include "World.h"
#include "MapEnvironment.h"

World::World(Environment* env) {
	environment = env;
}

void World::init() {
	entities = new EntHandler(this);
	environment->setWorld(this);

	shading = 0;
	pelletTexture = 0;

	texturing = false;

	pacman = new EntPacman;
	ghost1 = new EntBlinky(pacman);
	ghost2 = new EntPinky(pacman);
	ghost3 = new EntInky(pacman, (EntBlinky*)ghost1);
	ghost4 = new EntClyde(pacman);


	//ghost1 = new EntGhost(vect(1.0, 0.0, 0.0)); // red
	//ghost2 = new EntGhost(vect(1.0, 0.5, 0.5)); // pink
	//ghost3 = new EntGhost(vect(1.0, 0.5, 0.0)); // orange
	//ghost4 = new EntGhost(vect(0.0, 1.0, 1.0)); // cyan
	light1 = new EntLight();
	light2 = new EntLight();
	light3 = new EntLight();
	light4 = new EntLight();
	light5 = new EntLight(); // Ambient
	proxy_light1 = new EntProxy();
	proxy_light2 = new EntProxy();
	proxy_light3 = new EntProxy();
	proxy_light4 = new EntProxy();
	camera_pacman = new EntRenderer;
	camera_ghost1 = new EntRenderer;
	camera_ghost2 = new EntRenderer;
	camera_ghost3 = new EntRenderer;
	camera_ghost4 = new EntRenderer;
	camera_light1 = new EntRenderer;
	camera_light2 = new EntRenderer;
	camera_light3 = new EntRenderer;
	camera_light4 = new EntRenderer;
	camera_world = new EntRenderer;

	entities->add(pacman);
	entities->add(ghost1);
	entities->add(ghost2);
	entities->add(ghost3);
	entities->add(ghost4);
	entities->add(light1);
	entities->add(light2);
	entities->add(light3);
	entities->add(light4);
	entities->add(light5);
	entities->add(proxy_light1);
	entities->add(proxy_light2);
	entities->add(proxy_light3);
	entities->add(proxy_light4);
	entities->add(camera_pacman);
	entities->add(camera_ghost1);
	entities->add(camera_ghost2);
	entities->add(camera_ghost3);
	entities->add(camera_ghost4);
	entities->add(camera_light1);
	entities->add(camera_light2);
	entities->add(camera_light3);
	entities->add(camera_light4);
	entities->add(camera_world);

	camera_light1->setOrientation(quat(0,-0.3,0.1,-0.350).calculateW());
	camera_light2->setOrientation(quat(0,-0.1,0.3,-0.875).calculateW());
	camera_light3->setOrientation(quat(0,-0.3,-0.1,0.350).calculateW());
	camera_light4->setOrientation(quat(0,-0.1,-0.3,0.875).calculateW());

	camera_world->setOrientation(quat(0,0.2,0,0).calculateW());

	pacman->setPosition(vect(-64, 128, 0));
	pacman->setOrientation(quat(0,0,0,1).calculateW());
	ghost1->setPosition(vect(-32, 128, 0));
	ghost2->setPosition(vect(0, 128, 0)); 
	ghost3->setPosition(vect(32, 128, 0));
	ghost4->setPosition(vect(64, 128, 0));
	light1->setPosition(vect(-228, -228, 0));
	light2->setPosition(vect(-228, 260, 0));
	light3->setPosition(vect(212, -228, 0));
	light4->setPosition(vect(212, 260, 0));
	light5->setPosition(vect(0, 0, 1024));
	light5->setOrientation(quat(0.70710678118654757, -0.70710678118654757, 0, 0));
	proxy_light1->setPosition(vect(-228, -228, 50));
	proxy_light2->setPosition(vect(-228, 260, 50));
	proxy_light3->setPosition(vect(212, -228, 50));
	proxy_light4->setPosition(vect(212, 260, 50));
	camera_pacman->setTarget(pacman);
	camera_ghost1->setTarget(ghost1);
	camera_ghost2->setTarget(ghost2);
	camera_ghost3->setTarget(ghost3);
	camera_ghost4->setTarget(ghost4);
	camera_light1->setTarget(proxy_light1);
	camera_light2->setTarget(proxy_light2);
	camera_light3->setTarget(proxy_light3);
	camera_light4->setTarget(proxy_light4);

	ghost1->setName("ghost1");
	ghost2->setName("ghost2");
	ghost3->setName("ghost3");
	ghost4->setName("ghost4");
	pacman->setName("pacman");
	light1->setName("light1");
	light2->setName("light2");
	light3->setName("light3");
	light4->setName("light4");
	light5->setName("light5");
	proxy_light1->setName("proxy1");
	proxy_light2->setName("proxy2");
	proxy_light3->setName("proxy3");
	proxy_light4->setName("proxy4");
	camera_pacman->setName("camera_pacman");
	camera_ghost1->setName("camera_ghost1");
	camera_ghost2->setName("camera_ghost2");
	camera_ghost3->setName("camera_ghost3");
	camera_ghost4->setName("camera_ghost4");
	camera_light1->setName("camera_light1");
	camera_light2->setName("camera_light2");
	camera_light3->setName("camera_light3");
	camera_light4->setName("camera_light4");
	camera_world->setName("camera_world");

	((MapEnvironment*)environment)->initObjects();
//	light5->toggleOff();
}

World::~World() {
	delete entities;
	delete environment;
}

Environment* World::getEnvironment() {
	return environment;
}

EntHandler* World::getEntities() {
	return entities;
}

void World::think() {
	environment->think();
	entities->thinkAll();	
}

void World::draw() {
	glEnable(GL_LIGHTING);

	GLfloat diffuse[] = {0.2,0.2,0.2,1.0};
	glMaterialfv(GL_FRONT,GL_SPECULAR, diffuse);
	glEnable(GL_COLOR_MATERIAL);

	//vect pos = light1->getPosition();
	//GLfloat l_position[4] = {pos[0],pos[1],pos[2],0};

	environment->draw();
	entities->drawAll();	

}
