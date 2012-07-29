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

#include "TextureHandler.h"
#include <iostream>
#include <fstream>

TextureHandler* TextureHandler::instance = 0;

TextureHandler* TextureHandler::getInstance() {
	if (!instance)
		instance = new TextureHandler;
	return instance;
}

TextureHandler::TextureHandler() {
	names.clear();
	commitQueue.clear();
	started = false;

	// create a default texture to fall back on;
	TextureImage img;
	img.width = 256;
	img.height = 256;
	img.data = new GLubyte[img.width * img.height * 4];
	for (GLuint y = 0; y < img.height; ++y) {
		for (GLuint x = 0; x < img.height; ++x) {
			int a = x ^ y;
			img.data[(y * img.width + x) * 4 + 0] = a + 51;
			img.data[(y * img.width + x) * 4 + 1] = a + 102;
			img.data[(y * img.width + x) * 4 + 2] = a + 153;
			img.data[(y * img.width + x) * 4 + 3] = 255;
		}
	}

	commit(".1", img);

	img.width = 1;
	img.height = 1;
	img.data = new GLubyte[4];
	*(int*)(img.data) = ~0;

	commit("none", img);

	wireframe = false;
	texture = false;
	smooth = true;
	quadric = 0;
}

GLuint TextureHandler::load(std::string name) {
	if ((names.find(name) == names.end()) && (commitQueue.find(name) == commitQueue.end())) {
	
		TextureImage img = loadppm(name);

		if (img.data) { // is not null
			commit(name, img);
		}

	}

	return use(name);

}

GLuint TextureHandler::use(std::string name) {
	if (started) {
		std::map<std::string, GLuint>::const_iterator it;
		it = names.find(name);
		if (it == names.end())
			it = names.find(".1");
		GLuint id = it->second;
		glBindTexture(GL_TEXTURE_2D, id);
		return id;
	}
	return 0;
}

TextureImage TextureHandler::loadppm(std::string name) {
	TextureImage ret;
	ret.width = 0;
	ret.height = 0;
	ret.data = 0;

	std::string fn = name + ".ppm";

	std::cerr << "Attempting to load image " << fn << std::endl;
	std::ifstream image(fn.c_str(), std::ios::binary);

	if (image.fail()) {
		std::cerr << "Failed..." << std::endl;
		return ret;
	}

	std::string type;
	int maxval = 0;
	bool ascii = false;

	image >> type >> ret.width >> ret.height >> maxval;
	image.get();

	if ((type != "P3") && (type != "P6")) {
		std::cerr << "Image is not a strict valid PPM image" << std::endl;
		return ret;
	}
	
	if (type == "P3") {
		std::cerr << "Image is type 3 (ASCII), ";
		ascii = true;
	} else {
		std::cerr << "Image is type 6 (RAW), ";
	}

	std::cerr << "Image is " << ret.width << 'x' << ret.height << std::endl;

	ret.data = new GLubyte[ret.width * ret.height * 4]; // RGBA

	// TODO: clear data
	
	for (GLuint y = 0; y < ret.height; ++y) {
		for (GLuint x = 0; x < ret.width; ++x) {
			int r;
			int g;
			int b;
			int a = 255;

			if (ascii) { // text
				image >> r;
				image >> g;
				image >> b;
			} else { // binary
				r = image.get();
				g = image.get();
				b = image.get();
			}

			//r = r * 255 / maxval;
			//g = g * 255 / maxval;
			//b = b * 255 / maxval;

			ret.data[(y * ret.width + x) * 4 + 0] = r;
			ret.data[(y * ret.width + x) * 4 + 1] = g;
			ret.data[(y * ret.width + x) * 4 + 2] = b;
			ret.data[(y * ret.width + x) * 4 + 3] = a;
		}
	}

	return ret;
}

void TextureHandler::commit(std::string name, TextureImage& img) {
	if (started) {
		if (!img.data)
			return;

		GLuint id = 0;
		glGenTextures(1, &id);
		names.insert(std::pair<std::string, GLuint>(name, id));

		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, name.size()>1?GL_LINEAR:GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, name.size()>1?GL_LINEAR:GL_NEAREST);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.width, img.height, GL_RGBA, GL_UNSIGNED_BYTE, img.data);

		std::cerr << "tex#" << id << " loaded from " << name << std::endl;
		delete img.data;
	} else {
		commitQueue.insert(std::pair<std::string, TextureImage>(name, img));
	}
}

void TextureHandler::init() {
	started = true;
	std::map<std::string, TextureImage>::iterator it;
	for (it = commitQueue.begin(); it != commitQueue.end(); ++it) {
		commit(it->first, it->second);
	}
	commitQueue.clear();
	update();
}

GLUquadric* TextureHandler::getQuadric() {
	if (!started)
		return 0;

	if (!quadric) {
		quadric = gluNewQuadric();
	}

	update();

	return quadric;
}

void TextureHandler::update() {
	if (!started)
		return;

	glPolygonMode( GL_FRONT, wireframe?GL_LINE:GL_FILL );
	(texture?glEnable:glDisable)(GL_TEXTURE_2D);
	(texture?glEnable:glDisable)(GL_SMOOTH);
	glShadeModel(smooth?GL_SMOOTH:GL_FLAT);

	if (!quadric)
		return;

	gluQuadricDrawStyle(quadric, wireframe?GLU_LINE:GLU_FILL);
	gluQuadricTexture(quadric, texture);
	gluQuadricNormals(quadric, smooth?GLU_SMOOTH:GLU_FLAT);
}

void TextureHandler::setWireframe(bool w) {
	wireframe = w;
	update();
}
bool TextureHandler::getWireframe() { return wireframe; }

void TextureHandler::setTexture(bool t) {
	texture = t;
	update();
}
bool TextureHandler::getTexture() { return texture; }

void TextureHandler::setSmooth(bool s) {
	smooth = s;
	update();
}
bool TextureHandler::getSmooth() { return smooth; }
