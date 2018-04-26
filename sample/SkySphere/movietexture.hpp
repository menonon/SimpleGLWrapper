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
#include "error.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <string>
#include <iostream>

class MovieTexture {

public:
	MovieTexture(std::string filename);
	~MovieTexture();

	void readFrame();
	void graphicsInit(int uniqueIndex);
	void graphicsUpdate(int uniqueIndex);
	void bind(int uniqueIndex);
	void unbind(int uniqueIndex);
	GLint getTextureUnit(int uniqueIndex);

private:
	static bool isInitialized;

	void initMovieTexture(std::string filename);


	AVFormatContext *pFormatCtx;
	int videoStream;
	AVCodecContext *pCodecCtxOrig;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	AVFrame *pFrame;
	AVFrame *pFrameRGB;
	uint8_t *buffer;
	struct SwsContext *sws_ctx;

	std::map<int, Texture<GLubyte>* > textures;

	uint8_t *imageBuffer;
	int imageBufferSize;

};
