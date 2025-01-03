#include "lab_m1/tema1/tema1.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

tema1::tema1() :
    terrainBuilder(static_cast<float>(window->GetResolution().x), static_cast<float>(window->GetResolution().y)),
    tanks({Tank(100.0f), Tank(700.0f)})
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
    unsigned int id = (unsigned int)tank.projectiles.size();
    printf("queued id: %u\n", id);
    Projectile proj(tank.x, tank.y, tank.cannonAngle, tank.slope, id);
    tank.projectiles.push_back(proj);
}

void tema1::drawTank(float deltaTimeSeconds, Tank &tank) {
    if (tank.hp <= 0) return;
    float threshold = 1.0f;
    float landslideX, landslideY;
    landslideX = 0.0f;
    landslideY = 0.0f;
    glm::mat3 tankMat(1);
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
    glm::mat3 hpMat(1);
    hpMat *= transform2D::Translate(0, 30.0f);
    hpMat *= tankMat;
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
    RenderMesh2D(ObjectsGeometry::getProjTrajectoryMesh(tank.x, tank.y, tank.slope, tank.cannonAngle), shaders["VertexColor"], glm::mat3(1));
    vector<Mesh*> hpbar = ObjectsGeometry::getHpBar();
    for (int i = 0; i < tank.hp + 1; i++) {
        RenderMesh2D(hpbar[i], shaders["VertexColor"], hpMat);
    }
    drawProjectiles(tank, deltaTimeSeconds);
}
void tema1::drawTerrain(float deltaTime) {
    if (collisionHappened) {
        bool ret = terrainBuilder.updateHeightMap(deltaTime);
        collisionHappened = ret;
    }
    Mesh* square = new Mesh("square");
    vector<VertexFormat> vertices;
    vector<unsigned int> indices = { 0, 1 , 2, 2, 3, 0 };
    vertices.push_back(VertexFormat(glm::vec3(0, -1, 0), glm::vec3(0.1f, 0.9f, 0.1f)));
    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.1f, 0.9f, 0.1f)));
    vertices.push_back(VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0.1f, 0.9f, 0.1f)));
    vertices.push_back(VertexFormat(glm::vec3(1, -1, 0), glm::vec3(0.1f, 0.9f, 0.1f)));
    square->InitFromData(vertices, indices);

    for (int i = 0; i < terrainBuilder.heightMap.size() - 1; ++i) {
        glm::vec2 A((float)i, terrainBuilder.heightMap[i]);
        glm::vec2 B((float)(i + 1), terrainBuilder.heightMap[i + 1]);
        glm::mat3 modelMat(1);
        modelMat *= transform2D::Translate(A.x, A.y);
        modelMat *= transform2D::ShearX((B.y - A.y) / (B.x - A.x));
        modelMat *= transform2D::Scale(B.x - A.x, max(B.y, A.y));
        RenderMesh2D(square, shaders["VertexColor"], modelMat);
    }
}

float tema1::interpolFunc(float a, float b, float interpolCoef) {
    return a * (1 - interpolCoef) + b * interpolCoef;
}

void tema1::deform_terrain(float fx) {
    printf("deformin terrain\n");
    float radius = 60.0f;
    int x = (int)floor(fx);
    float theta = 0.0f;
    float cx = fx;
    float cy = terrainBuilder.heightMap[x];
    for (int i = max(0, x - (int)radius); i < min(x + (int)radius, window->GetResolution().x); i++) {
        float oldx = (float) i;
        float oldy = terrainBuilder.heightMap[i];
        float dist = sqrt(pow((oldx - cx), 2) + pow((oldy - cy), 2));
        if (dist < radius) {
            float diff = 0;
            while (true) {
                float newdist = sqrt(pow((oldx - cx), 2) + pow((oldy - diff - cy), 2));
                if (newdist >= radius) break;
                diff += 0.01f;
            }
            terrainBuilder.heightMap[i] -= diff;
        }
    }
    collisionHappened = true;
}


void tema1::drawProjectiles(Tank& tank, float deltaTime) {
    vector<Projectile>surv_proj;
    for (Projectile& proj : tank.projectiles) {
        /* Rendering the projectile */
        glm::mat3 modelMatrix(1);
        modelMatrix *= transform2D::Translate(proj.x, proj.y);
        modelMatrix *= transform2D::Scale(5.0f, 5.0f);
        RenderMesh2D(proj.getMesh(), shaders["VertexColor"], modelMatrix);

        /* Trajectory updates */
        float velocity = 500.0f;
        float g = 300.0f;
        proj.x += proj.sign * velocity * cos(proj.angle) * deltaTime;
        proj.y = proj.y0 + (velocity * sin(proj.angle) * proj.time) - 0.5f * g * (proj.time * proj.time);
        proj.time += deltaTime;
        /* Projectile out of the window bounds check */
        if (proj.x <= 0.0f || proj.x >= 1280.0f) {
            proj.destroyed = true;
            continue;
        }
        /* Terrain collisions detection */
        auto bounds = terrainBuilder.getSegmentBounds(floor(proj.x));
        float x1 = get<0>(bounds);
        float y1 = get<1>(bounds);
        float x2 = get<2>(bounds);
        float y2 = get<3>(bounds);

        float interpolCoef = (proj.x - x1) / (x2 - x1);
        float yI = interpolFunc(y1, y2, interpolCoef);
        if (proj.y - yI < 2.0f) {   
            proj.destroyed = true;
            deform_terrain(proj.x);
            printf("terrain coll\n");
            continue;
        }

        /* Tanks collisions detection */
        float radius = 40.0f;
        for (Tank& tnk : tanks) {
            if (&tnk == &tank) {    /* No friendly fire */
                continue;
            }
            float distX = proj.x - tnk.x;
            float distY = proj.y - tnk.y;
            float distance = sqrt(distX * distX + distY * distY);
            if (distance <= radius) {
                printf("tank coll\n");
                proj.destroyed = true;
                tnk.hp -= 1;
                break;
            }
        }
    }
    /* Updating the tank's proejctiles array, filtering out collided / out of bounds projectiles */
    for (Projectile& proj : tank.projectiles) {
        if (!proj.destroyed) {
            surv_proj.push_back(proj);
        }
    }
    tank.projectiles = surv_proj;
}

void tema1::Update(float deltaTimeSeconds)
{
    drawTerrain(deltaTimeSeconds);
    for (Tank& tank : tanks) {
        drawTank(deltaTimeSeconds, tank);
    }
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
    if (window->KeyHold(GLFW_KEY_N)) {
        for (int i = 0; i < terrainBuilder.heightMap.size(); i++) {
            printf("x: %d || y : %f\n", i, terrainBuilder.heightMap[i]);
        }
    }
    if (window->KeyHold(GLFW_KEY_K)) {
        GetSceneCamera()->SetPosition(glm::vec3(0, -500, 50));
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tanks[0].incX(75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        tanks[0].incX(-75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        tanks[0].incCannonAngle(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        tanks[0].incCannonAngle(-deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tanks[1].incX(75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tanks[1].incX(-75.0f * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        tanks[1].incCannonAngle(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        tanks[1].incCannonAngle(-deltaTime);
    }
}

void tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE) {
        queueProjectile(tanks[0]);
    }
    if (key == GLFW_KEY_ENTER) {
        queueProjectile(tanks[1]);
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
