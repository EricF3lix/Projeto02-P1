#include "raylib.h"
#include <stdlib.h>
//C:\Users\Eric Felix\Projeto2PI1


typedef struct usuario {
    char nome[20];
    int vida;
    int pontuacao;
} Usuario;

void menuJogador(Usuario jogadores[], int quantidadePlayer) {
    
    for (int i = 0; i < quantidadePlayer; i++) {
        jogadores[i].vida = 5;
        jogadores[i].pontuacao = 0;
        jogadores[i].nome[0] = '\0';
    }

    Rectangle caixa1 = { 200, 180, 400, 50 };
    Rectangle caixa2 = { 200, 300, 400, 50 };

    bool ativa1 = false;
    bool ativa2 = false;

    int len1 = 0;
    int len2 = 0;

    while (!WindowShouldClose()) {

        Vector2 mouse = GetMousePosition();

        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            if (CheckCollisionPointRec(mouse, caixa1)) {
                ativa1 = true;
                ativa2 = false;
            }
            else if (quantidadePlayer == 2 && CheckCollisionPointRec(mouse, caixa2)) {
                ativa2 = true;
                ativa1 = false;
            }
            else {
                ativa1 = ativa2 = false;
            }
        }

        
        if (ativa1) {
            int key = GetCharPressed();
            while (key > 0) {
                if (len1 < 19) {
                    jogadores[0].nome[len1] = (char)key;
                    jogadores[0].nome[len1+1] = '\0';
                    len1++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && len1 > 0) {
                len1--;
                jogadores[0].nome[len1] = '\0';
            }
        }

        
        if (ativa2) {
            int key = GetCharPressed();
            while (key > 0) {
                if (len2 < 19) {
                    jogadores[1].nome[len2] = (char)key;
                    jogadores[1].nome[len2+1] = '\0';
                    len2++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && len2 > 0) {
                len2--;
                jogadores[1].nome[len2] = '\0';
            }
        }

        
        if (IsKeyPressed(KEY_ENTER)) {

            if (quantidadePlayer == 1 && len1 > 0) {
                break;
            }

            if (quantidadePlayer == 2 && len1 > 0 && len2 > 0) {
                break;
            }
        }

        
        
        

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Jogador 1 - Digite seu nome:", 200, 150, 20, BLACK);
        DrawRectangleRec(caixa1, ativa1 ? LIGHTGRAY : GRAY);
        DrawRectangleLinesEx(caixa1, 2, DARKGRAY);
        DrawText(jogadores[0].nome, caixa1.x + 10, caixa1.y + 12, 20, BLACK);

        
        if (ativa1) {
            DrawText("_", caixa1.x + 10 + MeasureText(jogadores[0].nome, 20),
                     caixa1.y + 12, 20, BLACK);
        }

        if (quantidadePlayer == 2) {
            DrawText("Jogador 2 - Digite seu nome:", 200, 270, 20, BLACK);
            DrawRectangleRec(caixa2, ativa2 ? LIGHTGRAY : GRAY);
            DrawRectangleLinesEx(caixa2, 2, DARKGRAY);
            DrawText(jogadores[1].nome, caixa2.x + 10, caixa2.y + 12, 20, BLACK);

            if (ativa2) {
                DrawText("_", caixa2.x + 10 + MeasureText(jogadores[1].nome, 20),
                         caixa2.y + 12, 20, BLACK);
            }
        }

        DrawText("Pressione ENTER para confirmar", 200, 380, 22, DARKGRAY);

        EndDrawing();
    }
}




int menuMultijogador(Music musica){
        const char *texto[] = {"Você deseja jogar",
                              "SINGLE PLAYER",
                              "OU",
                              "DUAL PLAYER"};
        
        Rectangle botao1Player = {250, 300, 300, 60 };
        Rectangle botao2Player = {250, 370, 300, 60};
        while (!WindowShouldClose()){
            UpdateMusicStream(musica);
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


int menu(Music musica) {
    
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
           
            return 1;   
        }
        if (placar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            
            return 2;  
        }
        if (sair && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            
            return 0;   
        }
    }
    
}



int main(){
    
    int escolha = 10;
    int quantidadePlayer;
    const int larguraTela = 800;
    const int alturaTela = 450;
    
    InitAudioDevice();
    InitWindow(larguraTela, alturaTela, "Projeto de programação");
    
    Music musica = LoadMusicStream("musicaMenu.mp3");
    PlayMusicStream(musica);
   
    
    while(escolha!=0 && !WindowShouldClose()){
        escolha = menu(musica);
        
        switch(escolha){
            case 0:
                while(!WindowShouldClose()){
                    BeginDrawing();
                        ClearBackground(BLACK);
                        DrawText("Arregou! FIM DE JOGO!", 40, 180, 60, MAROON);
                    EndDrawing();
                }
                break;
            
            case 1:
                Usuario jogadores[2];
                quantidadePlayer = menuMultijogador(musica);
                menuJogador(jogadores, quantidadePlayer);
                break;
                
                
        }
        
    }
    CloseAudioDevice();
}