#ifndef BOARDHPP
#define BOARDHPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../include/multiBlock.hpp"
#include "../include/bloco.hpp"

class Board {
private:
    int matriz[17][9];
    std::vector<Bloco> blocos;
    bool validatePos(glm::vec2 pos);
    
public:
    Board(std::vector<Bloco> b);
    void setBlocos(std::vector<Bloco> b);
    
    bool checkLines();
    
    bool isFree(std::vector<glm::vec2> pos); //DOWN
    bool isFreeRight(std::vector<glm::vec2> pos);
    bool isFreeLeft(std::vector<glm::vec2> pos);
    
    std::vector<glm::vec2> nextDown(MultiBlock m);
    std::vector<glm::vec2> nextRight(MultiBlock m);
    std::vector<glm::vec2> nextLeft(MultiBlock m);
    
    int getValue(glm::vec2 pos);
    void setValue(glm::vec2 pos, int val);
    
    void print();
};

#endif
