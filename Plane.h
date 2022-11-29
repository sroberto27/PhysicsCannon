#include <GLFW\glfw3.h>
#include <SOIL2\soil2.h>
#include <glm\gtx\quaternion.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <glm\gtc\type_ptr.hpp> // glm::value_ptr
#include <glm\gtc\matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
class Plane {

private:

	

public:

	glm::vec3 x, y, z, c;
	void set3Points(glm::vec3 xt, glm::vec3 yt, glm::vec3 zt) {
		x = xt;
		y = yt;
		z = zt;
	}
	glm::vec3 getX() {
		return x;
	}
	glm::vec3 getY() {
		return y;
	}
	glm::vec3 getZ() {
		return z;
	}

};