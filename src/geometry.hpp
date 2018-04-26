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

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>

class Geometry{

public:
  
  enum ePrimitiveMode{ POINTS = GL_POINTS,
		       
		       LINES = GL_LINES,
		       LINE_S = GL_LINE_STRIP,
		       LINE_L = GL_LINE_LOOP,
		       LINES_A = GL_LINES_ADJACENCY,
		       LINE_S_A = GL_LINE_STRIP_ADJACENCY,

		       TRIANGLES = GL_TRIANGLES,
		       TRIANGLE_S = GL_TRIANGLE_STRIP,
		       TRIANGLE_F = GL_TRIANGLE_FAN,
		       TRIANGLES_A = GL_TRIANGLES_ADJACENCY,
		       TRIANGLE_S_A = GL_TRIANGLE_STRIP_ADJACENCY,
               PATCHES = GL_PATCHES 
  };
  
  Geometry(ePrimitiveMode primitiveMode);
  ~Geometry();

  void setVertices(std::vector<glm::vec3>* vertices);
  void setNormals(std::vector<glm::vec3>* normals);
  void setIndices(std::vector<GLuint>* indices);
  void setTexCoords(std::vector<glm::vec3>* texCoords);

  std::vector<glm::vec3>& getVertices();
  std::vector<glm::vec3>& getNormals();
  std::vector<GLuint>& getIndices(); 
  std::vector<glm::vec3>& getTexCoords();

  ePrimitiveMode getPrimitiveMode();
  
  void bind();
  void bind_legacy();
  void unbind();
  void unbind_legacy();
  
  void draw();

private:
  int mDimension;
  
  bool mIsIndexed;
  bool mIsNormaled;
  bool mIsTextured;

  bool mIsVerticesChanged;
  bool mIsNormalsChanged;
  bool mIsIndicesChanged;
  bool mIsTexCoordsChanged;

  ePrimitiveMode mPrimitiveMode;
  GLuint mVertexBufferID;
  GLuint mNormalBufferID;
  GLuint mTexCoordBufferID;
  GLuint mIndexBufferID;

  std::vector<glm::vec3> mVertices;
  std::vector<glm::vec3> mNormals;
  std::vector<GLuint> mIndices;
  std::vector<glm::vec3> mTexCoords;

};
