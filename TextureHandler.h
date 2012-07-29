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

#ifndef __TEXTURE__
#define __TEXTURE__
#include "gl.h"
#include <map>
#include <string>

struct TextureImage {
	GLuint width;
	GLuint height;
	GLubyte* data;
};

class TextureHandler {
private:
	TextureHandler();
	~TextureHandler();
	static TextureHandler* instance;

	std::map<std::string, GLuint> names;

	std::map<std::string, TextureImage> commitQueue;
	bool started;

	bool wireframe;
	bool texture;
	bool smooth;

	GLUquadric* quadric;
	void update();

public:
	static TextureHandler* getInstance();

	GLuint load(std::string);

	void init();

	GLuint use(std::string);

	TextureImage loadppm(std::string);
	void commit(std::string, TextureImage&);

	// texture related functions

	GLUquadric* getQuadric();

	void setWireframe(bool);
	bool getWireframe();

	void setTexture(bool);
	bool getTexture();

	void setSmooth(bool);
	bool getSmooth();
};

#endif
