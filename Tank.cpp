#include "Tank.h"
#include "ObjectsGeometry.h"

using namespace std;

Tank::Tank(float initialX) {
	this->x = initialX;
	this->cannonAngle = 0.0f;
	this->brake = true;
	this->meshes = ObjectsGeometry::getTankMeshes();
	this->y = 0;
}
void Tank::setY(float y) {
	this->y = y;
}
vector<Mesh*> Tank::getMeshes() {
	return this->meshes;
}
void Tank::incCannonAngle(float radians) {
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
void Tank::incX(float tX) {
	this->x += tX;
}
