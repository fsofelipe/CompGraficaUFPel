#ifndef MODELMANAGERHPP
#define MODELMANAGERHPP

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include <iostream>

using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/spline.hpp>
using namespace glm;

#include <shader.hpp>
#include <texture.hpp>
#include <controls.hpp>
#include <objloader.hpp>
#include <vboindexer.hpp>
#include <glerror.hpp>

#include "Mesh.hpp"
#include "Model.hpp"

class ModelManager {
	private:
		std::vector<Mesh> meshes;
		std::vector<Model> models;

	public:
		void addMesh(char *path);
		Mesh getMesh(int meshID);

		int addModel(int meshID);
		int addModel(int meshID, glm::vec3 pos);

		glm::mat4 scale(glm::vec3 s);
		glm::mat4 translate(glm::vec3 v);
		glm::mat4 rotation(float angle, glm::vec3 v);
		glm::mat4 shear(glm::vec2 v);

		void addStatic(int model, glm::mat4 m);

		void transform(int model, glm::mat4 m);
		void transform(int model);

		void addTranslation(int model, glm::vec3 v, double time);
		void addRotationAxis(int model, glm::vec3 axis, double angle, double time);
		void addRotationPoint(int model, glm::vec3 central, glm::vec3 axis, glm::vec3 initialPos, double angle, double time);
		void addScale(int model, glm::vec3 scale, double time);
		void addShear(int model, glm::vec3 shear, double time);

		int getNumberOfModel();

		void draw(unsigned int g_nWidth, unsigned int g_nHeight, GLuint MatrixID, GLuint ModelMatrixID, GLuint ViewMatrixID, GLuint LightID, GLuint Texture, GLuint TextureID);
		void del();
};

#endif
