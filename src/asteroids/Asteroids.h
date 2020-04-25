#pragma once

#include <engine/CRenderer.h>
#include <vector>
#include <cstdint>

#include <Player.h>

class CWindow;

using std::vector;

struct sSpaceObject
{
    float x;
    float y;
    float dx;
    float dy;
    int nSize;
    float angle;
};

class Asteroids
{
public:
    Asteroids(CWindow *window);
    ~Asteroids();

    vector<sSpaceObject *> vecAsteroids;
    Player *player;

    void OnEnterFrame(CWindow *window);

    void HandleUserInput();
    void Animate(sSpaceObject &a);
    void Render(sSpaceObject &a);
    void RenderPlayer();

    CRenderer &mRenderer;

    vector<pair<float,float>> vecModelAsteroid;

    const float F=200.0f;
protected:
    CWindow *mWindow {nullptr};
};