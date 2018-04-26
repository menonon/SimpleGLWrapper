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


#include "movietexture.hpp"

bool MovieTexture::isInitialized = false;

MovieTexture::MovieTexture(std::string filename) {


	pFormatCtx = NULL;
	pCodecCtxOrig = NULL;
	pCodecCtx = NULL;
	pCodec = NULL;
	pFrame = NULL;
	pFrameRGB = NULL;
	buffer = NULL;
	sws_ctx = NULL;

	initMovieTexture(filename);

}

MovieTexture::~MovieTexture() {

	av_free(buffer);
	av_free(pFrameRGB);

	av_free(pFrame);

	avcodec_close(pCodecCtx);
	avcodec_close(pCodecCtxOrig);

	avformat_close_input(&pFormatCtx);


}


void MovieTexture::initMovieTexture(std::string filename) {
	/*
	  Reference is http://dranger.com/ffmpeg/tutorial01.html
	  How to use libav* and ffmpeg
	*/

	if (isInitialized) {

	}
	else {
		av_register_all();
		isInitialized = true;
	}

	if (avformat_open_input(&pFormatCtx, filename.c_str(), NULL, NULL) != 0) {
		std::cout << "Couldn't open file. (avformat_open_input)" << std::endl;
	}

	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		std::cout << "Couldn't find stream information. (avformat_find_stream_info)" << std::endl;
	}

	av_dump_format(pFormatCtx, 0, filename.c_str(), 0);

	videoStream = -1;

	for (int i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}

	if (videoStream == -1) {
		std::cout << "Dont't find video stream." << std::endl;
	}

	pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;

	pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
	if (pCodec == NULL) {
		std::cout << "Unsupported codec." << std::endl;
	}

	pCodecCtx = avcodec_alloc_context3(pCodec);

	if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
		std::cout << "Couldn't copy context. " << std::endl;
	}

	std::cout << "width:" << pCodecCtx->width << std::endl;

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		std::cout << "Couldn't open codec." << std::endl;
	}

	pFrame = av_frame_alloc();

	pFrameRGB = av_frame_alloc();
	if (pFrameRGB == NULL) {
		std::cout << "Couldn't alloc frame" << std::endl;
	}

	int numBytes;
	numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
	buffer = reinterpret_cast<uint8_t*>(av_malloc(numBytes * sizeof(uint8_t)));

	avpicture_fill(reinterpret_cast<AVPicture*>(pFrameRGB), buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

	std::cout << "MovieTexture constructer A" << std::endl;

	sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
	//sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
	//sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGBA, SWS_BILINEAR, NULL, NULL, NULL);

	imageBufferSize = pCodecCtx->width + pCodecCtx->height * 3;
	imageBuffer = new uint8_t[imageBufferSize];
	for (int i = 0; i < imageBufferSize; i++) {
		imageBuffer[i] = 1;
		std::cout << imageBuffer[i] << std::endl;
	}

	std::cout << "MovieTexture constructer finished" << std::endl;

}




void MovieTexture::readFrame() {

	int frameFinished;
	AVPacket packet;
	static int i = 0;

	av_read_frame(pFormatCtx, &packet);
	if (packet.stream_index == videoStream) {
		avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

		if (frameFinished) {
			sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
		}
	}

	//av_image_copy_to_buffer(imageBuffer, imageBufferSize, (uint8_t const * const *)(pFrameRGB->data), pFrameRGB->linesize, PIX_FMT_RGB24, pFrameRGB->width, pFrame->height, 1);

	//av_free_packet(&packet);

}


void MovieTexture::graphicsInit(int uniqueIndex) {

	textures[uniqueIndex] = new Texture<GLubyte>(Texture<GLubyte>::T_2D, GL_RGB8UI);

}


void MovieTexture::graphicsUpdate(int uniqueIndex) {

	textures[uniqueIndex]->bind();
	textures[uniqueIndex]->setImageData(pFrameRGB->data[0], pCodecCtx->width, pCodecCtx->height, 1, GL_RGB8UI);
	//textures[uniqueIndex]->setImageData(imageBuffer, pCodecCtx->width, pCodecCtx->height, 1, GL_RGB8UI);


}

void MovieTexture::bind(int uniqueIndex) {
	textures[uniqueIndex]->bind();
}

void MovieTexture::unbind(int uniqueIndex) {
	textures[uniqueIndex]->unbind();
}

GLint MovieTexture::getTextureUnit(int uniqueIndex) {

	return textures[uniqueIndex]->getTextureUnit();

}