#include <cmath>
#include <vector>
#include <glm\glm.hpp>
using namespace std;
class Sphera
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
	Sphera();
	Sphera(int prec);
	int getNumVertices();
	int getNumIndices();
	void lunchBall(stack <glm::mat4> (&mvStack), glm::vec3 cannonDir, float dt);
	void drawSphera(Sphera &mySphera, GLuint(&vboSphere)[4], GLuint(&Vao)[1], stack <glm::mat4>(&mvStack), GLuint gemTexture, glm::mat4 &pMat, glm::mat4 &invTrMat, GLuint &mvLoc, GLuint &projLoc, GLuint &nLoc);
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getTangents();
};