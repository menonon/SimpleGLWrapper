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
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "shader.hpp"
#include "geometry.hpp"
#include "VAO.hpp"
#include "error.hpp"

bool geomON = false;
bool rotON = false;

int gNumSegments = 10;
int gNumStrips = 1;



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		geomON = !geomON;
	}
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		rotON = !rotON;
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		gNumSegments++;
		std::cout << gNumSegments << std::endl;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		gNumSegments--;
		std::cout << gNumSegments << std::endl;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		gNumStrips++;
		std::cout << gNumStrips << std::endl;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		gNumStrips--;
		std::cout << gNumStrips << std::endl;
	}


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
	double eyeHeight = my / static_cast<double>(wh) * 5.0 - 1.5;




	glm::mat4 ret = glm::lookAt(glm::vec3(5 * cos(yAxisRotation), eyeHeight, 5 * sin(yAxisRotation)),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	return ret;


}

int main(void)
{

	std::vector<glm::vec3> gVertexBufferData{
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(1.0, 0.0, 1.0),
		glm::vec3(1.0, 1.0, 2.0),
		glm::vec3(0.0, 1.0, 3.0),
		glm::vec3(0.0, 2.0, 0.0),
		glm::vec3(1.0, -2.0, 1.0),
		glm::vec3(2.0, -3.0, 2.0),
		glm::vec3(3.0, -2.0, 3.0),
		glm::vec3(3.0, -2.0, 0.0),
		glm::vec3(-1.0, -1.0, -1.0)
	};


	std::vector<GLuint> gIndexBufferData{
		0,1,2,3,
		3,4,5,6,
		6,7,8,9
	};


	GLFWwindow* window1;



	/* Initialize the library */

	if (!glfwInit()) {
		std::cout << "glfwInit failed !" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window1 = glfwCreateWindow(1280, 960, "OpenGL Tessellation Shader super simple sample", NULL, NULL);


	if (!window1)
	{
		std::cout << "glfwCreateWindow failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window1);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)std::cout << "glew initialization failed!!" << std::endl;


	initDebugMessage();

	VAO* vao = new VAO();
	vao->bind();

	Geometry* geom = new Geometry(Geometry::PATCHES);
	geom->setVertices(&gVertexBufferData);
	geom->setIndices(&gIndexBufferData);



	// load shader
	Shader* shader2 = new Shader();

	shader2->compileShader(GL_VERTEX_SHADER, "../../shader/tess_simple.vert");
	shader2->compileShader(GL_FRAGMENT_SHADER, "../../shader/tess_simple.frag");
	shader2->compileShader(GL_TESS_CONTROL_SHADER, "../../shader/tess_simple.tesc");
	shader2->compileShader(GL_TESS_EVALUATION_SHADER, "../../shader/tess_simple.tese");

	shader2->linkProgram();

	shader2->setPatchParameter(4);

	// end of load shader



	/* Make the window's context current */

	glClearColor(0.0, 0.0, 0.0, 1);

	glfwSetKeyCallback(window1, key_callback);

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 MVP = projection * view * model;

	glm::vec4 LC = glm::vec4(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window1))
	{


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader2->use();
		shader2->setUniform("MVP", MVP);
		shader2->setUniform("LineColor", LC);
		shader2->setUniform("NumSegments", gNumSegments);
		shader2->setUniform("NumStrips", gNumStrips);

		geom->bind();

		geom->draw();

		geom->unbind();
		Shader::unuse();

		glfwSwapBuffers(window1);
		glfwPollEvents();
		view = getViewMatrix(window1);
		MVP = projection * view * model;
	}

	glfwTerminate();
	return 0;
}
