#include "../include/scene.hpp"

std::ostream &operator<< (std::ostream &out, const glm::mat4 &m) {
    out << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << "\n"
    << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << "\n"
    << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << "\n"
    << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << "\n";
    
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec3 &m) {
    out << "<" << m[0] << ", " << m[1] << ", " << m[2] << ">";
    
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec2 &m) {
    out << "<" << m[0] << ", " << m[1] << ">";
    
    return out;
}

Scene::Scene() {
    this->cube = Mesh("../mesh/cube_small.obj", 1);
    
    createScenario();
    
    //addModel(glm::vec2(16, 0));
    //addModel(con.getPos(glm::vec2(0, 0)));
    
    
    
    /*
     // MEDIDAS BOARD
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
    
    
}

void Scene::draw(unsigned int g_nWidth, unsigned int g_nHeight, GLuint MatrixID, GLuint ModelMatrixID, GLuint ViewMatrixID, GLuint LightID, GLuint Texture, GLuint TextureID) {
    int nUseMouse = 0;
    for (int i = 0; i < blocos.size(); i++) {
        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs(nUseMouse, g_nWidth, g_nHeight);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        
        glm::mat4 ViewMatrix = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
        
        
        //models[i].transform();
        
        glm::mat4 ModelMatrix;
        
        ModelMatrix = blocos[i].getMatrix();
        
        //cout << "modelo: " << i << endl;
        //cout << ModelMatrix << "\n= = = = =\n" << endl;
        
        
        glm::mat4 NormalMatrix = glm::mat4(1.0);
        
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
        
        glm::vec3 lightPos = glm::vec3(0, 0, 7) + middlePoint;
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, blocos[i].getVertexBuffer());
        glVertexAttribPointer(
                              0,                  // attribute
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, blocos[i].getUvBuffer());
        glVertexAttribPointer(
                              1,                                // attribute
                              2,                                // size
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        
        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, blocos[i].getNormalBuffer());
        glVertexAttribPointer(
                              2,                                // attribute
                              3,                                // size
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blocos[i].getElementBuffer());
        
        // Draw the triangles !
        glDrawElements(
                       GL_TRIANGLES,           // mode
                       blocos[i].getIndiceSize(),   // count
                       GL_UNSIGNED_SHORT,      // type
                       (void*)0                // element array buffer offset
                       );
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
    
}

void Scene::del() {
    for (int i = 0; i < blocos.size(); i++) {
        glDeleteBuffers(1, &blocos[i].getVertexRef());
        glDeleteBuffers(1, &blocos[i].getUvRef());
        glDeleteBuffers(1, &blocos[i].getNormalRef());
        glDeleteBuffers(1, &blocos[i].getElementRef());
    }
}

int Scene::addModel(glm::vec3 pos) {
    blocos.push_back(Bloco(pos, cube));
    return (blocos.size() - 1);
}

int Scene::addModel(glm::vec2 pos) {
    blocos.push_back(Bloco(pos, cube));
    return (blocos.size() - 1);
}

void Scene::createMultiBlock(int type) {
    if (running == false) {
        cout << "Criando bloco!" << endl;
        double space = 0.05;
        int b1 = -1, b2 = -1, b3 = -1, b4 = -1;
        
        double bSpace = 0.3 + space; //blockSpace
        
        if (board.getValue(glm::vec2(16, 3)) == 1 || board.getValue(glm::vec2(16, 4)) == 1 || board.getValue(glm::vec2(16, 5)) == 1 || board.getValue(glm::vec2(16, 6)) == 1 || board.getValue(glm::vec2(15, 3)) == 1 ||  board.getValue(glm::vec2(15, 4)) == 1 || board.getValue(glm::vec2(15, 5)) == 1){
            gameOver = true;
            running = false;
            return;
        }
        
        switch (type) {
                
            case 1:
                b1 = addModel(glm::vec2(16, 4));
                b2 = addModel(glm::vec2(16, 5));
                b3 = addModel(glm::vec2(15, 5));
                b4 = addModel(glm::vec2(15, 4));
                break;
            case 2:
                b1 = addModel(glm::vec2(16, 3));
                b2 = addModel(glm::vec2(16, 4));
                b3 = addModel(glm::vec2(16, 5));
                b4 = addModel(glm::vec2(16, 6));
                break;
                
            case 3:
                b1 = addModel(glm::vec2(16, 3));
                b2 = addModel(glm::vec2(15, 3));
                b3 = addModel(glm::vec2(15, 4));
                b4 = addModel(glm::vec2(15, 5));
                break;
                
            case 4:
                b1 = addModel(glm::vec2(15, 3));
                b2 = addModel(glm::vec2(15, 4));
                b3 = addModel(glm::vec2(15, 5));
                b4 = addModel(glm::vec2(16, 5));
                break;
                
            case 5:
                b1 = addModel(glm::vec2(15, 3));
                b2 = addModel(glm::vec2(15, 4));
                b3 = addModel(glm::vec2(16, 4));
                b4 = addModel(glm::vec2(16, 5));
                break;
                
            case 6:
                b1 = addModel(glm::vec2(16, 3));
                b2 = addModel(glm::vec2(16, 4));
                b3 = addModel(glm::vec2(15, 4));
                b4 = addModel(glm::vec2(15, 5));
                break;
                
            case 7:
                b1 = addModel(glm::vec2(15, 3));
                b2 = addModel(glm::vec2(15, 4));
                b3 = addModel(glm::vec2(15, 5));
                b4 = addModel(glm::vec2(16, 4));
                break;
        }
        running = true;
        ativo = MultiBlock(type, 0, glm::vec4(b1, b2, b3, b4));
        

    }
}

void Scene::check(){
    blocos.clear();
    createScenario();
    for (int i = 0; i < 17; i++){
        for (int j = 0; j < 9; j++){
            
            if(board.getValue(glm::vec2(i, j)) == 1){
                blocos.push_back(Bloco(glm::vec2(i, j), cube));
            }
            
        }
    }
}

void Scene::update() {
    board.setBlocos(blocos);
}

bool Scene::moveDown() {
    update();
    if (running == true){
        std::vector<glm::vec2> newPos = board.nextDown(ativo);
        
        if (board.isFree(newPos)) {
            blocos[ativo.getBlock(0)].move(newPos[0]);
            blocos[ativo.getBlock(1)].move(newPos[1]);
            blocos[ativo.getBlock(2)].move(newPos[2]);
            blocos[ativo.getBlock(3)].move(newPos[3]);
            
            return true;
        } else {
            board.setValue(blocos[ativo.getBlock(0)].getPosBoard(), 1);
            board.setValue(blocos[ativo.getBlock(1)].getPosBoard(), 1);
            board.setValue(blocos[ativo.getBlock(2)].getPosBoard(), 1);
            board.setValue(blocos[ativo.getBlock(3)].getPosBoard(), 1);
        
            ativo = MultiBlock();
            running = false;
            
            if (board.checkLines()){
                lines++;
                check();
            }
            return false;
        }
    }
    
    
    
    return true;
}

bool Scene::moveRight() {
    update();
    if (blocos[ativo.getBlock(0)].getPosBoard()[1] < 8 &&
        blocos[ativo.getBlock(1)].getPosBoard()[1] < 8 &&
        blocos[ativo.getBlock(2)].getPosBoard()[1] < 8 &&
        blocos[ativo.getBlock(3)].getPosBoard()[1] < 8){
        
        
        std::vector<glm::vec2> newPos = board.nextRight(ativo);

        if (board.isFreeRight(newPos)){
            blocos[ativo.getBlock(0)].move(newPos[0]);
            blocos[ativo.getBlock(1)].move(newPos[1]);
            blocos[ativo.getBlock(2)].move(newPos[2]);
            blocos[ativo.getBlock(3)].move(newPos[3]);
        }else{
            board.setValue(blocos[ativo.getBlock(0)].getPosBoard(), 1);
            board.setValue(blocos[ativo.getBlock(1)].getPosBoard(), 1);
            board.setValue(blocos[ativo.getBlock(2)].getPosBoard(), 1);
            board.setValue(blocos[ativo.getBlock(3)].getPosBoard(), 1);
            
            ativo = MultiBlock();
            running = false;
            return false;
        }
        
        
    }
    return true;
}

bool Scene::moveLeft() {
    update();
    if (blocos[ativo.getBlock(0)].getPosBoard()[1] > 0 &&
        blocos[ativo.getBlock(1)].getPosBoard()[1] > 0 &&
        blocos[ativo.getBlock(2)].getPosBoard()[1] > 0 &&
        blocos[ativo.getBlock(3)].getPosBoard()[1] > 0){
        
        
        std::vector<glm::vec2> newPos = board.nextLeft(ativo);
        
        if (board.isFreeRight(newPos)){
            blocos[ativo.getBlock(0)].move(newPos[0]);
            blocos[ativo.getBlock(1)].move(newPos[1]);
            blocos[ativo.getBlock(2)].move(newPos[2]);
            blocos[ativo.getBlock(3)].move(newPos[3]);
        }else{
            board.setValue(blocos[ativo.getBlock(0)].getPosBoard(), 1);
            board.setValue(blocos[ativo.getBlock(1)].getPosBoard(), 1);
            board.setValue(blocos[ativo.getBlock(2)].getPosBoard(), 1);
            board.setValue(blocos[ativo.getBlock(3)].getPosBoard(), 1);
            
            ativo = MultiBlock();
            running = false;
            return false;
        }
        
        
    }
    return true;
}

bool Scene::rotate(){
    if (running == true){
        switch(ativo.getType()){
            case 1:
            {
            return true;
            break;
        }
            case 2:
            {
            std::vector<glm::vec2> newPos;
            glm::vec2 pos = blocos[ativo.getBlock(0)].getPosBoard();
            
            if (ativo.getOrientation() == 0){
                newPos.push_back(pos);
                newPos.push_back(glm::vec2(pos[0]-1, pos[1]));
                newPos.push_back(glm::vec2(pos[0]-2, pos[1]));
                newPos.push_back(glm::vec2(pos[0]-3, pos[1]));
                
                if (board.isFree(newPos)){
                    //cout << "Free" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(1);
                    return true;
                }
                
            }else{
                newPos.clear();
                newPos.push_back(pos);
                newPos.push_back(glm::vec2(pos[0], pos[1] + 1));
                newPos.push_back(glm::vec2(pos[0], pos[1] + 2));
                newPos.push_back(glm::vec2(pos[0], pos[1] + 3));
                
                if (board.isFree(newPos)){
                    //cout << "Free" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(0);
                    return true;
                }
            }
            
            break;
        }
            case 3:
            {
            std::vector<glm::vec2> newPos;
            glm::vec2 pos = blocos[ativo.getBlock(0)].getPosBoard();
            
            if (ativo.getOrientation() == 0){
                newPos.push_back(glm::vec2(pos[0], pos[1]+1));
                newPos.push_back(pos);
                newPos.push_back(glm::vec2(pos[0] - 1, pos[1]));
                newPos.push_back(glm::vec2(pos[0] - 2, pos[1]));
                
                if (board.isFree(newPos)){
                    cout << "AQUI 0" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(1);
                    return true;
                }
            }
            
            if (ativo.getOrientation() == 1){
                newPos.push_back(glm::vec2(pos[0] - 1, pos[1]));
                newPos.push_back(glm::vec2(pos[0], pos[1]));
                newPos.push_back(glm::vec2(pos[0], pos[1] - 1));
                newPos.push_back(glm::vec2(pos[0], pos[1] - 2));
                
                if (board.isFree(newPos)){
                    //cout << "AQUI 1" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(2);
                    return true;
                }
            }
            
            if (ativo.getOrientation() == 2){
                newPos.push_back(glm::vec2(pos[0] - 1, pos[1] - 1));
                newPos.push_back(glm::vec2(pos[0] - 1, pos[1]));
                newPos.push_back(glm::vec2(pos[0], pos[1]));
                newPos.push_back(glm::vec2(pos[0] + 1, pos[1]));
                
                if (board.isFree(newPos)){
                    //cout << "AQUI 2" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(3);
                    return true;
                }
            }
            if (ativo.getOrientation() == 3){
                newPos.push_back(glm::vec2(pos[0] + 1, pos[1]));
                newPos.push_back(glm::vec2(pos[0], pos[1]));
                newPos.push_back(glm::vec2(pos[0], pos[1] + 1));
                newPos.push_back(glm::vec2(pos[0], pos[1] + 2));
                
                if (board.isFree(newPos)){
                    //cout << "AQUI 3" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(0);
                    return true;
                }
            }

            
            break;
        }
            case 4:
            {
            std::vector<glm::vec2> newPos;
            glm::vec2 pos = blocos[ativo.getBlock(0)].getPosBoard();
            
            if (ativo.getOrientation() == 0){
                newPos.push_back(glm::vec2(pos[0] + 2, pos[1] + 1));
                newPos.push_back(glm::vec2(pos[0] + 1, pos[1] + 1));
                newPos.push_back(glm::vec2(pos[0], pos[1] + 1));
                newPos.push_back(glm::vec2(pos[0], pos[1] + 2));
                
                if (board.isFree(newPos)){
                    //cout << "AQUI 0" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(1);
                    return true;
                }
            }
            if (ativo.getOrientation() == 1){
                newPos.push_back(glm::vec2(pos[0], pos[1] + 2));
                newPos.push_back(glm::vec2(pos[0], pos[1] + 1));
                newPos.push_back(glm::vec2(pos[0], pos[1]));
                newPos.push_back(glm::vec2(pos[0] - 1, pos[1]));
                    
                if (board.isFree(newPos)){
                    //cout << "AQUI 1" << endl;
                        
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                        
                    ativo.setOrientation(2);
                    return true;
                }
                
            }
            if (ativo.getOrientation() == 2){
                newPos.push_back(glm::vec2(pos[0] - 2, pos[1]));
                newPos.push_back(glm::vec2(pos[0] - 1, pos[1]));
                newPos.push_back(glm::vec2(pos[0], pos[1]));
                newPos.push_back(glm::vec2(pos[0], pos[1] - 1));
                
                if (board.isFree(newPos)){
                    //cout << "AQUI 3" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(3);
                    return true;
                }
                
            }
            if (ativo.getOrientation() == 3){
                newPos.push_back(glm::vec2(pos[0], pos[1] - 2));
                newPos.push_back(glm::vec2(pos[0], pos[1] - 1));
                newPos.push_back(glm::vec2(pos[0], pos[1]));
                newPos.push_back(glm::vec2(pos[0] + 1, pos[1]));
                
                if (board.isFree(newPos)){
                    //cout << "AQUI 3" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(0);
                    return true;
                }
                
            }
            break;
        }
            case 5:
            {
            std::vector<glm::vec2> newPos;
            glm::vec2 pos = blocos[ativo.getBlock(0)].getPosBoard();
            
            if (ativo.getOrientation() == 0){
                newPos.push_back(glm::vec2(pos[0] - 1, pos[1] + 1));
                newPos.push_back(glm::vec2(pos[0], pos[1] + 1));
                newPos.push_back(glm::vec2(pos[0], pos[1]));
                newPos.push_back(glm::vec2(pos[0] + 1, pos[1]));
                
                if (board.isFree(newPos)){
                    //cout << "AQUI 0" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(1);
                    return true;
                }
            }
            
            if (ativo.getOrientation() == 1){
                newPos.push_back(glm::vec2(pos[0], pos[1] - 1));
                newPos.push_back(glm::vec2(pos[0], pos[1]));
                newPos.push_back(glm::vec2(pos[0] + 1, pos[1]));
                newPos.push_back(glm::vec2(pos[0] + 1, pos[1] + 1));
                
                if (board.isFree(newPos)){
                    //cout << "AQUI 1" << endl;
                    
                    blocos[ativo.getBlock(0)].move(newPos[0]);
                    blocos[ativo.getBlock(1)].move(newPos[1]);
                    blocos[ativo.getBlock(2)].move(newPos[2]);
                    blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                    ativo.setOrientation(0);
                    return true;
                }
            }

            break;
        }
            case 6:
            {
                std::vector<glm::vec2> newPos;
                glm::vec2 pos = blocos[ativo.getBlock(0)].getPosBoard();
            
                if (ativo.getOrientation() == 0){
                    newPos.push_back(glm::vec2(pos[0], pos[1] + 1));
                    newPos.push_back(glm::vec2(pos[0] - 1, pos[1] + 1));
                    newPos.push_back(glm::vec2(pos[0] - 1, pos[1]));
                    newPos.push_back(glm::vec2(pos[0] - 2, pos[1]));
                
                    if (board.isFree(newPos)){
                    //cout << "AQUI 0" << endl;
                    
                        blocos[ativo.getBlock(0)].move(newPos[0]);
                        blocos[ativo.getBlock(1)].move(newPos[1]);
                        blocos[ativo.getBlock(2)].move(newPos[2]);
                        blocos[ativo.getBlock(3)].move(newPos[3]);
                    
                        ativo.setOrientation(1);
                        return true;
                    }
                }
                
                if (ativo.getOrientation() == 1){
                    newPos.push_back(glm::vec2(pos[0], pos[1] - 1));
                    newPos.push_back(glm::vec2(pos[0], pos[1]));
                    newPos.push_back(glm::vec2(pos[0] - 1, pos[1]));
                    newPos.push_back(glm::vec2(pos[0] - 1, pos[1] + 1));
                    
                    if (board.isFree(newPos)){
                        cout << "AQUI 1" << endl;
                        
                        blocos[ativo.getBlock(0)].move(newPos[0]);
                        blocos[ativo.getBlock(1)].move(newPos[1]);
                        blocos[ativo.getBlock(2)].move(newPos[2]);
                        blocos[ativo.getBlock(3)].move(newPos[3]);
                        
                        ativo.setOrientation(0);
                        return true;
                    }
                }
                

                break;
            }
            case 7:
            {
                std::vector<glm::vec2> newPos;
                glm::vec2 pos = blocos[ativo.getBlock(0)].getPosBoard();
                
                if (ativo.getOrientation() == 0){
                    newPos.push_back(glm::vec2(pos[0] + 1, pos[1] + 1));
                    newPos.push_back(glm::vec2(pos[0], pos[1] + 1));
                    newPos.push_back(glm::vec2(pos[0], pos[1] + 2));
                    newPos.push_back(glm::vec2(pos[0] - 1, pos[1] + 1));
                    
                    if (board.isFree(newPos)){
                        //cout << "AQUI 0" << endl;
                        
                        blocos[ativo.getBlock(0)].move(newPos[0]);
                        blocos[ativo.getBlock(1)].move(newPos[1]);
                        blocos[ativo.getBlock(2)].move(newPos[2]);
                        blocos[ativo.getBlock(3)].move(newPos[3]);
                        
                        ativo.setOrientation(1);
                        return true;
                    }
                }
                
                if (ativo.getOrientation() == 1){
                    newPos.push_back(glm::vec2(pos[0], pos[1] + 1));
                    newPos.push_back(glm::vec2(pos[0], pos[1]));
                    newPos.push_back(glm::vec2(pos[0], pos[1] - 1));
                    newPos.push_back(glm::vec2(pos[0] - 1, pos[1]));
                    
                    if (board.isFree(newPos)){
                        //cout << "AQUI 0" << endl;
                        
                        blocos[ativo.getBlock(0)].move(newPos[0]);
                        blocos[ativo.getBlock(1)].move(newPos[1]);
                        blocos[ativo.getBlock(2)].move(newPos[2]);
                        blocos[ativo.getBlock(3)].move(newPos[3]);
                        
                        ativo.setOrientation(2);
                        return true;
                    }
                }
                
                if (ativo.getOrientation() == 2){
                    newPos.push_back(glm::vec2(pos[0] - 1, pos[1] - 1));
                    newPos.push_back(glm::vec2(pos[0], pos[1] - 1));
                    newPos.push_back(glm::vec2(pos[0], pos[1] - 2));
                    newPos.push_back(glm::vec2(pos[0] + 1, pos[1] - 1));
                    
                    if (board.isFree(newPos)){
                        //cout << "AQUI 0" << endl;
                        
                        blocos[ativo.getBlock(0)].move(newPos[0]);
                        blocos[ativo.getBlock(1)].move(newPos[1]);
                        blocos[ativo.getBlock(2)].move(newPos[2]);
                        blocos[ativo.getBlock(3)].move(newPos[3]);
                        
                        ativo.setOrientation(3);
                        return true;
                    }
                }
                
                if (ativo.getOrientation() == 3){
                    newPos.push_back(glm::vec2(pos[0] + 1, pos[1] - 1));
                    newPos.push_back(glm::vec2(pos[0] + 1, pos[1]));
                    newPos.push_back(glm::vec2(pos[0] + 2, pos[1]));
                    newPos.push_back(glm::vec2(pos[0] + 1, pos[1] + 1));
                    
                    if (board.isFree(newPos)){
                        //cout << "AQUI 0" << endl;
                        
                        blocos[ativo.getBlock(0)].move(newPos[0]);
                        blocos[ativo.getBlock(1)].move(newPos[1]);
                        blocos[ativo.getBlock(2)].move(newPos[2]);
                        blocos[ativo.getBlock(3)].move(newPos[3]);
                        
                        ativo.setOrientation(0);
                        return true;
                    }
                }

                
                break;
            }
            
        
        }
    }
    return false;
}

bool Scene::isRunning(){
    return running;
}

void Scene::createScenario() {
    
    int topBar = addModel(glm::vec3(0, 3.15, 0) + middlePoint);
    blocos[topBar].scale(glm::vec3(12, 1, 1));
    //cout << "TopBar: " << topBar << endl;
    
    int rightBar = addModel(glm::vec3(1.75, 0, 0) + middlePoint);
    blocos[rightBar].scale(glm::vec3(1, 22, 1));
    //cout << "RightBar: " << rightBar << endl;
    
    int bottomBar = addModel(glm::vec3(0, -3.15, 0) + middlePoint);
    blocos[bottomBar].scale(glm::vec3(12, 1, 1));
    
    int leftBar = addModel(glm::vec3(-1.75, 0, 0) + middlePoint);
    blocos[leftBar].scale(glm::vec3(1, 22, 1));
    
    
}

void Scene::printBoard() {
    board.print();
}

int Scene::getLines(){
    return lines;
}

bool Scene::getGame(){
    return gameOver;
}
