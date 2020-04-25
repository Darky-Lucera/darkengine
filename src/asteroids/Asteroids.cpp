#include "Asteroids.h"
#include <engine/CWindow.h>
#include <engine/CRenderer.h>
#include <CRendererAsteroids.h>

#include <Common/Math/math_funcs.h>
#include <Common/Math/constants.h>

using namespace MindShake;

Asteroids::Asteroids(CWindow *window) : mRenderer(window->GetRenderer()) {
    mWindow = window;

    vecAsteroids.emplace_back(
        new sSpaceObject({
            70.0f, 40.0f,
            20.0f, -50.0f,
            (int)16,
            0.0f
        })
    );

    player = new Player(
        mRenderer.GetWidth() / 2.0f, mRenderer.GetHeight() / 2.0f,
        0, 0,
        0
    );

    // init asteroids;
    int verts = 20;
    for (int i=0; i < verts; i++) {
        float radius = 5.0f;
        float a = ((float)i / (float)verts) * 6.28318f;

        vecModelAsteroid.push_back(make_pair(radius * Sin(a), radius * Cos(a)));
    }

    // mWindow->AddOnEnterFrame(this, &Asteroids::OnEnterFrame);
    mWindow->AddOnEnterFrame(this, &Asteroids::OnEnterFrame);
};

Asteroids::~Asteroids() {
    for (auto *a : vecAsteroids) delete a;
    for (auto *b : vecBullets) delete b;
    for (auto *na : vecNewAsteroids) delete na;
    delete player;
};

void
Asteroids::OnEnterFrame(CWindow *window) {
    mRenderer.Clear(0x00);

    HandleUserInput();

    // render asteroids
    for (auto *a : vecAsteroids)
    {
        a->x += a->dx * mWindow->GetDeltaTime();
        a->y += a->dy * mWindow->GetDeltaTime();
        mRenderer.WrapCoordinates(a->x, a->y, a->x, a->y);

        mRenderer.DrawWireframeModel(vecModelAsteroid, a->x, a->y, 1, a->nSize, 0xffffff);
    }

    // render bullets
    for (int i = 0; i < (int)vecBullets.size(); ++i)
    {
        auto* b = vecBullets[i];
 
        b->x += b->dx * mWindow->GetDeltaTime();
        b->y += b->dy * mWindow->GetDeltaTime();

        // check if bullet colides with asteroid


        for (auto &a : vecAsteroids)
        {
            if (IsPointInsideCircle(b->x, b->y, a->nSize, a->x, a->y))
            {
                // Asteroid hit
                // throw bullet off-screen
                b->x = -1000;

                if (a->nSize > 4)
                {
                    float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
                    float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;

                    vecNewAsteroids.emplace_back(new sSpaceObject({
                        a->x, a->y,
                        10.0f * Sin(angle1), -10.0f * Cos(angle1),
                        (int) a->nSize >> 1,
                        0
                    }));

                    // mark hitted asteroid to be destroyed
                    a->x = -100;
                }
            }
        }

        // Add recently created asteroids if there is a hit
        for (auto &a : vecNewAsteroids) 
            vecAsteroids.emplace_back(a);

        // delete bullet if out of boundries of the screen
        if (b->x < 0 || b->x >= mRenderer.GetWidth() || b->y < 0 || b->y >= mRenderer.GetHeight()) {
            delete vecBullets[i];
            vecBullets.erase(vecBullets.begin() + i);
            --i;
            break;
        }


        mRenderer.WrapCoordinates(b->x, b->y, b->x, b->y);
        mRenderer.DrawRectangle(b->x, b->y, 5, 5, 0xCCCCCC);
    }

    player->Render(&mRenderer);
};

void
Asteroids::HandleUserInput() {
   uint8_t *keys = const_cast<uint8_t *>(mWindow->GetKeyBuffer());

    // Quit
    if (keys[KB_KEY_Q]) exit(0);

    // Reset
    if (keys[KB_KEY_R]) player->Reset(mRenderer.GetWidth() / 2.0f, mRenderer.GetHeight() / 2.0f);

    // Steer
    if (keys[KB_KEY_J]) player->SteerLeft(mWindow->GetDeltaTime());
    if (keys[KB_KEY_K]) player->SteerRight(mWindow->GetDeltaTime());

    // Thrust
    if (keys[KB_KEY_SPACE]) player->Thrust(mWindow->GetDeltaTime());

    // Fire!
    if (keys[KB_KEY_F])
    {
        keys[KB_KEY_F] = false;
        vecBullets.emplace_back(new sSpaceObject({
            player->pos->x, player->pos->y,
            player->vel->x + (500.0f * Sin(player->angle)), player->vel->y - (500.0f * Cos(player->angle)),
            0, 0
        }));
    }

    player->Update(mWindow->GetDeltaTime());
};
