#include "Projectile.h"
#include "lab_m1/lab3/transform2D.h"

using namespace std;

Projectile::Projectile(float x, float y, float radians, float slope) {
	 /* Y Axis distance from the tank base to the head's centre on a flat surface (when the slope is 0) */
	/* Note that the logic offset would be 2.0, but we multiply by 15.0 in the scaling transform during drawTank() */
	float offset = 30.0f;
	/* Adjust the initial projectile position since the function arguments x, y are the pos of the tank lower base */
	this->x = x -  offset * sin(slope);
	this->y0 = y + offset * cos(slope);
	this->y = 0;
	this->time = 0.0f;
	this->angle = abs(radians);
	this->sign = (radians < 0.0f) ? -1.0f : 1.0f;
	this->projMesh = ObjectsGeometry::getProjectileMesh();
}

Mesh *Projectile::getMesh() {
	return this->projMesh;
}