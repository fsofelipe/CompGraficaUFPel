#ifndef STRUCTURESHPP
#define STRUCTURESHPP

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>


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


std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec);

glm::vec3 pontoMedio(glm::vec3 v1, glm::vec3 v2);

bool isEquals(glm::vec3 v1, glm::vec3 v2);


class State {
	private:
		std::vector<unsigned short> indexes;
		std::vector<glm::vec3> indexed_vertices;
		std::map <int, int> priorList;
			
	public:
		State(std::vector<unsigned short> indexes, std::vector<glm::vec3> indexed_vertices, std::map <int, int> priorList);

		std::vector<unsigned short> getIndexes();
		std::vector<glm::vec3> getIndexedVertex();
		std::map <int, int> getPriorList();
};
#endif