#include "lab_m1/tema1/tema1.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

tema1::tema1() :
    terrainBuilder(static_cast<float>(window->GetResolution().x), static_cast<float>(window->GetResolution().y)),
    tank1(50.0f),
    tank2(700.0f)
{}


tema1::~tema1()
{
}


void tema1::Init()
{
    glDisable(GL_CULL_FACE);
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    terrainBuilder.buildTerrainGeometry();
}


void tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void tema1::drawProjectile(Tank& tank, float deltaTimeSeconds) {
    
}

void tema1::drawTank(float deltaTimeSeconds, Tank &tank) {
    float threshold = 1.0f;
    float e, landslideX, landslideY;
    landslideX = 0.0f;
    landslideY = 0.0f;
    glm::mat3 tankMat(1);
    float sampleSize = terrainBuilder.getSampleSize();
    // place the first tank between some two points of the terrain
    // make sure the points dont exceed the window size since we'll seg fault
    if (tank.getX() >= window->GetResolution().x - 1 || tank.getX() <= 1) return;
    auto bounds = terrainBuilder.getSegmentBounds(floor(tank.getX()));
    float Ax, Ay, Bx, By;
    Ax = get<0>(bounds);
    Ay = get<1>(bounds);
    Bx = get<2>(bounds);
    By = get<3>(bounds);
    float tankY;

    float deltaH = (Ay - By);
    if (abs(deltaH) > threshold) {
        // sign indices if the tank is sliding in the axis orientation, or opposed to it
        float sign = (deltaH > 0) ? 1.0f : -1.0f;
        landslideX = sign * deltaTimeSeconds * 35.0f;
        tank.incrementX(landslideX);
    }
    float t = (tank.getX() - Ax) / (Bx - Ax);
    tankY = Ay + t * (By - Ay);
    tank.setY(tankY);
    float Vy = By - Ay;
    float Vx = Bx - Ax;


    tankMat *= transform2D::Translate(tank.getX(), tankY);
    tankMat *= transform2D::Scale(15.0f, 15.0f);
    tankMat *= transform2D::Rotate(atan2(Vy, Vx));
    for (Mesh* mesh : tank.getMeshes()) {
        if (strcmp(mesh->GetMeshID(), "Cannon") == 0) {
            /* Rotate the cannon around the head's circle centre, so translate to the origin first, rotate and then translate back */
            tankMat *= transform2D::Translate(0, 2.0f);
            tankMat *= transform2D::Rotate(tank.getCannonAngle());
            tankMat *= transform2D::Translate(0, -2.0f);
        }
        RenderMesh2D(mesh, shaders["VertexColor"], tankMat);
    }

}
void tema1::drawTerrain() {
    for (const auto& mesh: terrainBuilder.getTerrainMeshes()) {
        RenderMesh2D(mesh, glm::mat3(1), glm::vec3(0, 1, 0));
    }
}

void tema1::Update(float deltaTimeSeconds)
{
    drawTerrain();
    drawTank(deltaTimeSeconds, tank1);
    drawTank(deltaTimeSeconds, tank2);
    if (tank1.proj) drawProjectile(tank1, deltaTimeSeconds);
    if (tank2.proj) drawProjectile(tank2, deltaTimeSeconds);
}


void tema1::FrameEnd()
{
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_D)) {
        tank1.incrementX(75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        tank1.decrementX(75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        tank1.incrementCannonAngle(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        tank1.decrementCannonAngle(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tank2.incrementX(75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tank2.decrementX(75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        tank2.incrementCannonAngle(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        tank2.decrementCannonAngle(deltaTime);
    }
}

void tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE) {
       tank1.proj = true;
    }
    if (key == GLFW_KEY_ENTER) {
       tank2.proj = true;
    }
}


void tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema1::OnWindowResize(int width, int height)
{
}
