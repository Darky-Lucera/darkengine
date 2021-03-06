#pragma once

#include <CRendererAsteroids.h>
#include <MindShake/Math/types/CVector2.h>
#include <MindShake/Math/types/CVector3.h>

using namespace MindShake;

class Player {
public:
    Player(float x, float y, float dx, float dy, float angle);
    ~Player();
    void Thrust(double deltaTime);
    void SteerRight(double deltaTime);
    void SteerLeft(double deltaTime);
    void Update(double deltaTime);
    void Render(CRenderer *renderer);
    void Reset(float x = 0.0f, float y = 0.0f);
    bool isDead();
    bool Hit() { --lives; return isDead(); };

    CVector2 *pos;
    CVector2 *vel;
    float angle = 0;
    uint32_t color = 0x00FF00;
    float acc=200.0f;
    float steerDegrees=5.0f;
    int lives = 3;
    bool dead = false;

private:
    vector<pair<float, float>> vecModelShip = {
        { 0.0f, -5.0f },
        { -2.5f, +2.5f },
        { +2.5f, +2.5f },
    };
};