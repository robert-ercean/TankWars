#pragma once

#include <core/gpu/mesh.h>
#include "ObjectsGeometry.h"

using namespace std;

class Projectile {
public:
	Projectile(float x, float y, float radians, float slope);
	Mesh* getMesh();
	float x, y;
	float y0;
	float sign;
	float angle;
	float time;
private:
	Mesh* projMesh;
};