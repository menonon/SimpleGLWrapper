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
#include <IL/il.h>
#include <IL/ilu.h>
#include <boost/regex.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

template<class DATATYPE>
class Texture{
public:
  
  enum eTextureTarget { T_1D = GL_TEXTURE_1D,
			T_2D = GL_TEXTURE_2D,
			T_3D = GL_TEXTURE_3D,
			T_RECT = GL_TEXTURE_RECTANGLE,
			T_1D_A = GL_TEXTURE_1D_ARRAY,
			T_2D_A = GL_TEXTURE_2D_ARRAY,
			T_CUBE_M = GL_TEXTURE_CUBE_MAP,
			T_CUBE_M_A = GL_TEXTURE_CUBE_MAP_ARRAY,
			T_BUFF = GL_TEXTURE_BUFFER,
			T_2D_MS = GL_TEXTURE_2D_MULTISAMPLE,
			T_2D_MS_A = GL_TEXTURE_2D_MULTISAMPLE_ARRAY };
  
  Texture(eTextureTarget target);
  Texture(eTextureTarget target, GLenum aInternal);

  Texture(std::string filename, eTextureTarget target);
  Texture(std::string filename, eTextureTarget target, GLenum aInternal);
  ~Texture();
  
  void setImageData(DATATYPE* imageData, int width, int height, int depth, GLenum aGLInternalDataType);
  void setTextureUnit(GLint unit);
  GLint getTextureUnit();
  void setInternalDataType(GLenum aIDT);

  void bind();
  void unbind();

  glm::ivec3 getSize();
  glm::vec2 getDataRange();

  void createSampler();
  void createSampler(GLint aMagFilter, GLint aMinFilter, GLint aWarp);

  
private:
  eTextureTarget mTextureTarget;
  GLuint mTextureID;
  GLuint mBufferID;
  GLint mTextureUnit;
  std::vector<ILuint> mImageIDs;
  DATATYPE* mImageData;
  int mWidth;
  int mHeight;
  int mDepth;

  DATATYPE mMinData;
  DATATYPE mMaxData;

  ILenum mILDataType;
  GLenum mGLDataType;
  GLenum mGLInternalDataType;
  GLenum mGLInternalDataTypeBase;
  GLint mNumColor;

  GLuint mSamplerID;

  void init();

  void loadImages(std::string filename_regex);

  void loadTexture();
  void loadTexture3D();
  void loadTexture2D();
  void loadTexture1D();
  void loadTextureBuff();
  void detectType();
  void detectInternalType();

  void setDataRange();
};

template<class DATATYPE>
void Texture<DATATYPE>::init(){
  mTextureID = 0;
  mBufferID= 0;
  mTextureUnit = 0;
  mSamplerID = 0;
  mTextureUnit = 0;
  
  mMinData = 0.0;
  mMaxData = 0.0;

}


template<class DATATYPE>
Texture<DATATYPE>::Texture(eTextureTarget target){

  init();
  mTextureTarget = target;
  mGLInternalDataType = GL_RGBA32F;

}


template<class DATATYPE>
Texture<DATATYPE>::Texture(eTextureTarget target, GLenum aInternal){

  init();
  mTextureTarget = target;
  mGLInternalDataType = aInternal;

}


template<class DATATYPE>
Texture<DATATYPE>::Texture(std::string filename, eTextureTarget target){

  init();
  mGLInternalDataType = GL_RGBA32F;
  loadImages(filename);
  setDataRange();

  mTextureTarget = target;

  loadTexture();
  
}

template<class DATATYPE>
Texture<DATATYPE>::Texture(std::string filename, eTextureTarget target, GLenum aInternal){

  init();
  mGLInternalDataType = aInternal;
  loadImages(filename);
  setDataRange();

  mTextureTarget = target;

  loadTexture();

}

template<class DATATYPE>
glm::ivec3 Texture<DATATYPE>::getSize(){
  return glm::ivec3(mWidth, mHeight,mDepth);
}

template<class DATATYPE>
glm::vec2 Texture<DATATYPE>::getDataRange(){
  return glm::vec2(mMinData, mMaxData);
}

template<class DATATYPE>
void Texture<DATATYPE>::setDataRange(){
  
  DATATYPE min=0;
  DATATYPE max=0;
  int arraySize = mWidth * mHeight * mDepth;
  min = max = mImageData[0];
  for(size_t i=1; i<arraySize; i++){
    if(mImageData[i] > max){
      max = mImageData[i];
    }
    if(min > mImageData[i]){
      min = mImageData[i];
    }
  }
  
  mMinData = min;
  mMaxData = max;

}


template<class DATATYPE>
void Texture<DATATYPE>::setImageData(DATATYPE* imageData, int width, int height, int depth, GLenum aGLInternalDataType){
  mImageData = imageData;
  mWidth = width;
  mHeight = height;
  mDepth = depth;
  mGLInternalDataType = aGLInternalDataType;

  setDataRange();

  if(glIsTexture(mTextureID))glDeleteTextures(1, &mTextureID);
  
  loadTexture();
}


template<class DATATYPE>
void Texture<DATATYPE>::loadImages(std::string filename_regex){
  detectType();
  detectInternalType();
  // search file series  
  namespace bf = boost::filesystem;
  bf::path fullpath(filename_regex);
  fullpath = bf::absolute(fullpath);
  std::cout << fullpath.parent_path().string() << std::endl;
  fullpath = bf::canonical(fullpath.parent_path()) /= fullpath.filename();
  boost::regex re(fullpath.filename().string());
  std::vector<bf::path> sequence;
  bf::directory_iterator end;
  for(bf::directory_iterator it = bf::directory_iterator(fullpath.parent_path()); it!=end; it++){
    if(boost::regex_match(it->path().filename().string(), re)){
		sequence.push_back(it->path());
      std::cout << "[Texture::loadImages] : " << it->path().filename().string() << std::endl;
    }
  }
  
  
  // load images
  ilInit();
  iluInit();
  std::cout << sequence.size() << std::endl;
  mImageIDs.resize(sequence.size());
  ilGenImages(sequence.size(), &mImageIDs[0]);
  for(size_t i=0;i<sequence.size();i++){
    std::cout << "imgID:" << mImageIDs[i] << std::endl;
    ilBindImage(mImageIDs[i]);
    ilLoadImage(sequence[i].string().c_str());
    ilBindImage(0);
  }
  ilBindImage(mImageIDs[0]);
  mWidth = ilGetInteger(IL_IMAGE_WIDTH);
  mHeight = ilGetInteger(IL_IMAGE_HEIGHT);
  //int imgDepth = ilGetInteger(IL_IMAGE_DEPTH); // DevIL does not support multi page tiff
  mDepth = sequence.size();
  std::cout << "w:h:d:s = " << mWidth << ":" << mHeight << ":" << imgDepth << ":" << mDepth << std::endl;


  int imageDepth = ilGetInteger(IL_IMAGE_DEPTH);
  int numColor  = 4;
  std::cout << "mNumColor:" << mNumColor << std::endl;
  ilBindImage(0);

  int imgSize = mWidth*mHeight*mDepth*numColor;
  mImageData = new DATATYPE[imgSize];
  for(size_t i=0; i<imgSize; i++){
    mImageData[i] = i/static_cast<float>(imgSize);
  }
  for(size_t i=0; i<sequence.size(); i++){
    ilBindImage(mImageIDs[i]);
    ilCopyPixels(0,0,0, mWidth,mHeight,1, IL_RGBA,mILDataType,&mImageData[mWidth*mHeight*i*numColor]);
    ilBindImage(0);
  }

  ilDeleteImages(sequence.size(), &mImageIDs[0]);
  
  
}

template<class DATATYPE>
void Texture<DATATYPE>::loadTexture(){
  detectType();
  detectInternalType();
  switch(mTextureTarget){
  case T_3D:
  case T_2D_A:
  case T_CUBE_M_A:
    loadTexture3D();
    createSampler();
    break;

  case T_2D:
  case T_1D_A:
  case T_RECT:
    loadTexture2D();
    createSampler();
    break;
    
  case T_1D:
    loadTexture1D();
    createSampler();
    break;

  case T_BUFF:
    loadTextureBuff();
    break;

  default:
    std::cout << "unsupported format" << std::endl;
    break;
  }


}

template<class DATATYPE>
void Texture<DATATYPE>::createSampler(GLint aMagFilter, GLint aMinFilter, GLint aWarp){
  if(glIsSampler(mSamplerID)){
    glDeleteSamplers(1, &mSamplerID);
  }
  glGenSamplers(1, &mSamplerID);
  glSamplerParameteri(mSamplerID, GL_TEXTURE_MAG_FILTER, aMagFilter);
  glSamplerParameteri(mSamplerID, GL_TEXTURE_MIN_FILTER, aMinFilter);
  glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_S, aWarp);
  glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_T, aWarp);
  glSamplerParameteri(mSamplerID, GL_TEXTURE_WRAP_R, aWarp);
}

template<class DATATYPE>
void Texture<DATATYPE>::createSampler(){
  
  createSampler(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);

}

template<class DATATYPE>
void Texture<DATATYPE>::loadTexture3D(){
  // gen glTexture
  GLenum gleTarget = static_cast<GLenum>(mTextureTarget);
  glGenTextures(1, &mTextureID);
  glBindTexture(gleTarget, mTextureID);
  glTexStorage3D(gleTarget, 1, mGLInternalDataType, mWidth, mHeight, mDepth);
  glTexSubImage3D(gleTarget, 0, 0, 0, 0, mWidth, mHeight, mDepth, mGLInternalDataTypeBase, mGLDataType, mImageData);

}

template<class DATATYPE>
void Texture<DATATYPE>::loadTexture2D(){
  // gen glTexture
  GLenum gleTarget = static_cast<GLenum>(mTextureTarget);
  glGenTextures(1, &mTextureID);
  glBindTexture(gleTarget, mTextureID);

  switch(mTextureTarget){
  case T_2D:
  case T_RECT:
    glTexStorage2D(gleTarget, 1, mGLInternalDataType, mWidth, mHeight);
    glTexSubImage2D(gleTarget, 0, 0, 0, mWidth, mHeight, mGLInternalDataTypeBase, mGLDataType, mImageData);
    break;
  case T_1D_A:
    glTexStorage2D(gleTarget, 1, mGLInternalDataType, mWidth*mHeight, mDepth);
    glTexSubImage2D(gleTarget, 0, 0, 0, mWidth*mHeight, mDepth, mGLInternalDataTypeBase, mGLDataType, mImageData);
    break;
  default:
    break;
  }
}

template<class DATATYPE>
void Texture<DATATYPE>::loadTexture1D(){
  // gen glTexture
  GLenum gleTarget = static_cast<GLenum>(mTextureTarget);
  glGenTextures(1, &mTextureID);
  glBindTexture(gleTarget, mTextureID);
  glTexStorage1D(gleTarget, 1, mGLInternalDataType, mWidth*mHeight);
  glTexSubImage1D(gleTarget, 0, 0, mWidth*mHeight, mGLInternalDataTypeBase, mGLDataType, mImageData);
  
}

template<class DATATYPE>
void Texture<DATATYPE>::loadTextureBuff(){

  glGenBuffers(1, &mBufferID);
  glBindBuffer(GL_TEXTURE_BUFFER, mBufferID);
  //int numColor = 4;
  glBufferData(GL_TEXTURE_BUFFER, mWidth*mHeight*mDepth*mNumColor*sizeof(DATATYPE), mImageData, GL_STATIC_DRAW);
  
  glGenTextures(1, &mTextureID);
  glActiveTexture(GL_TEXTURE0 + mTextureUnit);
  glBindTexture(GL_TEXTURE_BUFFER, mTextureID);
  glTexBuffer(GL_TEXTURE_BUFFER, mGLInternalDataType, mBufferID);
}

template<class DATATYPE>
void Texture<DATATYPE>::bind(){
  glActiveTexture(GL_TEXTURE0 + mTextureUnit);
  glBindTexture(static_cast<GLenum>(mTextureTarget), mTextureID);
  if(mTextureTarget != T_BUFF){
    glBindSampler(mTextureUnit, mSamplerID);
  }else{
    glBindBuffer(GL_TEXTURE_BUFFER, mBufferID);
  }
}

template<class DATATYPE>
void Texture<DATATYPE>::setTextureUnit(GLint unit){
  GLint maxUnits;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);
  if(unit < maxUnits){
    mTextureUnit = unit;
  }else{
    std::cout << "over GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS" << std::endl;
  }
}

template<class DATATYPE>
GLint Texture<DATATYPE>::getTextureUnit(){
  return mTextureUnit;
}

template<class DATATYPE>
Texture<DATATYPE>::~Texture(){
  
  delete [] mImageData;

}

template<class DATATYPE>
void Texture<DATATYPE>::unbind(){
  glActiveTexture(GL_TEXTURE0 + mTextureUnit);
  glBindTexture(static_cast<GLenum>(mTextureTarget), 0);
  glBindSampler(mTextureUnit, 0);
  glActiveTexture(GL_TEXTURE0);
}

template<class DATATYPE>
void Texture<DATATYPE>::detectType(){
  const std::type_info& ti = typeid(DATATYPE);
  //std::cout << ti.name() << std::endl;
  if(ti == typeid(float)){
    mGLDataType = GL_FLOAT;
    mILDataType = IL_FLOAT;
  }else if(ti == typeid(double)){
    mGLDataType = GL_DOUBLE;
    mILDataType = IL_DOUBLE;
  }else if(ti == typeid(unsigned int)){
    mGLDataType = GL_UNSIGNED_INT;
    mILDataType = IL_UNSIGNED_INT;
  }else if(ti == typeid(int)){
    mGLDataType = GL_INT;
    mILDataType = IL_INT;
  }else if(ti == typeid(short)){
    mGLDataType = GL_SHORT;
    mILDataType = IL_SHORT;
  }else if(ti == typeid(unsigned short)){
    mGLDataType = GL_UNSIGNED_SHORT;
    mILDataType = IL_UNSIGNED_SHORT;
  }else if(ti == typeid(unsigned char)){
    mGLDataType = GL_UNSIGNED_BYTE;
    mILDataType = IL_UNSIGNED_BYTE;
  }else if(ti == typeid(char)){
    mGLDataType = GL_BYTE;
    mILDataType = IL_BYTE;
  }else{
    std::cout << "unsupported DataType: " << ti.name() << std::endl;
  }
}

template<class DATATYPE>
void Texture<DATATYPE>::detectInternalType(){
  
  switch(mGLInternalDataType){
  case GL_R8 :
  case GL_R8_SNORM :
  case GL_R16 :
  case GL_R16_SNORM :
  case GL_R16F :
  case GL_R32F :
  case GL_R8I :
  case GL_R8UI :
  case GL_R16I :
  case GL_R16UI :
  case GL_R32I :
  case GL_R32UI : 
    mGLInternalDataTypeBase = GL_RED;
    mNumColor = 1;
    break;
  
  case GL_RG8 :
  case GL_RG8_SNORM :
  case GL_RG16 :
  case GL_RG16_SNORM :
  case GL_RG16F :
  case GL_RG32F :
  case GL_RG8I :
  case GL_RG8UI :
  case GL_RG16I :
  case GL_RG16UI :
  case GL_RG32I :
  case GL_RG32UI :
    mGLInternalDataTypeBase = GL_RG;
    mNumColor = 2;
    break;

  case GL_R3_G3_B2 :
  case GL_RGB4 :
  case GL_RGB5 :
  case GL_RGB8 :
  case GL_RGB8_SNORM :
  case GL_RGB10 :
  case GL_RGB12 :
  case GL_RGB16_SNORM :
  case GL_RGBA2 :
  case GL_RGBA4 :
  case GL_SRGB8 :
  case GL_RGB16F :
  case GL_RGB32F :
  case GL_R11F_G11F_B10F :
  case GL_RGB9_E5 :
	mGLInternalDataTypeBase = GL_RGB;
	mNumColor = 3;
	break;

  case GL_RGB8I :
  case GL_RGB8UI :
  case GL_RGB16I :
  case GL_RGB16UI :
  case GL_RGB32I :
  case GL_RGB32UI :
	mGLInternalDataTypeBase = GL_RGB_INTEGER;
	mNumColor = 3;
	break;

  case GL_RGB5_A1 :
  case GL_RGBA8 :
  case GL_RGBA8_SNORM :
  case GL_RGB10_A2 :
  case GL_RGB10_A2UI :
  case GL_RGBA12 :
  case GL_RGBA16 :
  case GL_SRGB8_ALPHA8 :
  case GL_RGBA16F :
  case GL_RGBA32F :
  case GL_RGBA8I :
  case GL_RGBA8UI :
  case GL_RGBA16I :
  case GL_RGBA16UI :
  case GL_RGBA32I :
  case GL_RGBA32UI :
    mGLInternalDataTypeBase = GL_RGBA;
    mNumColor = 4;
    break;
    
  default: 
    std::cout << "unsupported internalDataType " << std::endl;
    
 }
  //std::cout << "internalFormat:" << mGLInternalDataTypeBase << ", " << mNumColor << std::endl;
  
}
