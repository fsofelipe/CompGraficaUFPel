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
#include "Camera.hpp"

class ModelManager {
private:
	std::vector<Mesh> meshes;
	std::vector<Model> models;
	std::vector<Camera> cams;

public:
	void addMesh(char *path);
	Mesh getMesh(int meshID);

	int addModel(int meshID, glm::vec3 pos);
	int getNumberOfModel();

	int addCam(glm::vec3 position, glm::vec3 front, glm::vec3 up);
	int addCam(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fieldOfView, float aspectRatio, float nearValue, float farValue);
	int getNumberOfCam();

	void addTranslation(int cam, glm::vec3 center, double time);

	void setViewMatrix(int cam, glm::vec3 position, glm::vec3 front, glm::vec3 up);
	void setProjectionMatrix(int cam, float fieldOfView, float aspectRatio, float nearValue, float farValue);

	void printCamera();

	void draw(int n_cam, unsigned int g_nWidth, unsigned int g_nHeight, GLuint MatrixID, GLuint ModelMatrixID, GLuint ViewMatrixID, GLuint LightID, GLuint Texture, GLuint TextureID);
	void del();
};

#endif
