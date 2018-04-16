#include "structures.hpp"

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
	out << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">";

	return out;
}

glm::vec3 pontoMedio(glm::vec3 v1, glm::vec3 v2) {
	glm::vec3 v;
	v.x = (v1.x + v2.x) / 2;
	v.y = (v1.y + v2.y) / 2;
	v.z = (v1.z + v2.z) / 2;

	return v;
}

bool isEquals(glm::vec3 v1, glm::vec3 v2) {
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) return true;
	return false;
}

State::State(std::vector<unsigned short> indexes, std::vector<glm::vec3> indexed_vertices, std::map <int, int> priorList) {
	this->indexes = indexes;
	this->indexed_vertices = indexed_vertices;
	this->priorList = priorList;
}

std::vector<unsigned short> State::getIndexes() {
	return indexes;
}

std::vector<glm::vec3> State::getIndexedVertex() {
	return indexed_vertices;
}

std::map <int, int> State::getPriorList() {
	return priorList;
}