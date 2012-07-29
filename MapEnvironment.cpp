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

#include <iostream>
#include <fstream>
#include <vector>

#include "gl.h"
#include "MapEnvironment.h"
#include "vect.h"

#include "World.h"
#include "EntPellet.h"
#include "EntPowerup.h"

MapEnvironment::MapEnvironment(const std::string& filename) {
	loadMap(filename);
	
	// Rust texture from: http://free3dmax.com/2010/05/seamless-rust-texture/
	TextureHandler::getInstance()->load("rust");
	
	// Blood texture from: http://forums.epicgames.com/threads/601110-Shade-s-Liquid-Texture-Samples-%28Slime-tar-oil-blood-water-textures-and-more%29?s=3bd6e39db474fde2fff3b98f57535c53
	TextureHandler::getInstance()->load("blood");
	
	// Wall texture from: http://forums.epicgames.com/threads/601110-Shade-s-Liquid-Texture-Samples-%28Slime-tar-oil-blood-water-textures-and-more%29?s=3bd6e39db474fde2fff3b98f57535c53
	TextureHandler::getInstance()->load("wall");

	// loaded elsewhere
	//TextureHandler::getInstance()->load("pellet");
	//TextureHandler::getInstance()->load("powerup");

	mapListCreated = false;
}

void MapEnvironment::loadMap(const std::string& mapname) {
	// open our file and get details such as width height and type
	// we use the Netpbm Format for maps
	std::ifstream game(mapname.c_str(), std::ios::binary);
	int width;
	int height;
	std::string type;
	int dummy; // bpp but should always be 255 so ignore it
	game >> type >> width >> height >> dummy;
	game.get();

	gamemap.clear(); // clear any existing map

	// load that map dependant on type
	if (type == "P2") { // ASCII numbers
		gamemap.empty();
		for (int i = 0; i < height; ++i) {
			gamemap.push_back(std::vector<char>());
			for (int j = 0; j < width; ++j) {
				int n;
				game >> n;
				gamemap[i].push_back((char)n);
			}
		}
	} else {
		if (type == "P5") { // BINARY bytes
			gamemap.empty();
			for (int i = 0; i < height; ++i) {
				gamemap.push_back(std::vector<char>());
				for (int j = 0; j < width; ++j) {
					gamemap[i].push_back(game.get());
				}
			}
		} else {
			std::cerr << "FAILED TO LOAD MAP" << std::endl;
			gamemap.push_back(std::vector<char>());
			gamemap[0].push_back(0x0d); // l block error case 
			gamemap[0].push_back(0x0d); // 5 x for ghosts and pacman
			gamemap[0].push_back(0x0d);
			gamemap[0].push_back(0x0d);
			gamemap[0].push_back(0x0d);
			//exit(1); // dont exit just load a 5x1 map to account for all positioning code
		}

	}

	mapListState = -1; // force rebuffer of map when draw gets called
}

void MapEnvironment::think() {

}

void MapEnvironment::initObjects() {
	// iterate over all Ents
	EntHandler* handler = world->getEntities();
	EntIterator it = handler->getIterator();

	std::vector<Ent*> removeList;
	removeList.clear();

        while (handler->more(it)) {
		// remove pellets
                EntPellet* pellet = dynamic_cast<EntPellet*>(*it);
                if (pellet)
			removeList.push_back(pellet);
		// remove powerups
                EntPowerup* powerup  = dynamic_cast<EntPowerup*>(*it);
                if (powerup)
			removeList.push_back(powerup);

                ++it;
        }

	// we remove separately since we dont want to invalidate our iterator
	while (removeList.size() > 0) {
		handler->remove(removeList.back());
		removeList.pop_back();
	}

	int mapHeight = gamemap.size();
	int mapWidth = gamemap[0].size();

	// fake a queue for positioning objects
	std::vector<Ent*> gameobjects;
	gameobjects.clear();
	gameobjects.push_back(world->getEntities()->find("pacman"));
	gameobjects.push_back(world->getEntities()->find("ghost4"));
	gameobjects.push_back(world->getEntities()->find("ghost3"));
	gameobjects.push_back(world->getEntities()->find("ghost2"));
	gameobjects.push_back(world->getEntities()->find("ghost1"));

	// set values for the objects such as score and scatter location since they are dependant on the map
	dynamic_cast<EntPacman*>(gameobjects[0])->score = 0;
	dynamic_cast<EntGhost*>(gameobjects[4])->setScatterLocation(VectFromTile(MapTile(mapWidth - 1,0)));
	dynamic_cast<EntGhost*>(gameobjects[3])->setScatterLocation(VectFromTile(MapTile(0,0)));
	dynamic_cast<EntGhost*>(gameobjects[2])->setScatterLocation(VectFromTile(MapTile(mapWidth - 1,mapHeight - 1)));
	dynamic_cast<EntGhost*>(gameobjects[1])->setScatterLocation(VectFromTile(MapTile(0,mapHeight - 1)));
	
	// step over the ma
	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			// get the center of the tile as vect p
			char tile = gamemap[y][x];
			vect p = vect((x - mapWidth / 2) * 16, -(y - mapHeight / 2) * 16, 0);

			if (tile & 8) { // bit 0x00001000 ==> position a game character here
				vect entpos = p - vect(8,0,0);
				EntGhost* ghost = dynamic_cast<EntGhost*>(gameobjects.back());
				if (ghost) ghost->setHomeLocation(p);
				EntPacman* pacman = dynamic_cast<EntPacman*>(gameobjects.back());
				if (pacman) pacman->setHomeLocation(p);
				gameobjects.back()->setPosition(entpos);
				gameobjects.back()->setOrientation(quat(0.70710678118654757, 0, 0, -0.70710678118654757));
				gameobjects.pop_back();
			}
			if (tile & 4) { // bit 0x00000100 ==> a power pellet belongs here
				EntPowerup* ep = new EntPowerup(p);
				world->getEntities()->add(ep);
			}
			if (tile & 2) { // bit 0x00000010 ==> a normal pellet goes here
				EntPellet* ep = new EntPellet(p);
				world->getEntities()->add(ep);
			}
		}
	}

}

void MapEnvironment::draw() {
	TextureHandler* texh = TextureHandler::getInstance();

	// force the ground to be black
	texh->use("none");
	glColor3d(0,0,0);
	scube(vect(-4096, -4096, -32), vect(4096, 4096, -24)); // 4096 is past the clipping plane X2
	
	// get a map list state (an int with each state in its own bit)
	int newMapListState =
		(texh->getSmooth()?1:0) |
		(texh->getTexture()?2:0) |
		(texh->getWireframe()?4:0);

	// if it differs from last time, recreate the list
	if (!mapListCreated || (mapListState != newMapListState))
		drawList();
	glCallList(mapList);
}

void MapEnvironment::drawList() {
	// we use a display list for performance sake

	// create it if needed
	if (!mapListCreated) {
		mapList = glGenLists(1);	
		mapListCreated = true;
	}

	// start our list
	glNewList(mapList, GL_COMPILE);
	
	// things we may use while drawing
	int mapHeight = gamemap.size();
	int mapWidth = gamemap[0].size();
	TextureHandler* texh = TextureHandler::getInstance();

	// set our new state of the list
	mapListState =
		(texh->getSmooth()?1:0) |
		(texh->getTexture()?2:0) |
		(texh->getWireframe()?4:0);
	
	// loops over the map
	for (int y = 0; y < mapHeight; ++y) {
		glPushMatrix();
		glTranslated(0, -(y - mapHeight / 2) * 16, 0); // negative (higher Y in worldcoords are lower in map coords)

		for (int x = 0; x < mapWidth; ++x) {
			char tile = gamemap[y][x];

			glPushMatrix();
			glTranslated((x - mapWidth / 2) * 16, 0, 0);

			// some constants for drawing
			const double dimE = 8.;
			const double dimA = 2.;
			const double dimB = 6.;
			const double dimH1 = -12.;
			const double dimH2 = 12.;

			// if texturing is enabled, use wall
			if( texh->getTexture() )
			{ 
				glColor3dv(vect::ONE); // color white * texture
				TextureHandler::getInstance()->use("wall");
			}
			else
				glColor3d(0,0,1); // color blue

			// create the walls
			// 0x11110000 ==> bitmask for the walls
			// use scube and the constants to draw them
			switch ((tile & 0xf0) >> 4) { // take the wall bits of the map
			case 0:	// nothing
				break;
			case 1:	// outer topleft
				scube(vect(-dimE, dimA, dimH1), vect(-dimA, dimB, dimH2), 0xf7);
				scube(vect(-dimB, dimB, dimH1), vect(-dimA, dimE, dimH2), 0xfc);
				break;
			case 2: // outer topright
				scube(vect(dimA, dimA, dimH1), vect(dimE, dimB, dimH2), 0xfb);
				scube(vect(dimA, dimB, dimH1), vect(dimB, dimE, dimH2), 0xfc);
				break;
			case 3: // top
				scube(vect(-dimE, dimA, dimH1),vect(dimE, dimB, dimH2), 0xf3);
				break;
			case 4: // outer bottomleft
				scube(vect(-dimE, -dimB, dimH1), vect(-dimA, -dimA, dimH2), 0xf7);	
				scube(vect(-dimB, -dimE, dimH1), vect(-dimA, -dimB, dimH2), 0xfc);
				break;
			case 5: // left
				scube(vect(-dimB, -dimE, dimH1),vect(-dimA, dimE, dimH2), 0xfc);
				break;
			case 7: // inner topleft
				scube(vect(-dimB, dimA, dimH1), vect(dimE, dimB, dimH2), 0xfb);	
				scube(vect(-dimB, -dimE, dimH1), vect(-dimA, dimA, dimH2), 0xfc);
				break;
			case 8: // outer bottomright
				scube(vect(dimA, -dimB, dimH1), vect(dimE, -dimA, dimH2), 0xfb);
				scube(vect(dimA, -dimE, dimH1), vect(dimB, -dimB, dimH2), 0xfc);
				break;
			case 10: //right
				scube(vect(dimA, -dimE, dimH1),vect(dimB, dimE, dimH2), 0xfc);
				break;
			case 11: // inner topright
				scube(vect(-dimE, dimA, dimH1),vect(dimB, dimB, dimH2), 0xf7);
				scube(vect(dimA, -dimE, dimH1),vect(dimB, dimA, dimH2), 0xfc);
				break;
			case 12: // bottom
				scube(vect(-dimE, -dimB, dimH1),vect(dimE, -dimA, dimH2), 0xf3);
				break;
			case 13: // inner bottom left
				scube(vect(-dimB, -dimB, dimH1),vect(dimE, -dimA, dimH2), 0xfb);
				scube(vect(-dimB, -dimA, dimH1),vect(-dimA, dimE, dimH2), 0xfc);
				break;
			case 14: // inner bottom right
				scube(vect(-dimE, -dimB, dimH1),vect(dimB, -dimA, dimH2), 0xf7);
				scube(vect(dimA, -dimA, dimH1),vect(dimB, dimE, dimH2), 0xfc);
				break;
			default:
				break;
			}
			if (tile & 1) { // bit 0x00000001 ==> walkable ground, put a floor
				drawTile();
			}
			
			glPopMatrix();
		}
		glPopMatrix();
	} //for loop
	glEndList();

}

void MapEnvironment::drawTile() {
	TextureHandler* texh = TextureHandler::getInstance();
	
	if (texh->getTexture())
	{
		glColor3dv(vect::ONE);
		TextureHandler::getInstance()->use("rust");
	}
	else
		glColor3d(1.0,0.5,0.25);

	
	// Main tile
	scube( vect(-8,-8,-18), vect(8,8,-15) );
	
	if (texh->getTexture()) {
		glColor3dv(vect::ONE);
		TextureHandler::getInstance()->use("blood");
	}
	else
		glColor3d(0.5,0,0);

	// Horizontal strips, from top to bottom
	scube( vect(-4,5,-15), vect(4,7,-14.5), 0x1f);
	scube( vect(-4,-1,-15), vect(4,1,-14.5), 0x1f);
	scube( vect(-4,-7,-15), vect(4,-5,-14.5), 0x1f);

	//Vertical strips: Top left, bottom left, bottom right
	scube( vect(-6,1,-15), vect(-4,5,-14.5), 0x1f);
	scube( vect(-6,-5,-15), vect(-4,-1,-14.5), 0x1f);
	scube( vect(4,-5,-15), vect(6,-1,-14.5), 0x1f);
}

// draw a scaled cube bounded by low and high, faces is a mask for which faces we really need
void MapEnvironment::scube(vect low, vect high, int faces) {

	glBegin(GL_QUADS);
	if ((faces & 0x02)) {
	glNormal3dv(vect::BACKWARD);
	glTexCoord2d( low.x /16.0, - high.z /16.0); glVertex3d(low.x, low.y, high.z);
	glTexCoord2d( low.x /16.0, - low.z /16.0); glVertex3d(low.x, low.y, low.z);
	glTexCoord2d( high.x /16.0, - low.z /16.0); glVertex3d(high.x, low.y, low.z);
	glTexCoord2d( high.x /16.0, - high.z /16.0); glVertex3d(high.x, low.y, high.z);
	}

	if ((faces & 0x04)) {
	glNormal3dv(vect::RIGHT);
	glTexCoord2d( low.y /16.0, - high.z /16.0); glVertex3d(high.x, low.y, high.z);
	glTexCoord2d( low.y /16.0, - low.z /16.0); glVertex3d(high.x, low.y, low.z);
	glTexCoord2d( high.y /16.0, - low.z /16.0); glVertex3d(high.x, high.y, low.z);
	glTexCoord2d( high.y /16.0, - high.z /16.0); glVertex3d(high.x, high.y, high.z);
	}

	if ((faces & 0x01)) {
	glNormal3dv(vect::FORWARD);
	glTexCoord2d( high.x /16.0, - high.z /16.0); glVertex3d(high.x, high.y, high.z);
	glTexCoord2d( high.x /16.0, - low.z /16.0); glVertex3d(high.x, high.y, low.z);
	glTexCoord2d( low.x /16.0, - low.z /16.0); glVertex3d(low.x, high.y, low.z);
	glTexCoord2d( low.x /16.0, - high.z /16.0); glVertex3d(low.x, high.y, high.z);
	}

	if ((faces & 0x08)) {
	glNormal3dv(vect::LEFT);
	glTexCoord2d( high.y /16.0, - high.z /16.0); glVertex3d(low.x, high.y, high.z);
	glTexCoord2d( high.y /16.0, - low.z /16.0); glVertex3d(low.x, high.y, low.z);
	glTexCoord2d( low.y /16.0, - low.z /16.0); glVertex3d(low.x, low.y, low.z);
	glTexCoord2d( low.y /16.0, - high.z /16.0); glVertex3d(low.x, low.y, high.z);
	}

	if ((faces & 0x10)) {
	glNormal3dv(vect::UP);
	glTexCoord2d( low.x /16.0, - low.y /16.0); glVertex3d(low.x, low.y, high.z);
	glTexCoord2d( high.x /16.0, - low.y /16.0); glVertex3d(high.x, low.y, high.z);
	glTexCoord2d( high.x /16.0, - high.y /16.0); glVertex3d(high.x, high.y, high.z);
	glTexCoord2d( low.x /16.0, - high.y /16.0); glVertex3d(low.x, high.y, high.z);
	}

	if ((faces & 0x20)) {
	glNormal3dv(vect::DOWN);
	glTexCoord2d( low.x /16.0, - high.y /16.0); glVertex3d(low.x, high.y, low.z);
	glTexCoord2d( high.x /16.0, - high.y /16.0); glVertex3d(high.x, high.y, low.z);
	glTexCoord2d( high.x /16.0, - low.y /16.0); glVertex3d(high.x, low.y, low.z);
	glTexCoord2d( low.x /16.0, - low.y /16.0); glVertex3d(low.x, low.y, low.z);
	}
	glEnd();
}

// map tiles to vect's
vect MapEnvironment::VectFromTile(const MapTile& t) {
        int mapHeight = gamemap.size();
        int mapWidth = gamemap[0].size();

	return vect((t.x - mapWidth / 2) * 16, -(t.y - mapHeight / 2) * 16, 0);
}

// map vect's to tiles
MapTile MapEnvironment::TileFromVect(const vect& v) {
        int mapHeight = gamemap.size();
        int mapWidth = gamemap[0].size();

	MapTile ret;
	ret.x = (int)(mapWidth / 2 + (v.x + 8) / 16);
	ret.y = (int)(mapHeight / 2 - (v.y - 8) / 16);
	return ret;
}

// get the map value of a particular tile
int MapEnvironment::getTile(const MapTile& t) {
	if ((t.y >= 0) && (t.x >= 0) && (t.y < gamemap.size()) && (t.x < gamemap[0].size()))
		return gamemap[t.y][t.x];
	return 0;
}
