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

#ifndef __WORLD__
#define __WORLD__

#include "EntHandler.h"
#include "Environment.h"

#include "EntPacman.h"
#include "EntGhost.h"
#include "EntBlinky.h"
#include "EntPinky.h"
#include "EntInky.h"
#include "EntClyde.h"
#include "EntLight.h"
#include "EntProxy.h"
#include "EntRenderer.h"

class World {
private:
	EntHandler* entities;
	Environment* environment;

	// game characters
	EntPacman* pacman;

	EntGhost* ghost1;
	EntGhost* ghost2;
	EntGhost* ghost3;
	EntGhost* ghost4;

	EntLight* light1;
	EntLight* light2;
	EntLight* light3;
	EntLight* light4;
	EntLight* light5;

	EntProxy* proxy_light1;
	EntProxy* proxy_light2;
	EntProxy* proxy_light3;
	EntProxy* proxy_light4;

	EntRenderer* camera_pacman;
	EntRenderer* camera_ghost1;
	EntRenderer* camera_ghost2;
	EntRenderer* camera_ghost3;
	EntRenderer* camera_ghost4;
	EntRenderer* camera_light1;
	EntRenderer* camera_light2;
	EntRenderer* camera_light3;
	EntRenderer* camera_light4;
	EntRenderer* camera_world;

	int shading;
	int pelletTexture;
	bool texturing;
public:
	World(Environment*);
	~World();
	Environment* getEnvironment();
	EntHandler* getEntities();

	void init();
	void draw();
	void think();
};

#endif
