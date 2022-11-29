#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <cmath>
#include <vector>
#include <random>
#include "Physics.h"
using namespace std;

glm::vec3 gravitationalForce = glm::vec3(0,9.80665f,0);
Physics::Physics(float m, glm::vec3 p , glm::vec3 v, glm::vec3 a) {
	mass = m;
	pos = p;
	velocity = v;
	acceleration = a;
}
void Physics::updatePhysics(float dt, glm::vec3 &posO, glm::vec3 &velocityO) {
	pos += velocity * dt + 0.5f * acceleration * (dt * dt);
	velocity += acceleration * dt;
	posO = pos;
	velocityO = velocity;
}

void Physics::force(glm::vec3 f) { acceleration += f / mass; }
void Physics::force(glm::vec3 dir, float mag) { force(dir*mag); }
void Physics::accelera(glm::vec3 f) { acceleration += f; }
void Physics::accelera(glm::vec3 dir, float mag) { accelera(dir*mag); }
void Physics::impulse(glm::vec3 f, float dt) { velocity += (f / mass)*dt; }
void Physics::impulse(glm::vec3 dir, float mag, float dt) { impulse( (dir * mag) , dt); }
void Physics::TransEnergy(float j){
	if (j==0)
		return;

	//KE = 1/2*m*v^2
	float deltaVelocity = sqrt(2*abs(j)/mass);
	velocity += j > 0 ? deltaVelocity : -deltaVelocity;// if + add dv if not add -dv

}

