#include "raylib.h"
#include <math.h>

int main(void) {
    SetTargetFPS(60);
    
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "Physics Simulation");

    float xPosition = (float)screenWidth/2;
    float yPosition = (float)screenHeight/2;

    Vector2 circlePosition = { xPosition, yPosition };
    float CircleRadius = 10;
    Vector2 velocity = { 5.0f, 5.0f };

    
    
    while (!WindowShouldClose()) {
        xPosition += velocity.x;
        yPosition += velocity.y;

        if (xPosition - CircleRadius == 0 || xPosition + CircleRadius == screenWidth) {
            velocity.x = -velocity.x;
        }

        if (yPosition - CircleRadius == 0 || yPosition + CircleRadius == screenHeight) {
            velocity.y = -velocity.y;
        }

        circlePosition = { xPosition, yPosition };

        BeginDrawing();
        
            ClearBackground(RAYWHITE);
            
            DrawCircleV(circlePosition, CircleRadius, SKYBLUE);
            DrawText("Physics Simulation", 10, 10, 20, DARKGRAY);
        
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
