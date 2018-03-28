#ifndef SCENEHPP
#define SCENEHPP

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
#include "../include/mesh.hpp"
#include "../include/multiBlock.hpp"
#include "../include/board.hpp"
#include "../include/aux.hpp"

class Scene {
private:
    //std::vector<Bloco> blocos;
    //std::vector<MultiBlock> multiBlocos;
    
    Mesh cube = Mesh();
    Board board = Board(blocos);
    glm::vec3 middlePoint = glm::vec3(0.7, -0.65, 0);
    Converter con = Converter(middlePoint);
    
    MultiBlock ativo = MultiBlock();
    bool running = false;
    int lines = 0;
    bool gameOver = false;
    
public:
    std::vector<Bloco> blocos;
    Scene();
    void draw(unsigned int g_nWidth, unsigned int g_nHeight, GLuint MatrixID, GLuint ModelMatrixID, GLuint ViewMatrixID, GLuint LightID, GLuint Texture, GLuint TextureID);
    void del();
    
    int addModel(glm::vec3 pos);
    int addModel(glm::vec2 pos);
    
    void createMultiBlock(int type);
    void check();
    
    void update();
    
    bool moveDown();
    bool moveRight();
    bool moveLeft();
    
    bool rotate();
    
    bool isRunning();
    
    void createScenario();
    
    void printBoard();
    
    int getLines();
    
    bool getGame();
};

#endif
