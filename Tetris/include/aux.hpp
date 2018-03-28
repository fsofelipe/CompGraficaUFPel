#ifndef aux_hpp
#define aux_hpp


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

using namespace std;

class Converter{
private:
    double linhas[17];
    double colunas[9];
public:
    Converter(glm::vec3 middlePoint);
    glm::vec2 getPosBoard(glm::vec3 pos);
    glm::vec3 getPos(glm::vec2 pos);
};


#endif /* aux_h */

/*

 addModel(glm::vec3(-1.4, 0, 0) + middlePoint);
 addModel(glm::vec3(-1.05, 0, 0) + middlePoint);
 addModel(glm::vec3(-0.7, 0, 0) + middlePoint);
 addModel(glm::vec3(-0.35, 0, 0) + middlePoint);
 
 addModel(glm::vec3(0.35, 0, 0) + middlePoint);
 addModel(glm::vec3(0.70, 0, 0) + middlePoint);
 addModel(glm::vec3(1.05, 0, 0) + middlePoint);
 addModel(glm::vec3(1.4, 0, 0) + middlePoint);
 
 addModel(glm::vec3(0, 2.8, 0) + middlePoint);   //8
 addModel(glm::vec3(0, 2.45, 0) + middlePoint);  //7
 addModel(glm::vec3(0, 2.1, 0) + middlePoint);   //6
 addModel(glm::vec3(0, 1.75, 0) + middlePoint);  //5
 addModel(glm::vec3(0, 1.4, 0) + middlePoint);   //4
 addModel(glm::vec3(0, 1.05, 0) + middlePoint);  //3
 addModel(glm::vec3(0, 0.7, 0) + middlePoint);   //2
 addModel(glm::vec3(0, 0.35, 0) + middlePoint);  //1
 addModel(glm::vec3(0, 0, 0) + middlePoint);     //0
 addModel(glm::vec3(0, -0.35, 0) + middlePoint); //1
 addModel(glm::vec3(0, -0.7, 0) + middlePoint);  //2
 addModel(glm::vec3(0, -1.05, 0) + middlePoint); //3
 addModel(glm::vec3(0, -1.4, 0) + middlePoint);  //4
 addModel(glm::vec3(0, -1.75, 0) + middlePoint); //5
 addModel(glm::vec3(0, -2.1,  0) + middlePoint); //6
 addModel(glm::vec3(0, -2.45, 0) + middlePoint); //7
 addModel(glm::vec3(0, -2.8,  0) + middlePoint); //8


*/
