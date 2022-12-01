/* HEADER-----
Assignment 5 CSCE 515

Name: Roberto Salazar ULID c00416436

date: 

\


*/
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
#include "Vase.h"
#include "Arrow.h"
#include "Barrel.h"
#include "Trunnion.h"
#include "Cheek.h"
#include "Hub.h"
#include "Stock.h"
#include "Rim.h"
#include "Spoke.h"
#include "Screw.h"
#include "Utils.h"
#include "Axle.h"
#include "Ground.h"
#include "Pedestal.h"
#include "SmallObj.h"
#include "Torus.h"
#include "Ball.h"
#include "FrustrumConne.h"
using namespace std;
int prevMouseX = 0, prevMouseY = 0;
bool mouseHeld = false;
float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 1
#define numVBOs 4
float AngleIncrement = 1.0f;
float cameraX, cameraY, cameraZ;
float torLocX, torLocY, torLocZ;
GLuint renderingProgram, renderingProgramCubeMap, renderingProgramPump;
GLuint vboBarrel[numVBOs];
GLuint vboHub[numVBOs];
GLuint vboSpoke[numVBOs];
GLuint vboScrew[numVBOs];
GLuint vboRim[numVBOs];
GLuint vboTrunnion[numVBOs];
GLuint vboCheek[numVBOs];
GLuint vboStock[numVBOs];
GLuint vboAxle[numVBOs];
GLuint vboArrow[numVBOs];
GLuint vboGround[numVBOs];
GLuint vboPedestal[numVBOs];
GLuint vboSmallObj[numVBOs];
GLuint vboCube[numVBOs];
GLuint vboSky[numVBOs];
GLuint vboSphere[numVBOs];
GLuint vao[numVAOs];
GLuint brickTexture;
GLuint arrowTexturez, arrowTexturex, arrowTexturey;
GLuint metalTexture;
GLuint woodTexture, woodNailTexture, skyboxTexture,gemTexture;
GLuint innerArrowTexturez, innerArrowTexturex, innerArrowTexturey;
glm::quat yRot, xRot, zRot,wheelRot;
glm::vec4 V;
float rotAmt = 0.0f;
bool shotFire = false;
bool inicia = true;
bool lightFlag = true;
bool V1toV2View = true; //true is for V1 false is for V2
bool  ViewFalg = true;
glm::mat4  elevation, azimuth,scale, restart, V2TzScalation;
//Camara Vars
float eyeX, eyeY, eyeZ, centerX,centerY,centerZ, upX, upY,upZ;
// variable for display
GLuint mvLoc, projLoc, mLoc;
GLuint mvALoc, projALoc, mALoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, M;
glm::mat4  MA, MA1, MA2, screwTx, Lrot;
glm::mat4 temp;
float Ax, Ay, Az,scaleV,wR,trunnionR,screwR,baseYr,baseZ;
stack <glm::mat4> mvStack;
//New light stuff
GLuint  nLoc;
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;
glm::mat4 invTrMat, rMat;
glm::vec3 currentLightPos, transformed;
float lightPos[3];
glm::vec3 lightLoc = glm::vec3(5.0f, 2.0f, 0.0f);
//  light
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// white material
float* matAmb = Utils::TransAmbient();
float* matDif = Utils::TransDiffuse();
float* matSpe = Utils::TransSpecular();
float matShi = Utils::TransShininess();


Arrow myArrow(10, 1, 30, 30, 10, 1, 160, 1);
Barrel myBarrel(10, 1, 30, 30, 10, 1, 160, 1);
Cheek myCheek(10, 1, 30, 30, 10, 1, 160, 1);
Stock myStock(10, 1, 30, 30, 10, 1, 160, 1);
Trunnion myTrunnion(10, 1, 30, 30, 10, 1, 160, 1);
Screw myScrew(10, 1, 30, 30, 10, 1, 160, 1);
Hub myHub(10, 1, 30, 30, 10, 1, 160, 1);
Rim myRim(1.5,0.2,48);
Spoke mySpoke(10, 1, 30, 30, 10, 1, 160, 1);
Axle myAxle(10, 1, 30, 30, 10, 1, 160, 1);
Ground myGround(10, 1, 30, 30, 10, 1, 160, 1);
Pedestal myPedestal(10, 1, 30, 30, 10, 1, 160, 1);
SmallObj mySmallObj(10, 1, 30, 30, 10, 1, 160, 1);
Torus myTorus(0.5f, 0.2f, 48);
Torus myCube(0.5f, 0.2f, 48);
Ball mySphera[20];
FrustumConne myFrustum();
//time
float dt, lastTime,currentTime;
int spheraN = 0;




glm::mat4 buildTranslate(float x, float y, float z) {
	glm::mat4 r = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
	return r;
}

glm::mat4 rotateX(float rad) {
	glm::mat4 r = glm::mat4(
		1, 0, 0, 0,
		0, cos(rad), sin(rad), 0,
		0, -sin(rad), cos(rad), 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 rotateY(float rad) {
	glm::mat4 r = glm::mat4(
		cos(rad), 0, -sin(rad), 0,
		0, 1, 0, 0,
		sin(rad), 0, cos(rad), 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 rotateZ(float rad) {
	glm::mat4 r = glm::mat4(
		cos(rad), sin(rad), 0, 0,
		-sin(rad), cos(rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	return r;
}
glm::mat4 buildScala(float x, float y, float z) {
	glm::mat4 r = glm::mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
	return r;
}




//MOUSE hover
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x_pos = 0.0;
	double y_pos = 0.0;
	glfwGetCursorPos(window, &x_pos, &y_pos);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mouseHeld = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mouseHeld = false;
		azimuth = elevation = buildTranslate(0, 0, 0);
	}
}
void mouse_movement_callback(GLFWwindow* window, double xpos, double ypos) {


	if (mouseHeld)
	{
		(xpos - prevMouseX == 0 ? (azimuth =  rotateY(toRadians(0))) : (xpos - prevMouseX > 0 ? (azimuth = rotateY(toRadians(-0.4))) : (azimuth = rotateY(toRadians(0.4)))));
		(ypos - prevMouseY == 0 ? (elevation =  rotateX(toRadians(0))) : (ypos - prevMouseY > 0 ? (elevation =  rotateX(toRadians(-0.4))) : (elevation = rotateX(toRadians(0.4)))));
		
		
	}
	prevMouseX = xpos;
	prevMouseY = ypos;
}
void setupVertices(void) {
	std::vector<int> ind;
	std::vector<glm::vec3> vert ;
	std::vector<glm::vec2> tex ;
	std::vector<glm::vec3> norm ;

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;
	

	//Setup Ground

	ind = myGround.getIndices();
	vert = myGround.getVertices();
	tex = myGround.getTexCoords();
	norm = myGround.getNormals();

	for (int i = 0; i < myGround.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboGround);

	glBindBuffer(GL_ARRAY_BUFFER, vboGround[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboGround[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboPedestal[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboGround[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Barrel SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myBarrel.getIndices();
	vert = myBarrel.getVertices();
	tex = myBarrel.getTexCoords();
	norm = myBarrel.getNormals();

	for (int i = 0; i < myBarrel.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboBarrel);

	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboBarrel[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////
	//SetUp CUBE

	float vertexPositions[108] = {
		-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vboCube);

	glBindBuffer(GL_ARRAY_BUFFER, vboCube[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

	//SetUp Sky

	float cubeVertexPositions[108] =
	{ -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};
	float cubeTextureCoord[72] =
	{ 1.00f, 0.6666666f, 1.00f, 0.3333333f, 0.75f, 0.3333333f,	// back face lower right
		0.75f, 0.3333333f, 0.75f, 0.6666666f, 1.00f, 0.6666666f,	// back face upper left
		0.75f, 0.3333333f, 0.50f, 0.3333333f, 0.75f, 0.6666666f,	// right face lower right
		0.50f, 0.3333333f, 0.50f, 0.6666666f, 0.75f, 0.6666666f,	// right face upper left
		0.50f, 0.3333333f, 0.25f, 0.3333333f, 0.50f, 0.6666666f,	// front face lower right
		0.25f, 0.3333333f, 0.25f, 0.6666666f, 0.50f, 0.6666666f,	// front face upper left
		0.25f, 0.3333333f, 0.00f, 0.3333333f, 0.25f, 0.6666666f,	// left face lower right
		0.00f, 0.3333333f, 0.00f, 0.6666666f, 0.25f, 0.6666666f,	// left face upper left
		0.25f, 0.3333333f, 0.50f, 0.3333333f, 0.50f, 0.0000000f,	// bottom face upper right
		0.50f, 0.0000000f, 0.25f, 0.0000000f, 0.25f, 0.3333333f,	// bottom face lower left
		0.25f, 1.0000000f, 0.50f, 1.0000000f, 0.50f, 0.6666666f,	// top face upper right
		0.50f, 0.6666666f, 0.25f, 0.6666666f, 0.25f, 1.0000000f		// top face lower left
	};

	int numTorusVertices = myTorus.getNumVertices();
	int numTorusIndices = myTorus.getNumIndices();

	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myTorus.getIndices();
	vert = myTorus.getVertices();
	tex = myTorus.getTexCoords();
	norm = myTorus.getNormals();

	for (int i = 0; i < numTorusVertices; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vboSky);

	glBindBuffer(GL_ARRAY_BUFFER, vboSky[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions) * 4, cubeVertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSky[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTextureCoord) * 4, cubeTextureCoord, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSky[2]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSky[3]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSky[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	//Setup Pedestal
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myPedestal.getIndices();
	vert = myPedestal.getVertices();
	tex = myPedestal.getTexCoords();
	norm = myPedestal.getNormals();

	for (int i = 0; i < myPedestal.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboPedestal);

	glBindBuffer(GL_ARRAY_BUFFER, vboPedestal[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboPedestal[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboPedestal[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboPedestal[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////
		//Setup Sphera
	/*ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = mySphera.getIndices();
	vert = mySphera.getVertices();
	tex = mySphera.getTexCoords();
	norm = mySphera.getNormals();
	for (int i = 0; i < mySphera.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboSphere);

	glBindBuffer(GL_ARRAY_BUFFER, vboSphere[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSphere[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSphere[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSphere[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
	*/

	//Setup SmallObject
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = mySmallObj.getIndices();
	vert = mySmallObj.getVertices();
	tex = mySmallObj.getTexCoords();
	norm = mySmallObj.getNormals();

	for (int i = 0; i < mySmallObj.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboSmallObj);

	glBindBuffer(GL_ARRAY_BUFFER, vboSmallObj[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSmallObj[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSmallObj[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSmallObj[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////


		//Axle 
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();

	ind = myAxle.getIndices();
	vert = myAxle.getVertices();
	tex = myAxle.getTexCoords();
	norm = myAxle.getNormals();

	for (int i = 0; i < myAxle.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboAxle);

	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboAxle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Hub SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myHub.getIndices();
	vert = myHub.getVertices();
	tex = myHub.getTexCoords();
	norm = myHub.getNormals();

	for (int i = 0; i < myHub.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboHub);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHub[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Rim SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myRim.getIndices();
	vert = myRim.getVertices();
	tex = myRim.getTexCoords();
	norm = myRim.getNormals();

	for (int i = 0; i < myRim.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboRim);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboRim[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Spoke SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = mySpoke.getIndices();
	vert = mySpoke.getVertices();
	tex = mySpoke.getTexCoords();
	norm = mySpoke.getNormals();

	for (int i = 0; i < mySpoke.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboSpoke);

	glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSpoke[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Cheek SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myCheek.getIndices();
	vert = myCheek.getVertices();
	tex = myCheek.getTexCoords();
	norm = myCheek.getNormals();

	for (int i = 0; i < myCheek.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboCheek);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCheek[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Stock SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myStock.getIndices();
	vert = myStock.getVertices();
	tex = myStock.getTexCoords();
	norm = myStock.getNormals();

	for (int i = 0; i < myStock.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboStock);

	glBindBuffer(GL_ARRAY_BUFFER, vboStock[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboStock[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboStock[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboStock[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Screw SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myScrew.getIndices();
	vert = myScrew.getVertices();
	tex = myScrew.getTexCoords();
	norm = myScrew.getNormals();

	for (int i = 0; i < myScrew.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboScrew);

	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboScrew[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Barrel SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myArrow.getIndices();
	vert = myArrow.getVertices();
	tex = myArrow.getTexCoords();
	norm = myArrow.getNormals();

	for (int i = 0; i < myArrow.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboArrow);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////



	//Trunnion SetUP
	ind.clear();
	vert.clear();
	tex.clear();
	norm.clear();

	pvalues.clear();
	tvalues.clear();
	nvalues.clear();
	ind = myTrunnion.getIndices();
	vert = myTrunnion.getVertices();
	tex = myTrunnion.getTexCoords();
	norm = myTrunnion.getNormals();

	for (int i = 0; i < myTrunnion.getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vboTrunnion);

	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboTrunnion[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);


	///////////








}
//too shiny
void installLights(glm::mat4 vMatrix) {
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	//glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	//glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	//glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	//glProgramUniform1f(renderingProgram, mshiLoc, matShi);
}
void reStartLight() {

	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);
}

void init(GLFWwindow* window) {
	  renderingProgramCubeMap = Utils::createShaderProgram("vertShaderOld.glsl", "fragShaderOld.glsl");
	  renderingProgram = Utils::createShaderProgram("vertShaderL.glsl", "fragShaderL.glsl");
	  renderingProgramPump = Utils::createShaderProgram("vertShaderExp.glsl", "fragShaderExp.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 12.0f;
	torLocX = 0.0f; torLocY = 0.0f; torLocZ = -0.5f;
	lastTime = 0;
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	//pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	
	pMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	);
	
	vMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	vMat = vMat * buildScala(0.6,0.6,0.6);
	
	//vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	 M = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	 V2TzScalation= Lrot = elevation=azimuth=MA = MA1 = MA2 = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	yRot = xRot = zRot=wheelRot = glm::quat(0, 0, 0, 0);
/*	MA = buildScala(0.5, 0.2, 0.2)*MA;
	MA1 = MA1 * rotateZ(toRadians(90));
	MA1 = buildScala(0.2, 0.5, 0.2)*MA1;
	MA2 = MA2 * rotateY(toRadians(-90));
	MA2 = buildScala(0.2, 0.2, 0.5)*MA2;*/
	baseZ = 0.6;
	baseYr  =Ax = Ay = Az = wR = trunnionR = screwR = 0;
	screwTx = buildTranslate(0, 0, 0);
	V = glm::vec4(0,0,0,0);
	scaleV = 1.0;
	scale = buildScala(scaleV,scaleV,scaleV);
	setupVertices();
	brickTexture = Utils::loadTexture("brick1.jpg");
	arrowTexturey = Utils::loadTexture("brick2.jpg");
	arrowTexturez = Utils::loadTexture("brick3.jpg");
	arrowTexturex = Utils::loadTexture("brick4.jpg");
	metalTexture = Utils::loadTexture("metal.jpg"); 
	woodTexture = Utils::loadTexture("wood.jpg"); 
	gemTexture = Utils::loadTexture("Gem.jpg");
	woodNailTexture = Utils::loadTexture("woodNail.jpg");
	innerArrowTexturex = Utils::loadTexture("innerArrow.jpg");
	innerArrowTexturey = Utils::loadTexture("innerArrow2.jpg");
	innerArrowTexturez = Utils::loadTexture("innerArrow3.jpg");
	skyboxTexture = Utils::loadTexture("alien.jpg");
	//skyboxTexture = Utils::loadCubeMap("cubeMap"); // expects a folder name
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_PRIMITIVE_RESTART);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(-2.0f, 2.0f);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

}

void display(GLFWwindow* window, double currentTime, float xTr, float yTr, float zTr, glm::mat4 &temp) {

	
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	reStartLight();
	glUseProgram(renderingProgram);
	mLoc = glGetUniformLocation(renderingProgram, "m_matrix");
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");//

	//MV
	vMat *= buildTranslate(0, 0, 0) * glm::inverse( azimuth *elevation*scale) ;
	mvStack.push(vMat);
	mvStack.push(mvStack.top());
	
	// Arrows
	//X
	//mvStack.push(mvStack.top());
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, arrowTexturex);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//Y
	/*mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(90));
	*/
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(0, 0, 0) * rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, arrowTexturey);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//Z
	/*mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(-90));*/
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(0, 0, 0) * rotateY(toRadians(-90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, arrowTexturez);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();


	//Ground
	//mvStack.push(mvStack.top
	glUseProgram(renderingProgram);
	restart = mvStack.top();
	mvStack.top() *= buildTranslate(0, 0, 0.52)*buildScala(3, 11, 0.1);//*rotateY(toRadians(-45));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboGround[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboGround[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboGround[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myGround.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();
	mvStack.top() = restart;//*buildScala(1,0.2,4);
	mvStack.push(mvStack.top());

	//Pedestal
	//mvStack.push(mvStack.top
	glUseProgram(renderingProgram);
	restart = mvStack.top();
	mvStack.top() *= buildTranslate(0.3, 0.7, 0.57)*buildScala(1, 1, 1)*rotateY(toRadians(90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboPedestal[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboPedestal[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboPedestal[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myPedestal.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();
	mvStack.top() = restart;//*buildScala(1,0.2,4);
	mvStack.push(mvStack.top());

	

	//cube
	glUseProgram(renderingProgram);
	restart = mvStack.top();
	mvStack.top() *= buildScala(0.35f, 0.35f, 0.35f)*rotateY(toRadians(90));
	mvStack.push(mvStack.top());
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc

	glBindBuffer(GL_ARRAY_BUFFER, vboCube[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	//glDrawArrays(GL_TRIANGLES, 0, 36);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
	//glDisable(GL_DEPTH_TEST);
	//glDrawArrays(GL_TRIANGLES, 0, 36);// uncoment/comment to draw cube
	//glEnable(GL_DEPTH_TEST);
	mvStack.pop();
	mvStack.top() = restart;//*buildScala(1,0.2,4);
	mvStack.push(mvStack.top());
	//SkyCube
	glUseProgram(renderingProgramCubeMap);
	restart = mvStack.top();
	mvStack.top() *= buildTranslate(0, 0, 0);//* buildScala(2.0f, 2.0f, 2.0f);// *rotateY(toRadians(90));
	mvStack.push(mvStack.top());
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc

	glBindBuffer(GL_ARRAY_BUFFER, vboSky[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboSky[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
	//glDisable(GL_DEPTH_TEST);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glEnable(GL_DEPTH_TEST);
	mvStack.pop();
	mvStack.top() = restart;//*buildScala(1,0.2,4);
	mvStack.push(mvStack.top());

	//SmalObj
	//mvStack.push(mvStack.top
	glUseProgram(renderingProgramPump);
	restart = mvStack.top();
	mvStack.top() *= Lrot * buildTranslate(0.3, 0.7, 0)*buildScala(0.3, 0.3, 0.3)*rotateY(toRadians(90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboSmallObj[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboSmallObj[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, gemTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSmallObj[3]);
	glDrawElements(GL_TRIANGLE_STRIP, mySmallObj.getIndices().size(), GL_UNSIGNED_INT, 0);
	//Light 

	glUseProgram(renderingProgram);
	currentLightPos = glm::vec3(lightLoc.x, lightLoc.y, lightLoc.z);
	currentLightPos = glm::vec3(Lrot * glm::vec4(currentLightPos, 1.0f));
	installLights(mvStack.top());
	invTrMat = glm::transpose(glm::inverse(mvStack.top()));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	mvStack.pop();
	mvStack.top() = restart;//*buildScala(1,0.2,4);
	mvStack.push(mvStack.top());




	//base
	/*mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	temp= mvStack.top();
	temp += ((glm::toMat4(xRot)*glm::toMat4(yRot)*glm::toMat4(zRot))*V);
	mvStack.top() = temp;
	mvStack.push(mvStack.top());
	mvStack.top() *= ((glm::toMat4(xRot)*glm::toMat4(yRot)*glm::toMat4(zRot))*glm::toMat4(zRot));
	mvStack.push(mvStack.top());
	mvStack.top() *= (glm::toMat4(yRot)*(glm::toMat4(xRot)*glm::toMat4(yRot)*glm::toMat4(zRot)));
	*/
	glUseProgram(renderingProgram);
	temp = mvStack.top();
	temp += ((glm::toMat4(xRot)*glm::toMat4(yRot)*glm::toMat4(zRot))*V);
	mvStack.top() *= rotateZ(toRadians(baseYr)) *  buildTranslate(baseZ, 0, 0)* temp * ((glm::toMat4(xRot)*glm::toMat4(yRot)*glm::toMat4(zRot))*glm::toMat4(zRot)) * (glm::toMat4(yRot)*(glm::toMat4(xRot)*glm::toMat4(yRot)*glm::toMat4(zRot)));
	mvStack.push(mvStack.top());

	// Arrows BASE
	//X
	glUseProgram(renderingProgram);

	mvStack.top() *= buildScala(0.6, 0.6, 0.6);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, innerArrowTexturex);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();


	//Y
	/*mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(0.6, 0.6, 0.6);*/
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(0, 0, 0) * rotateZ(toRadians(90)) * buildScala(0.6, 0.6, 0.6);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, innerArrowTexturey);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//Z
	/*mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0, 0, 0);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(-90));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(0.6, 0.6, 0.6);*/
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(0, 0, 0) * rotateY(toRadians(-90)) * buildScala(0.6, 0.6, 0.6);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboArrow[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, innerArrowTexturez);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboArrow[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myArrow.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//CArriage rify
	/*mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(-45));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(-0.5,0,-0.5);
	*/
	glUseProgram(renderingProgram);
	mvStack.top() *= rotateY(toRadians(80)) * rotateZ(toRadians(-15)) * buildTranslate(-0.5, -0.5, -1) * buildScala(0.8,0.8,0.8);
	mvStack.push(mvStack.top());

	//Axle -- dont metter

	//mvStack.top() *= buildScala(1.5, 1.5, 1.5);
	//mvStack.push(mvStack.top());
	glUseProgram(renderingProgram);
	mvStack.top() *= buildScala(1.5, 1.5, 1.5) * rotateY(toRadians(-90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboAxle[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboAxle[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myAxle.getIndices().size(), GL_UNSIGNED_INT, 0);
	//mvStack.pop();
	//Hub rigth
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(1.7, 0, 0) * rotateX(toRadians(wR)) *  rotateZ(toRadians(180));
	//mvStack.push(mvStack.top());
	//mvStack.top() *= buildTranslate(1, 0, 0);
	//mvStack.push(mvStack.top());
	//mvStack.top() *= rotateX(toRadians(wR));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboHub[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHub[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myHub.getIndices().size(), GL_UNSIGNED_INT, 0);
	//mvStack.pop();

	//RimRight
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(0.4, 0, 0) *  rotateZ(toRadians(90));
	//mvStack.push(mvStack.top());
	//mvStack.top() *= buildTranslate(0.4, 0, 0);
	//mvStack.push(mvStack.top());
	//mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotateX(wR not metter
	//mvStack.push(mvStack.top()); not metter
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboRim[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboRim[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myRim.getIndices().size(), GL_UNSIGNED_INT, 0);
	//mvStack.pop();
	//RimSpokesRight
	//1
	for (int i = 0; i < 4; i++)
	{
		glUseProgram(renderingProgram);
		mvStack.top() *= rotateZ(toRadians((i * 360) / 4)) * buildTranslate(0.4, 0, 0) * rotateX(toRadians(90));
		//mvStack.push(mvStack.top());
		//mvStack.top() *= rotateZ(toRadians((i * 360) / 4));
		//mvStack.push(mvStack.top());
		//mvStack.top() *= buildTranslate(0.4, 0, 0);
		//mvStack.push(mvStack.top());
		//mvStack.top() *= rotateX(toRadians(90));
		mvStack.push(mvStack.top());
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
		glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		reStartLight();
		glActiveTexture(GL_TEXTURE0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
		glBindTexture(GL_TEXTURE_2D, metalTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glPrimitiveRestartIndex(0xFFFF);
		glEnable(GL_PRIMITIVE_RESTART);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSpoke[3]);
		glDrawElements(GL_TRIANGLE_STRIP, mySpoke.getIndices().size(), GL_UNSIGNED_INT, 0);

	}

	//mvStack.pop();


	//Hub LEfty
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(0, 1, 0)  *rotateZ(toRadians(90));
	//mvStack.push(mvStack.top());
	//mvStack.top() *= buildTranslate(0, -2, 0);
	//mvStack.push(mvStack.top());
	//mvStack.top() *= rotateY(toRadians(wR));
	//mvStack.push(mvStack.top());
	//mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboHub[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHub[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHub[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myHub.getIndices().size(), GL_UNSIGNED_INT, 0);
	//mvStack.pop();

	//RimLeft
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(0.4, 0, 0) * rotateZ(toRadians(90));
	//mvStack.push(mvStack.top());
	//mvStack.top() *= buildTranslate(0.4, 0, 0);
	//mvStack.push(mvStack.top());
	//mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	//mvStack.top() *= rotateX(wR not metter
	//mvStack.push(mvStack.top()) not metter;
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboRim[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboRim[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboRim[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myRim.getIndices().size(), GL_UNSIGNED_INT, 0);
	//mvStack.pop();
	//RimSpokesLeft
	//1
	for (int i = 0; i < 4; i++)
	{
		glUseProgram(renderingProgram);
		mvStack.top() *= rotateZ(toRadians((i * 360) / 4)) * buildTranslate(0.4, 0, 0) * rotateX(toRadians(90));
		//mvStack.push(mvStack.top());
		//mvStack.top() *= rotateZ(toRadians((i * 360) / 4));
		//mvStack.push(mvStack.top());
		//mvStack.top() *= buildTranslate(0.4, 0, 0);
		//mvStack.push(mvStack.top());
		//mvStack.top() *= rotateX(toRadians(90));
		mvStack.push(mvStack.top());
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
		glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vboSpoke[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		reStartLight();
		glActiveTexture(GL_TEXTURE0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
		glBindTexture(GL_TEXTURE_2D, metalTexture);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glPrimitiveRestartIndex(0xFFFF);
		glEnable(GL_PRIMITIVE_RESTART);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSpoke[3]);
		glDrawElements(GL_TRIANGLE_STRIP, mySpoke.getIndices().size(), GL_UNSIGNED_INT, 0);

	}

	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();

	//Cheel lefr
	/*mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0.3,0.3 , -1.1);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(-90));*/
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(1, 0.4, -1) * rotateY(toRadians(-90)) * rotateX(toRadians(90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCheek[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myCheek.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();

	//Cheel right
	/*mvStack.push(mvStack.top());*/
	glUseProgram(renderingProgram);
	mvStack.top() *= buildTranslate(0, 1, 0);
	mvStack.push(mvStack.top());

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboCheek[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCheek[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myCheek.getIndices().size(), GL_UNSIGNED_INT, 0);
	mvStack.pop();


	//Trunio 
	/*
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(90));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(-1.5,-1, -0.1);
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(1.5, 0.7, 0.5);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateX(toRadians(trunnionR));*/
	glUseProgram(renderingProgram);
	mvStack.top() *= rotateZ(toRadians(90)) * buildTranslate(-1.5, -1, -0.1) * buildScala(1.5, 0.7, 0.5) * rotateX(toRadians(trunnionR));
	mvStack.push(mvStack.top());

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboTrunnion[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboTrunnion[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myTrunnion.getIndices().size(), GL_UNSIGNED_INT, 0);
	//mvStack.pop();

	//barrel

	/*mvStack.push(mvStack.top());
	mvStack.top() *= rotateZ(toRadians(-90));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(-1.6,0.68,-0.4);
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(6, 1.4, 3);*/
	glUseProgram(renderingProgram);
	mvStack.top() *= rotateZ(toRadians(-90)) * buildTranslate(-2, 0.62, -0.4)*buildScala(6, 1.4, 3);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboBarrel[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboBarrel[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myBarrel.getIndices().size(), GL_UNSIGNED_INT, 0);

	//Sphera
	/*
	//mvStack.push(mvStack.top
	glUseProgram(renderingProgram);
	//restart = mvStack.top();
	mvStack.top() *= buildTranslate(1.05, 0, 0)*buildScala(0.1, 0.2 ,0.2);
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

	reStartLight();
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
	*/
	//mvStack.top() = restart;//*buildScala(1,0.2,4);
	//mvStack.push(mvStack.top());
	glUseProgram(renderingProgram);
	reStartLight();
	if (shotFire)
	{
		mySphera[spheraN].lunchBall(mySphera,spheraN,mvStack, glm::vec3(mvStack.top()[3][0], mvStack.top()[3][1], mvStack.top()[3][2]), dt);
		cout << "Base --- x,y,z : " << mvStack.top()[3][0] << " , " << mvStack.top()[3][1] << " , " << mvStack.top()[3][2] << endl;
		mySphera[spheraN].drawBall(mySphera[spheraN], vboSphere, vao, mvStack, gemTexture, pMat, invTrMat, mvLoc, projLoc, nLoc);
		shotFire = false;
		spheraN++;
		cout << "sphera numeber: " << spheraN << endl;
	}
		for (int i = 0; i < spheraN; i++)
		{
			
			mySphera[i].lunchBall(mySphera, spheraN, mvStack, mySphera[i].BallPos, dt);
			mySphera[i].drawBall(mySphera[i], vboSphere, vao, mvStack, gemTexture, pMat, invTrMat, mvLoc, projLoc, nLoc);
		}

	
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	//Stock
	/*mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(1, 0.6, 0.6);
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(200));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(0,0, -0.9);*/
	glUseProgram(renderingProgram);
	mvStack.top() *= buildScala(5, 1.5, 1.5) * buildTranslate(0, 0.1, 0.6)* rotateX(toRadians(-90)) * rotateY(toRadians(35));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboStock[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboStock[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, woodNailTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboStock[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myStock.getIndices().size(), GL_UNSIGNED_INT, 0);

	//mvStack.pop();

	//Screw
	/*mvStack.push(mvStack.top());
	mvStack.top() *= rotateY(toRadians(250));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildTranslate(-0.6, 0, -0.2);
	mvStack.push(mvStack.top());
	mvStack.top() *= screwTx;
	mvStack.push(mvStack.top());
	mvStack.top() *= rotateX(toRadians(screwR));
	mvStack.push(mvStack.top());
	mvStack.top() *= buildScala(2.5, 0.6, 0.6);*/
	glUseProgram(renderingProgram);
	mvStack.top() *= rotateY(toRadians(228)) * buildTranslate(-0.6, 0, -0.2) * screwTx * rotateX(toRadians(screwR)) * buildScala(2, 0.5, 0.5);
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboScrew[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	reStartLight();
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//The wrapping function should go here not in init torus
	glBindTexture(GL_TEXTURE_2D, metalTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glPrimitiveRestartIndex(0xFFFF);
	glEnable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboScrew[3]);
	glDrawElements(GL_TRIANGLE_STRIP, myScrew.getIndices().size(), GL_UNSIGNED_INT, 0);




	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();
	mvStack.pop();

	glUseProgram(renderingProgramCubeMap);

	//mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cameraX, cameraY, cameraZ));
	//mvMat = vMat * mMat;
	//restart = mvStack.top();
	//mvStack.top() = glm::inverse(azimuth) *glm::inverse(elevation)*glm::inverse(scale);
	//mvStack.push(mvStack.top());
	mvLoc = glGetUniformLocation(renderingProgramCubeMap, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgramCubeMap, "proj_matrix");
	mvStack.top() *= buildScala(1.2, 1.2, 1.2)*rotateX(toRadians(-90));
	mvStack.push(mvStack.top());
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc
	glBindBuffer(GL_ARRAY_BUFFER, vboSky[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboSky[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
	//glDisable(GL_DEPTH_TEST);
	//glDrawArrays(GL_TRIANGLES, 0, 36);//drawSky
	//glEnable(GL_DEPTH_TEST);
	mvStack.pop();
	//mvStack.top() = restart;
	//mvStack.push(mvStack.top());

	

	//Sky simple
	glUseProgram(renderingProgramCubeMap);
	restart = mvStack.top();
	mvStack.top() *= buildScala(5.0f, 5.0f, 5.0f);// *rotateY(toRadians(90));
	mvStack.push(mvStack.top());

	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));//Light new nloc

	glBindBuffer(GL_ARRAY_BUFFER, vboSky[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vboSky[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyboxTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
	//glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glEnable(GL_DEPTH_TEST);
	mvStack.pop();
	mvStack.top() = restart;//*buildScala(1,0.2,4);
	mvStack.push(mvStack.top());
	
	












}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glm::mat4 Mtemp = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {

		V2TzScalation *= buildTranslate(0, 0, 0.1);

	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		std::vector<int> ind;
		std::vector<glm::vec3> vert;
		std::vector<glm::vec2> tex;
		std::vector<glm::vec3> norm;

		std::vector<float> pvalues;
		std::vector<float> tvalues;
		std::vector<float> nvalues;
		ind.clear();
		vert.clear();
		tex.clear();
		norm.clear();

		pvalues.clear();
		tvalues.clear();
		nvalues.clear();
		ind = mySphera[spheraN].getIndices();
		vert = mySphera[spheraN].getVertices();
		tex = mySphera[spheraN].getTexCoords();
		norm = mySphera[spheraN].getNormals();
		for (int i = 0; i < mySphera[spheraN].getNumVertices(); i++) {
			pvalues.push_back(vert[i].x);
			pvalues.push_back(vert[i].y);
			pvalues.push_back(vert[i].z);
			tvalues.push_back(tex[i].s);
			tvalues.push_back(tex[i].t);
			nvalues.push_back(norm[i].x);
			nvalues.push_back(norm[i].y);
			nvalues.push_back(norm[i].z);
		}

		glGenVertexArrays(1, vao);
		glBindVertexArray(vao[0]);
		glGenBuffers(4, vboSphere);

		glBindBuffer(GL_ARRAY_BUFFER, vboSphere[0]);
		glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vboSphere[1]);
		glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vboSphere[2]);
		glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboSphere[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
		shotFire = true;

	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		V2TzScalation *= buildTranslate(0, 0, -0.1);
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {

		V1toV2View = true;
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		V1toV2View = false;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {

		renderingProgramCubeMap = Utils::createShaderProgram("vertShaderOld.glsl", "fragShaderOld.glsl");
		renderingProgram = Utils::createShaderProgram("vertShaderL.glsl", "fragShaderL.glsl");
		renderingProgramPump = Utils::createShaderProgram("vertShaderExp.glsl", "fragShaderExp.glsl");
		reStartLight();
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		renderingProgramCubeMap = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
		renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
		renderingProgramPump = Utils::createShaderProgram("vertShaderExp.glsl", "fragShaderExp.glsl");
		reStartLight();
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		
			renderingProgramCubeMap = Utils::createShaderProgram("vertShaderOld.glsl", "fragShaderOld.glsl");
			renderingProgram = Utils::createShaderProgram("vertShaderL.glsl", "fragShaderL.glsl");
			renderingProgramPump = Utils::createShaderProgram("vertShaderExp.glsl", "fragShaderExp.glsl");
			reStartLight();

	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		renderingProgramCubeMap = Utils::createShaderProgram("vertShaderOld.glsl", "fragShaderOld.glsl");
		renderingProgram = Utils::createShaderProgram("vertShaderOld.glsl", "fragShaderOld.glsl");
		renderingProgramPump = Utils::createShaderProgram("vertShaderExp.glsl", "fragShaderExp.glsl");
		reStartLight();
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		Lrot *= rotateZ(toRadians(10));
		


	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		Lrot *= rotateZ(toRadians(-10));
		
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		Lrot *= rotateX(toRadians(10));



	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		Lrot *= rotateX(toRadians(-10));

	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		Az += 10;
		zRot = glm::quat(cosf(toRadians(Az) / 2), 0, 0, sinf(toRadians(Az) / 2));


	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		Az -= 10;
		zRot = glm::quat(cosf(toRadians(Az) / 2), 0, 0, sinf(toRadians(Az) / 2));
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		Ay += 10;
		yRot = glm::quat(cosf(toRadians(Ay) / 2), 0, sinf(toRadians(Ay) / 2), 0);

	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		Ay -= 10;
		yRot = glm::quat(cosf(toRadians(Ay) / 2), 0, sinf(toRadians(Ay) / 2), 0);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		trunnionR += 5;
		screwR += 10;
		screwTx *= buildTranslate(-0.02, 0, 0);
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		trunnionR -= 5;
		screwR -= 10;
		screwTx *= buildTranslate(0.02, 0, 0);
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		wR += 10;
		wheelRot = glm::quat(cosf(toRadians(Ay) / 2), sinf(toRadians(wR) / 2), 0, 0);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		wR -= 10;
		wheelRot = glm::quat(cosf(toRadians(Ay) / 2), sinf(toRadians(wR) / 2), 0, 0);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		if (action == GLFW_PRESS)
		{
			scaleV += 0.01;
			scale = buildScala(scaleV, scaleV, scaleV);
		}

	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		if (action == GLFW_PRESS)
		{
			scaleV -= 0.01;
			scale = buildScala(scaleV, scaleV, scaleV);
		}
		

	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		//V += glm::vec4(0,0.1,0,0);
		wR += 30;
		wheelRot = glm::quat(cosf(toRadians(Ay) / 2), sinf(toRadians(wR) / 2), 0, 0);
		baseZ += 0.03;



	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		//V -= glm::vec4(0, 0.1, 0, 0);
		wR -= 30;
		wheelRot = glm::quat(cosf(toRadians(Ay) / 2), sinf(toRadians(wR) / 2), 0, 0);
		baseZ -= 0.03;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		//V -= glm::vec4(0.1, 0, 0, 0);
		baseYr += 3;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		//V += glm::vec4(0.1, 0, 0, 0);
		baseYr -= 3;

	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		V -= glm::vec4(0, 0, 0.1, 0);
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		V += glm::vec4(0, 0, 0.1, 0);
	}

}






//P MAtrix
glm::mat4 FrustromMat4(float r, float l, float n, float f, float b, float t ) {
	return glm::mat4(
		(2*n/(r-l)), 0, 0, 0,
		0, (2 * n / (t - b)), 0, 0,
		((r+l)/(r-l)), ((t + b) / (t - b)), (-(f + n) / (f - n)), -1,
		0, 0, (-2 * n / (f - n)), 0
	);
}
void upDatePmat(stack <glm::mat4>  vMatemp) {
	glm::mat4 look = glm::lookAt(glm::vec3(-1 * (vMatemp.top()[3][0] + 0.3f), 1 * (vMatemp.top()[3][1] + 0.4f), -1 * (vMatemp.top()[3][2]) + 0.9f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	pMat = FrustromMat4(0.6, -0.6, 0.2, 25, -0.6, 0.6) * look ;

}
void upDatePmat2(stack <glm::mat4>  vMatemp) {

		glm::mat4 look = glm::lookAt(glm::vec3(-1 * (vMatemp.top()[3][0] + 0.3f), 1 * (vMatemp.top()[3][1] + 0.4f), -1 * (vMatemp.top()[3][2]) + 0.9f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		pMat = FrustromMat4(0.6, -0.6, 2, 25, -0.6, 0.6) * look * V2TzScalation ;




}
void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	if (V1toV2View)
	{
		upDatePmat(mvStack);
	}
	else
	{
		upDatePmat2(mvStack);
	}
	/*pMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	);*/
}
int main(void) {

	height = 1080;
	width = 1920;
	/* pMat = glm::mat4(
		((2* -0.2)/(0.8+0.8)), 0, 0, 0,
		0, ((2 * -0.2) / (0.7 + 0.7)), 0, 0,
		 ((0.8 - 0.8) / (0.8 + 0.8)), ((0.7 - 0.7) / (0.7 + 0.7)), -((0.8 - 0.2) / (0.8 + 0.2)), -1,
		0, 0, -((2 * 0.8* -0.2) / (0.8 + 0.2)), 0
	);*/
	/*pMat = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	);*/
	//pMat = glm::frustum(-0.9,0.9,-0.9,0.9,0.1,1.5);

	//pMat = FrustromMat4();
	Ax = Ay = 0;
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(width, height, "You have to make the rules, not follow them.", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window, window_size_callback);

	init(window);


	//display(window, glfwGetTime(), Ax, Ay, Az, temp);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_movement_callback);

	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		mvMat = mMat * vMat;
		glfwSwapBuffers(window);
		display(window, glfwGetTime(), Ax, Ay, Az, temp);
		glfwSetKeyCallback(window, key_callback);
		glfwWaitEvents();
		if (V1toV2View)
		{
			upDatePmat(mvStack);
		}
		else
		{
			upDatePmat2(mvStack);
		}
		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}