#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//C:\Users\Eric Felix\Projeto2PI1


typedef struct usuario {
    char nome[20];
    int vida;
    int pontuacao;
} Usuario;

void salvaArquivo(Usuario rank[]);
//lembrar de colocar as assinaturas das funções pra nn dar bronca

void desenhaPlacar(Usuario rank[], Music vitoria){
    Rectangle botaoVoltar = {0, 0, 60, 30};
    int parar = 1;
    while(parar!=0 && !WindowShouldClose()){
        UpdateMusicStream(vitoria);
        Vector2 mouse = GetMousePosition();
        bool voltar = CheckCollisionPointRec(mouse, botaoVoltar);
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            DrawText("RANK TOP 10 DO DEU A LOUCA NO QUADRADO!", 150, 20, 20, MAROON);
            int eixoY = 50;
            char jogador[100];
            for(int i = 0 ; i < 10 ; i++){
                snprintf(jogador, sizeof(jogador), "%dº - %s %d", i+1, rank[i].nome, rank[i].pontuacao);
             
                if(i<=2){  
                    DrawText(jogador, 300, eixoY, 20, MAROON);
                } else {
                    DrawText(jogador, 300, eixoY, 20, BLACK);
                    
                }
                eixoY+=35;
            }
            
            DrawRectangleRec(botaoVoltar, voltar ? LIGHTGRAY : GRAY);
            DrawText("VOLTAR", botaoVoltar.x+5, botaoVoltar.y+10, 10, BLACK);
        EndDrawing();
        if (voltar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            parar = 0;
            
        }
    }
}






void ordenaTop10(Usuario todosUsuarios[]){
    for(int i = 0; i < 11; i++){
        for(int j = i + 1; j < 12; j++){
            if(todosUsuarios[j].pontuacao > todosUsuarios[i].pontuacao){
                Usuario temp = todosUsuarios[i];
                todosUsuarios[i] = todosUsuarios[j];
                todosUsuarios[j] = temp;
            }
        }
    }
}


void comparaComUsuarios(Usuario jogadores[], Usuario rank[]){
    Usuario todosUsuarios[12];
    
    for(int i = 0; i < 10; i++){
        todosUsuarios[i] = rank[i];
    }

    
    todosUsuarios[10] = jogadores[0];
    todosUsuarios[11] = jogadores[1];

   
    ordenaTop10(todosUsuarios);

   
    for(int i = 0; i < 10; i++){
       rank[i] = todosUsuarios[i];
    }
    salvaArquivo(rank);
}

void carregarArquivo(Usuario rank[]){
    FILE *arquivo = fopen("PlacarLideranca.txt", "r");
    if(arquivo){
        char titulo[150];
        fgets(titulo, sizeof(titulo), arquivo);
        int contaPosicao = 0;
        while(contaPosicao < 10){
            if(fscanf(arquivo, "%s %d", rank[contaPosicao].nome, &rank[contaPosicao].pontuacao)!=2){
                break;
            }
            contaPosicao++;
            
        }
        fclose(arquivo);
    }
}

void salvaArquivo(Usuario rank[]){
    FILE *arquivo = fopen("PlacarLideranca.txt", "w");
    if(arquivo){
        fprintf(arquivo, "RANK TOP 10 DO DEU A LOUCA NO QUADRADO!\n");
        for(int i = 0 ; i < 10 ; i++){
            fprintf(arquivo, "%s %d\n", rank[i].nome, rank[i].pontuacao);
        }
        fclose(arquivo);
        
    }
}


void menuComoJogar(int quantidadePlayer, Music musica) {
    int enter = 1;

    while (enter != 0) {
        UpdateMusicStream(musica);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Título
        DrawText("COMO JOGAR", 200, 20, 30, MAROON);

        if (quantidadePlayer == 1) {

            DrawText("Modo selecionado: SINGLE PLAYER", 100, 70, 25, BLACK);

            DrawText("Controles:", 100, 120, 23, MAROON);

            
            DrawText("UP", 100, 160, 20, RED);
            DrawText(" - Aumenta a quantidade de quadrados", 140, 160, 20, BLACK);

           
            DrawText("DOWN", 100, 190, 20, RED);
            DrawText(" - Diminui a quantidade de quadrados", 160, 190, 20, BLACK);
        }
        else {

            DrawText("Modo selecionado: DUAL PLAYER", 100, 70, 25, BLACK);


            DrawText("Jogador 1", 80, 130, 23, MAROON);

            DrawText("UP", 80, 170, 20, RED);
            DrawText(" - Aumenta os quadrados", 120, 170, 20, BLACK);

            DrawText("DOWN", 80, 200, 20, RED);
            DrawText(" - Diminui os quadrados", 150, 200, 20, BLACK);

           
            DrawText("Jogador 2", 420, 130, 23, MAROON);

            DrawText("W", 420, 170, 20, RED);
            DrawText(" - Aumenta os quadrados", 460, 170, 20, BLACK);

            DrawText("S", 420, 200, 20, RED);
            DrawText(" - Diminui os quadrados", 460, 200, 20, BLACK);
        }

        DrawText("Pressione ENTER para continuar", 180, 380, 22, DARKGRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            enter = 0;
        }
    }
}



void menuJogador(Usuario jogadores[], int quantidadePlayer, Music musica) {
    
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
        UpdateMusicStream(musica);

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
    Music derrota = LoadMusicStream("derrota.mp3");
    Music vitoria = LoadMusicStream("musicaVitoria.mp3");
    PlayMusicStream(musica);
    PlayMusicStream(derrota);
    PlayMusicStream(vitoria);
   
    Usuario *rank = malloc(10*sizeof(Usuario));
    Usuario *jogadores = malloc(2*sizeof(Usuario));
    carregarArquivo(rank);
    while(escolha!=0 && !WindowShouldClose()){
        escolha = menu(musica);
        
        switch(escolha){
            case 0:
                while(!WindowShouldClose()){
                    UpdateMusicStream(derrota);
                    BeginDrawing();
                        ClearBackground(BLACK);
                        DrawText("Arregou! FIM DE JOGO!", 40, 180, 60, MAROON);
                    EndDrawing();
                }
                break;
            
            case 1:
                quantidadePlayer = menuMultijogador(musica);
                menuJogador(jogadores, quantidadePlayer, musica);
                menuComoJogar(quantidadePlayer, musica);
                //vai ter o jogo aq e tal
                comparaComUsuarios(jogadores, rank);
                break;
            
            case 2:
                desenhaPlacar(rank, vitoria);
                break;
                
               
                
                
        }
        
    }
    free(rank);
    free(jogadores);
    UnloadMusicStream(musica);
    UnloadMusicStream(derrota);
    UnloadMusicStream(vitoria);
    
    CloseAudioDevice();
}