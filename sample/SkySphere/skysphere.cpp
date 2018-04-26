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


#include "skysphere.hpp"

SkySphere::SkySphere(int u, int v, double r, bool flip_u, bool flip_v, bool flip_normal, bool flip_surf, std::string filename)
{

	createUVSphere(u, v, r, flip_u, flip_v, flip_normal, flip_surf);
	this->filename = filename;

	boost::regex re(".*(mp4|MP4|avi|AVI|wmv|WMV|mpeg|MPEG)$");
	if (boost::regex_match(filename, re))
	{
		isMovieTexture = true;
		movieTexture = new MovieTexture(filename);
		std::cout << "movie texture: " << filename << std::endl;
	}
	else
	{
		isMovieTexture = false;
		std::cout << "image texture: " << filename << std::endl;
	}
}

SkySphere::~SkySphere() {}

void SkySphere::graphicsInit(int uniqueIndex)
{

	if (isMovieTexture)
	{
		movieTexture->graphicsInit(uniqueIndex);
	}
	else
	{
		textures[uniqueIndex] = new Texture<GLfloat>(filename, Texture<GLfloat>::T_2D);
	}

	geometries[uniqueIndex] = new Geometry(Geometry::TRIANGLES);
	geometries[uniqueIndex]->setVertices(&vertices);
	geometries[uniqueIndex]->setTexCoords(&texcoords);
	geometries[uniqueIndex]->setIndices(&indices);
	geometries[uniqueIndex]->setNormals(&normals);
}

void SkySphere::draw(int uniqueIndex)
{

	if (isMovieTexture)
	{
		movieTexture->graphicsUpdate(uniqueIndex);
		movieTexture->bind(uniqueIndex);
	}
	else
	{
		textures[uniqueIndex]->bind();
	}

	geometries[uniqueIndex]->bind();
	geometries[uniqueIndex]->draw();
	geometries[uniqueIndex]->unbind();

	if (isMovieTexture)
	{
		movieTexture->unbind(uniqueIndex);
	}
	else
	{
		textures[uniqueIndex]->unbind();
	}
}

void SkySphere::update()
{

	if (isMovieTexture)
	{
		movieTexture->readFrame();
	}
}

GLint SkySphere::getTextureUnit(int uniqueIndex)
{

	if (isMovieTexture)
	{
		return movieTexture->getTextureUnit(uniqueIndex);
	}
	else
	{
		return textures[uniqueIndex]->getTextureUnit();
	}
}

void SkySphere::createUVSphere(int u, int v, double r, bool flip_u, bool flip_v, bool flip_normal, bool flip_surf)
{

	//vertices, normals, texcoords
	for (int i = 0; i < u + 1; i++)
	{
		for (int j = 0; j < v + 1; j++)
		{

			double x, y, z, a;
			double theta, phi;

			theta = static_cast<double>(i) / static_cast<double>(u) * 2 * M_PI;
			phi = static_cast<double>(j) / static_cast<double>(v) * M_PI;

			y = r * cos(phi);
			a = r * sin(phi);
			x = a * cos(theta);
			z = a * sin(theta);

			vertices.push_back(glm::vec3(x, y, z));

			if (flip_normal)
			{
				normals.push_back(glm::normalize(glm::vec3(-x, -y, -z)));
			}
			else
			{
				normals.push_back(glm::normalize(glm::vec3(x, y, z)));
			}

			double s, t;
			if (flip_u)
			{
				s = static_cast<double>(u - i) / static_cast<double>(u);
			}
			else
			{
				s = static_cast<double>(i) / static_cast<double>(u);
			}

			if (flip_v)
			{
				t = static_cast<double>(v - j) / static_cast<double>(v);
			}
			else
			{
				t = static_cast<double>(j) / static_cast<double>(v);
			}
			texcoords.push_back(glm::vec3(s, t, 0));
		}
	}

	//indices
	for (int i = 0; i < u; i++)
	{
		for (int j = 0; j < v; j++)
		{

			double a = i * (v + 1) + j;
			double b = a + 1;
			double c = a + (v + 1);
			double d = c + 1;

			if (flip_surf)
			{
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);

				indices.push_back(b);
				indices.push_back(d);
				indices.push_back(c);
			}
			else
			{
				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(b);

				indices.push_back(b);
				indices.push_back(c);
				indices.push_back(d);
			}
		}
	}
}
