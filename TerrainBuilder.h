#pragma once

#include <tuple>
#include <vector>
#include <glm/vec3.hpp>
#include <cstdlib>
#include <ctime>
#include <core/gpu/vertex_format.h>
#include "lab_m1/lab3/transform2D.h"
#include <unordered_map>
#include "core/gpu/mesh.h"

/* Maximum value of f(x) = sin(x) +  0.3sin(4x) + pi/2 */
#define SAMPLING_FUNCTION_MAX_VALUE 2.8f
#define SAMPLING_FUNCTION_X_MODIFIER 0.01f

using namespace std;

class TerrainBuilder {
public:
    /* Constructor for the terrain, sets the maximum width of the terrain
     * and the maximum possible height of any point inside the height map */
    TerrainBuilder(float windowX, float windowY);
    /* Initializes the building procedures associated with the terrain, such 
     * as constructing the height map and the meshes
     * calls setHeightMap and buildTerrainMeshes */
    void buildTerrainGeometry();
    /* Populates the heightMap with height values, sampled at fixed intervals
     * based on generateHeight */
    void setHeightMap();
    /* Returns the heightMap */
    vector<float> getHeightMap();
    /* Returns the sampling segment size */
    float getSampleSize();
    /* Populates the terrainMeshes vector with a series of meshes that will
     * build up the terrain */
    void buildTerrainMeshes();
    /* Returns the terrain meshes */
    std::vector<Mesh*> getTerrainMeshes();
    tuple<float, float, float, float> TerrainBuilder::getSegmentBounds(float tankX);

private:
    /* Value used to populate the heightMap */
    float heightModifier;
    /* Distance between any two points of the heightMap */
    float sampleSize;
    /* Maximum width of the terrain, set to the viewport width size */
    float maxWidth;
    /* HeightMap used for rasterizing the terrain' squares */
    vector<float> heightMap;
    /* vector containing multiple paralelipiped meshes that will be rasterized
     * inside the Update() method, building the terrain */
    std::vector<Mesh*> terrainMeshes;
    /* Generates a height value based on the height sampling function 
     * f(x) = sin(x) +  0.3sin(4x) + pi/2, whose output will be 
     * multiplied by the heightModifier to generate an appropiate y 
     * value for a given point, also each input x value will be multiplied
     * by a small fraction (i.e. 0.01) to maintain the continuity appearance
     * of the sinusoidal functions sum */
    float generateHeight(float x);
    /* Returns the heightModifer value used to build Y coordinates of the terrain's meshes 
     * Since we don't want any point of the terrain to exceed half of the window's height 
     * we calculate this based on the formula: heightModifier = (0.5 * windowY) / max(f(x)) */
    float setHeightModifier(float windowY);
};
