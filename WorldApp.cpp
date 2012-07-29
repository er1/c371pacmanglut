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

#include "WorldApp.h"
#include "vect.h"
#include "quat.h"
#include "mat4.h"

// debug me beautiful
#include <iostream>
using namespace std;
////


WorldApp::WorldApp(World* w) {
	world = w;
	w->init();
	camera_main = new EntRenderer;
	world->getEntities()->add(camera_main);
	camera_main->setPosition(vect(0, 0, 512));
	camera_main->setTarget(camera_main);
	camera_main->setName("camera_main");
	camera = camera_main;

	deltax = 0;
	deltay = 0;

	lastx = 512;
	lasty = 384;
	
	mouseStarted = false;

	fovyscale = 1.0;

	// Textures taken from the Valve Source Engine SDK (Half-Life^2)
	TextureHandler::getInstance()->load("N");
	TextureHandler::getInstance()->load("E");
	TextureHandler::getInstance()->load("W");
	TextureHandler::getInstance()->load("T");
	TextureHandler::getInstance()->load("S");
}

void drawskybox();

void WorldApp::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);

	/*
	Projection Matrix should be:
	I * Frustrum * (Orient Z up) * (Orient Camera) * (Move Camera)
	\			Turn screen space into world space			 /
	*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-8 * aspectRatio * fovyscale, 8 * aspectRatio * fovyscale, -8 * fovyscale, 8 * fovyscale, 16, 2048);
	glRotated(90,-1,0,0); 
	const vect campos = camera->getPosition();
	glMultMatrixd(camera->getOrientation().toMatrix().inverse()); 
	drawskybox();
	glTranslated(-campos.x, -campos.y, -campos.z);
	
	// everything else is model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

//	glTranslated(0, 0, -64);
//	glutWireCube(16);

	world->draw();
}

void WorldApp::keyboard(const unsigned char key) {
	//std::cout << "Key action:" << int(key) << std::endl;

	if (camera != (EntRenderer*)world->getEntities()->find("camera_light1") &&
		camera != (EntRenderer*)world->getEntities()->find("camera_light2") &&
		camera != (EntRenderer*)world->getEntities()->find("camera_light3") &&
		camera != (EntRenderer*)world->getEntities()->find("camera_light4")
		)
	{
		// due to libraries and requirements
		//rotation
		if (specialPressed(key, GLUT_KEY_UP)) { camera->setOrientation(quat(0,0.1,0,0).calculateW() * camera->getOrientation()); }
		if (specialPressed(key, GLUT_KEY_DOWN)) { camera->setOrientation(quat(0,-0.1,0,0).calculateW() * camera->getOrientation()); }
		if (specialPressed(key, GLUT_KEY_LEFT)) { camera->setOrientation(quat(0,0,0,0.1).calculateW() * camera->getOrientation()); }
		if (specialPressed(key, GLUT_KEY_RIGHT)) { camera->setOrientation(quat(0,0,0,-0.1).calculateW() * camera->getOrientation()); }
	}

	//Cycle camera
	if ((key == '1') && keyPressed('1')) { //camera->setTarget(world->getEntities()->find("pacman")); }
		camera = (EntRenderer*)world->getEntities()->find("camera_pacman");
	}
	if ((key == '2') && keyPressed('2')) { //camera->setTarget(world->getEntities()->find("ghost1")); }
		camera = (EntRenderer*)world->getEntities()->find("camera_ghost1");
	}
	if ((key == '3') && keyPressed('3')) { //camera->setTarget(world->getEntities()->find("ghost2")); }
		camera = (EntRenderer*)world->getEntities()->find("camera_ghost2");
	}
	if ((key == '4') && keyPressed('4')) { //camera->setTarget(world->getEntities()->find("ghost3")); }
		camera = (EntRenderer*)world->getEntities()->find("camera_ghost3");
	}
	if ((key == '5') && keyPressed('5')) { //camera->setTarget(world->getEntities()->find("ghost4")); }
		camera = (EntRenderer*)world->getEntities()->find("camera_ghost4");
	}
	if ((key == '6') && keyPressed('6')) { //camera->setTarget(world->getEntities()->find("proxy1")); }
		camera = (EntRenderer*)world->getEntities()->find("camera_light1");
	}
	if ((key == '7') && keyPressed('7')) { //camera->setTarget(world->getEntities()->find("proxy2")); }
		camera = (EntRenderer*)world->getEntities()->find("camera_light2");
	}
	if ((key == '8') && keyPressed('8')) { //camera->setTarget(world->getEntities()->find("proxy3")); }
		camera = (EntRenderer*)world->getEntities()->find("camera_light3");
	}
	if ((key == '9') && keyPressed('9')) { //camera->setTarget(world->getEntities()->find("proxy4")); }
		camera = (EntRenderer*)world->getEntities()->find("camera_light4");
	}
	if ((key == '0') && keyPressed('0')) { camera = camera_main; }//camera->setTarget(/*NULL*/0); }
	if ((key == '[') && keyPressed('[')) {
		camera = (EntRenderer*)world->getEntities()->find("camera_world");
	}
	if ((key == ']') && keyPressed(']')) {
		camera = (EntRenderer*)world->getEntities()->find("camera_world");
	}

	// all lamps on
	if ((key == 't') && keyPressed('t')) { 
		dynamic_cast<EntLight*>(world->getEntities()->find("light1"))->toggleOn();
		dynamic_cast<EntLight*>(world->getEntities()->find("light2"))->toggleOn(); 
		dynamic_cast<EntLight*>(world->getEntities()->find("light3"))->toggleOn(); 
		dynamic_cast<EntLight*>(world->getEntities()->find("light4"))->toggleOn(); 
	}

	// all lamps off
	if ((key == 'T') && keyPressed('T')) { 
		dynamic_cast<EntLight*>(world->getEntities()->find("light1"))->toggleOff();
		dynamic_cast<EntLight*>(world->getEntities()->find("light2"))->toggleOff(); 
		dynamic_cast<EntLight*>(world->getEntities()->find("light3"))->toggleOff(); 
		dynamic_cast<EntLight*>(world->getEntities()->find("light4"))->toggleOff(); 
	}
	
	if ((key == 'G') && keyPressed('G')) { 
		((MapEnvironment*)world->getEnvironment())->loadMap("pacman.pgm");
		((MapEnvironment*)world->getEnvironment())->initObjects();
	}
	if ((key == 'g') && keyPressed('g')) { 
		((MapEnvironment*)world->getEnvironment())->loadMap("pacman2.pgm");
		((MapEnvironment*)world->getEnvironment())->initObjects();
	}

	//Toggle light
	if (((key == 'z') && keyPressed('z')) || ((key == 'Z') && keyPressed('Z'))) { dynamic_cast<EntLight*>(world->getEntities()->find("light1"))->toggle(); }
	if (((key == 'x') && keyPressed('x')) || ((key == 'X') && keyPressed('X'))) { dynamic_cast<EntLight*>(world->getEntities()->find("light2"))->toggle(); }
	if (((key == 'c') && keyPressed('c')) || ((key == 'C') && keyPressed('C'))) { dynamic_cast<EntLight*>(world->getEntities()->find("light3"))->toggle(); }
	if (((key == 'v') && keyPressed('v')) || ((key == 'V') && keyPressed('V'))) { dynamic_cast<EntLight*>(world->getEntities()->find("light4"))->toggle(); }
	if (((key == 'b') && keyPressed('b')) || ((key == 'B') && keyPressed('B'))) { dynamic_cast<EntLight*>(world->getEntities()->find("light5"))->toggle(); }

	//Toggle shading
	// we simulate a state machine here when P gets pressed we do the only transition
	if (((key == 'p') && keyPressed('p')) || ((key == 'P') && keyPressed('P'))) {
		TextureHandler* th = TextureHandler::getInstance();

		bool wireframe = th->getWireframe();
		bool smooth =  th->getSmooth();

		if ( wireframe ) {
			th->setWireframe(false);
			th->setSmooth(false);
		} else {
			if (!smooth) {
				th->setSmooth(true);
			} else {
				th->setWireframe(true);
				th->setSmooth(false);
			}
		}
	}
	
	//Toggle texturing
	if (((key == 'n') && keyPressed('n')) || ((key == 'N') && keyPressed('N'))) {
		TextureHandler* th = TextureHandler::getInstance();
		th->setTexture(!th->getTexture());
	}	

	//Toggle pellets only texturing
	//if (((key == 'm') && keyPressed('m')) || ((key == 'M') && keyPressed('M'))) {
	//	// derp... this was a stupid feature
	//}

	//Toggle pellets only texturing
	if ((key == ',') && keyPressed(',')) { dynamic_cast<EntPacman*>(world->getEntities()->find("pacman"))->nextTexture(); }

	// Handle Full Screen
	if ((key == 'F') && keyPressed('F')) {
		glutFullScreen();
	}
	if ((key == 'f') && keyPressed('f')) {
		glutReshapeWindow(1024, 768);
	}

	//Display help menu
	if (((key == 'h') && keyPressed('h')) || ((key == 'H') && keyPressed('H')))
	{
		cout << "Keystroke Guide\n";
		cout << "---------------\n";
		cout << '\n';

		cout << "Miscellaneous:\n";
		cout << '\n';
		cout << "h, H - Brings up a help menu describing all the keystrokes\n";
		cout << "r, R - Resets all camera viewing parameters (position, orientation)\n";
		cout << "p, P - Cycle through smooth shading/flat shading/wireframe mode\n";
		cout << "F    - Enable full screen\n";
		cout << "f    - Leave full screen\n";
		cout << "G    - Restart On Map 1 'pacman.pgm'\n";
		cout << "g    - Restart On Map 2 'pacman2.pgm'\n";

		cout << '\n';

		cout << "Camera Navigation:\n";
		cout << '\n';

		cout << "0 - Switch to Free Camera\n";
		cout << "1 - Switch to Pac Man Camera\n";
		cout << "2 - Switch to Ghost 1 Camera\n";
		cout << "3 - Switch to Ghost 2 Camera\n";
		cout << "4 - Switch to Ghost 3 Camera\n";
		cout << "5 - Switch to Ghost 4 Camera\n";
		cout << "6 - Switch to Light 1 Camera\n";
		cout << "7 - Switch to Light 2 Camera\n";
		cout << "8 - Switch to Light 3 Camera\n";
		cout << "9 - Switch to Light 4 Camera\n";
		cout << '\n';

		cout << "Free Camera Movement:\n";
		cout << '\n';

//		cout << "Up, i, I - Modify Pitch of camera clockwise (rotate upwards)\n";
//		cout << "Down, k, K - Modify Pitch of camera counter-clockwise (rotate downwards)\n";
//		cout << "Left, j, J - Modify Yaw of camera counter-clockwise (rotate left)\n";
//		cout << "Right, l, L - Modify Yaw of camera clockwise (rotate right)\n";
		cout << "Move Mouse - Adjust Yaw/Pitch of camera";
		cout << "q, Q - Modify Roll of camera counter-clockwise (barrel roll left)\n";
		cout << "e, E - Modify Roll of camera clockwise (barrel roll right)\n";
		cout << "w, W - Move camera forward\n";
		cout << "s, S - Move camera backward\n";
		cout << "a, A - Move camera left\n";
		cout << "d, D - Move camera right\n";
		cout << "[, ] - Switch to World rotator camera and move either left or right\n";
		cout << "+,-  - FOV Zoom control\n";
		cout << '\n';

		cout << "Light Control:\n";
		cout << '\n';

		cout << "z, Z - Toggle on/off Light 1\n";
		cout << "x, X - Toggle on/off Light 2\n";
		cout << "c, C - Toggle on/off Light 3\n";
		cout << "v, V - Toggle on/off Light 4\n";
		cout << "t - Toggle on all Lights\n";
		cout << "T - Toggle off all Lights\n";
		cout << '\n';
		cout << "b, B - Toggle on/off Ambient Light\n";
		cout << '\n';
		cout << "n, N - Toggle on/off Texturing\n";
		cout << '\n';
		cout << "comma[,] - Toggle Pacman Texture\n";
		cout << '\n';
		cout << "m, M - Toggle all/power only/none pellet texture\n";
		cout << '\n';
	}
}

void WorldApp::idle() {

}

void WorldApp::timer() {

	if (keyPressed(27)) {
		exit(0);
	}

	// camera movement
	if (keyPressed('w') || keyPressed('W')) {
		camera->setPosition(camera->getPosition() + camera->getOrientation().rotate(vect::FORWARD * 4));
	}
	if (keyPressed('s') || keyPressed('S')) {
		camera->setPosition(camera->getPosition() + camera->getOrientation().rotate(vect::BACKWARD * 4));
	}
	if (keyPressed('a') || keyPressed('A')) {
		camera->setPosition(camera->getPosition() + camera->getOrientation().rotate(vect::LEFT * 4));
	}
	if (keyPressed('d') || keyPressed('D')) {
		camera->setPosition(camera->getPosition() + camera->getOrientation().rotate(vect::RIGHT * 4));
	}
/*	if (keyPressed('x') || keyPressed('X')) {
		camera->setPosition(camera->getPosition() + camera->getOrientation().rotate(vect::UP * 4));
	}
	if (keyPressed('z') || keyPressed('Z')) {
		camera->setPosition(camera->getPosition() + camera->getOrientation().rotate(vect::DOWN * 4));
	}*/

	// TODO: keyvaules
	// rotation       (we shouldnt move certain cameras around
	if (camera != (EntRenderer*)world->getEntities()->find("camera_light1") &&
		camera != (EntRenderer*)world->getEntities()->find("camera_light2") &&
		camera != (EntRenderer*)world->getEntities()->find("camera_light3") &&
		camera != (EntRenderer*)world->getEntities()->find("camera_light4")
		)
	{
		// camera roll (not that anyone wants to use this, its nausea on a screen)
		if (keyPressed('e') || keyPressed('E')) { camera->setOrientation(quat(0,0,-0.02,0).calculateW() * camera->getOrientation()); }
		if (keyPressed('q') || keyPressed('Q')) { camera->setOrientation(quat(0,0,0.02,0).calculateW() * camera->getOrientation()); }

		// move pacman
		Ent* pacman = world->getEntities()->find("pacman");
		if (keyPressed('i') || keyPressed('I')) {
			pacman->setOrientation(quat(1,0,0,0));
		}
		if (keyPressed('k') || keyPressed('K')) {
			pacman->setOrientation(quat(0,0,0,1));
		}
		if (keyPressed('j') || keyPressed('J')) {
			pacman->setOrientation(quat(0.70710678118654757,0,0,-0.70710678118654757));
		}
		if (keyPressed('l') || keyPressed('L')) {
			pacman->setOrientation(quat(0.70710678118654757,0,0,0.70710678118654757));
		}

		// handled mouse movement
		camera->setOrientation(camera->getOrientation() * quat(0,0,0,max(-1.0, min(1.0, 0.005 * deltax))).calculateW());
		camera->setOrientation(quat(0,max(-1.0, min(1.0, 0.005 * deltay)),0,0).calculateW() * camera->getOrientation());
		deltax = deltay = 0;

		// move the roto cam
		if (keyPressed('[') || keyPressed('[')) {
			camera->setOrientation(camera->getOrientation() * quat(0,0,0,0.02).calculateW());
			camera->setPosition(camera->getOrientation().rotate(vect::BACKWARD * 512));
		}
		if (keyPressed(']') || keyPressed(']')) {
			camera->setOrientation(camera->getOrientation() * quat(0,0,0,-0.02).calculateW());
			camera->setPosition(camera->getOrientation().rotate(vect::BACKWARD * 512));
		}
	}

	// reset all the things
	if (keyPressed('r') || keyPressed('R')) {
		//camera->setPosition(vect::ZERO);
		//camera->setOrientation(quat::IDENTITY);

		// put our main camera somewhere useless and point it forward
		camera_main->setPosition(vect(0, 0, 512));
		camera_main->setOrientation(quat::IDENTITY);
		
		// get all the cameras to face forward
		world->getEntities()->find("camera_pacman")->setOrientation(quat::IDENTITY);
		world->getEntities()->find("camera_ghost1")->setOrientation(quat::IDENTITY);
		world->getEntities()->find("camera_ghost2")->setOrientation(quat::IDENTITY);
		world->getEntities()->find("camera_ghost3")->setOrientation(quat::IDENTITY);
		world->getEntities()->find("camera_ghost4")->setOrientation(quat::IDENTITY);

		//point the world cam down a bit and move it 512 away
		world->getEntities()->find("camera_world")->setOrientation(quat(0,0.2,0,0).calculateW());
		world->getEntities()->find("camera_world")->setPosition(quat(0,0.2,0,0).calculateW().rotate(vect::BACKWARD * 512));

		EntRenderer* resetcam = (EntRenderer*)world->getEntities()->find("camera_light1");
		resetcam->setOrientation(quat(0,0.3,-0.1,0.350).calculateW());
		resetcam = (EntRenderer*)world->getEntities()->find("camera_light2");
		resetcam->setOrientation(quat(0,0.1,-0.3,0.875).calculateW());
		resetcam = (EntRenderer*)world->getEntities()->find("camera_light3");
		resetcam->setOrientation(quat(0,0.3,0.1,-0.350).calculateW());
		resetcam = (EntRenderer*)world->getEntities()->find("camera_light4");
		resetcam->setOrientation(quat(0,0.1,0.3,-0.875).calculateW());

		fovyscale = 1.0;
	}

	if (keyPressed('-')) fovyscale += 0.01;
	if (keyPressed('=')) fovyscale -= 0.01;

	world->think();	
}

void WorldApp::init() {
	// show some messages
	std::cerr << "Starting World..." << std::endl;
	std::cerr << "Starting Texture Handler..." << std::endl;
	// start the texture handler (we need to do this now rather than earlier or else we get a seg fault)
	TextureHandler::getInstance()->init();
	std::cerr << "... OK" << std::endl;

}

// handle all kinds of mouse movement
void WorldApp::mouse(const int button, const int state, const int x, const int y) {
	if (mouseStarted) {
		// add to the change in mouse coords since last time (anything handling it will zero it out)
		deltax += x - lastx;
		deltay += y - lasty;
	} else {
		deltax = 0;
		deltay = 0;
		mouseStarted = true;
	}

	// we handled out delta so move old to new for next time
	lastx = x;
	lasty = y;
}

// draw one quad based on 4 double vectors
void drawquad(const double * a, const double * b, const double * c, const double * d) {
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex3dv(a);
        glTexCoord2d(1, 0);
        glVertex3dv(b);
        glTexCoord2d(1, 1);
        glVertex3dv(c);
        glTexCoord2d(0, 1);
        glVertex3dv(d);
        glEnd();
}

// draw a skybox
void drawskybox() {
	// this double array contains all permutions of 64 and -64
        const double cube[] = { 64, -64, 64, 64, 64, -64, -64, -64, 64, -64 };
        glClear(GL_COLOR_BUFFER_BIT); // clear the screen
	glColor3d(1,1,1); // use white
        glDisable(GL_LIGHTING); // screw lighting
        glDisable(GL_DEPTH_TEST); // the sky has no depth
	// pick textures and pass vector offsets to drawquad
        TextureHandler::getInstance()->use("N");
        drawquad(cube+2, cube+1, cube+7, cube+3);
        TextureHandler::getInstance()->use("S");
        drawquad(cube+6, cube+0, cube+4, cube+5);
        TextureHandler::getInstance()->use("E");
        drawquad(cube+0, cube+2, cube+3, cube+4);
        TextureHandler::getInstance()->use("W");
        drawquad(cube+1, cube+6, cube+5, cube+7);
        TextureHandler::getInstance()->use("T");
        drawquad(cube+6, cube+1, cube+2, cube+0);
        glEnable(GL_DEPTH_TEST); // the rest of the world likes depth tests though
}
