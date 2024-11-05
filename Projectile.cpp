#include "Projectile.h"

using namespace std;

Projectile::Projectile(float x, float y, float radians) {
	this->x = x;
	this->y0 = y;
	this->y = 0;
	this->time = 0.0f;
	this->angle = abs(radians);
	this->sign = (radians < 0.0f) ? 1.0f : -1.0f;
	this->projMesh = ObjectsGeometry::getProjectileMesh();
}

Mesh *Projectile::getMesh() {
	return this->projMesh;
}