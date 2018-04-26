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

#include "shader.hpp"

Shader::Shader() {
	mLinked = false;
	mIsTransformFeedback = false;
	mIsPatchParameter = false;
}


Shader::~Shader() {
	glDeleteProgram(mProgramID);
}

void Shader::compileShader(eShaderType shaderType, std::string filename) {
	compileShader(static_cast<GLenum>(shaderType), filename);
}

void Shader::compileShader(GLenum shaderType, std::string filename) {
	GLuint shaderID;
	shaderID = glCreateShader(shaderType);

	std::string shaderCode;
	std::ifstream shaderStream(filename.c_str());
	std::string linebuffer;
	while (std::getline(shaderStream, linebuffer)) {
		shaderCode += (linebuffer + "\n");
	}
	shaderStream.close();
	//std::cout << shaderCode << std::endl;
	char const * sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	// error check
	GLint result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	std::cout << "[shader compile status]" << shaderID << ":" << filename << " : " << getStatus(result) << std::endl;
	int infoLogLength;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> shaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
		std::cout << &shaderErrorMessage[0] << std::endl;
	}

	mShaderIDs.push_back(shaderID);

}

void Shader::setProgramParameter(GLenum pname, GLint value) {
	mProgramParameters[pname] = value;

}

void Shader::setTransformFeedbackVaryings(const char ** varyings) {
	mTransformFeedbackVaryings = varyings;
	mIsTransformFeedback = true;
}

void Shader::setPatchParameter(GLint vertices) {
	mPatchVertices = vertices;
	mIsPatchParameter = true;

}

void Shader::linkProgram() {

	if (mLinked == true) {
		return;
	}

	GLuint programID;
	programID = glCreateProgram();

	for (size_t i = 0; i < mShaderIDs.size(); i++) {
		glAttachShader(programID, mShaderIDs[i]);
	}
	if (!mProgramParameters.empty()) {
		glProgramParameteri(programID, GL_GEOMETRY_INPUT_TYPE, mProgramParameters[GL_GEOMETRY_INPUT_TYPE]);
		glProgramParameteri(programID, GL_GEOMETRY_OUTPUT_TYPE, mProgramParameters[GL_GEOMETRY_OUTPUT_TYPE]);
		glProgramParameteri(programID, GL_GEOMETRY_VERTICES_OUT, mProgramParameters[GL_GEOMETRY_VERTICES_OUT]);
	}
	if (mIsTransformFeedback) {
		//glTransformFeedbackVaryings(programID, sizeof(mTransformFeedbackVaryings) / sizeof(mTransformFeedbackVaryings[0]), mTransformFeedbackVaryings, GL_INTERLEAVED_ATTRIBS);
		//glTransformFeedbackVaryings(programID, sizeof(mTransformFeedbackVaryings) / sizeof(mTransformFeedbackVaryings[0]), mTransformFeedbackVaryings, GL_SEPARATE_ATTRIBS);
		glTransformFeedbackVaryings(programID, 2, mTransformFeedbackVaryings, GL_SEPARATE_ATTRIBS);

		//std::cout << "glTransformFeedbackVaryings:" << sizeof(mTransformFeedbackVaryings) / sizeof(mTransformFeedbackVaryings[0]) << std::endl;
		//std::cout << "varings: " << mTransformFeedbackVaryings[1] << std::endl;

	}
	glLinkProgram(programID);

	GLint result;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	std::cout << "[program link status]" << programID << " : " << getStatus(result) << std::endl;
	int infoLogLength;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		std::cout << &programErrorMessage[0] << std::endl;
	}

	for (size_t i = 0; i < mShaderIDs.size(); i++) {
		glDeleteShader(mShaderIDs[i]);
	}

	mLinked = true;
	mProgramID = programID;

}

void Shader::use() {
	if (mLinked == false)return;

	if (mIsPatchParameter) {
		glPatchParameteri(GL_PATCH_VERTICES, mPatchVertices);
	}
	glUseProgram(mProgramID);
}

void Shader::setUniform(const char* name, const glm::mat4& mat) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(const char* name, const glm::mat3& mat) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
}


void Shader::setUniform(const char* name, const glm::vec3& vec) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::setUniform(const char* name, const glm::ivec3& vec) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniform3i(location, vec.x, vec.y, vec.z);
}

void Shader::setUniform(const char* name, const glm::vec4& vec) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniform(const char* name, const glm::ivec4& vec) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniform4i(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniform(const char* name, int val) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniform1i(location, val);
}


void Shader::setUniform(const char* name, float val) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniform1f(location, val);
}

void Shader::setUniform(const char* name, bool val) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniform1i(location, val);
}

void Shader::setUniform(const char* name, GLuint val) {
	GLuint location = glGetUniformLocation(mProgramID, name);
	glUniform1ui(location, val);
}

std::string Shader::getStatus(GLint status) {
	if (status)return std::string("Succeeded");
	else return std::string("Failed");
}

void Shader::unuse() {

	glUseProgram(0);

}

GLuint Shader::getProgramID() {
	return mProgramID;
}
