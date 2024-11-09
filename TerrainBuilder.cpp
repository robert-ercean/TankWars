#pragma once

#include "TerrainBuilder.h"

using namespace std;

TerrainBuilder::TerrainBuilder(float windowX, float windowY) {
    this->maxWidth = static_cast<float>(windowX);
    this->sampleSize = 1.0f;
    this->heightModifier = (windowY * 0.5f) / SAMPLING_FUNCTION_MAX_VALUE;
    setInitialHeightMap();
}
void TerrainBuilder::setInitialHeightMap() {
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
bool TerrainBuilder::updateHeightMap(float deltaTime) {
    bool ret = false;
    float d;
    float threshold = 1.0f;
    for (int i = 0; i < heightMap.size() - 1; i++) {
        d = abs(heightMap[i] - heightMap[i + 1]);
        if (d > threshold) {
            ret = true;
            float e = deltaTime * 10.0f;
            if (heightMap[i] < heightMap[i + 1]) {
                heightMap[i] += e;
                heightMap[i + 1] -= e;
            }
            else {
                heightMap[i] -= e;
                heightMap[i + 1] += e;
            }
        }
    }
    return ret;
}
tuple<float, float, float, float> TerrainBuilder::getSegmentBounds(float x) {
    /* Get the terrain mesh that the tank currently stands on */
    float Ax, Ay, Bx, By;
    Ax = x;
    Bx = x + 1;
    Ay = heightMap[(int)x];
    By = heightMap[(int)(x + 1)];
    return { Ax, Ay, Bx, By };
}
