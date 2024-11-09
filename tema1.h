#pragma once

#include <vector>
#include <iostream>
#include "components/simple_scene.h"
#include "lab_m1/lab3/object2D.h"
#include <cmath>
#include "TerrainBuilder.h"
#include "ObjectsGeometry.h"
#include "Tank.h"
#include "Projectile.h"

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

        void drawTerrain(float deltaTime);
        void drawTank(float deltaTimeSeconds, Tank& tank);
        void queueProjectile(Tank& tank);
        void drawProjectiles(Tank& tank, float deltaTimeSeconds);
        float interpolFunc(float a, float b, float interpolCoef);
        void deform_terrain(float x);

    protected:
        bool collisionHappened = false;
        glm::mat3 modelMatrix;
        TerrainBuilder terrainBuilder;
        vector<Tank> tanks;
        vector<Projectile> projectiles;
    };
}   // namespace m1
