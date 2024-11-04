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
	void incrementX(float tX);
	void decrementX(float tX);
	void decrementCannonAngle(float radians);
	void incrementCannonAngle(float radians);
	float getCannonAngle();
	void setY(float y);
	bool proj;
	glm::vec2 projVect;
	glm::vec2 projCoord;
	glm::vec2& getProjCoord();
	glm::vec2& getProjVect();
private:
	vector<Mesh *> meshes;
	float x;
	float y;
	float cannonAngle;
	bool brake;
};
