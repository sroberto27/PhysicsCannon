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
void Physics::collisionFloor(glm::vec3 ballPos, float dt, glm::vec3 velo, float yF){
	if (ballPos.y<yF)
	{
		cout << "GROUND hit HERE!!!!!!!" << endl;
		
		glm::vec3 tempV = collisionFloorReaction(velo, 0.5f);
		updatePhysics(dt, ballPos, tempV);
	}
}
glm::vec3 Physics::collisionFloorReaction(glm::vec3 ballVelo, float coeficiente){
	return -1 * coeficiente*ballVelo;
}
void Physics::collisionBallsClones(Ball sPhArray[20],int sphNum) {
	float c = 0.3f;
	for (int i = 0; i < sphNum; i++)
	{
		for (int j = 0; j < sphNum; i++)
		{
			if (i!=j)
			{
				if ((glm::length((sPhArray[i].BallPos - sPhArray[j].BallPos))) < (2.0f * (48.0f*0.2f)));//*0.2));
				{
					glm::vec3 n = glm::normalize(sPhArray[i].BallPos - sPhArray[j].BallPos);
					if (glm::dot((sPhArray[i].BallV - sPhArray[j].BallV),n)<0)
					{
						glm::vec3 vN = glm::normalize(glm::dot((sPhArray[i].BallV - sPhArray[j].BallV),n)*n);
						sPhArray[i].BallV = sPhArray[i].BallV - ((1+c) / 2)*vN;
						sPhArray[j].BallV = sPhArray[j].BallV - ((1 + c) / 2)*vN;
					}
				}
			}
		}
	}
}




