#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <glm\gtx\quaternion.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <stack>
//Note I took and example from learn openGL and modified for this not everthing is my own code. ------------------IMPORTANT
using namespace std;
// Represents a single particle and its state
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float     Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
	// constructor
	ParticleGenerator(GLuint renderingProgram, GLuint texture, unsigned int amount);
	// update all particles
	void Update(float dt, glm::vec3 &object, glm::vec3 &objectV, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// render all particles
	void Draw(stack <glm::mat4>(&mvStack));
	// state
	std::vector<Particle> particles;
	unsigned int amount;
	// render state
	GLuint renderingProgram;
	GLuint texture;
	unsigned int VAO;
	// initializes buffer and vertex attributes
	void init();
	// returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	unsigned int firstUnusedParticle();
	// respawns particle
	void respawnParticle(Particle &particle, glm::vec3 &object, glm::vec3 &objectV, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};