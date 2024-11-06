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
Mesh* Tank::getProjTrajMesh(float deltaTimeSeconds) {
	return ObjectsGeometry::getProjTrajectoryMesh(x, y, slope, cannonAngle);
}
vector<Mesh*> Tank::getMeshes() {
	return this->meshes;
}
void Tank::incCannonAngle(float radians) {
	this->cannonAngle += radians;
}
bool Tank::is_brake() {
	return this->brake;
}
void Tank::incX(float tX) {
	this->x += tX;
}
