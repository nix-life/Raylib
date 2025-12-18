#include "raylib.h"
#include "raymath.h"
#include <math.h>

typedef struct {
    Vector2 position;
    Vector2 velocity;
} Body;

typedef struct {
    Vector2 start;
    Vector2 end;
} Line;

Body collLine(Body body, float radius, Vector2 lineStart, Vector2 lineEnd);

#define MAX_BALLS 10

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const float gravity = 0.3f;
    const float radius = 10.0f;

    InitWindow(screenWidth, screenHeight, "Physics Simulation");
    SetTargetFPS(60);

    Body balls[MAX_BALLS];
    int ballCount = MAX_BALLS;

    for (int i = 0; i < ballCount; i++) {
        balls[i].position = (Vector2){ 100.0f + i * 25.0f, 50.0f };
        balls[i].velocity = (Vector2){ (float)(i - 5), 0.0f };
    }

    Line lines[] = {
        { { 0, 430 }, { 800, 450 } },
        { { 20, 0 }, { 0, 450 } },
        { { 0, 0 }, { 800, 20 } },
        { { 800, 0 }, { 780, 450 } },
        { { 100, 250 }, { 500, 300 } }
    };

    int lineCount = sizeof(lines) / sizeof(lines[0]);

    while (!WindowShouldClose())
    {
        for (int b = 0; b < ballCount; b++)
        {
            balls[b].velocity.y += gravity;

            balls[b].position.x += balls[b].velocity.x;
            balls[b].position.y += balls[b].velocity.y;

            for (int i = 0; i < lineCount; i++) {
                if (CheckCollisionCircleLine(
                        balls[b].position, radius,
                        lines[i].start, lines[i].end))
                {
                    balls[b] = collLine(
                        balls[b], radius,
                        lines[i].start, lines[i].end);
                }
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < lineCount; i++)
                DrawLineV(lines[i].start, lines[i].end, BLACK);

            for (int b = 0; b < ballCount; b++)
                DrawCircleV(balls[b].position, radius, SKYBLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

Body collLine(Body body, float radius, Vector2 lineStart, Vector2 lineEnd)
{
    const float restitution = 0.6f;
    const float friction = 0.03f;
    const float restVel = 0.15f;

    Vector2 dir = {
        lineEnd.x - lineStart.x,
        lineEnd.y - lineStart.y
    };

    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len == 0.0f)
        return body;

    Vector2 d = { dir.x / len, dir.y / len };

    Vector2 normal = { -d.y, d.x };
    if (body.velocity.x * normal.x + body.velocity.y * normal.y > 0)
        normal = (Vector2){ d.y, -d.x };

    Vector2 toCenter = {
        body.position.x - lineStart.x,
        body.position.y - lineStart.y
    };

    float proj = toCenter.x * d.x + toCenter.y * d.y;
    proj = Clamp(proj, 0.0f, len);

    Vector2 closest = {
        lineStart.x + d.x * proj,
        lineStart.y + d.y * proj
    };

    Vector2 diff = {
        body.position.x - closest.x,
        body.position.y - closest.y
    };

    float dist = sqrtf(diff.x * diff.x + diff.y * diff.y);
    float penetration = radius - dist;

    if (penetration <= 0.0f)
        return body;

    body.position.x += normal.x * penetration;
    body.position.y += normal.y * penetration;

    float vn = body.velocity.x * normal.x + body.velocity.y * normal.y;

    if (vn < 0.0f) {
        body.velocity.x -= (1.0f + restitution) * vn * normal.x;
        body.velocity.y -= (1.0f + restitution) * vn * normal.y;
    }

    Vector2 tangent = { -normal.y, normal.x };
    float vt = body.velocity.x * tangent.x + body.velocity.y * tangent.y;

    float maxFriction = friction * fabsf(vn);
    if (fabsf(vt) < maxFriction)
        maxFriction = fabsf(vt);

    if (vt > 0) {
        tangent.x = -tangent.x;
        tangent.y = -tangent.y;
    }

    body.velocity.x += maxFriction * tangent.x;
    body.velocity.y += maxFriction * tangent.y;

    if (fabsf(vn) < restVel) {
        body.velocity.x -= vn * normal.x;
        body.velocity.y -= vn * normal.y;
    }

    return body;
}
