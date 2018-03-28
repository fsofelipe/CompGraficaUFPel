#include "../include/aux.hpp"

Converter::Converter(glm::vec3 middlePoint){
    linhas[0]  = -2.80 + middlePoint.y;
    linhas[1]  = -2.45 + middlePoint.y;
    linhas[2]  = -2.10 + middlePoint.y;
    linhas[3]  = -1.75 + middlePoint.y;
    linhas[4]  = -1.40 + middlePoint.y;
    linhas[5]  = -1.05 + middlePoint.y;
    linhas[6]  = -0.70 + middlePoint.y;
    linhas[7]  = -0.35 + middlePoint.y;
    linhas[8]  = +0.00 + middlePoint.y;
    linhas[9]  = +0.35 + middlePoint.y;
    linhas[10] = +0.70 + middlePoint.y;
    linhas[11] = +1.05 + middlePoint.y;
    linhas[12] = +1.40 + middlePoint.y;
    linhas[13] = +1.75 + middlePoint.y;
    linhas[14] = +2.10 + middlePoint.y;
    linhas[15] = +2.45 + middlePoint.y;
    linhas[16] = +2.80 + middlePoint.y;
    
    colunas[0] = -1.40 + middlePoint.x;
    colunas[1] = -1.05 + middlePoint.x;
    colunas[2] = -0.70 + middlePoint.x;
    colunas[3] = -0.35 + middlePoint.x;
    colunas[4] = +0.00 + middlePoint.x;
    colunas[5] = +0.35 + middlePoint.x;
    colunas[6] = +0.70 + middlePoint.x;
    colunas[7] = +1.05 + middlePoint.x;
    colunas[8] = +1.40 + middlePoint.x;
}

glm::vec3 Converter::getPos(glm::vec2 pos){
    return glm::vec3 (colunas[int(pos.y)], linhas[int(pos.x)], 0);
}

glm::vec2 Converter::getPosBoard(glm::vec3 pos){
    
    int l = 0;
    int c = 0;
    
    cout << "pos.x: " << pos.x << endl;
    cout << "pos.y: " << pos.y << endl;
    cout << "linhas: " << linhas[0] << endl;
    
    cout << typeid(pos.x).name() << endl;
    cout << typeid(linhas[0]).name() << endl;
    
    
    
    for (l = 0; l < 17; l++){
        cout << "linha: " << l << " | linha: " << linhas[l] << endl;
        if (linhas[l] == pos.x){
            cout << "linha: " << l << endl;
            cout << "AQUI" << endl;
            break;
        }
    }
    
    for (c = 0; c < 8; c++){
        if (colunas[c] == pos.y){
            break;
        }
    }
    
    return glm::vec2(l, c);
}
