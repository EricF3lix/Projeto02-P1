#include "raylib.h"
#include <stdlib.h>
#include <time.h>


typedef struct usuario {
    char nome[20];
    int vida;
    int pontuação;  
}Usuario;


int menuMultijogador(){
        const char *texto[] = {"Você deseja jogar",
                              "SINGLE PLAYER",
                              "OU",
                              "DUAL PLAYER"};
        
        Rectangle botao1Player = {250, 300, 300, 60 };
        Rectangle botao2Player = {250, 370, 300, 60};
        while (!WindowShouldClose()){
            Vector2 mouse = GetMousePosition();
            bool player1 = CheckCollisionPointRec(mouse, botao1Player);
            bool player2 = CheckCollisionPointRec(mouse, botao2Player);
            
            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText(texto[0], 200, 20, 40, BLACK);
                DrawText(texto[1], 270, 90, 30, MAROON);
                DrawText(texto[2], 370, 140, 30, BLACK);
                DrawText(texto[3], 270, 190, 35, MAROON);
                
                DrawRectangleRec(botao1Player, player1 ? LIGHTGRAY : GRAY);
                DrawText("SINGLE PLAYER", botao1Player.x+50, botao1Player.y+20, 25, BLACK);
                
                DrawRectangleRec(botao2Player, player2? LIGHTGRAY : GRAY);
                DrawText ("DUAL PLAYER", botao2Player.x+50, botao2Player.y + 20, 25, BLACK);
            
            EndDrawing();
            if (player1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                return 1;
            }
            if (player2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                return 2;
                
            }   
                        
        }
        return -1;
    
}


int menu() {
    Music musica = LoadMusicStream("teste.mp3");
    PlayMusicStream(musica);
    const char *texto[] = {
        "Olá, seja bem vindo ao",
        "Deu a Louca no Quadrado!",
        "Escolha uma das opções abaixo:"
    };

       
    Rectangle botaoJogar  = { 250, 240, 300, 60 };
    Rectangle botaoPlacar = { 250, 310, 300, 60 };
    Rectangle botaoSair   = { 250, 380, 300, 60 };

    while (!WindowShouldClose()) {
        UpdateMusicStream(musica);
        Vector2 mouse = GetMousePosition();

        bool jogar  = CheckCollisionPointRec(mouse, botaoJogar);
        bool placar = CheckCollisionPointRec(mouse, botaoPlacar);
        bool sair   = CheckCollisionPointRec(mouse, botaoSair);

        BeginDrawing();
        ClearBackground(RAYWHITE);

           
        DrawText(texto[0], 150, 20, 40, BLACK);
        DrawText(texto[1], 40, 80, 55, MAROON);
        DrawText(texto[2], 220, 150, 20, DARKGRAY);

           
        DrawRectangleRec(botaoJogar, jogar ? LIGHTGRAY : GRAY);
        DrawText("JOGAR", botaoJogar.x+105, botaoJogar.y + 20, 25, BLACK);

             
        DrawRectangleRec(botaoPlacar, placar ? LIGHTGRAY : GRAY);
        DrawText("PLACAR", botaoPlacar.x + 95, botaoPlacar.y + 20, 25, BLACK);

            
        DrawRectangleRec(botaoSair, sair ? LIGHTGRAY : GRAY);
        DrawText("SAIR", botaoSair.x + 115, botaoSair.y + 20, 25, BLACK);

        EndDrawing();

           
        if (jogar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            UnloadMusicStream(musica);
            return 1;   
        }
        if (placar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            UnloadMusicStream(musica);
            return 2;  
        }
        if (sair && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            UnloadMusicStream(musica);
            return 0;   
        }
    }
    
}



int main(){
    int escolha = 10;
    int quantidadePlayer;
    const int larguraTela = 800;
    const int alturaTela = 450;
    
    InitWindow(larguraTela, alturaTela, "Projeto de programação");
    InitAudioDevice();
    while(escolha!=0 && !WindowShouldClose()){
        escolha = menu();
        switch(escolha){
            case 0:
                while(!WindowShouldClose()){
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawText("Arregou! FIM DE JOGO!", 40, 180, 60, MAROON);
                    EndDrawing();
                }
                CloseAudioDevice();
                break;
            case 1:
                quantidadePlayer = menuMultijogador();
                if (quantidadePlayer==1){
                    
                } else if (quantidadePlayer ==2){
                    
                }
                
                break;
                
                
        }
    
            
 
        
        
    }
}