#include "Tank.h"
#include "ObjectsGeometry.h"

using namespace std;

Tank::Tank(float initialX) {
	this->x = initialX;
	this->cannonAngle = 0.0f;
	this->brake = true;
	this->meshes = ObjectsGeometry::getTankMeshes();
	this->y = 0;
	this->proj = false;
	this->projCoord = { 0, 0 };
	this->projVect = {cos(cannonAngle) * 30.0f, sin(cannonAngle) * 30.0f};
}
glm::vec2& Tank::getProjCoord() {
	return this->projCoord;
}
glm::vec2& Tank::getProjVect() {
	return this->projVect;
}
void Tank::setY(float y) {
	this->y = y;
}
vector<Mesh*> Tank::getMeshes() {
	return this->meshes;
}
void Tank::decrementCannonAngle(float radians) {
	this->cannonAngle -= radians;
}
void Tank::incrementCannonAngle(float radians) {
	this->cannonAngle += radians;
}
float Tank::getCannonAngle() {
	return this->cannonAngle;
}
float Tank::getX() {
	return this->x;
}
float Tank::getY() {
	return this->y;
}
bool Tank::is_brake() {
	return this->brake;
}
void Tank::incrementX(float tX) {
	this->x += tX;
}
void Tank::decrementX(float tX) {
	this->x -= tX;
}
