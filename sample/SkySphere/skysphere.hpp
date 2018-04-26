/*
The MIT License (MIT)

Copyright (c) 2013-2018 menonon

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "texture.hpp"
#include "geometry.hpp"
#include "movietexture.hpp"

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <cmath>
#include <map>

#include <boost/regex.hpp>

class SkySphere {

public:
	SkySphere(int u, int v, double r, bool flip_u, bool flip_v, bool flip_normal, bool flip_surf, std::string filename);
	~SkySphere();

	void graphicsInit(int uniqueIndex);
	void draw(int uniqueIndex);
	void update();

	GLint getTextureUnit(int uniqueIndex);

private:

	void createUVSphere(int u, int v, double r, bool flip_u, bool flip_v, bool flip_normal, bool flip_surf);

	std::string filename;


	std::map<int, Texture<GLfloat>* > textures;
	MovieTexture* movieTexture;
	bool isMovieTexture;

	std::map<int, Geometry*> geometries;

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> texcoords;



};
