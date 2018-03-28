#include "Camera.hpp"
Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) {
	viewMatrix = glm::lookAt(position, position + front, up);
	projectionMatrix = getProjectionMatrix();

	animation = 0;
	this->I_position = position;
	this->I_front = front;
	this->I_up = up;
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fieldOfView, float aspectRatio, float nearValue, float farValue) {

	viewMatrix = glm::lookAt(position, position + front, up);
	projectionMatrix = glm::perspective(fieldOfView, aspectRatio, nearValue, farValue);

	animation = 0;
	this->I_position = position;
	this->I_front = front;
	this->I_up = up;
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->nearValue = nearValue;
	this->farValue = farValue;
}


glm::mat4 Camera::getViewMatrix() {
	return viewMatrix;
}

void Camera::setViewMatrix(glm::vec3 position, glm::vec3 front, glm::vec3 up) {
	viewMatrix = viewMatrix = glm::lookAt(position, position + front, up);
}

void Camera::setViewMatrix(glm::mat4 m) {
	viewMatrix = m;
}

glm::mat4 Camera::getProjectionMatrix() {
	return projectionMatrix;
}

void Camera::setProjectionMatrix(glm::mat4 m) {
	projectionMatrix = m;
}

int Camera::getAnimation() {
	return animation;
}

double Camera::getTime() {
	return time;
}
double Camera::getInitTime() {
	return initTime;
}

glm::vec3 Camera::getI_position() {
	return I_position;
}
glm::vec3 Camera::getF_position() {
	return F_position;
}

glm::vec3 Camera::getI_front() {
	return I_front;
}
glm::vec3 Camera::getI_up() {
	return I_up;
}

void Camera::setTranslation(glm::vec3 position, double time, double initTime) {
	this->animation = 1;
	this->F_position = position;
	this->time = time;
	this->initTime = initTime;
}
