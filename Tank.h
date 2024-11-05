#pragma once

#include <vector>
#include <core/gpu/mesh.h>

using namespace std;

class Tank {
public:
	Tank(float initialX);
	vector<Mesh *> getMeshes();
	float getX();
	float getY();
	bool is_brake();
	void incX(float tX);
	void incCannonAngle(float radians);
	float getCannonAngle();
	void setY(float y);
private:
	vector<Mesh *> meshes;
	float x;
	float y;
	float cannonAngle;
	bool brake;
};
