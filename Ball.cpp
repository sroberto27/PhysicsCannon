#include <cmath>
#include <vector>
#include <iostream>
#include <glm\glm.hpp>
#include <stack>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <glm\gtx\quaternion.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Ball.h"
#include "Physics.h"
using namespace std;



Physics myFisica(1.0f, glm::vec3(1.05, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
Ball::Ball() {
	init(48);
}

Ball::Ball(int prec) {
	init(prec);
}

float Ball::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }



void Ball::init(int prec) {

	y = 1.05f;
	x = z = 0.0f;
	BallPos = glm::vec3(x, y, z);
	BallV = glm::vec3(0, 0, 0);
	myFisica.accelera(glm::vec3(0, 0, 1), -9.8f);// applay gravity
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { tangents.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

	// calculate triangle vertices
	for (int i = 0; i <= prec; i++) {
		for (int j = 0; j <= prec; j++) {
			float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
			float x = -(float)cos(toRadians(j*360.0f / prec))*(float)abs(cos(asin(y)));
			float z = (float)sin(toRadians(j*360.0f / (float)(prec)))*(float)abs(cos(asin(y)));
			vertices[i*(prec + 1) + j] = glm::vec3(x, y, z);
			texCoords[i*(prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
			normals[i*(prec + 1) + j] = glm::vec3(x, y, z);

			// calculate tangent vector
			if (((x == 0) && (y == 1) && (z == 0)) || ((x == 0) && (y == -1) && (z == 0))) {
				tangents[i*(prec + 1) + j] = glm::vec3(0.0f, 0.0f, -1.0f);
			}
			else {
				tangents[i*(prec + 1) + j] = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(x, y, z));
			}
		}
	}
	// calculate triangle indices
	for (int i = 0; i < prec; i++) {
		for (int j = 0; j < prec; j++) {
			indices[6 * (i*prec + j) + 0] = i * (prec + 1) + j;
			indices[6 * (i*prec + j) + 1] = i * (prec + 1) + j + 1;
			indices[6 * (i*prec + j) + 2] = (i + 1)*(prec + 1) + j;
			indices[6 * (i*prec + j) + 3] = i * (prec + 1) + j + 1;
			indices[6 * (i*prec + j) + 4] = (i + 1)*(prec + 1) + j + 1;
			indices[6 * (i*prec + j) + 5] = (i + 1)*(prec + 1) + j;
		}
	}
}
glm::mat4 buildTranslate2(float x, float y, float z) {
	glm::mat4 r = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
	return r;
}

glm::mat4 rotateX2(float rad) {
	glm::mat4 r = glm::mat4(
		1, 0, 0, 0,
		0, cos(rad), sin(rad), 0,
		0, -sin(rad), cos(rad), 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 rotateY2(float rad) {
	glm::mat4 r = glm::mat4(
		cos(rad), 0, -sin(rad), 0,
		0, 1, 0, 0,
		sin(rad), 0, cos(rad), 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 rotateZ2(float rad) {
	glm::mat4 r = glm::mat4(
		cos(rad), sin(rad), 0, 0,
		-sin(rad), cos(rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 buildScala2(float x, float y, float z) {
	glm::mat4 r = glm::mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
	return r;
}
glm::vec3 unitVector(glm::vec3 v) {
	return glm::normalize(v);
}
void Ball::lunchBall(Ball (&sPhArray)[20], int sphNum,stack <glm::mat4>(&mvStack), glm::vec3 cannonDir, float dt) {
	//Sphera
//mvStack.push(mvStack.top
//glUseProgram(renderingProgram);
//restart = mvStack.top();
	mvStack.top() *= buildTranslate2(BallPos.x, BallPos.y, BallPos.z)*buildScala2(0.1, 0.2, 0.2);
	//myFisica.accelera(glm::vec3(0, 0, 1), 0.98f);
	myFisica.accelera(glm::vec3(0, 0, 1), 0.98);
	myFisica.impulse(unitVector(cannonDir), 0.0005, dt);
	myFisica.updatePhysics(dt, BallPos, BallV);
	myFisica.collisionFloor(BallPos, dt, BallV, -0.003);
	//myFisica.collisionBallsClones(sPhArray, sphNum);
	cout << "x,y,z : " << BallPos.x << " , " << BallPos.y << " , " << BallPos.z << endl;
	cout << " velozidad x,y,z : " << BallV.x << " , " << BallV.y << " , " << BallV.z << endl;
	cout << " velozidad sph: " << BallV.length() << endl;
	cout << " velozidad physics: " << myFisica.velocity.length() << endl;
	cout << " accceleration x,y,z : " << BallV.x << " , " << BallV.y << " , " << BallV.z << endl;
	cout << " accceleration : " << myFisica.acceleration.length() << endl;
	cout << "paseed by physic" << endl;
	//mvStack.top() *= startPos * scaleBall;


}
void Ball::lunchBall2(glm::vec3 cannonDir, float dt) {
	//Sphera
//mvStack.push(mvStack.top
//glUseProgram(renderingProgram);
//restart = mvStack.top();
	//myFisica.force(glm::vec3(0, 0, 1), -9.8);
	myFisica.accelera(glm::vec3(0, 0, 1), 0.98);
	myFisica.impulse(unitVector(cannonDir), 0.0005, dt);
	myFisica.updatePhysics(dt, BallPos, BallV);
	cout << "x,y,z : " << BallPos.x << " , " << BallPos.y << " , " << BallPos.z << endl;
	cout << " velozidad : " << BallV.length() << endl;
	cout << " accceleration : " << myFisica.acceleration.length() << endl;
	cout << "paseed by physic" << endl;
	//mvStack.top() *= startPos * scaleBall;


}
void Ball::drawBall(Ball &mySphera, GLuint(&vboSphere)[4], GLuint(&Vao)[1], stack <glm::mat4>(&mvStack), GLuint gemTexture, glm::mat4 &pMat, glm::mat4 &invTrMat, GLuint &mvLoc, GLuint &projLoc, GLuint &nLoc) {
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboSphere[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboSphere[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, gemTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSphere[3]);
	glDrawElements(GL_TRIANGLE_STRIP, mySphera.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();
}

int Ball::getNumVertices() { return numVertices; }
int Ball::getNumIndices() { return numIndices; }
std::vector<int> Ball::getIndices() { return indices; }
std::vector<glm::vec3> Ball::getVertices() { return vertices; }
std::vector<glm::vec2> Ball::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Ball::getNormals() { return normals; }
std::vector<glm::vec3> Ball::getTangents() { return tangents; }