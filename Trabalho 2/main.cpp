// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* g_pWindow;
unsigned int g_nWidth = 1024, g_nHeight = 768;

// Include AntTweakBar
#include <AntTweakBar.h>
TwBar *tw_Modelo;

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

#include "Mesh.hpp"
#include "Model.hpp"
#include "ModelManager.hpp"


void WindowSizeCallBack(GLFWwindow *pWindow, int nWidth, int nHeight) {

	g_nWidth = nWidth;
	g_nHeight = nHeight;
	glViewport(0, 0, g_nWidth, g_nHeight);
	TwWindowSize(g_nWidth, g_nHeight);
}

void TW_CALL ButtonAddModel(void * clientData) {
	bool * p = (bool *)clientData;
	*p = true;
}

int main(void) {
	int nUseMouse = 0;


	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	g_pWindow = glfwCreateWindow(g_nWidth, g_nHeight, "CG UFPel", NULL, NULL);
	if (g_pWindow == NULL) {
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
	tw_Modelo = TwNewBar("CG UFPel ToolBar");
	TwSetTopBar(tw_Modelo);

	// Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
	double speed = 0.0;

	// Add 'bgColor' to 'bar': it is a modifable variable of type TW_TYPE_COLOR3F (3 floats color)
	vec3 oColor(0.0f);


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(g_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(g_pWindow, g_nWidth / 2, g_nHeight / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

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
	GLuint programID = LoadShaders("shaders/StandardShading.vertexshader", "shaders/StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texture
	GLuint Texture = loadDDS("mesh/uvmap.DDS");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	////////////////////////////////////////////////////////////////////////////////////////////
	ModelManager meuGerente = ModelManager();
	meuGerente.addMesh("mesh/cube.obj");
	meuGerente.addMesh("mesh/suzanne.obj");
	meuGerente.addMesh("mesh/goose.obj");


	bool addModel = false;
	double startPoint[3] = { 0,0,0 };
	int models = meuGerente.getNumberOfModel(), model = 0, axis = 0, axisP = 0;

	//TwAddVarRW(g_pToolBar, "speed", TW_TYPE_DOUBLE, &speed, " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");
	TwAddVarRO(tw_Modelo, "Modelos", TW_TYPE_INT32, &models, " label='Models:'");

	typedef enum { CUBE, SUZANNE, GOOSE } MESH_TYPE;
	MESH_TYPE mesh = CUBE;
	TwEnumVal meshes[] = { { CUBE, "Cube" },{ SUZANNE, "Suzanne" },{ GOOSE, "Goose" } };
	TwType meshType = TwDefineEnum("TipoMalha", meshes, 3);

	TwAddVarRW(tw_Modelo, "Model: ", meshType, &mesh, NULL);
	TwAddVarRW(tw_Modelo, "Start Point X:", TW_TYPE_DOUBLE, &startPoint[0], "step = 0.01");
	TwAddVarRW(tw_Modelo, "Start Point Y:", TW_TYPE_DOUBLE, &startPoint[1], "step = 0.01");
	TwAddVarRW(tw_Modelo, "Start Point Z:", TW_TYPE_DOUBLE, &startPoint[2], "step = 0.01");
	TwAddButton(tw_Modelo, "ADD", ButtonAddModel, &addModel, NULL);


	////////////////////////////////////////////////////////////////////////////////////////////


	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	do {
		check_gl_error();

		if (addModel) {
			meuGerente.addModel(mesh, glm::vec3(startPoint[0], startPoint[1], startPoint[2]));
			models = meuGerente.getNumberOfModel();
			cout << "Modelo adicionado: " << mesh << endl;
			cout << "pos: " << startPoint[0] << ", " << startPoint[1] << "," << startPoint[2] << endl;

			addModel = false;
		}

		//use the control key to free the mouse
		/*if (glfwGetKey(g_pWindow, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
		nUseMouse = 1;
		else
		nUseMouse = 0;
		*/

		// Measure speed
		/*
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
		// printf and reset
		printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		nbFrames  = 0;
		lastTime += 1.0;
		}
		*/

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		meuGerente.draw(g_nWidth, g_nHeight, MatrixID, ModelMatrixID, ViewMatrixID, LightID, Texture, TextureID);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Draw tweak bars
		TwDraw();

		// Swap buffers
		glfwSwapBuffers(g_pWindow);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(g_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(g_pWindow) == 0);

	// Cleanup VBO and shader

	meuGerente.del();




	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Terminate AntTweakBar and GLFW
	TwTerminate();
	glfwTerminate();

	return 0;
}

