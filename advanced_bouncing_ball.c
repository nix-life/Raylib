#include "raylib.h"
#include <math.h>

typedef struct {
    Vector2 position;
    Vector2 velocity;
} Body;

Body collLine(Body body, float radius, Vector2 lineStart, Vector2 lineEnd);

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Physics Simulation");
    SetTargetFPS(60);

    Body ball;
    ball.position = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    ball.velocity = (Vector2){ -5.0f, 5.0f };

    float circleRadius = 10.0f;

    Vector2 l1Start = { 0.0f, 430.0f };
    Vector2 l1End   = { 800.0f, 450.0f };

    Vector2 l2Start = { 20.0f, 0.0f };
    Vector2 l2End   = { 0.0f, 450.0f };
    
    Vector2 l3Start = { 0.0f, 0.0f };
    Vector2 l3End   = { 800.0f, 20.0f };
    
    Vector2 l4Start = { 800.0f, 0.0f };
    Vector2 l4End   = { 780.0f, 450.0f };
    

    while (!WindowShouldClose())
    {
        ball.position.x += ball.velocity.x;
        ball.position.y += ball.velocity.y;

        if (CheckCollisionCircleLine(ball.position, circleRadius, l1Start, l1End))
            ball = collLine(ball, circleRadius, l1Start, l1End);

        if (CheckCollisionCircleLine(ball.position, circleRadius, l2Start, l2End))
            ball = collLine(ball, circleRadius, l2Start, l2End);
        
        if (CheckCollisionCircleLine(ball.position, circleRadius, l3Start, l3End))
            ball = collLine(ball, circleRadius, l3Start, l3End);
        
        if (CheckCollisionCircleLine(ball.position, circleRadius, l4Start, l4End))
            ball = collLine(ball, circleRadius, l4Start, l4End);

        if (ball.position.x - circleRadius <= 0 ||
            ball.position.x + circleRadius >= screenWidth)
            ball.velocity.x = -ball.velocity.x;

        if (ball.position.y - circleRadius <= 0 ||
            ball.position.y + circleRadius >= screenHeight)
            ball.velocity.y = -ball.velocity.y;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawLineV(l1Start, l1End, BLACK);
            DrawLineV(l2Start, l2End, BLACK);
            DrawLineV(l3Start, l3End, BLACK);
            DrawLineV(l4Start, l4End, BLACK);
            DrawCircleV(ball.position, circleRadius, SKYBLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

Body collLine(Body body, float radius, Vector2 lineStart, Vector2 lineEnd)
{
    Vector2 dir = {
        lineEnd.x - lineStart.x,
        lineEnd.y - lineStart.y
    };

    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    Vector2 d = { dir.x / len, dir.y / len };

    Vector2 n1 = { -d.y,  d.x };
    Vector2 n2 = {  d.y, -d.x };

    Vector2 normal = n1;
    if (body.velocity.x * normal.x + body.velocity.y * normal.y > 0)
        normal = n2;

    float dot = body.velocity.x * normal.x + body.velocity.y * normal.y;

    body.velocity.x -= 2.0f * dot * normal.x;
    body.velocity.y -= 2.0f * dot * normal.y;

    body.position.x += normal.x * 0.5f;
    body.position.y += normal.y * 0.5f;

    return body;
}
