#ifndef MODELHPP
#define MODELHPP

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <glerror.hpp>

#include "Mesh.hpp"


class Model {
	private:
		GLuint vertexbuffer;
		GLuint uvbuffer;
		GLuint normalbuffer;
		GLuint elementbuffer;

		glm::mat4 modelMatrix = glm::mat4(1.0);
		glm::vec3 initialPos;
		int meshID;

	public:
		Model(int meshID, Mesh m, glm::vec3 initialPos);

		int getMeshID();

		glm::mat4 getMatrix();
		void setMatrix(glm::mat4 m);

		GLuint getVertexBuffer();
		GLuint getUvBuffer();
		GLuint getNormalBuffer();
		GLuint getElementBuffer();

		GLuint &getVertexRef();
		GLuint &getUvRef();
		GLuint &getNormalRef();
		GLuint &getElementRef();
};

#endif
