#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jogo.h"
//C:\Users\Eric Felix\Projeto2PI1


void salvaArquivo(Usuario rank[]);
void carregarArquivo(Usuario rank[]);
void desenhaPlacar(Usuario rank[], Music vitoria);
void ordenaTop10(Usuario todosUsuarios[]);
void comparaComUsuarios(Usuario jogadores[], Usuario rank[]);
void menuDerrota(int quantidadePlayer, Usuario jogador[]);
void menuComoJogar(int quantidadePlayer, Music musica);
void menuJogador(Usuario jogadores[], int quantidadePlayer, Music musica);
int menuMultijogador(Music musica);
int menu(Music musica);
void inicializaVetor(Usuario vetor[], int tamanho);
float controleContadoraPlayer1(float contadora1);
float controleContadoraPlayer2(float contadora2);
void atualizarTelaFaseDeJogo1Dual(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float tempo);
void atualizarTelaFaseDeJogo2Dual(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes);
void desenharMapa(float larguraTotal, float alturaDoMapa);
int *sortearPosEQtd(int tamanho);
void pintarQuadrados(float larguraTotal, float alturaDoMapa, int pos);
void jogoSinglePlayer(Usuario jogadores[], Music musicaFaseDeJogo, float larguraTotal, float alturaDoMapa);


void desenhaPlacar(Usuario rank[], Music vitoria){
    Rectangle botaoVoltar = {20, 20, 120, 50};
    int parar = 1;
    while(parar!=0 && !WindowShouldClose()){
        UpdateMusicStream(vitoria);
        Vector2 mouse = GetMousePosition();
        bool voltar = CheckCollisionPointRec(mouse, botaoVoltar);
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            
            
            DrawText("RANK TOP 10 DO DEU A LOUCA NO QUADRADO!", 220, 30, 40, MAROON);

            int eixoY = 120;  
            char jogador[100];
            for(int i = 0 ; i < 10 ; i++){
                snprintf(jogador, sizeof(jogador), "%dº - %s %d", i+1, rank[i].nome, rank[i].pontuacao);

                
                DrawRectangle(300, eixoY - 5, 840, 40, LIGHTGRAY);

                
                if(i<=2){  
                    DrawText(jogador, 310, eixoY, 30, MAROON);
                } else {
                    DrawText(jogador, 310, eixoY, 30, BLACK);
                }

                eixoY += 60; 
            }

          
            DrawRectangleRec(botaoVoltar, voltar ? LIGHTGRAY : GRAY);
            DrawText("VOLTAR", botaoVoltar.x+15, botaoVoltar.y+10, 25, BLACK);

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


void menuDerrota(int quantidadePlayer, Usuario jogador[]){
    int enter = 1;

    while(enter != 0 && !WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Retângulos de destaque
            DrawRectangle(420, 90, 600, 60, LIGHTGRAY);  // para o texto1
            DrawRectangle(420, 150, 600, 50, LIGHTGRAY); // para o texto2
            DrawRectangle(400, 680, 640, 50, LIGHTGRAY); // para instrução

            // Texto principal
            char texto1[100];
            char texto2[100];
            snprintf(texto1, sizeof(texto1), "%s - VOCÊ PERDEU!", jogador[0].nome);
            snprintf(texto2, sizeof(texto2), "Sua pontuação total foi: %d", jogador[0].pontuacao);

            DrawText(texto1, 440, 100, 40, MAROON);
            DrawText(texto2, 440, 155, 40, MAROON);

            // Texto de instrução
            DrawText("Pressione ENTER para continuar", 420, 690, 35, DARKGRAY);

        EndDrawing();

        if(IsKeyPressed(KEY_ENTER)){
            enter = 0;
        }
    }
}



void menuComoJogar(int quantidadePlayer, Music musica) {
    int enter = 1;

    while (enter != 0 && !WindowShouldClose()) {
        UpdateMusicStream(musica);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Título centralizado
        DrawText("COMO JOGAR", 540, 40, 60, MAROON);

        if (quantidadePlayer == 1) {

            DrawText("Modo selecionado: SINGLE PLAYER", 440, 150, 40, BLACK);
            DrawText("Controles:", 600, 230, 36, MAROON);

            DrawText("W", 500, 300, 32, RED);
            DrawText(" - Aumenta a quantidade de quadrados", 540, 300, 32, BLACK);

            DrawText("S", 500, 360, 32, RED);
            DrawText(" - Diminui a quantidade de quadrados", 585, 360, 32, BLACK);
        } 
        else {

            DrawText("Modo selecionado: DUAL PLAYER", 470, 150, 40, BLACK);

            DrawText("Jogador 1", 400, 230, 36, MAROON);

            DrawText("UP", 400, 290, 32, RED);
            DrawText(" - Aumenta os quadrados", 440, 290, 32, BLACK);

            DrawText("DOWN", 400, 350, 32, RED);
            DrawText(" - Diminui os quadrados", 485, 350, 32, BLACK);

            DrawText("Jogador 2", 900, 230, 36, MAROON);

            DrawText("W", 900, 290, 32, RED);
            DrawText(" - Aumenta os quadrados", 940, 290, 32, BLACK);

            DrawText("S", 900, 350, 32, RED);
            DrawText(" - Diminui os quadrados", 940, 350, 32, BLACK);
        }

        DrawText("Pressione ENTER para continuar", 500, 700, 36, DARKGRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            enter = 0;
        }
    }
}







void menuJogador(Usuario jogadores[], int quantidadePlayer, Music musica) {
    Rectangle caixa1 = { 520, 250, 400, 60 };
    Rectangle caixa2 = { 520, 430, 400, 60 };

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
            if (quantidadePlayer == 1 && len1 > 0) break;
            if (quantidadePlayer == 2 && len1 > 0 && len2 > 0) break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

       
        DrawText("Jogador 1 - Digite seu nome:", 520, 200, 32, BLACK);
        DrawRectangleRec(caixa1, ativa1 ? LIGHTGRAY : GRAY);
        DrawRectangleLinesEx(caixa1, 3, DARKGRAY);
        DrawText(jogadores[0].nome, caixa1.x + 10, caixa1.y + 15, 30, BLACK);

        if (ativa1) {
            DrawText("_", caixa1.x + 10 + MeasureText(jogadores[0].nome, 30), caixa1.y + 15, 30, BLACK);
        }

        if (quantidadePlayer == 2) {

            DrawText("Jogador 2 - Digite seu nome:", 520, 380, 32, BLACK);
            DrawRectangleRec(caixa2, ativa2 ? LIGHTGRAY : GRAY);
            DrawRectangleLinesEx(caixa2, 3, DARKGRAY);
            DrawText(jogadores[1].nome, caixa2.x + 10, caixa2.y + 15, 30, BLACK);

            if (ativa2) {
                DrawText("_", caixa2.x + 10 + MeasureText(jogadores[1].nome, 30), caixa2.y + 15, 30, BLACK);
            }
        }

        DrawText("Pressione ENTER para confirmar", 480, 650, 36, DARKGRAY);

        EndDrawing();
    }
}





int menuMultijogador(Music musica){
    const char *texto[] = {"Você deseja jogar",
                           "SINGLE PLAYER",
                           "OU",
                           "DUAL PLAYER"};
    
    Rectangle botao1Player = {570, 500, 300, 70};
    Rectangle botao2Player = {570, 600, 300, 70};

    while (!WindowShouldClose()){
        UpdateMusicStream(musica);
        Vector2 mouse = GetMousePosition();
        bool player1 = CheckCollisionPointRec(mouse, botao1Player);
        bool player2 = CheckCollisionPointRec(mouse, botao2Player);
        
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText(texto[0], 450, 80, 60, BLACK);

           
            DrawText(texto[1], 530, 180, 40, MAROON);
            DrawText(texto[2], 680, 240, 40, BLACK);
            DrawText(texto[3], 540, 300, 45, MAROON);

            
            DrawRectangleRec(botao1Player, player1 ? LIGHTGRAY : GRAY);
            DrawText("SINGLE PLAYER", botao1Player.x + 40, botao1Player.y + 22, 30, BLACK);
            
            
            DrawRectangleRec(botao2Player, player2 ? LIGHTGRAY : GRAY);
            DrawText("DUAL PLAYER", botao2Player.x + 70, botao2Player.y + 22, 30, BLACK);
        
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

    
    Rectangle botaoJogar  = { 570, 500, 300, 70 };
    Rectangle botaoPlacar = { 570, 600, 300, 70 };
    Rectangle botaoSair   = { 570, 700, 300, 70 };

    while (!WindowShouldClose()) {
        UpdateMusicStream(musica);
        
        Vector2 mouse = GetMousePosition();

        bool jogar  = CheckCollisionPointRec(mouse, botaoJogar);
        bool placar = CheckCollisionPointRec(mouse, botaoPlacar);
        bool sair   = CheckCollisionPointRec(mouse, botaoSair);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        
        DrawText(texto[0], 400, 80, 50, BLACK);
        DrawText(texto[1], 250, 150, 75, MAROON);
        DrawText(texto[2], 430, 250, 30, DARKGRAY);

       
        DrawRectangleRec(botaoJogar, jogar ? LIGHTGRAY : GRAY);
        DrawText("JOGAR", botaoJogar.x + 95, botaoJogar.y + 20, 35, BLACK);

       
        DrawRectangleRec(botaoPlacar, placar ? LIGHTGRAY : GRAY);
        DrawText("PLACAR", botaoPlacar.x + 85, botaoPlacar.y + 20, 35, BLACK);

        
        DrawRectangleRec(botaoSair, sair ? LIGHTGRAY : GRAY);
        DrawText("SAIR", botaoSair.x + 110, botaoSair.y + 20, 35, BLACK);

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

void inicializaVetor(Usuario vetor[], int tamanho){
    for (int i = 0; i < tamanho; i++) {
        vetor[i].vida = 5;
        vetor[i].pontuacao = 0;
        vetor[i].nome[0] = '\0';
    }
}







float controleContadoraPlayer1(float contadoraPlayer1){
    if (IsKeyPressed(KEY_W)) contadoraPlayer1++;
    if (IsKeyPressed(KEY_S)) contadoraPlayer1--;
    if(IsKeyPressed(KEY_E)) contadoraPlayer1+=5;
    if(IsKeyPressed(KEY_Q)) contadoraPlayer1-=5;
    
    if (contadoraPlayer1 < 0) contadoraPlayer1 = 0;
    if (contadoraPlayer1 > 25) contadoraPlayer1 = 25;

    return contadoraPlayer1;
}

float controleContadoraPlayer2(float contadoraPlayer2){
    if (IsKeyPressed(KEY_UP)) contadoraPlayer2++;
    if (IsKeyPressed(KEY_DOWN)) contadoraPlayer2--;
     if(IsKeyPressed(KEY_LEFT)) contadoraPlayer2+=5;
    if(IsKeyPressed(KEY_RIGHT)) contadoraPlayer2-=5;

    if (contadoraPlayer2 < 0) contadoraPlayer2 = 0;
    if (contadoraPlayer2 > 25) contadoraPlayer2 = 25;

    return contadoraPlayer2;
}



void desenharMapa(float larguraTotal, float alturaDoMapa){
    DrawLine(larguraTotal/5,0,larguraTotal/5,alturaDoMapa,BLACK);
    DrawLine((larguraTotal*2)/5,0,(larguraTotal*2)/5,alturaDoMapa,BLACK);
    DrawLine((larguraTotal*3)/5,0,(larguraTotal*3)/5,alturaDoMapa,BLACK);
    DrawLine((larguraTotal*4)/5,0,(larguraTotal*4)/5,alturaDoMapa,BLACK);
    DrawLine(0,alturaDoMapa/5,larguraTotal,alturaDoMapa/5,BLACK);
    DrawLine(0,(alturaDoMapa*2)/5,larguraTotal,(alturaDoMapa*2)/5,BLACK);
    DrawLine(0,(alturaDoMapa*3)/5,larguraTotal,(alturaDoMapa*3)/5,BLACK);
    DrawLine(0,(alturaDoMapa*4)/5,larguraTotal,(alturaDoMapa*4)/5,BLACK);
}



void atualizarTelaFaseDeJogo1Dual(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float tempo){
    UpdateMusicStream(musicaFaseDeJogo);

    BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(0,0,larguraTotal,alturaDoMapa,WHITE);
        desenharMapa(larguraTotal,alturaDoMapa);
        char tempoPrintavel[4];
        char zero[2];
        snprintf(tempoPrintavel,sizeof(tempoPrintavel),"%.0f",tempo);
        snprintf(zero,sizeof(zero),"0");
        //colocar um draw botando uma pergunta de quantos quadrados
        //e um pressione enter para confirmar
        DrawText(tempoPrintavel,(larguraTotal/2)-45,alturaDoMapa,100,RED);
        DrawText(zero,40,alturaDoMapa+50,100,BLUE);
        DrawText(zero,larguraTotal-90,alturaDoMapa+50,100,GREEN);
    EndDrawing();
}

void atualizarTelaFaseDeJogo2Dual(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float contadoraPlayer1, float contadoraPlayer2, int tamanho,int *posicoes){
    UpdateMusicStream(musicaFaseDeJogo);
    char contadora1Printavel[4];
    char contadora2Printavel[4];
    BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(0,0,larguraTotal,alturaDoMapa,WHITE);
        for (int i = 0; i<tamanho;i++) pintarQuadrados(larguraTotal,alturaDoMapa,posicoes[i]);
        desenharMapa(larguraTotal,alturaDoMapa);
        snprintf(contadora1Printavel,sizeof(contadora1Printavel),"%.0f",contadoraPlayer1);
        snprintf(contadora2Printavel,sizeof(contadora2Printavel),"%.0f",contadoraPlayer2);
        DrawText(contadora1Printavel,40,alturaDoMapa+50,100,BLUE);
        DrawText(contadora2Printavel,larguraTotal-90,alturaDoMapa+50,100,GREEN);
        DrawText("Quantos quadrados há?", 500, 800, 40, DARKGRAY);
    EndDrawing();
}

int *sortearPosEQtd(int tamanho){
    
    int total = 25;
    int *todas = malloc(total * sizeof(int));


    for (int i = 0; i < total; i++){
        todas[i] = i;
    }


    for (int i = total - 1; i > 0; i--){
        int j = GetRandomValue(0, i);
        int tmp = todas[i];
        todas[i] = todas[j];
        todas[j] = tmp;
    }


    int *selecionadas = malloc(tamanho * sizeof(int));
    for (int i = 0; i < tamanho; i++){
        selecionadas[i] = todas[i];
    }

    free(todas);
    return selecionadas;
}



void pintarQuadrados(float larguraTotal, float alturaDoMapa, int pos){
    float largura = larguraTotal / 5;
    float altura = alturaDoMapa / 5;

    int linha = pos / 5;
    int coluna = pos % 5;

    DrawRectangle(coluna * largura, linha * altura, largura, altura, RED);
}


void atualizarTelaFaseDeJogo1Singal(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float tempo){
    UpdateMusicStream(musicaFaseDeJogo);

    BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(0,0,larguraTotal,alturaDoMapa,WHITE);
        desenharMapa(larguraTotal,alturaDoMapa);
        char tempoPrintavel[4];
        char zero[2];
        snprintf(tempoPrintavel,sizeof(tempoPrintavel),"%.0f",tempo);
        snprintf(zero,sizeof(zero),"0");
        
        DrawText(tempoPrintavel,(larguraTotal/2)-45,alturaDoMapa,100,RED);
        DrawText(zero,40,alturaDoMapa+50,100,BLUE);
        
    EndDrawing();
}

void atualizarTelaFaseDeJogo2Singal(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float contadoraPlayer1, float contadoraPlayer2, int tamanho,int *posicoes){
    UpdateMusicStream(musicaFaseDeJogo);
    char contadora1Printavel[4];
    char contadora2Printavel[4];
    BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(0,0,larguraTotal,alturaDoMapa,WHITE);
        for (int i = 0; i<tamanho;i++) pintarQuadrados(larguraTotal,alturaDoMapa,posicoes[i]);
        desenharMapa(larguraTotal,alturaDoMapa);
        
        snprintf(contadora1Printavel,sizeof(contadora1Printavel),"%.0f",contadoraPlayer1);
        
        DrawText(contadora1Printavel,40,alturaDoMapa+50,100,BLUE);
     
        DrawText("Quantos quadrados há?", 500, 800, 40, DARKGRAY);
    EndDrawing();
}



void jogoSinglePlayer(Usuario jogadores[], Music musicaFaseDeJogo, float larguraTotal, float alturaDoMapa){
    float contadoraPlayer1 = 0;
    int tamanho;
    int *posicoes = NULL;
    float tempo = 0;
    int qtdQuadrados = 25;
    int turnoDeJogo = 1;
    int quadradosVisiveis = 1; 

    while(jogadores[0].vida > 0 && !WindowShouldClose()){
        if(turnoDeJogo == 1){
            atualizarTelaFaseDeJogo1Singal(musicaFaseDeJogo, larguraTotal, alturaDoMapa, tempo);
            tempo += GetFrameTime();
            if(tempo >= 5){
                turnoDeJogo = 2;
                contadoraPlayer1 = 0;
                tamanho = GetRandomValue(1, qtdQuadrados);
                posicoes = sortearPosEQtd(tamanho);
                tempo = 0;
                quadradosVisiveis = 1; 
            }
        } else if(turnoDeJogo == 2){
            contadoraPlayer1 = controleContadoraPlayer1(contadoraPlayer1);
            tempo += GetFrameTime();

            
            if(tempo < 3){
                atualizarTelaFaseDeJogo2Singal(musicaFaseDeJogo, larguraTotal, alturaDoMapa, contadoraPlayer1, 0, tamanho, posicoes);
            } else {
                
                if(quadradosVisiveis){
                   
                    free(posicoes);
                    posicoes = NULL;
                    quadradosVisiveis = 0;
                }

                
                atualizarTelaFaseDeJogo2Singal(musicaFaseDeJogo, larguraTotal, alturaDoMapa, contadoraPlayer1, 0, 0, NULL);
                

                
                if(IsKeyPressed(KEY_ENTER)){
                    int diferenca1 = (int)(contadoraPlayer1 + 0.5) - tamanho;
                    if(diferenca1 == 0){
                        jogadores[0].pontuacao += 150; 
                    } else if(diferenca1 == 1 || diferenca1 == -1){
                        jogadores[0].pontuacao += 50;
                        jogadores[0].vida--;
                    } else {
                        jogadores[0].vida--;
                    }

                    tempo = 0;
                    turnoDeJogo = 1;
                }
            }
        }
    }
}




















int main(){
    int escolha = 10;
    int quantidadePlayer;
    const int larguraTela = 1440;
    const int alturaTela = 900;
    float alturaDoMapa = (alturaTela*3)/4;
    InitAudioDevice();
    InitWindow(larguraTela, alturaTela, "Projeto de programação");
    
    Music musica = LoadMusicStream("musicaMenu.mp3");
    Music derrota = LoadMusicStream("derrota.mp3");
    Music vitoria = LoadMusicStream("musicaVitoria.mp3");
    Music musicaFaseDeJogo = LoadMusicStream("musicaJogo.mp3");
    PlayMusicStream(musica);
    PlayMusicStream(derrota);
    PlayMusicStream(vitoria);
    PlayMusicStream(musicaFaseDeJogo);
    SetTargetFPS(60);
   
    Usuario *rank = malloc(10*sizeof(Usuario));
    Usuario *jogadores = malloc(2*sizeof(Usuario));
    
    inicializaVetor(rank, 10);
    inicializaVetor(jogadores, 2);
    carregarArquivo(rank);
    
    
    
    while(escolha!=0 && !WindowShouldClose()){
        escolha = menu(musica);
        
        switch(escolha){
            case 0:
                //falta criar uma funcao aqui e arrumar a centralização do texto
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
                if (quantidadePlayer ==1){
                    jogoSinglePlayer(jogadores, musicaFaseDeJogo,larguraTela , alturaDoMapa);
                    menuDerrota(quantidadePlayer, jogadores);
                    
                    
                }
                comparaComUsuarios(jogadores, rank);
                inicializaVetor(jogadores, 2);
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