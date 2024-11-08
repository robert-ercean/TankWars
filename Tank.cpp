#include "Tank.h"
#include "ObjectsGeometry.h"

using namespace std;

Tank::Tank(float initialX) {
	this->hp = 4;
	this->x = initialX;
	this->cannonAngle = 0.0f;
	this->brake = true;
	this->meshes = ObjectsGeometry::getTankMeshes();
	this->y = 0;
}
vector<Mesh*> Tank::getMeshes() {
	return this->meshes;
}
void Tank::incCannonAngle(float radians) {
	this->cannonAngle += radians;
	if (this->cannonAngle > glm::pi<float>()) {
		this->cannonAngle = glm::pi<float>();
	}
	if (this->cannonAngle < 0) {
		this->cannonAngle = 0;
	}
}
bool Tank::is_brake() {
	return this->brake;
}
void Tank::incX(float tX) {
	this->x += tX;
}
