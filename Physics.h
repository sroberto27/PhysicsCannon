#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <fstream>
#include <string>
#include <iostream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <cmath>
#include <vector>
#include <random>
class Physics {
public:
	Physics(float massa, glm::vec3 position, glm::vec3 Velocita, glm::vec3 accelar);
	float mass;
	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	void updatePhysics(float dt, glm::vec3 &posO, glm::vec3 &velocityO);
	void force(glm::vec3 force);
	void force(glm::vec3 dir, float magnitud);
	void accelera(glm::vec3 acelearazione);
	void accelera(glm::vec3 dir, float magnitud);
	void impulse(glm::vec3 force, float deltaTime);
	void impulse(glm::vec3 dir, float magnitud, float deltaTime);
	void TransEnergy(float julios);

};