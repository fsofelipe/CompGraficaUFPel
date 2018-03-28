#ifndef MULTIBLOCKHPP
#define MULTIBLOCKHPP

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


#include "../include/bloco.hpp"

class MultiBlock{
private:
    int type;
    int orientation; //0 up; 1 right; 2 down; 3 left;
    glm::vec4 blocksID;
    
public:
    MultiBlock();
    MultiBlock(int type, int orientation, glm::vec4 blocksID);
    
    glm::vec4 getBlocks();
    int getBlock(int b);
    int getType();
    int getOrientation();
    void setOrientation(int o);
};

#endif
