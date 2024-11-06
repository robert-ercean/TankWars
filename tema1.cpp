#include "lab_m1/tema1/tema1.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

tema1::tema1() :
    terrainBuilder(static_cast<float>(window->GetResolution().x), static_cast<float>(window->GetResolution().y)),
    tank1(100.0f),
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

void tema1::queueProjectile(Tank& tank) {
    float x = tank.x;
    float y = tank.y;
    float radians = tank.cannonAngle;
    float slope = tank.slope;
    Projectile proj(x, y, radians, slope);
    tank.projectiles.push_back(proj);
}

void tema1::drawTank(float deltaTimeSeconds, Tank &tank) {
    float threshold = 1.0f;
    float landslideX, landslideY;
    landslideX = 0.0f;
    landslideY = 0.0f;
    glm::mat3 tankMat(1);
    float sampleSize = terrainBuilder.getSampleSize();
    // place the first tank between some two points of the terrain
    // make sure the points dont exceed the window size since we'll seg fault
    if (tank.x >= window->GetResolution().x - 1 || tank.x <= 1) return;
    auto bounds = terrainBuilder.getSegmentBounds(floor(tank.x));
    float Ax, Ay, Bx, By;
    Ax = get<0>(bounds);
    Ay = get<1>(bounds);
    Bx = get<2>(bounds);
    By = get<3>(bounds);

    float tankY;
    float deltaH = (Ay - By);

    if (abs(deltaH) > threshold) {
        // sign is showing if the tank should be sliding in the axis orientation, or opposed to it
        float sign = (deltaH > 0) ? 1.0f : -1.0f;
        landslideX = sign * deltaTimeSeconds * 30.0f;
        tank.incX(landslideX);
    }
    float t = (tank.x - Ax) / (Bx - Ax);
    tankY = Ay + t * (By - Ay);
    tank.y = tankY;
    float Vy = By - Ay;
    float Vx = Bx - Ax;
    float slope = atan2(Vy, Vx);
    tank.slope = slope;
    glm::mat3 trajMat(1);
    tankMat *= transform2D::Translate(tank.x, tankY);
    tankMat *= transform2D::Scale(15.0f, 15.0f);
    tankMat *= transform2D::Rotate(slope);

    for (Mesh* mesh : tank.getMeshes()) {
        if (strcmp(mesh->GetMeshID(), "Cannon") == 0) {
            /* Rotate the cannon around the head's circle centre, so translate to the origin first, rotate and then translate back */
            tankMat *= transform2D::Translate(0, 2.0f);
            /* Subtract the slope from the cannon rotation angle since the cannon's position should stay independent from sliding */
            tankMat *= transform2D::Rotate(tank.cannonAngle - slope);
            tankMat *= transform2D::Translate(0, -2.0f);
            /* The trajectory mesh will follow the same transformations as the cannon, as it comes right after the cannon mesh in the array */
            RenderMesh2D(mesh, shaders["VertexColor"], tankMat);
            continue;
        }
        RenderMesh2D(mesh, shaders["VertexColor"], tankMat);
    }
    for (Projectile& proj : tank.projectiles) {
        drawProjectile(proj, deltaTimeSeconds);
    }
    RenderMesh2D(ObjectsGeometry::getProjTrajectoryMesh(tank.x, tank.y, tank.slope, tank.cannonAngle), shaders["VertexColor"], glm::mat3(1));
}
void tema1::drawTerrain() {
    for (const auto& mesh: terrainBuilder.getTerrainMeshes()) {
        RenderMesh2D(mesh, shaders["VertexColor"], glm::mat3(1));
    }
}

void tema1::drawProjectile(Projectile& proj, float deltaTime) {
    glm::mat3 modelMatrix(1);
    modelMatrix *= transform2D::Translate(proj.x, proj.y);
    modelMatrix *= transform2D::Scale(5.0f, 5.0f);
    RenderMesh2D(proj.getMesh(), shaders["VertexColor"], modelMatrix);

    float velocity = 500.0f;
    float g = 300.0f;

    proj.x += proj.sign * velocity * cos(proj.angle) * deltaTime;
    proj.y = proj.y0 + (velocity * sin(proj.angle) * proj.time) - 0.5f * g * (proj.time * proj.time);
    proj.time += deltaTime;
}

void tema1::Update(float deltaTimeSeconds)
{
    drawTerrain();
    drawTank(deltaTimeSeconds, tank1);
    drawTank(deltaTimeSeconds, tank2);
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
        tank1.incX(75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        tank1.incX(-75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        tank1.incCannonAngle(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        tank1.incCannonAngle(-deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tank2.incX(75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tank2.incX(-75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        tank2.incCannonAngle(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        tank2.incCannonAngle(-deltaTime);
    }
}

void tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE) {
        queueProjectile(tank1);
    }
    if (key == GLFW_KEY_ENTER) {
        queueProjectile(tank2);
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
