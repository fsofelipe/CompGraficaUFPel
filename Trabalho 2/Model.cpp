#include "Model.hpp"



Model::Model(int meshID, Mesh m, glm::vec3 initialPos) {
	this->meshID = meshID;

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, m.getVertices().size() * sizeof(glm::vec3), &m.getVertices()[0], GL_STATIC_DRAW);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, m.getUvs().size() * sizeof(glm::vec2), &m.getUvs()[0], GL_STATIC_DRAW);


	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, m.getNormals().size() * sizeof(glm::vec3), &m.getNormals()[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well

	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.getIndices().size() * sizeof(unsigned short), &m.getIndices()[0], GL_STATIC_DRAW);

	this->initialPos = initialPos;
	modelMatrix = glm::translate(glm::mat4(1.0), initialPos);
}



glm::mat4 Model::getMatrix() {
	return modelMatrix;
}

void Model::setMatrix(glm::mat4 m) {
	modelMatrix = m;
}
void Model::setMatrix(glm::mat4 m, double angle) {
	modelMatrix = m;
	currentAngle = angle;
}

void Model::setMatrixAxis(glm::mat4 m, glm::vec3 v) {
	modelMatrix = m;
	axis = v;
}

int Model::getMeshID() {
	return meshID;
}


bool Model::isStarted() {
	return start;
}
void Model::setStart(bool s) {
	start = s;
}


int Model::getAnimation() {
	return animation;
}

double Model::getTime() {
	return time;
}

double Model::getInitialTime() {
	return initialTime;
}

double Model::getAngle() {
	return angle;
}
double Model::getCurrentAngle() {
	return currentAngle;
}
glm::vec3 Model::getAxis() {
	return axis;
}

glm::vec3 Model::getFinalPosition() {
	return finalPos;
}

glm::vec3 Model::getInitialPosition() {
	return initialPos;
}

void Model::setTranslation(glm::vec3 v, double time, double initialTime) {
	this->animation = 1;
	this->finalPos = glm::vec3(v.x + modelMatrix[3][0], v.y + modelMatrix[3][1], v.z + modelMatrix[3][2]);

	this->time = time;
	this->initialTime = initialTime;
}

void Model::setRotationAxis(glm::vec3 axis, double angle, double time, double initialTime) {
	this->animation = 2;
	this->axis = axis;
	this->angle = angle;

	this->time = time;
	this->initialTime = initialTime;
}

void Model::setRotationPoint(glm::vec3 central, glm::vec3 axis, double angle, glm::vec3 initialPos, double time, double initialTime) {
	this->animation = 3;
	this->finalPos = central;

	this->axis = axis;
	this->angle = angle;
	this->currentAngle = 0;

	this->time = time;
	this->initialTime = initialTime;

	this->initialPos = initialPos;
	modelMatrix = glm::translate(glm::mat4(1.0), initialPos);
}

void Model::setScale(glm::vec3 scale, double time, double initialTime) {
	this->animation = 4;
	this->finalPos = scale;

	this->time = time;
	this->initialTime = initialTime;

	this->axis = glm::vec3(1, 1, 1);
}

void Model::setShear(glm::vec3 shear, double time, double initialTime) {
	this->animation = 5;
	this->finalPos = shear;

	this->initialPos = initialPos;
	modelMatrix = glm::translate(glm::mat4(1.0), initialPos);
}

GLuint Model::getVertexBuffer() {
	return vertexbuffer;
}
GLuint Model::getUvBuffer() {
	return uvbuffer;
}
GLuint Model::getNormalBuffer() {
	return normalbuffer;
}
GLuint Model::getElementBuffer() {
	return elementbuffer;
}

GLuint& Model::getVertexRef() {
	return vertexbuffer;
}
GLuint &Model::getUvRef() {
	return uvbuffer;
}
GLuint &Model::getNormalRef() {
	return normalbuffer;
}
GLuint &Model::getElementRef() {
	return elementbuffer;
}

void Model::addTransformation(glm::mat4 t) {
	transformations.push_back(t);
}

std::vector<glm::mat4> Model::getTransformations() {
	return transformations;
}

void Model::transform() {
	for (int i = 0; i < transformations.size(); i++)
		modelMatrix = modelMatrix * transformations[i];

	transformations.clear();
}