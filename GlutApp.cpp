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

#include "GlutApp.h"

bool GlutApp::GlutAppKmap[256];
GlutApp* GlutApp::instance = 0;

void GlutAppResize(const int x, const int y) {
	GlutApp::instance->aspectRatio = (double)x / (double)y;
	glViewport(0, 0, x, y);
}

void GlutAppKeyboard(const unsigned char key, const int x, const int y) {
	GlutApp::GlutAppKmap[key] = 1;
	GlutApp::instance->keyboard(key);
}

void GlutAppKeyboardUp(const unsigned char key, const int x, const int y) {
	GlutApp::GlutAppKmap[key] = 0;
	GlutApp::instance->keyboard(key);
}

void GlutAppSpecial(const int key, const int, const int) {
	GlutApp::instance->keyboard(key & 0x7f | 0x80);
}

void GlutAppMouse(const int button, const int state, const int x, const int y) {
	GlutApp::instance->mouse(button, state, x, y);
}

void GlutAppMotion(const int x, const int y) {
	const int button = -1;
	const int state = 0;
	GlutApp::instance->mouse(button, state, x, y);
}

void GlutAppPassiveMotion(const int x, const int y) {
	const int button = -1;
	const int state = 0;
	GlutApp::instance->mouse(button, state, x, y);
}

void GlutAppTimer(const int nil) {
	glutTimerFunc(20, GlutAppTimer, 0);
	GlutApp::instance->timer();
}

void GlutAppDisplay() {
	GlutApp::instance->display();
	glutSwapBuffers();
	glutPostRedisplay();
}

void GlutAppIdle() {
	GlutApp::instance->idle();
}

GlutApp::GlutApp() {

}

void GlutApp::run(int argc, char** argv) {
	instance = this;

	for (int i = 0; i < 256; ++i) {
		GlutAppKmap[i] = 0;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("GlutApp");
	this->init();
	glutReshapeFunc(GlutAppResize);
	glutKeyboardFunc(GlutAppKeyboard);
	glutKeyboardUpFunc(GlutAppKeyboardUp);
	glutSpecialFunc(GlutAppSpecial);
	glutMouseFunc(GlutAppMouse);
	glutMotionFunc(GlutAppMotion);
	glutPassiveMotionFunc(GlutAppPassiveMotion);
	glutTimerFunc(20, GlutAppTimer, 0);
	glutDisplayFunc(GlutAppDisplay);
	glutIdleFunc(GlutAppIdle);
	glutMainLoop();
}

void GlutApp::display() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}
void GlutApp::keyboard(const unsigned char key) {

}
void GlutApp::mouse(const int button, const int state, const int x, const int y) {

}
void GlutApp::timer() {

}
void GlutApp::idle() {

}
void GlutApp::init() {

}

bool GlutApp::keyPressed(const unsigned char key) {
	return GlutAppKmap[key];
}

bool GlutApp::specialPressed(const unsigned char key, const int ref) {
	return (((key & 0x80) == 0x80) && ((key & 0x7f) == ref));
}
