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

int Model::getMeshID() {
	return meshID;
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