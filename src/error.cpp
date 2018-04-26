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

#include "error.hpp"

std::string glGetErrorToString() {
	GLenum err = glGetError();
	std::string errstr;

	switch (err) {
	case GL_NO_ERROR: errstr = "GL_NO_ERROR"; break;
	case GL_INVALID_ENUM: errstr = "GL_INVALID_ENUM"; break;
	case GL_INVALID_VALUE: errstr = "GL_INVALID_VALUE"; break;
	case GL_INVALID_OPERATION: errstr = "GL_INVALID_OPERATION"; break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: errstr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
	case GL_OUT_OF_MEMORY: errstr = "GL_OUT_OF_MEMORY"; break;
	case GL_STACK_UNDERFLOW: errstr = "GL_STACK_UNDERFLOW"; break;
	case GL_STACK_OVERFLOW: errstr = "GL_STACK_OVERFLOW"; break;
	default: errstr = "UNKNOWN_ERROR_TYPE";
	}
	return errstr;
}

void glGetErrorToStdOut() {
	std::cout << glGetErrorToString() << std::endl;
}

void glGetErrorToStdOut(const char* aFile, int aLine) {

	std::cout << aFile << ":" << aLine << ": " << glGetErrorToString() << std::endl;
}

void debugCallback(GLenum aSource, GLenum aType, GLuint aID, GLenum aSeverity, GLsizei aLength, const GLchar* aMessage, const void* aParam) {

	std::string source;
	switch (aSource) {
	case GL_DEBUG_SOURCE_API: source = "GL_DEBUG_SOURCE_API"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: source = "GL_DEBUG_SOURCE_SHADER_COMPILER"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source = "GL_DEBUG_SOURCE_WINDOW_SYSTEM"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: source = "GL_DEBUG_SOURCE_THIRD_PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION: source = "GL_DEBUG_SOURCE_APPLICATION"; break;
	case GL_DEBUG_SOURCE_OTHER: source = "GL_DEBUG_SOURCE_OTHER"; break;
	default: source = "UNKNOWN_DEBUG_SOURCE";
	}

	std::string type;
	switch (aType) {
	case GL_DEBUG_TYPE_ERROR: type = "GL_DEBUG_TYPE_ERROR"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: type = "GL_DEBUG_TYPE_PERFORMANCE"; break;
	case GL_DEBUG_TYPE_PORTABILITY: type = "GL_DEBUG_TYPE_PORTABILITY"; break;
	case GL_DEBUG_TYPE_MARKER: type = "GL_DEBUG_TYPE_MARKER"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: type = "GL_DEBUG_TYPE_PUSH_GROUP"; break;
	case GL_DEBUG_TYPE_POP_GROUP: type = "GL_DEBUG_TYPE_POP_GROUP"; break;
	case GL_DEBUG_TYPE_OTHER: type = "GL_DEBUG_TYPE_OTHER"; break;
	default: type = "UNKNOWN_DEBUG_TYPE";
	}

	std::string sev;
	switch (aSeverity) {
	case GL_DEBUG_SEVERITY_LOW: sev = "GL_DEBUG_SEVERITY_LOW"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: sev = "GL_DEBUG_SEVERITY_MEDIUM"; break;
	case GL_DEBUG_SEVERITY_HIGH: sev = "GL_DEBUG_SEVERITY_HIGH"; break;
	default: sev = "UNKNOWN_DEBUG_SEVERITY";
	}

	std::cout << "===== OpenGL Debug Message Start =====" << std::endl;
	std::cout << "Source: " << source << std::endl;
	std::cout << "Type: " << type << std::endl;
	std::cout << "Severity: " << sev << std::endl;
	std::cout << "ID: " << aID << std::endl;
	std::cout << "Message(" << aLength << "): " << aMessage << std::endl;
	std::cout << "===== OpenGL Debug Message End =====" << std::endl;
}



void initDebugMessage() {

	glDebugMessageCallback((GLDEBUGPROC)debugCallback, nullptr);

}






