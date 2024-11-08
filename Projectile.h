#pragma once

#include <core/gpu/mesh.h>
#include "ObjectsGeometry.h"

using namespace std;

class Projectile {
public:
	Projectile(float x, float y, float radians, float slope, unsigned int id);
	Mesh* getMesh();
	unsigned int id;
	float x, y;
	float y0;
	float sign;
	bool destroyed;
	float angle;
	float time;
private:
	Mesh* projMesh;
};