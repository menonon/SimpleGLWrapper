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
#include <glm/gtc/matrix_transform.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <boost/regex.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "texture.hpp"
#include "geometry.hpp"
#include "shader.hpp"

class MarchingCubesTF{

public:
  MarchingCubesTF();
  ~MarchingCubesTF();
  
  void setData(Texture<GLfloat>* aTexture);
  void generateGeometry();  
  void initTransformFeedback();

  void setThreshold(GLfloat aPercentage);

  void bind();
  void unbind();

  void render(glm::mat4 aProjection, glm::mat4 aView);


private:

  Texture<GLfloat>* mData;
  Geometry* mCells;
  static Shader* mMCShaderTransformFeedback;
  static Shader* mMCShaderRender;

  glm::ivec3 mSlices;
  glm::mat4 mModel;
  GLfloat mThreshold;
  glm::vec3 mColor;

  bool mIsThresholdChanged;

  static bool isInitialized;
  static GLint* mEdgeConnection;
  static GLint* mEdgeFlags;
  static Texture<GLint>* mTexEdgeConnection;

  GLuint mTransformFeedbackVBOID;
  GLuint mTransformFeedbackVBONormalID;
  GLuint mTransformFeedbackObjectID;
  

  void staticInit();
  void initTables();
  void initBuffers();
  void initShader();



};
