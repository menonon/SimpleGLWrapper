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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "shader.hpp"
#include "texture.hpp"
#include "geometry.hpp"
#include "VAO.hpp"
#include "marchingcubes_tf.hpp"
#include "error.hpp"

bool geomON = false;
bool rotON = false;



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		geomON = !geomON;
		std::cout << geomON << std::endl;
	}
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		rotON = !rotON;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {

	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {

	}

}

float gScrX = -1.5;
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	gScrX += static_cast<float>(xoffset);

}


glm::mat4 getViewMatrix(GLFWwindow* window) {

	double mx, my;
	glfwGetCursorPos(window, &mx, &my);

	int ww, wh;
	glfwGetWindowSize(window, &ww, &wh);

	double pi = glm::pi<double>();
	double twopi = 2.0 * glm::pi<double>();

	//  std::cout << mx << ", " << my << " : " << ww << ", " << wh << std::endl;

	double yAxisRotation = mx / static_cast<double>(ww) * twopi - pi;
	double eyeHeight = my / static_cast<double>(wh) * 10.0 - gScrX;




	glm::mat4 ret = glm::lookAt(glm::vec3(2 * cos(yAxisRotation), eyeHeight, 5 * sin(yAxisRotation)),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	return ret;


}

int main(void) {
	std::vector<glm::vec3> gVertexBufferData;
	gVertexBufferData.push_back(glm::vec3(0, 0, 0));
	gVertexBufferData.push_back(glm::vec3(2, 0, 0));
	gVertexBufferData.push_back(glm::vec3(2, 0, 2));
	gVertexBufferData.push_back(glm::vec3(0, 0, 2));

	gVertexBufferData.push_back(glm::vec3(0, 2, 0));
	gVertexBufferData.push_back(glm::vec3(2, 2, 0));
	gVertexBufferData.push_back(glm::vec3(2, 2, 2));
	gVertexBufferData.push_back(glm::vec3(0, 2, 2));


	std::vector<glm::vec3> gTexCoordBufferData;
	gTexCoordBufferData.push_back(glm::vec3(0, 0, 0.0));
	gTexCoordBufferData.push_back(glm::vec3(1, 0, 0.0));
	gTexCoordBufferData.push_back(glm::vec3(1, 0, 1.0));
	gTexCoordBufferData.push_back(glm::vec3(0, 0, 1.0));

	gTexCoordBufferData.push_back(glm::vec3(0, 1, 0.0));
	gTexCoordBufferData.push_back(glm::vec3(1, 1, 0.0));
	gTexCoordBufferData.push_back(glm::vec3(1, 1, 1.0));
	gTexCoordBufferData.push_back(glm::vec3(0, 1, 1.0));

	std::vector<GLuint> gIndexBufferData;
	gIndexBufferData.push_back(4);
	gIndexBufferData.push_back(5);
	gIndexBufferData.push_back(1);

	gIndexBufferData.push_back(4);
	gIndexBufferData.push_back(0);
	gIndexBufferData.push_back(1);

	gIndexBufferData.push_back(3);
	gIndexBufferData.push_back(7);
	gIndexBufferData.push_back(6);

	gIndexBufferData.push_back(3);
	gIndexBufferData.push_back(6);
	gIndexBufferData.push_back(2);

	gIndexBufferData.push_back(2);
	gIndexBufferData.push_back(4);
	gIndexBufferData.push_back(1);



	GLFWwindow* window1;

	/* Initialize the library */

	if (!glfwInit()) {
		std::cout << "glfwInit failed !" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); //for Debug(OpenGL4.3)

	/* Create a windowed mode window and its OpenGL context */
	window1 = glfwCreateWindow(1920, 1080, "GLFW TEST", NULL, NULL);


	if (!window1)
	{
		std::cout << "glfwCreateWindow failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window1);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)std::cout << "glew initialization failed!!" << std::endl;

	//DebugMessage
	initDebugMessage();

	// Swap Interval (0:no V-sync  1:default)
	glfwSwapInterval(0);

	// create VAO
	VAO* vao = new VAO();
	vao->bind();

	// create geometry
	Geometry* geom1 = new Geometry(Geometry::TRIANGLES);
	Geometry* geom2 = new Geometry(Geometry::TRIANGLES);

	geom1->setVertices(&gVertexBufferData);
	geom1->setTexCoords(&gTexCoordBufferData);
	geom1->setIndices(&gIndexBufferData);

	geom2->setVertices(&gVertexBufferData);
	geom2->setTexCoords(&gTexCoordBufferData);
	geom2->setIndices(&gIndexBufferData);
	std::cout << "before texture" << std::endl;
	// create texture
	Texture<GLfloat>* texture2 = new Texture<GLfloat>("../../data/simple[0-9][.]png", Texture<GLfloat>::T_3D);
	//Texture<GLfloat>* texture2 = new Texture<GLfloat>("../../data/ForMarchingCubesTest[.]xcf-[0-9][0-9][.]png", Texture<GLfloat>::T_3D);

	Texture<GLfloat>* texture1 = new Texture<GLfloat>("../../data/test[.]png", Texture<GLfloat>::T_2D);

	MarchingCubesTF* marchingcubes = new MarchingCubesTF();
	marchingcubes->setData(texture2);
	marchingcubes->generateGeometry();
	marchingcubes->initTransformFeedback();
	marchingcubes->setThreshold(50.0);

	// load shader
	Shader* shader1 = new Shader();
	Shader* shader2 = new Shader();

	shader1->compileShader(GL_VERTEX_SHADER, "../../shader/basic.vert");
	shader1->compileShader(GL_FRAGMENT_SHADER, "../../shader/tex2d.frag");
	shader1->linkProgram();

	shader2->compileShader(GL_VERTEX_SHADER, "../../shader/basic.vert");
	shader2->compileShader(GL_FRAGMENT_SHADER, "../../shader/tex3d.frag");
	shader2->linkProgram();

	// end of load shader

	/* Make the window's context current */

	glClearColor(1.0, 0.5, 0.5, 1);

	glfwSetKeyCallback(window1, key_callback);
	glfwSetScrollCallback(window1, scroll_callback);

	glm::mat4 projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::translate(glm::vec3(-1, -1, -1));
	glm::mat4 MVP = projection * view * model;
	glm::mat4 modelview = view * model;

	glEnable(GL_DEPTH_TEST);

	static double framecount = 0.0;
	int framecount_i = 0;
	double previousTime = glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window1))
	{


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (geomON) {
			shader1->use();
			shader1->setUniform("uModelView", modelview);
			shader1->setUniform("uProjection", projection);
			geom1->bind();
			texture1->bind();
			geom1->draw();
			texture1->unbind();
			geom1->unbind();
		}
		else {
			glm::mat4 model2 = glm::translate(glm::vec3(1.0, -1.0, -1.0));
			glm::mat4 modelview2 = view * model2;

			shader2->use();
			shader2->setUniform("uModelView", modelview2);
			shader2->setUniform("uProjection", projection);
			shader2->setUniform("uTexture", texture2->getTextureUnit());
			geom2->bind();
			texture2->bind();
			geom2->draw();
			texture2->unbind();
			geom2->unbind();

			Shader::unuse();
			marchingcubes->render(projection, view);
		}


		Shader::unuse();
		glfwSwapBuffers(window1);

		gScrX = 1.0;

		if (rotON) {
			float t = framecount * M_PI / 180 / 3;

			gVertexBufferData.at(2)[2] = fabs(2 * sin(t));
			gVertexBufferData.at(3)[2] = fabs(2 * sin(t));
			gVertexBufferData.at(6)[2] = fabs(2 * sin(t));
			gVertexBufferData.at(7)[2] = fabs(2 * sin(t));

			gTexCoordBufferData.at(2)[2] = fabs(sin(t));
			gTexCoordBufferData.at(3)[2] = fabs(sin(t));
			gTexCoordBufferData.at(6)[2] = fabs(sin(t));
			gTexCoordBufferData.at(7)[2] = fabs(sin(t));

			framecount += 1.0;

			geom2->setVertices(&gVertexBufferData);
			geom2->setTexCoords(&gTexCoordBufferData);

			marchingcubes->setThreshold(100 * fabs(sin(t)));
		}

		// calculate frame rate
		double currentTime = glfwGetTime();
		framecount_i++;
		if (currentTime - previousTime > 1.0) {
			std::string fps = std::to_string(framecount_i);
			glfwSetWindowTitle(window1, fps.c_str());
			previousTime = currentTime;
			framecount_i = 0;
		}

		glfwPollEvents();
		view = getViewMatrix(window1);
		MVP = projection * view * model;
		modelview = view * model;
	}

	glfwTerminate();
	return 0;
}
