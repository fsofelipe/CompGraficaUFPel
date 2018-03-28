#include "ModelManager.hpp"
std::ostream &operator<< (std::ostream &out, const glm::mat4 &m) {
	out << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << "\n"
		<< m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << "\n"
		<< m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << "\n"
		<< m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << "\n";

	return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec3 &v) {
	out << "<" << v.x << ", " << v.y << ", " << v.z << ">";
	return out;
}

void ModelManager::addMesh(char *path) {
	meshes.push_back(Mesh(path, meshes.size()));
}

Mesh ModelManager::getMesh(int meshID) {
	return meshes.at(meshID);
}

int ModelManager::addModel(int meshID) {
	models.push_back(Model(meshID, getMesh(meshID), glm::vec3(0, 0, 0)));
	return (models.size() - 1);
}

int ModelManager::addModel(int meshID, glm::vec3 pos) {
	models.push_back(Model(meshID, getMesh(meshID), pos));
	return (models.size() - 1);
}

glm::mat4 ModelManager::scale(glm::vec3 s) {
	glm::mat4 m = glm::mat4(1.0);
	return glm::scale(m, s);
}

glm::mat4 ModelManager::translate(glm::vec3 v) {
	glm::mat4 m = glm::mat4(1.0);
	return glm::translate(m, v);
}

glm::mat4 ModelManager::rotation(float angle, glm::vec3 v) {
	glm::mat4 m = glm::mat4(1.0);
	//modelMatrix = glm::rotate(angle, rotationAxis);
	return glm::rotate(m, angle, v);
}

glm::mat4 ModelManager::shear(glm::vec2 v) {
	glm::mat4 m;
	m[0][0] = 1;
	m[0][1] = v.y;
	m[0][2] = 0;
	m[0][3] = 0;

	m[1][0] = v.x;
	m[1][1] = 1;
	m[1][2] = 0;
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 1;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	return m;
}

void ModelManager::addStatic(int model, glm::mat4 m) {
	models[model].addTransformation(m);
}

void ModelManager::transform(int model) {
	models[model].transform();
}

void ModelManager::transform(int model, glm::mat4 m) {
	models[model].addTransformation(m);
}

void ModelManager::addTranslation(int model, glm::vec3 v, double time) {
	models[model].setTranslation(v, time, glfwGetTime());
}

void ModelManager::addRotationAxis(int model, glm::vec3 axis, double angle, double time) {
	models[model].setRotationAxis(axis, angle, time, glfwGetTime());
}

void ModelManager::addScale(int model, glm::vec3 scale, double time) {
	models[model].setScale(scale, time, glfwGetTime());
}

void ModelManager::addRotationPoint(int model, glm::vec3 central, glm::vec3 axis, glm::vec3 initialPos, double angle, double time) {
	models[model].setRotationPoint(central, axis, angle, initialPos, time, glfwGetTime());
}

void ModelManager::addShear(int model, glm::vec3 shear, double time) {
	models[model].setShear(shear, time, glfwGetTime());
}

void ModelManager::draw(unsigned int g_nWidth, unsigned int g_nHeight, GLuint MatrixID, GLuint ModelMatrixID, GLuint ViewMatrixID, GLuint LightID, GLuint Texture, GLuint TextureID) {
	int nUseMouse = 0;
	for (int i = 0; i < models.size(); i++) {
		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(nUseMouse, g_nWidth, g_nHeight);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		//models[i].transform();

		glm::mat4 ModelMatrix;

		switch (models[i].getAnimation()) {
			case 1:

				//TRANSLACAO
				if ((models[i].getTime() + models[i].getInitialTime()) >= glfwGetTime()) {
					glm::vec3 p;
					double endTime = models[i].getInitialTime() + models[i].getTime();
					double currentTime = glfwGetTime();

					p.x = ((models[i].getFinalPosition().x - models[i].getInitialPosition().x) * currentTime) / endTime;
					p.y = ((models[i].getFinalPosition().y - models[i].getInitialPosition().y) * currentTime) / endTime;
					p.z = ((models[i].getFinalPosition().z - models[i].getInitialPosition().z) * currentTime) / endTime;

					cout << p << endl;

					if (models[i].isStarted()) {
						p.x = p.x - (models[i].getMatrix()[3][0] - models[i].getInitialPosition().x);
						p.y = p.y - (models[i].getMatrix()[3][1] - models[i].getInitialPosition().y);
						p.z = p.z - (models[i].getMatrix()[3][2] - models[i].getInitialPosition().z);

					} else {
						models[i].setStart(true);
					}

					models[i].setMatrix(glm::translate(models[i].getMatrix(), p));

				} else {
					models[i].setMatrix(glm::translate(glm::mat4(1.0), models[i].getFinalPosition()));
				}
				break;
			case 2:
				//ROTACAO EM UM EIXO
				if ((models[i].getTime() + models[i].getInitialTime()) >= glfwGetTime()) {
					double endTime = models[i].getInitialTime() + models[i].getTime();
					double currentTime = glfwGetTime();

					double rot = (models[i].getAngle() * currentTime) / endTime;
					double a = rot;
					rot = rot - models[i].getCurrentAngle();

					models[i].setMatrix(glm::rotate(models[i].getMatrix(), (float)rot, models[i].getAxis()), a);

				}/* else {
				 glm::mat4 m = glm::translate(glm::mat4(1.0), models[i].getInitialPosition());

				 models[i].setMatrix(glm::rotate(m, (float)models[i].getAngle(), models[i].getAxis()));
				 }*/
				break;
			case 3:
				//ROTACAO EM UM PONTO
				if ((models[i].getTime() + models[i].getInitialTime()) >= glfwGetTime()) {
					double endTime = models[i].getInitialTime() + models[i].getTime();
					double currentTime = glfwGetTime();

					double rot = (models[i].getAngle() * currentTime) / endTime;
					double a = rot;
					rot = rot - models[i].getCurrentAngle();

					cout << "models[i].getMatrix()" << endl;
					cout << models[i].getMatrix() << endl;


					glm::vec3 temp = glm::vec3(models[i].getMatrix()[3][0], models[i].getMatrix()[3][1], models[i].getMatrix()[3][2]);

					cout << "temp" << temp << endl;

					//envia para a origem
					glm::mat4 m = glm::translate(glm::mat4(1.0), models[i].getFinalPosition());

					cout << "M0" << endl;
					cout << m << endl;

					//rotaciona
					m = glm::rotate(m, (float)rot, models[i].getAxis());

					//translaciona de volta
					m = glm::translate(m, temp);

					cout << "M" << endl;
					cout << m << endl;

					models[i].setMatrix(m, a);
				}
				break;

			case 4:
				//ESCALA
				if ((models[i].getTime() + models[i].getInitialTime()) >= glfwGetTime()) {


					double endTime = models[i].getInitialTime() + models[i].getTime();
					double currentTime = glfwGetTime();

					glm::vec3 s, temp;

					s.x = ((models[i].getFinalPosition().x) * currentTime) / endTime;
					s.y = ((models[i].getFinalPosition().y) * currentTime) / endTime;
					s.z = ((models[i].getFinalPosition().z) * currentTime) / endTime;

					temp = s;

					glm::mat4 m = glm::translate(glm::mat4(1.0), models[i].getInitialPosition());
					m = glm::scale(m, s);

					models[i].setMatrixAxis(m, temp);
				}
				break;
			case 5:
				if ((models[i].getTime() + models[i].getInitialTime()) >= glfwGetTime()) {
					cout << "Matriz" << endl;
					cout << models[i].getMatrix() << endl;

					double endTime = models[i].getInitialTime() + models[i].getTime();
					double currentTime = glfwGetTime();

					glm::vec3 shear = models[i].getFinalPosition();

					shear.x = (models[i].getFinalPosition().x * currentTime) / endTime;
					shear.y = (models[i].getFinalPosition().y * currentTime) / endTime;
					shear.z = (models[i].getFinalPosition().z * currentTime) / endTime;

					glm::vec3 temp = shear;

					cout << "shear: " << shear << endl;
					cout << "atual: " << models[i].getAxis() << endl;
					cout << "final: " << models[i].getFinalPosition() << endl;


					shear.x = shear.x - models[i].getAxis().x;
					shear.y = shear.y - models[i].getAxis().y;
					shear.z = shear.z - models[i].getAxis().z;

					glm::mat4 m = glm::mat4(1.0);
					m = glm::shearX3D(m, (float)shear.y, (float)shear.z);
					m = glm::shearY3D(m, (float)shear.x, (float)shear.z);
					m = glm::shearZ3D(m, (float)shear.x, (float)shear.y);

					//m = models[i]

					models[i].setMatrixAxis(m, temp);
				}

				/*
				this->ModelMatrix = glm::shearX3D(this->ModelMatrix, y, z);
				this->ModelMatrix = glm::shearY3D(this->ModelMatrix, x, z);
				this->ModelMatrix = glm::shearZ3D(this->ModelMatrix, x, y);
				*/


				break;
		}


		ModelMatrix = models[i].getMatrix();




		glm::mat4 NormalMatrix = glm::mat4(1.0);

		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;



		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);




		glm::vec3 lightPos = glm::vec3(0, 5, 0);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, models[i].getVertexBuffer());
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
		glBindBuffer(GL_ARRAY_BUFFER, models[i].getUvBuffer());
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
		glBindBuffer(GL_ARRAY_BUFFER, models[i].getNormalBuffer());
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[i].getElementBuffer());

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,           // mode
			meshes.at(models[i].getMeshID()).getIndices().size(),   // count
			GL_UNSIGNED_SHORT,      // type
			(void*)0                // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

}

int ModelManager::getNumberOfModel() {
	return models.size();
}

void ModelManager::del() {
	for (int i = 0; i < models.size(); i++) {
		glDeleteBuffers(1, &models[i].getVertexRef());
		glDeleteBuffers(1, &models[i].getUvRef());
		glDeleteBuffers(1, &models[i].getNormalRef());
		glDeleteBuffers(1, &models[i].getElementRef());
	}
}
