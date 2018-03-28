#include "../include/board.hpp"

Board::Board(std::vector<Bloco> b) {
    blocos = b;
    for (int i = 0; i < 17; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j] = 0;
        }
    }
    
    
}

void Board::setBlocos(std::vector<Bloco> b) {
    blocos = b;
}

bool Board::checkLines(){
    bool ret = false;
    for (int i = 0; i < 17; i++) {
    
        if (matriz[i][0] == 1 && matriz[i][1] == 1 && matriz[i][2] == 1 &&
            matriz[i][3] == 1 && matriz[i][4] == 1 && matriz[i][5] == 1 &&
            matriz[i][6] == 1 && matriz[i][7] == 1 && matriz[i][8]){
            ret = true;
            //cout << "TEM QUE LIMPAR"<< endl;
            
            int k = i;
            for (int j = i + 1; j < (17 - i); j++){
                matriz[k][0] = matriz[j][0];
                matriz[k][1] = matriz[j][1];
                matriz[k][2] = matriz[j][2];
                matriz[k][3] = matriz[j][3];
                matriz[k][4] = matriz[j][4];
                matriz[k][5] = matriz[j][5];
                matriz[k][6] = matriz[j][6];
                matriz[k][7] = matriz[j][7];
                matriz[k][8] = matriz[j][8];
                k++;
            }
            
        }
    }
    //print();
    
    return ret;
}

bool Board::validatePos(glm::vec2 pos) {
    int linha = pos[0];
    int col = pos[1];
    
    //cout << "Linha: " << linha << " col: " << col << endl;
    
    if (linha >= 0 && linha < 17)
        if (col >= 0 && col < 9) {
            if (linha == 0)
                matriz[linha][col] = 1; //QUANDO ATINGE A BASE COLOCA
            return true;
        }
    
    return false;
}

bool Board::isFree(std::vector<glm::vec2> pos) {
    int v1 = getValue(pos[0]);
    int v2 = getValue(pos[1]);
    int v3 = getValue(pos[2]);
    int v4 = getValue(pos[3]);
    
    if (!validatePos(pos[0]))
        return false;
    if (!validatePos(pos[1]))
        return false;
    if (!validatePos(pos[2]))
        return false;
    if (!validatePos(pos[3]))
        return false;
    
    
    if (v1 == 0 && v2 == 0 && v3 == 0 && v4 == 0)
        return true;
    
    return false;
}

bool Board::isFreeRight(std::vector<glm::vec2> pos){
    //cout << pos[0][1] << endl;
    
    if ((getValue(glm::vec2(pos[0][0], pos[0][1] + 1)) == 0) && (getValue(glm::vec2(pos[1][0], pos[1][1] + 1)) == 0) &&
        (getValue(glm::vec2(pos[2][0], pos[2][1] + 1)) == 0) && (getValue(glm::vec2(pos[3][0], pos[3][1] + 1)) == 0))
        return true;

    return false;
}

bool Board::isFreeLeft(std::vector<glm::vec2> pos){
    //cout << pos[0][1] << endl;
    
    if ((getValue(glm::vec2(pos[0][0], pos[0][1] - 1)) == 0) && (getValue(glm::vec2(pos[1][0], pos[1][1] - 1)) == 0) &&
        (getValue(glm::vec2(pos[2][0], pos[2][1] - 1)) == 0) && (getValue(glm::vec2(pos[3][0], pos[3][1] - 1)) == 0))
        return true;
    
    
    
    return false;
}

std::vector<glm::vec2> Board::nextDown(MultiBlock m){
    std::vector<glm::vec2> newPos;
    
    glm::vec2 p1 = blocos[m.getBlock(0)].getPosBoard();
    glm::vec2 p2 = blocos[m.getBlock(1)].getPosBoard();
    glm::vec2 p3 = blocos[m.getBlock(2)].getPosBoard();
    glm::vec2 p4 = blocos[m.getBlock(3)].getPosBoard();
    
    newPos.push_back(glm::vec2(p1[0] - 1, p1[1]));
    newPos.push_back(glm::vec2(p2[0] - 1, p2[1]));
    newPos.push_back(glm::vec2(p3[0] - 1, p3[1]));
    newPos.push_back(glm::vec2(p4[0] - 1, p4[1]));
    
    return newPos;
}

std::vector<glm::vec2> Board::nextRight(MultiBlock m) {
    std::vector<glm::vec2> newPos;
    
    glm::vec2 p1 = blocos[m.getBlock(0)].getPosBoard();
    glm::vec2 p2 = blocos[m.getBlock(1)].getPosBoard();
    glm::vec2 p3 = blocos[m.getBlock(2)].getPosBoard();
    glm::vec2 p4 = blocos[m.getBlock(3)].getPosBoard();
    
    if (p1[1] < 8) {
        newPos.push_back(glm::vec2(p1[0], p1[1] + 1));
        newPos.push_back(glm::vec2(p2[0], p2[1] + 1));
        newPos.push_back(glm::vec2(p3[0], p3[1] + 1));
        newPos.push_back(glm::vec2(p4[0], p4[1] + 1));
    } else {
        newPos.push_back(glm::vec2(p1[0], p1[1]));
        newPos.push_back(glm::vec2(p2[0], p2[1]));
        newPos.push_back(glm::vec2(p3[0], p3[1]));
        newPos.push_back(glm::vec2(p4[0], p4[1]));
    }
    return newPos;
}

std::vector<glm::vec2> Board::nextLeft(MultiBlock m) {
    std::vector<glm::vec2> newPos;
    
    glm::vec2 p1 = blocos[m.getBlock(0)].getPosBoard();
    glm::vec2 p2 = blocos[m.getBlock(1)].getPosBoard();
    glm::vec2 p3 = blocos[m.getBlock(2)].getPosBoard();
    glm::vec2 p4 = blocos[m.getBlock(3)].getPosBoard();
    
    if (p1[1] > 0) {
        newPos.push_back(glm::vec2(p1[0], p1[1] - 1));
        newPos.push_back(glm::vec2(p2[0], p2[1] - 1));
        newPos.push_back(glm::vec2(p3[0], p3[1] - 1));
        newPos.push_back(glm::vec2(p4[0], p4[1] - 1));
    } else {
        newPos.push_back(glm::vec2(p1[0], p1[1]));
        newPos.push_back(glm::vec2(p2[0], p2[1]));
        newPos.push_back(glm::vec2(p3[0], p3[1]));
        newPos.push_back(glm::vec2(p4[0], p4[1]));
    }
    return newPos;
}

int Board::getValue(glm::vec2 pos) {
    return matriz[int(pos[0])][int(pos[1])];
}

void Board::setValue(glm::vec2 pos, int val) {
    matriz[int(pos[0])][int(pos[1])] = val;
}

void Board::print() {
    cout << "\t0\t1\t2\t3\t4\t5\t6\t7\t8 " << endl;
    
    for (int i = 16; i >= 0; i--) {
        cout << i << ":\t" << matriz[i][0] << "\t" << matriz[i][1] << "\t" << matriz[i][2] << "\t" << matriz[i][3] << "\t" << matriz[i][4] << "\t" << matriz[i][5] << "\t"
        << matriz[i][6] << "\t" << matriz[i][7] << "\t" << matriz[i][8] << endl;
    }
}

