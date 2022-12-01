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
#include <stack>
using namespace std;
class Ball
{
private:
	int numVertices;
	int numIndices;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	void init(int);
	float toRadians(float degrees);

public:
	Ball();
	Ball(int prec);
	int getNumVertices();
	glm::vec3 BallPos, BallV;
	float x, y, z;
	int getNumIndices();
	void lunchBall2(glm::vec3 cannonDir, float dt);
	void lunchBall(Ball (&sPhArray)[20], int sphNum,stack <glm::mat4> (&mvStack), glm::vec3 cannonDir, float dt);
	void drawBall(Ball &mySphera, GLuint(&vboSphere)[4], GLuint(&Vao)[1], stack <glm::mat4>(&mvStack), GLuint gemTexture, glm::mat4 &pMat, glm::mat4 &invTrMat, GLuint &mvLoc, GLuint &projLoc, GLuint &nLoc);
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getTangents();
};