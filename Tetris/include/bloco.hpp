#ifndef BLOCOHPP
#define BLOCOHPP

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

#include "../include/mesh.hpp"
#include "../include/aux.hpp"

class Bloco {
private:
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;
    int indiceSize;
    
    glm::vec3 initialPos;
    
    glm::mat4 modelMatrix = glm::mat4(1.0);
    
    glm::vec2 posBoard;
    
    glm::vec3 middlePoint = glm::vec3(0.7, -0.65, 0);
    
    
    Converter con = Converter(middlePoint);
    
public:
    Bloco(glm::vec3 initialPos, Mesh m);
    Bloco(glm::vec2 posBoard, Mesh m);
    glm::mat4 getMatrix();
    
    GLuint getVertexBuffer();
    GLuint getUvBuffer();
    GLuint getNormalBuffer();
    GLuint getElementBuffer();
    int getIndiceSize();
    
    GLuint &getVertexRef();
    GLuint &getUvRef();
    GLuint &getNormalRef();
    GLuint &getElementRef();
    
    
    glm::vec3 getPosicao();
    glm::vec2 getPosBoard();
    
    void scaleDown(double s);
    
    void scale(glm::vec3 s);
    void move(glm::vec3 p);
    void move(glm::vec2 p);
};



#endif
