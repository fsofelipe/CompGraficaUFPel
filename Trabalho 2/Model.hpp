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
		std::vector<glm::mat4> transformations;


		glm::vec3 initialPos = glm::vec3(0, 0, 0);
		glm::vec3 finalPos = glm::vec3(0, 0, 0);
		int animation = 0;
		double time = 0;
		double initialTime = 0;

		double angle = 0;
		double currentAngle = 0;
		double range = 0;

		glm::vec3 axis = glm::vec3(0, 0, 0);
		bool start = false;
		int meshID;

	public:
		Model(int meshID, Mesh m, glm::vec3 initialPos);

		int getMeshID();

		glm::mat4 getMatrix();
		void setMatrix(glm::mat4 m);
		void setMatrix(glm::mat4 m, double angle);
		void setMatrixAxis(glm::mat4 m, glm::vec3 v);
		
		int getAnimation();
		double getTime();
		double getInitialTime();

		glm::vec3 getFinalPosition();
		glm::vec3 getInitialPosition();

		bool isStarted();
		void setStart(bool s);

		
		double getAngle();
		double getCurrentAngle();
		glm::vec3 getAxis();

		void setTranslation(glm::vec3 v, double time, double initialTime);
		void setRotationAxis(glm::vec3 axis, double angle, double time, double initialTime);
		void setRotationPoint(glm::vec3 central, glm::vec3 axis,  double angle, glm::vec3 initialPos, double time, double initialTime);
		void setScale(glm::vec3 scale, double time, double initialTime);
		void setShear(glm::vec3 shear, double time, double initialTime);

		GLuint getVertexBuffer();
		GLuint getUvBuffer();
		GLuint getNormalBuffer();
		GLuint getElementBuffer();

		GLuint &getVertexRef();
		GLuint &getUvRef();
		GLuint &getNormalRef();
		GLuint &getElementRef();

		void addTransformation(glm::mat4 t);

		std::vector<glm::mat4> getTransformations();
		void transform();
};

#endif
