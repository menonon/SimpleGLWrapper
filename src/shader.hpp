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

/**
   
   usege:
   Shader* shader = new Shader();

   shader.compileShader(GL_VERTEX_SHADER, "sample.vert");
   shader.compileShader(GL_GEOMETRY_SHADER, "sample.geom");
   shader.compileShader(GL_FRAGMENT_SHADER, "sample.frag");

   shader.setProgramParameter(GL_GEOMETRY_INPUT_TYPE, xxxxx);
   shader.setProgramParameter(GL_GEOMETRY_OUTPUT_TYPE, xxxxx);
   shader.setProgramParameter(GL_GEOMETRY_VERTICES_OUT, xxxxx);
   
   shader.linkProgram();

   shader.use();
   shader.setUniform();   
 **/
class Shader{
public:
  Shader();
  ~Shader();

  enum eShaderType  { VERT=GL_VERTEX_SHADER, 
		      TESC=GL_TESS_CONTROL_SHADER,
		      TESE=GL_TESS_EVALUATION_SHADER,
		      GEOM=GL_GEOMETRY_SHADER,
		      COMP=GL_COMPUTE_SHADER };

  void compileShader(GLenum shaderType, std::string filename);
  void compileShader(eShaderType shaderType, std::string filename);
  void setProgramParameter(GLenum pname, GLint value);
  void setTransformFeedbackVaryings(const char** varyings);
  void setPatchParameter(GLint vertices);
  void linkProgram();
  void use();
  void setUniform(const char* name, const glm::mat4& mat);
  void setUniform(const char* name, const glm::mat3& mat);
  void setUniform(const char* name, const glm::vec4& vec);
  void setUniform(const char* name, const glm::ivec4& vec);
  void setUniform(const char* name, const glm::vec3& vec);
  void setUniform(const char* name, const glm::ivec3& vec);
  void setUniform(const char* name, int val);
  void setUniform(const char* name, float val);
  void setUniform(const char* name, bool val);
  void setUniform(const char* name, GLuint val);
  GLuint getProgramID();

  static void unuse();

private:
  std::vector<GLuint> mShaderIDs;
  GLuint mProgramID;
  std::map<std::string,int> mUniformLocations;
  std::map<GLenum,GLint> mProgramParameters;
  const char** mTransformFeedbackVaryings;
  bool mIsTransformFeedback;
  GLint mPatchVertices;
  bool mIsPatchParameter;

  bool mLinked;

  std::string getStatus(GLint status);


};
