#pragma once

#include "TerrainBuilder.h"

using namespace std;

TerrainBuilder::TerrainBuilder(float windowX, float windowY) {
    this->maxWidth = static_cast<float>(windowX);
    this->sampleSize = 1.0f;
    this->heightModifier = setHeightModifier(windowY);
}
float TerrainBuilder::setHeightModifier(float windowY) {
    return (windowY * 0.5f) / SAMPLING_FUNCTION_MAX_VALUE;
}

std::vector<Mesh*> TerrainBuilder::getTerrainMeshes() {
    return terrainMeshes;
}

vector<float> TerrainBuilder::getHeightMap() {
    return heightMap;
}

float TerrainBuilder::getSampleSize() {
    return sampleSize;
}

void TerrainBuilder::setHeightMap() {
    for (float x = 0.0f; x <= maxWidth; x += sampleSize) {
        float y = generateHeight(x);
        heightMap.emplace_back(y);
    }
}

float TerrainBuilder::generateHeight(float x) {
    x *= SAMPLING_FUNCTION_X_MODIFIER;
    float y = (sin(x) + 0.2f * sin(4.0f * x) + glm::half_pi<float>()) * heightModifier;
    return y;
}


void TerrainBuilder::buildTerrainGeometry() {
    setHeightMap();
    buildTerrainMeshes();
}

void TerrainBuilder::buildTerrainMeshes() {
    unsigned int i = 0;
    float vertX;
    float vertY;
    for (float x = 0.0f; x < maxWidth; x += sampleSize) {
        vector<VertexFormat> vertices;
        // down left corner
        vertX = x;
        vertY = 0;
        vertices.push_back(VertexFormat(glm::vec3(vertX, vertY, 0)));
          
        // up left corner
        vertY = heightMap[i];
        vertices.push_back(VertexFormat(glm::vec3(vertX, vertY, 0)));
        // down right corner
        vertX = x + sampleSize;
        vertY = 0;
        vertices.push_back(VertexFormat(glm::vec3(vertX, vertY, 0)));
          
        // up right corner
        vertY = heightMap[++i];
        vertices.push_back(VertexFormat(glm::vec3(vertX, vertY, 0)));
        vector<unsigned int> indices =
        { 
        0, 1 ,2,
        1, 3, 2
        };
        std::string uniqueID = "terrain_mesh_" + std::to_string(static_cast<int>(x));
        Mesh* newMesh = new Mesh(uniqueID.c_str());
        newMesh->InitFromData(vertices, indices);
        terrainMeshes.push_back(newMesh);
    }
}

tuple<float, float, float, float> TerrainBuilder::getSegmentBounds(float tankX) {
    /* Get the terrain mesh that the tank currently stands on */
    Mesh* currMesh = terrainMeshes[static_cast<int>(tankX)];
    vector<VertexFormat> currVertices = currMesh->vertices;
    float Ax, Ay, Bx, By;
    Ax = currVertices[1].position.x;
    Ay = currVertices[1].position.y;
    Bx = currVertices[3].position.x;
    By = currVertices[3].position.y;
    return { Ax, Ay, Bx, By };
}