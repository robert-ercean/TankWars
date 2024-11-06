#pragma once

#include <vector>
#include <core/gpu/mesh.h>
#include "Projectile.h"

using namespace std;

class Tank {
public:
	Tank(float initialX);
	vector<Mesh *> getMeshes();
	Mesh* getProjTrajMesh(float deltaTimeSeconds);
	bool is_brake();
	void incX(float tX);
	void incCannonAngle(float radians);
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
