#include "raylib.h"
#include <time.h> //tempo das rodadas
#include <stdlib.h>  //random ta aqui
// caminho pro git hub C:\Users\Eric Felix\Projeto2PI1

int main(){
    
    const int coordenadaX = 800;
    const int coordenadaY = 450;

    InitWindow(coordenadaX, coordenadaY, "Jogo dos quadrado");
    Vector2 ballPosition = {(float)coordenadaX/2, (float)coordenadaY/2};

    SetTargetFPS(60);              
    

    
    while (!WindowShouldClose())   
    {
        BeginDrawing();
            
            ClearBackground(RAYWHITE);

            DrawText("Olá, Mundo!", 190, 200, 20, DARKGRAY);
            DrawCircleV(ballPosition, 50, MAROON);

        EndDrawing();
        
    }

    
    CloseWindow();        
    

    return 0;
}