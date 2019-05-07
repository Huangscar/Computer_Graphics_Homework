#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Camera {
private:
	
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 worldUp;
	float pitch;
	float yaw;
public:
	glm::vec3 cameraPosition;
	Camera() {
		cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
		pitch = 0.0f;
		yaw = 0.0f;
	}
	glm::mat4 getViewMatrix() {
		return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	}
	void moveForward(float const distance) {
		cameraPosition += cameraFront * distance;
	}
	void moveBackword(float const distance) {
		cameraPosition -= cameraFront * distance;
	}
	void moveLeft(float const distance) {
		cameraPosition -= cameraRight * distance;
	}
	void moveRight(float const distance) {
		cameraPosition += cameraRight * distance;
	}
	void rotate(float const xOffset, float const yOffset) {
		yaw += xOffset;
		pitch += yOffset;
		if (yaw > 89.9f) {
			yaw = 89.9f;
		}
		if (pitch > 89.9f) {
			pitch = 89.9f;
		}
		cameraFront = glm::normalize(glm::vec3(cos(glm::radians(yaw))*cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw))*cos(glm::radians(pitch))));
		cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
	}
};

#endif