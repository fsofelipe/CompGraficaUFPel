#include "../include/multiblock.hpp"

MultiBlock::MultiBlock(){}

MultiBlock::MultiBlock(int type, int orientation, glm::vec4 blocksID){
    this->type = type;
    this->orientation = orientation;
    this->blocksID = blocksID;
}

glm::vec4 MultiBlock::getBlocks(){ return blocksID;}

int MultiBlock::getBlock(int b){
    return blocksID[b];
}

int MultiBlock::getType(){ return type;}
int MultiBlock::getOrientation(){ return orientation;}

void MultiBlock::setOrientation(int o){
    orientation = o;
}
