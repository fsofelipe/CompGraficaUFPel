// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* g_pWindow;
unsigned int g_nWidth = 800, g_nHeight = 600;

// Include AntTweakBar
#include <AntTweakBar.h>
TwBar *tw_control;

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
#include <random>


#include "../include/scene.hpp"
#include "../include/bloco.hpp"
#include "../include/multiBlock.hpp"
#include "../include/aux.hpp"

void TW_CALL ButtonStart(void * clientData){
    bool *p = (bool *)clientData;
    *p = true;
}

void WindowSizeCallBack(GLFWwindow *pWindow, int nWidth, int nHeight) {
    
    g_nWidth = nWidth;
    g_nHeight = nHeight;
    glViewport(0, 0, g_nWidth, g_nHeight);
    TwWindowSize(g_nWidth, g_nHeight);
}

int main(void){
    
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(1, 7);
    int nUseMouse = 0;
    
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    g_pWindow = glfwCreateWindow(g_nWidth, g_nHeight, "CG UFPel", NULL, NULL);
    if (g_pWindow == NULL){
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(g_pWindow);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    check_gl_error();//OpenGL error from GLEW
    
    // Initialize the GUI
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(g_nWidth, g_nHeight);
    
    // Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
    glfwSetMouseButtonCallback(g_pWindow, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
    glfwSetCursorPosCallback(g_pWindow, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
    glfwSetScrollCallback(g_pWindow, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
    glfwSetKeyCallback(g_pWindow, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
    glfwSetCharCallback(g_pWindow, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar
    glfwSetWindowSizeCallback(g_pWindow, WindowSizeCallBack);
    
    //create the toolbar
    tw_control = TwNewBar("TETRIS");
    // Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
    
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(g_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPos(g_pWindow, g_nWidth / 2, g_nHeight / 2);
    
    // Dark blue background
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearColor(235.0f, 235.0f, 235.0f, 0.0f);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    // Create and compile our GLSL program from the shaders
    //GLuint programID = LoadShaders("../shaders/toonShader.vertexshader", "../shaders/toonShader.fragmentshader");
    GLuint programID = LoadShaders("../shaders/StandardShading.vertexshader", "../shaders/StandardShading.fragmentshader");
    
    
    
    //GLuint programID = LoadShaders("../StandardShading.vertexshader", "../StandardShading.fragmentshader");  //MAC
    //GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader" );
    
    // Get a handle for our "MVP" uniform
    GLuint MatrixID      = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID  = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");
    
    // Load the texture
    GLuint Texture = loadDDS("../mesh/uvmap.DDS");
    //GLuint Texture = loadDDS("uvmap.DDS"); //MAC
    
    
    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
    
    ///////////////////////////////////////////////////////////////////////////////////
    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    
    Scene Cena = Scene();
    

    

    

    int lines = 0;
    
    bool start = false, can = true;
    
    
    TwAddButton(tw_control, "START!", ButtonStart, &start, NULL);
    TwAddSeparator(tw_control, "stats", NULL);
    TwAddVarRW(tw_control, "Linhas: ", TW_TYPE_INT32, &lines, NULL);
    
    // For speed computation
    double lastTime = glfwGetTime();
    do{
        if (Cena.getGame()){
            start = false;
            cout << "GAME OVER!" << endl;
            break;
        }

        
        lines = Cena.getLines();
        
        if (start == true && can == true){
            can = false;
        }
        
        double currentTime = glfwGetTime();

        if (currentTime - lastTime >= 1.0){ // If last prinf() was more than
            Cena.moveDown();
            lastTime += 1.0;
        }
        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(glfwGetKey(g_pWindow, GLFW_KEY_LEFT) == GLFW_PRESS){
            if(glfwGetKey(g_pWindow, GLFW_KEY_LEFT) == GLFW_RELEASE){
                Cena.moveLeft();
            }
        }
        
        if(glfwGetKey(g_pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS){
            if(glfwGetKey(g_pWindow, GLFW_KEY_RIGHT) == GLFW_RELEASE){
                Cena.moveRight();
            }
        }
        
        if(glfwGetKey(g_pWindow, GLFW_KEY_DOWN) == GLFW_PRESS){
            if(glfwGetKey(g_pWindow, GLFW_KEY_DOWN) == GLFW_RELEASE){
                Cena.moveDown();
            }
        }
        
        if(glfwGetKey(g_pWindow, GLFW_KEY_UP) == GLFW_PRESS){
            if(glfwGetKey(g_pWindow, GLFW_KEY_UP) == GLFW_RELEASE){
                Cena.rotate();
            }
        }
        
        
        
        
        // Use our shader
        glUseProgram(programID);
        
        Cena.draw(g_nWidth, g_nHeight, MatrixID, ModelMatrixID, ViewMatrixID, LightID, Texture, TextureID);

        
        
         // define the range

        
        if(!Cena.isRunning() && start == true){
            Cena.createMultiBlock(int(distr(eng)));
            //Cena.printBoard();
        }
        
        // Draw tweak bars
        TwDraw();
        
        // Swap buffers
        glfwSwapBuffers(g_pWindow);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(g_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(g_pWindow) == 0);
    
    Cena.del();
    
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);
    
    // Terminate AntTweakBar and GLFW
    TwTerminate();
    glfwTerminate();
    
    return 0;
}

