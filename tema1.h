#pragma once

#include <vector>
#include <iostream>
#include "components/simple_scene.h"
#include "lab_m1/lab3/object2D.h"
#include <cmath>
#include "TerrainBuilder.h"
#include "ObjectsGeometry.h"
#include "Tank.h"

namespace m1
{
    class tema1 : public gfxc::SimpleScene
    {
    public:
        tema1();
        ~tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void drawTerrain();
        void tema1::drawTank(float deltaTimeSeconds, Tank &tank);
        void tema1::drawProjectile(Tank& tank, float deltaTimeSeconds);
    protected:
        glm::mat3 modelMatrix;
        TerrainBuilder terrainBuilder;
        Tank tank1;
        Tank tank2;
    };
}   // namespace m1
