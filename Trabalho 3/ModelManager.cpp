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

int ModelManager::addModel(int meshID, glm::vec3 pos) {
	models.push_back(Model(meshID, getMesh(meshID), pos));
	return (models.size() - 1);
}

int ModelManager::addCam(glm::vec3 position, glm::vec3 front, glm::vec3 up) {
	cams.push_back(Camera(position, front, up));
	return (cams.size() - 1);
}

int ModelManager::addCam(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fieldOfView, float aspectRatio, float nearValue, float farValue) {
	cams.push_back(Camera(position, front, up, fieldOfView, aspectRatio, nearValue, farValue));
	return (cams.size() - 1);
}

int ModelManager::getNumberOfCam() {
	return cams.size();
}

void ModelManager::addTranslation(int cam, glm::vec3 center, double time) {
	cams[cam].setTranslation(center, time, glfwGetTime());
}


void ModelManager::setViewMatrix(int cam, glm::vec3 position, glm::vec3 front, glm::vec3 up) {
	glm::mat4 m = glm::lookAt(position, front, up);
	cams[cam].setViewMatrix(m);
}
void ModelManager::setProjectionMatrix(int cam, float fieldOfView, float aspectRatio, float nearValue, float farValue) {
	glm::mat4 m = glm::perspective(fieldOfView, aspectRatio, nearValue, farValue);
	cams[cam].setProjectionMatrix(m);
}

void ModelManager::printCamera() {
	cout << "Cam [" << cams.size() - 1 << "]\n" << cams[cams.size() - 1].getViewMatrix() << endl;

}

void ModelManager::draw(int n_cam, unsigned int g_nWidth, unsigned int g_nHeight, GLuint MatrixID, GLuint ModelMatrixID, GLuint ViewMatrixID, GLuint LightID, GLuint Texture, GLuint TextureID) {
	int nUseMouse = 0;
	glm::mat4 ViewMatrix = cams[n_cam].getViewMatrix();
	glm::mat4 ProjectionMatrix = cams[n_cam].getProjectionMatrix();
	switch (cams[n_cam].getAnimation()) {
		case 1:
			cout << "" << cams[n_cam].getF_position() << endl;
			if ((cams[n_cam].getTime() + cams[n_cam].getInitTime()) >= glfwGetTime()) {
				glm::vec3 step;
				double endTime = cams[n_cam].getInitTime() + cams[n_cam].getTime();
				double currentTime = glfwGetTime();

				/*
				step.x = (cams[n_cam].getF_eye().x - cams[n_cam].getI_eye().x) * (currentTime / endTime);
				step.y = (cams[n_cam].getF_eye().y - cams[n_cam].getI_eye().y) * (currentTime / endTime);
				step.z = (cams[n_cam].getF_eye().z - cams[n_cam].getI_eye().z) * (currentTime / endTime);
				*/

				step.x = (cams[n_cam].getF_position().x) * (currentTime / endTime);
				step.y = (cams[n_cam].getF_position().y) * (currentTime / endTime);
				step.z = (cams[n_cam].getF_position().z) * (currentTime / endTime);



				cout << "inicio: " << step << endl;

				//step = step + cams[n_cam].getI_eye();

				//cout << "lookat: "<< step << endl;

				cams[n_cam].setViewMatrix(step, cams[n_cam].getI_front(), cams[n_cam].getI_up());
			}

			break;

	}

	for (int i = 0; i < models.size(); i++) {
		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(nUseMouse, g_nWidth, g_nHeight);
		//models[i].transform();

		glm::mat4 ModelMatrix = models[i].getMatrix();

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
