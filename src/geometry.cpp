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

#include "geometry.hpp"

Geometry::Geometry(ePrimitiveMode primitiveMode) {
	mIsNormaled = false;
	mIsIndexed = false;
	mIsTextured = false;
	mIsVerticesChanged = false;
	mIsNormalsChanged = false;
	mIsTexCoordsChanged = false;
	mDimension = 3;

	mPrimitiveMode = primitiveMode;

}
Geometry::~Geometry() {}

void Geometry::setVertices(std::vector<glm::vec3>* vertices) {
	mIsVerticesChanged = true;
	mVertices = *vertices;
	glGenBuffers(1, &mVertexBufferID);

}

void Geometry::setNormals(std::vector<glm::vec3>* normals) {
	mIsNormalsChanged = true;
	mIsNormaled = true;
	mNormals = *normals;
	glGenBuffers(1, &mNormalBufferID);
}

void Geometry::setIndices(std::vector<GLuint>* indices) {
	mIsIndicesChanged = true;
	mIsIndexed = true;
	mIndices = *indices;
	glGenBuffers(1, &mIndexBufferID);
}

void Geometry::setTexCoords(std::vector<glm::vec3>* texCoords) {
	mIsTexCoordsChanged = true;
	mIsTextured = true;
	mTexCoords = *texCoords;
	glGenBuffers(1, &mTexCoordBufferID);
}

std::vector<glm::vec3>& Geometry::getVertices() { return mVertices; }
std::vector<glm::vec3>& Geometry::getNormals() { return mNormals; }
std::vector<GLuint>& Geometry::getIndices() { return mIndices; }
std::vector<glm::vec3>& Geometry::getTexCoords() { return mTexCoords; }

Geometry::ePrimitiveMode Geometry::getPrimitiveMode() {
	return mPrimitiveMode;
}

void Geometry::bind() {

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	if (mIsVerticesChanged) {
		glBufferData(GL_ARRAY_BUFFER, mDimension*mVertices.size() * sizeof(GLfloat), &mVertices[0][0], GL_STATIC_DRAW);
		mIsVerticesChanged = false;
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (mIsNormaled) {
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mNormalBufferID);
		if (mIsNormalsChanged) {
			glBufferData(GL_ARRAY_BUFFER, mDimension*mNormals.size() * sizeof(GLfloat), &mNormals[0][0], GL_STATIC_DRAW);
			mIsNormalsChanged = false;
		}
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (mIsTextured) {
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferID);
		int texDim = 3;

		if (mIsTexCoordsChanged) {
			glBufferData(GL_ARRAY_BUFFER, texDim*mTexCoords.size() * sizeof(GLfloat), &mTexCoords[0][0], GL_STATIC_DRAW);
			mIsTexCoordsChanged = false;
		}
		glVertexAttribPointer(2, texDim, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (mIsIndexed) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
		if (mIsIndicesChanged) {
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
			mIsIndicesChanged = false;
		}
	}

}

void Geometry::bind_legacy() {


	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	if (mIsVerticesChanged) {
		glBufferData(GL_ARRAY_BUFFER, mDimension*mVertices.size() * sizeof(GLfloat), &mVertices[0][0], GL_STATIC_DRAW);
		mIsVerticesChanged = false;
	}
	glVertexPointer(3, GL_FLOAT, 0, 0);


	if (mIsNormaled) {

		glBindBuffer(GL_ARRAY_BUFFER, mNormalBufferID);
		if (mIsNormalsChanged) {
			glBufferData(GL_ARRAY_BUFFER, mDimension*mNormals.size() * sizeof(GLfloat), &mNormals[0][0], GL_STATIC_DRAW);
			mIsNormalsChanged = false;
		}
		glNormalPointer(GL_FLOAT, 0, 0);

	}

	if (mIsTextured) {

		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferID);
		int texDim = 3;

		if (mIsTexCoordsChanged) {
			glBufferData(GL_ARRAY_BUFFER, texDim*mTexCoords.size() * sizeof(GLfloat), &mTexCoords[0][0], GL_STATIC_DRAW);
			mIsTexCoordsChanged = false;
		}
		glTexCoordPointer(3, GL_FLOAT, 0, 0);

	}

	if (mIsIndexed) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
		if (mIsIndicesChanged) {
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
			mIsIndicesChanged = false;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


}

void Geometry::unbind() {

	glDisableVertexAttribArray(0);

	if (mIsNormaled) {
		glDisableVertexAttribArray(1);
	}

	if (mIsTextured) {
		glDisableVertexAttribArray(2);
	}

	if (mIsIndexed) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
void Geometry::unbind_legacy() {

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}



void Geometry::draw() {

	if (mIsIndexed) {
		glDrawElements(static_cast<GLenum>(mPrimitiveMode), mIndices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(static_cast<GLenum>(mPrimitiveMode), 0, mVertices.size());
	}

}
