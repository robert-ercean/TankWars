#pragma once

#include <vector>
#include <core/gpu/mesh.h>
#include "Projectile.h"

using namespace std;

class Tank {
public:
	Tank(float initialX);
	vector<Mesh *> getMeshes();
	bool is_brake();
	void incX(float tX);
	void incCannonAngle(float radians);
	int hp;
	float x;
	float y;
	float slope;
	float cannonAngle;
	vector<Projectile> projectiles;
private:
	Mesh* projTrajMesh;
	vector<Mesh *> meshes;
	bool brake;
};
