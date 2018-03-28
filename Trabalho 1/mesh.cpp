#include "mesh.hpp"
Mesh::Mesh(char * path) {
	bool res = loadOBJ(path, vertices, uvs, normals);

	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	createPriorList();
}

std::vector<glm::vec3> Mesh::getVertices() {
	return indexed_vertices;
}

std::vector<glm::vec2> Mesh::getUvs() {
	return indexed_uvs;
}

std::vector<glm::vec3> Mesh::getNormals() {
	return indexed_normals;
}

std::vector<unsigned short> Mesh::getIndices() {
	return indices;
}

void Mesh::createPriorList() {
	//priorList.clear();
	for (int i = 0; i < indices.size(); i++) {
		try {
			priorList.at(indices[i]) = priorList.at(indices[i]) + 1;
		} catch (const std::out_of_range& oor) {
			priorList.insert(std::pair<int, int>(indices[i], 1));
		}
	}
	
	//sort(priorList.begin(), priorList.end(), sortBySec);
}

void Mesh::setIndex(std::vector<unsigned short> indices){
	this->indices = indices;
}
void Mesh::setIndexedVertex(std::vector<glm::vec3> indexed_vertices){
	this->indexed_vertices = indexed_vertices;
}
void Mesh::setPriorList(std::map <int, int> priorList){
	this->priorList = priorList;
}

std::map <int, int> Mesh::getPriorList() {
	return priorList;
}

/* Retorna o vertice com o maior número de arestas e remove este da lista de prioridade */
int Mesh::getRemovingVertex() {
	std::map<int, int>::iterator it = std::max_element(priorList.begin(), priorList.end(),
													   [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
		return p1.second < p2.second; });

	std::pair<int, int> ret = std::make_pair(it->first, it->second);

	priorList.erase(it);
//	cout << "aqui: " << ret.first << endl;
	return ret.first;
}

int Mesh::findEdgePair(int index1) {
	int mod = index1 % 3;
	int ret = -1;
	
	if (mod == 0 || mod == 1)
		ret = index1 + 1;

	if (mod == 2)
		ret = index1 - 2;

	return ret;
}

/* Remove uma aresta formada pelos vertices v1, v2 ao remover é inserido um novo vertice v que é o ponto médio entre v1 e v2 */
void Mesh::removeEdge(int i1, int i2) {
	if (i1 < 0 && i2 < 0)
		return;

	glm::vec3 v1 = indexed_vertices[i1];
	glm::vec3 v2 = indexed_vertices[i2];


	glm::vec3 v = pontoMedio(v1, v2);

	for (int i = 0; i < indexed_vertices.size(); i++){
		if (isEquals(indexed_vertices[i], v1) || isEquals(indexed_vertices[i], v2)) {
			indexed_vertices[i] = v;
		}
	}
	
}

void Mesh::remove() {
	if (!isFinal()) {
		int primeiro = getRemovingVertex();

		int segundo = findEdgePair(primeiro);

		removeEdge(primeiro, segundo);
	}
}

bool Mesh::isFinal() {
	int count = 0;
	
	std::map<int, int>::iterator it = priorList.begin();
	for (auto address_entry : priorList){

		if (address_entry.second == 1) {
			count++;
		}
	}
	
	if ((count + 3) == priorList.size() - 1)
		return true;

	

	return false;
}

void Mesh::printPriorList() {
	cout << "Prior List: [" << priorList.size() << "]" << endl;
	std::map<int, int>::iterator it = priorList.begin();
	for (it = priorList.begin(); it != priorList.end(); ++it)
		cout << "<" << it->first << " : " << it->second << ">" << endl;

	
}

void Mesh::printIndex() {
	for (int i = 0; i < indices.size(); i++) {
		cout << i << ": " << indices[i] << endl;
	}
}

void Mesh::printVertices() {
	cout << "tamanho: " << vertices.size() << endl;
	for (int i = 0; i < vertices.size(); i++) {
		cout << i << " : <" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ">" << endl;
	}
}