#pragma once

#include <vector>
#include <core/gpu/vertex_format.h>
#include <core/gpu/mesh.h>

using namespace std;

class ObjectsGeometry {
	public: 
		inline static Mesh* getProjTrajectoryMesh(float angle, float deltaTime) {
			Mesh* traj = new Mesh("projTrajectory");
			vector<VertexFormat> vertices;
			vector<unsigned int> indices;
			float g = 10.0f;
			float v = 30.0f;
			float vx = v * cos(angle);
			float vy = v * sin(angle);
			unsigned int idx = 0;

			float x0, y0;
			/* Offsets for the cannon pos */
			x0 = 3.0f;
			y0 = 2.0f;
			float y = 0;
			while (y <= 0) {
				float x = x0 + vx * deltaTime;
				y = y0 + vy * deltaTime - 0.5f * (deltaTime * deltaTime) * g;
				vertices.push_back(VertexFormat(glm::vec3(x, y, 0)));
				indices.push_back(idx++);
				deltaTime *= 2.0f;
			}

			traj->SetDrawMode(GL_LINE_STRIP);
			traj->InitFromData(vertices, indices);
			return traj;
		}
		inline static Mesh *getProjectileMesh() {
			Mesh* proj = new Mesh("Projectile");
			vector<VertexFormat> vertices;
			vector<unsigned int> indices;
			float radius = 1.0f;
			int idx = 1;
			for (float angle = 0; angle <= glm::two_pi<float>(); angle += 0.01f, idx++) {
				float x = radius * cos(angle);
				float y = radius * sin(angle);
				VertexFormat vertex(glm::vec3(x, y, 0), glm::vec3(1, 0.0f, 1.0f));
				vertices.push_back(vertex);
				indices.push_back(0);
				indices.push_back(idx);
				indices.push_back(idx + 1);
			}
			proj->InitFromData(vertices, indices);
			return proj;

		}
		inline static vector <Mesh *> getTankMeshes() {
			Mesh* upper = new Mesh("upperPart");
			Mesh* lower = new Mesh("lowerPart");
			vector<VertexFormat> verticesLower;
			vector<VertexFormat> verticesUpper;
			vector<unsigned int> indicesUpper = { 0, 1, 2 , 2, 3, 0 };
			vector<unsigned int> indicesLower = { 0, 1, 2 , 2, 3, 0 };

			verticesUpper.push_back(VertexFormat(glm::vec3(-4, 1, 0), glm::vec3(1, 0.62f ,0)));
			verticesUpper.push_back(VertexFormat(glm::vec3(-3, 2, 0), glm::vec3(1, 0.42f, 0)));
			verticesUpper.push_back(VertexFormat(glm::vec3(3, 2, 0), glm::vec3(1, 0.42f, 0)));
			verticesUpper.push_back(VertexFormat(glm::vec3(4, 1, 0), glm::vec3(1, 0.62f, 0)));

			verticesLower.push_back(VertexFormat(glm::vec3(-2, 0, 0), glm::vec3(1, 0.5f, 0)));
			verticesLower.push_back(VertexFormat(glm::vec3(-3, 1, 0), glm::vec3(1, 0.2f, 0)));
			verticesLower.push_back(VertexFormat(glm::vec3(3, 1, 0), glm::vec3(1, 0.2f, 0)));
			verticesLower.push_back(VertexFormat(glm::vec3(2, 0, 0), glm::vec3(1, 0.5f, 0)));

			upper->InitFromData(verticesUpper, indicesUpper);
			lower->InitFromData(verticesLower, indicesLower);
			
			Mesh* semiCircle = new Mesh("semiCircle");
			vector<VertexFormat> vertices;
			vector<unsigned int> indices;
			vertices.push_back(VertexFormat(glm::vec3(0, 0, 0)));
			indices.push_back(0);
			float radius = 1.5f;
			int idx = 1;
			for (float angle = 0; angle <= glm::pi<float>(); angle += 0.01f, idx++) {
				float x = radius * cos(angle);
				float y = radius * sin(angle);
				VertexFormat vertex(glm::vec3(x, y + 2.0f, 0), glm::vec3(1, 0.52f, 0));
				vertices.push_back(vertex);
				indices.push_back(idx);
			}
			semiCircle->SetDrawMode(GL_TRIANGLE_FAN);
			semiCircle->InitFromData(vertices, indices);
			
			Mesh* cannon = new Mesh("Cannon");
			vector<VertexFormat> cannonVertices;
			vector<unsigned int> cannonIndices = { 0, 1, 2 , 2, 3, 0};

			cannonVertices.push_back(VertexFormat(glm::vec3(0, 1.75f, 0), glm::vec3(0.44f, 0.25f, 0)));
			cannonVertices.push_back(VertexFormat(glm::vec3(0, 2.25f, 0), glm::vec3(0.44f, 0.25f, 0)));
			cannonVertices.push_back(VertexFormat(glm::vec3(3.0f, 2.25f, 0), glm::vec3(0.44f, 0.25f, 0)));
			cannonVertices.push_back(VertexFormat(glm::vec3(3.0f, 1.75f, 0), glm::vec3(0.44f, 0.25f, 0)));
			cannon->InitFromData(cannonVertices, cannonIndices);
			return {upper, lower, semiCircle, cannon};
		}
};