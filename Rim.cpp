#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <cmath>
#include <vector>
#include "Rim.h"
using namespace std;

Rim::Rim() {
	prec = 48;
	inner = 0.8f;
	outer = 0.2f;
	init();
}

Rim::Rim(float in, float out, int precIn) {
	prec = precIn;
	inner = in;
	outer = out;
	init();
}

float Rim::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Rim::init() {
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { sTangents.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { tTangents.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

	// calculate first ring
	for (int i = 0; i < prec + 1; i++) {
		float amt = toRadians(i*360.0f / prec);

		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 initPos(rMat * glm::vec4(outer, 0.0f, 0.0f, 1.0f));

		vertices[i] = glm::vec3(initPos + glm::vec3(inner, 0.0f, 0.0f));
		texCoords[i] = glm::vec2(0.0f, ((float)i / (float)prec));

		//Per-Vertex
		//rMat = glm::rotate(glm::mat4(1.0f), amt, vertices[i-1]);
		//tTangents[i] = glm::vec3(rMat * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
		tTangents[i] = glm::vec3(vertices[i - 1].x - vertices[i].x, vertices[i - 1].y - vertices[i].y, 0.0f);
		//sTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f,0.0f));
		sTangents[i] = glm::vec3(vertices[i].x, vertices[i].y, 0.0f);
		//normals[i] = glm::cross(tTangents[i], sTangents[i]);
		normals[i] = glm::normalize(sTangents[i] + tTangents[i]);
	}
	// rotate the first ring about Y to get the other rings
	for (int ring = 1; ring < prec + 1; ring++) {
		for (int i = 0; i < prec + 1; i++) {
			float amt = (float)toRadians((float)ring * 360.0f / (prec));

			glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			vertices[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(vertices[i], 1.0f));

			texCoords[ring*(prec + 1) + i] = glm::vec2((float)ring*2.0f / (float)prec, texCoords[i].t);
			//if (texCoords[ring*(prec + 1) + i].s > 1.0) texCoords[ring*(prec+1)+i].s -= 1.0f; // deleted and added someting at end of inint

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			sTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(sTangents[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			tTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(tTangents[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			normals[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(normals[i], 1.0f));
		}
	}
	// calculate triangle indices
	for (int ring = 0; ring < prec; ring++) {
		for (int i = 0; i < prec; i++) {
			indices[((ring*prec + i) * 2) * 3 + 0] = ring * (prec + 1) + i;
			indices[((ring*prec + i) * 2) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices[((ring*prec + i) * 2) * 3 + 2] = ring * (prec + 1) + i + 1;
			indices[((ring*prec + i) * 2 + 1) * 3 + 0] = ring * (prec + 1) + i + 1;
			indices[((ring*prec + i) * 2 + 1) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices[((ring*prec + i) * 2 + 1) * 3 + 2] = (ring + 1)*(prec + 1) + i + 1;
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}
int Rim::getNumVertices() { return numVertices; }
int Rim::getNumIndices() { return numIndices; }
std::vector<int> Rim::getIndices() { return indices; }
std::vector<glm::vec3> Rim::getVertices() { return vertices; }
std::vector<glm::vec2> Rim::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Rim::getNormals() { return normals; }
std::vector<glm::vec3> Rim::getStangents() { return sTangents; }
std::vector<glm::vec3> Rim::getTtangents() { return tTangents; }