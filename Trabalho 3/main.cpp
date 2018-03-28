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
TwBar *tw_Camera;

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
	int barPos[2] = { 5, 5 };
	int barSize[2] = { 250, 212 };

	tw_Modelo = TwNewBar("Gerenciar Modelo");
	TwSetParam(tw_Modelo, NULL, "position", TW_PARAM_INT32, 2, &barPos);
	TwSetParam(tw_Modelo, NULL, "size", TW_PARAM_INT32, 2, &barSize);


	tw_Camera = TwNewBar("Gerenciar Camera");
	int C_barPos[2] = { 5, 230 };
	int C_barSize[2] = { 250, 212 };
	TwSetParam(tw_Camera, NULL, "position", TW_PARAM_INT32, 2, &C_barPos);
	//TwSetParam(tw_Modelo, NULL, "size", TW_PARAM_INT32, 2, &barSize);

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
	meuGerente.addMesh("mesh/suzanne.obj");
	meuGerente.addMesh("mesh/cube.obj");	
	meuGerente.addMesh("mesh/goose.obj");

	bool addModel = false;
	glm::vec3 M_startPoint(0,0,0);
	int models = meuGerente.getNumberOfModel();
	
	//TwAddVarRW(g_pToolBar, "speed", TW_TYPE_DOUBLE, &speed, " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");
	TwAddVarRO(tw_Modelo, "Modelos", TW_TYPE_INT32, &models, " label='Models:'");

	typedef enum { SUZANNE, CUBE,  GOOSE } MESH_TYPE;
	MESH_TYPE mesh = SUZANNE;
	TwEnumVal meshes[] = { { SUZANNE, "Suzanne" },{ CUBE, "Cube" },{ GOOSE, "Goose" } };
	TwType meshType = TwDefineEnum("TipoMalha", meshes, 3);

	TwAddVarRW(tw_Modelo, "Model: ", meshType, &mesh, NULL);
	TwAddVarRW(tw_Modelo, "Start Point:", TW_TYPE_DIR3F, &M_startPoint, NULL);
	TwAddButton(tw_Modelo, "ADD", ButtonAddModel, &addModel, NULL);


	//////////////////////////////////////////////////////////////////////////////////////////////
	int cams = meuGerente.getNumberOfCam(), C_active = 0;

	double time_translation = 4;
	float C_fieldOfView = 45.f, C_aspectRatio = (640.0f / 480.0f), C_nearValue = 0.1f, C_farValue = 1000000.0f;

	glm::vec3 C_eye(0, 0, 10), C_up(0, 1, 0), C_Center(0, 0, -1),
		C_translationEye(5, 0, 10);

	meuGerente.addCam(C_eye, C_Center, C_up, C_fieldOfView, C_aspectRatio, C_nearValue, C_farValue);

	bool addCam = false, addTranslation = false;

	TwAddVarRO(tw_Camera, "NCameras", TW_TYPE_INT32, &cams, " label='Num Cams:'");
	TwAddVarRW(tw_Camera, "Cameras", TW_TYPE_INT32, &C_active, "label='Active Cam:' step=1 min=0 max=15");

	TwAddSeparator(tw_Camera, "Create", NULL);
	TwAddVarRW(tw_Camera, "Eye X:", TW_TYPE_DIR3F, &C_eye, NULL);
	TwAddVarRW(tw_Camera, "Center X:", TW_TYPE_DIR3F, &C_Center, NULL);
	TwAddVarRW(tw_Camera, "Up X:", TW_TYPE_DIR3F, &C_up, NULL);

	TwAddVarRW(tw_Camera, "fov", TW_TYPE_FLOAT, &C_fieldOfView, "label='fov:' group='Perspective' step=0.1 min=0");
	TwAddVarRW(tw_Camera, "asp", TW_TYPE_FLOAT, &C_aspectRatio, "label='aspect ratio:' group='Perspective' step=0.1 min=0");
	TwAddVarRW(tw_Camera, "nv", TW_TYPE_FLOAT, &C_nearValue, "label='near value:' group='Perspective' step=0.1 min=0");
	TwAddVarRW(tw_Camera, "fv", TW_TYPE_FLOAT, &C_farValue, "label='far value:' group='Perspective' step=0.1 min=0");



	TwAddButton(tw_Camera, "ADD", ButtonAddModel, &addCam, NULL);

	TwAddSeparator(tw_Camera, "Animations", NULL);
	TwAddVarRW(tw_Camera, "Eye T:", TW_TYPE_DIR3F, &C_translationEye, NULL);
	TwAddVarRW(tw_Camera, "TimeT:", TW_TYPE_DOUBLE, &time_translation, "label='Time:' step=0.1 min=0");
	TwAddButton(tw_Camera, "Translation", ButtonAddModel, &addTranslation, NULL);

	TwAddSeparator(tw_Camera, "Transformacao", NULL);
	bool change = false;
	TwAddVarRW(tw_Camera, "Alterar", TW_TYPE_BOOL8, &change, " true=Enabled false=Disabled ");
	
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
			meuGerente.addModel(mesh, M_startPoint);
			models = meuGerente.getNumberOfModel();
			cout << "Modelo adicionado: " << mesh << endl;
			cout << "pos: " << M_startPoint.x << ", " << M_startPoint.y << "," << M_startPoint.z << endl;

			addModel = false;
		}

		if (addCam) {
			meuGerente.addCam(C_eye, C_Center, C_up, C_fieldOfView, C_aspectRatio, C_nearValue, C_farValue);
			cams = meuGerente.getNumberOfCam();
			meuGerente.printCamera();
			addCam = false;
		}

		if (addTranslation && change == false) {
			meuGerente.addTranslation(C_active, C_translationEye, time_translation);

			addTranslation = false;
		}

		if (change) {
			meuGerente.setViewMatrix(C_active, C_eye, C_Center, C_up);
			meuGerente.setProjectionMatrix(C_active, C_fieldOfView, C_aspectRatio, C_nearValue, C_farValue);
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

		if (C_active > (meuGerente.getNumberOfCam() - 1))
			C_active = (meuGerente.getNumberOfCam() - 1);

		if (C_active < 0)
			C_active = 0;

		meuGerente.draw(C_active, g_nWidth, g_nHeight, MatrixID, ModelMatrixID, ViewMatrixID, LightID, Texture, TextureID);

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
