#pragma once

#include <vector>
#include <core/gpu/vertex_format.h>
#include <core/gpu/mesh.h>

using namespace std;

#define MEDIAN_DELTA_TIME_SECONDS 0.016f

class ObjectsGeometry {
	public: 
		inline static Mesh* getProjTrajectoryMesh(float x, float y, float slope, float cannonAngle) {
			Mesh* traj = new Mesh("projTrajectory");
			vector<VertexFormat> vertices;
			vector<unsigned int> indices;

			float offset = 30.0f;
			x = x - offset * sin(slope);
			float y0 = y + offset * cos(slope);
			y = 0;
			float time = 0.0f;
			float angle = abs(cannonAngle);
			float sign = (cannonAngle < 0.0f) ? -1.0f : 1.0f;

			float velocity = 500.0f;
			float g = 300.0f;
			unsigned int idx = 0;
			while (y >= 0) {
				x += sign * velocity * cos(angle) * MEDIAN_DELTA_TIME_SECONDS;
				y = y0 + (velocity * sin(angle) * time) - 0.5f * g * (time * time);
				vertices.push_back(VertexFormat(glm::vec3(x, y, 0)));
				indices.push_back(idx++);

				//printf("%f  ||  %f\n", x, y);
				time += MEDIAN_DELTA_TIME_SECONDS;
			}
			traj->SetDrawMode(GL_LINE_STRIP);
			traj->InitFromData(vertices, indices);
			return traj;
		}
		inline static Mesh *getProjectileMesh() {
			Mesh* proj = new Mesh("Projectile");
			vector<VertexFormat> vertices;
			vector<unsigned int> indices;
			indices.push_back(0);
			vertices.push_back(VertexFormat(glm::vec3(0, 0, 0)));
			float radius = 1.0f;
			int idx = 1;
			for (float angle = 0; angle <= glm::two_pi<float>(); angle += 0.01f) {
				float x = radius * cos(angle);
				float y = radius * sin(angle);
				VertexFormat vertex(glm::vec3(x, y, 0), glm::vec3(1, 0.0f, 1.0f));
				vertices.push_back(vertex);
				indices.push_back(idx++);
			}
			proj->SetDrawMode(GL_TRIANGLE_FAN);
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
			vertices.push_back(VertexFormat(glm::vec3(0, 2, 0)));
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