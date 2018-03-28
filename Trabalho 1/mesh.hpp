#ifndef MESHHPP
#define MESHHPP

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <functional>

using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
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


#include "structures.hpp"
class Mesh{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	//        <indice, repeticoes>
	std::map <int, int> priorList;

	void createPriorList();

	int getRemovingVertex();
	int findEdgePair(int index1);
	void removeEdge(int v1, int v2);

public:
	Mesh(char * path);
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getUvs();
	std::vector<glm::vec3> getNormals();
	std::vector<unsigned short> getIndices();

	std::map <int, int> getPriorList();
	
	void setIndex(std::vector<unsigned short> indices);
	void setIndexedVertex(std::vector<glm::vec3> indexed_vertices);
	void setPriorList(std::map <int, int> priorList);

	void remove();

	bool isFinal();
	
	void printPriorList();
	void printIndex();
	void printVertices();
};

#endif
