#include "../include/bloco.hpp"

Bloco::Bloco(glm::vec3 initialPos, Mesh m) {
    
    
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
    
    indiceSize = m.getIndices().size();
    
    this->initialPos = initialPos;
    modelMatrix = glm::translate(glm::mat4(1.0), initialPos);
    
    this->posBoard = glm::vec2(10, 10);
}

Bloco::Bloco(glm::vec2 posBoard, Mesh m) {
    //Mesh m = Mesh("../mesh/cube.obj", 1);
    
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
    
    indiceSize = m.getIndices().size();
    
    this->initialPos = con.getPos(posBoard);
    
    //scaleDown(0.12);
    
    this->posBoard = posBoard;
    
    
    
    modelMatrix = glm::translate(modelMatrix, initialPos);
    
    
    
}

glm::mat4 Bloco::getMatrix() { return modelMatrix; }


GLuint Bloco::getVertexBuffer() { return vertexbuffer; }
GLuint Bloco::getUvBuffer() { return uvbuffer; }
GLuint Bloco::getNormalBuffer() { return normalbuffer; }
GLuint Bloco::getElementBuffer() { return elementbuffer; }

int Bloco::getIndiceSize() { return indiceSize; }

GLuint &Bloco::getVertexRef() { return vertexbuffer; }
GLuint &Bloco::getUvRef() { return uvbuffer; }
GLuint &Bloco::getNormalRef() { return normalbuffer; }
GLuint &Bloco::getElementRef() { return elementbuffer; }


glm::vec3 Bloco::getPosicao() {
    return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
}
glm::vec2 Bloco::getPosBoard() {
    return posBoard;
}

void Bloco::scaleDown(double s) {
    modelMatrix = glm::scale(modelMatrix, glm::vec3(s, s, s));
}

void Bloco::scale(glm::vec3 s) {
    modelMatrix = glm::scale(modelMatrix, s);
}

void Bloco::move(glm::vec3 p) {
    //modelMatrix = glm::translate(modelMatrix, p);
    modelMatrix[3][0] = p.x;
    modelMatrix[3][1] = p.y;
    modelMatrix[3][2] = p.z;
    initialPos = p;
}

void Bloco::move(glm::vec2 p) {
    this->initialPos = con.getPos(p);
    
    this->posBoard = p;
    //modelMatrix = glm::translate(modelMatrix, initialPos);
    
    modelMatrix[3][0] = initialPos.x;
    modelMatrix[3][1] = initialPos.y;
    modelMatrix[3][2] = initialPos.z;
    
}
