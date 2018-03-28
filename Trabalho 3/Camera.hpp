#ifndef CAMERAHPP
#define CAMERAHPP

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

class Camera {

private:
	float fieldOfView, aspectRatio, nearValue, farValue;

	glm::mat4 viewMatrix, projectionMatrix;

	int animation;
	double time, initTime;
	glm::vec3 I_position, I_front, I_up;
	glm::vec3 F_position, F_front, F_up;

public:
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up);
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fieldOfView, float aspectRatio, float nearValue, float farValue);

	glm::mat4 getViewMatrix();
	void setViewMatrix(glm::vec3 position, glm::vec3 front, glm::vec3 up);
	void setViewMatrix(glm::mat4 m);

	glm::mat4 getProjectionMatrix();
	void setProjectionMatrix(glm::mat4 m);

	int getAnimation();
	double getTime();
	double getInitTime();

	glm::vec3 getI_position();
	glm::vec3 getF_position();

	glm::vec3 getI_front();
	glm::vec3 getI_up();

	void setTranslation(glm::vec3 eye, double time, double initTime);


};




#endif