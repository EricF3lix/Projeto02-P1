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


Sound click;
Sound beep;

Music musicaMenu;
Music musicaDerrota;
Music musicaVitoria;
Music musicaJogo;

void salvaArquivo(Usuario rank[]);
void carregarArquivo(Usuario rank[]);
void desenhaPlacar(Usuario rank[]);
void ordenaTop10(Usuario todosUsuarios[]);
void comparaComUsuarios(Usuario jogadores[], Usuario rank[]);
void menuDerrota(int quantidadePlayer, Usuario jogador[]);
void menuComoJogar(int quantidadePlayer);     
void menuJogador(Usuario jogadores[], int quantidadePlayer); 
int menuMultijogador();                       
int menu();                                   
void inicializaVetor(Usuario vetor[], int tamanho);
float controleContadoraPlayer1(float contadora1);
float controleContadoraPlayer2(float contadora2);
void atualizarTelaFaseDeJogo1Dual(float larguraTotal, float alturaDoMapa, float tempo); // removido Music
void atualizarTelaFaseDeJogo2Dual(float larguraTotal, float alturaDoMapa, float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes, Color *cores); // removido Music
int *sortearPosEQtd(int tamanho);
void pintarQuadrados(float larguraTotal, float alturaDoMapa, int pos, Color cor);
void jogoSinglePlayer(Usuario jogadores[], float larguraTotal, float alturaDoMapa, Texture2D imagemRodape); // removido Music
void desenharQuadradosDaRodada(float larguraTotal, float alturaDoMapa, int tamanho, int *posicoes, Color coresSorteadas[]);



void desenhaPlacar(Usuario rank[]){
    
    PlayMusicStream(musicaVitoria);
    

    Rectangle botaoVoltar = {20, 20, 120, 50};
    int parar = 1;

    while(parar!=0 && !WindowShouldClose()){
        UpdateMusicStream(musicaVitoria);
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

    
    StopMusicStream(musicaVitoria);
    
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

void mostraAcerto(int quantidadePlayer,int contadoraPlayer1,int contadoraPlayer2,int tamanho,Usuario jogadores[],Texture2D imagemRodape,float larguraTotal,float alturaDoMapa,int *posicoes,Color coresSorteadas[], int rodadaAtual) {
    float tempo = 0;
    char texto[200];

    while (tempo < 5 && !WindowShouldClose()) {

        UpdateMusicStream(musicaJogo);
        tempo += GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        
        desenharQuadradosDaRodada(larguraTotal, alturaDoMapa, tamanho, posicoes, coresSorteadas);

        
        DrawRectangle(0, alturaDoMapa, larguraTotal, 300, BLACK);

        DrawText("Confira seus acertos!", larguraTotal/2 - 250, alturaDoMapa + 20, 50, MAROON);

        
        snprintf(texto, sizeof(texto), "%s contou: %d", jogadores[0].nome, contadoraPlayer1);
        DrawText(texto, 50, alturaDoMapa + 100, 40, BLUE);

        snprintf(texto, sizeof(texto), "Vida: %d   |   Pontos: %d",
                 jogadores[0].vida, jogadores[0].pontuacao);
        DrawText(texto, 50, alturaDoMapa + 150, 35, RAYWHITE);

        
        snprintf(texto, sizeof(texto), "Quantidade correta: %d", tamanho);
        DrawText(texto, larguraTotal/2 - 180, alturaDoMapa + 80, 35, YELLOW);
        
        snprintf(texto, sizeof(texto), "Rodada: %d", rodadaAtual);
        DrawText(texto, larguraTotal/2 -20, alturaDoMapa + 180, 20, ORANGE);
        
        if (quantidadePlayer == 2) {
            snprintf(texto, sizeof(texto), "%s contou: %d", jogadores[1].nome, contadoraPlayer2);
            DrawText(texto, larguraTotal - 450, alturaDoMapa + 100, 40, GREEN);

            snprintf(texto, sizeof(texto), "Vida: %d   |   Pontos: %d",
                     jogadores[1].vida, jogadores[1].pontuacao);
            DrawText(texto, larguraTotal - 450, alturaDoMapa + 150, 35, RAYWHITE);
        }

        EndDrawing();
    }
}




void menuDerrota(int quantidadePlayer, Usuario jogador[]){
    
    int enter = 1;

    while(enter != 0 && !WindowShouldClose()){
        UpdateMusicStream(musicaMenu);
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangle(420, 90, 600, 60, LIGHTGRAY);
            DrawRectangle(420, 150, 600, 50, LIGHTGRAY);
            DrawRectangle(400, 680, 640, 50, LIGHTGRAY);

            char texto1[100];
            char texto2[100];
            snprintf(texto1, sizeof(texto1), "%s - VOCÊ PERDEU!", jogador[0].nome);
            snprintf(texto2, sizeof(texto2), "Sua pontuação total foi: %d", jogador[0].pontuacao);

            DrawText(texto1, 440, 100, 40, MAROON);
            DrawText(texto2, 440, 155, 40, MAROON);

            DrawText("Pressione ENTER para continuar", 420, 690, 35, DARKGRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            PlaySound(beep);   
            enter = 0;
        }
    }
    
}





void menuComoJogar(int quantidadePlayer) {
    int enter = 1;

    while (enter != 0 && !WindowShouldClose()) {
        UpdateMusicStream(musicaMenu);

        BeginDrawing();
        ClearBackground(RAYWHITE);

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
            PlaySound(beep); 
        }
    }
}








void menuJogador(Usuario jogadores[], int quantidadePlayer) {
    Rectangle caixa1 = { 520, 250, 400, 60 };
    Rectangle caixa2 = { 520, 430, 400, 60 };

    bool ativa1 = false;
    bool ativa2 = false;

    int len1 = 0;
    int len2 = 0;

    while (!WindowShouldClose()) {
        UpdateMusicStream(musicaMenu);

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
            PlaySound(beep);
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





int menuMultijogador() {
    const char *texto[] = {"Você deseja jogar",
                           "SINGLE PLAYER",
                           "OU",
                           "DUAL PLAYER"};
    
    Rectangle botao1Player = {570, 500, 300, 70};
    Rectangle botao2Player = {570, 600, 300, 70};

    while (!WindowShouldClose()){
        UpdateMusicStream(musicaMenu);
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
            PlaySound(beep);
            return 1;
        }
        if (player2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(beep);
            return 2;
        }               
    }

    return -1;
}




int menu() {
    PlayMusicStream(musicaMenu);
    const char *texto[] = {
        "Olá, seja bem vindo ao",
        "Deu a Louca no Quadrado!",
        "Escolha uma das opções abaixo:"
    };

    Rectangle botaoJogar  = { 570, 500, 300, 70 };
    Rectangle botaoPlacar = { 570, 600, 300, 70 };
    Rectangle botaoSair   = { 570, 700, 300, 70 };

    while (!WindowShouldClose()) {
        UpdateMusicStream(musicaMenu);   

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

        if (jogar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ 
            PlaySound(beep);
            return 1;
            
        }
        if (placar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(beep);
            return 2;
            
        }
        if (sair && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(beep); 
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
     if (IsKeyPressed(KEY_W)){
        contadoraPlayer1++;
        PlaySound(click);
    }

    if (IsKeyPressed(KEY_S)){
        contadoraPlayer1--;
        PlaySound(click);
    }
    if(IsKeyPressed(KEY_E)){
        contadoraPlayer1+=5;
        PlaySound(click);
    }
    if(IsKeyPressed(KEY_Q)){
        contadoraPlayer1-=5;
        PlaySound(click);
    }
    if (contadoraPlayer1 < 0){
        contadoraPlayer1 = 0;
        PlaySound(click);
    }
    if (contadoraPlayer1 > 25){
        contadoraPlayer1 = 25;
        PlaySound(click);
    }
    return contadoraPlayer1;
}

float controleContadoraPlayer2(float contadoraPlayer2){
    if (IsKeyPressed(KEY_UP)){
        contadoraPlayer2++;
        PlaySound(click);
    }

    if (IsKeyPressed(KEY_DOWN)){
        contadoraPlayer2--;
        PlaySound(click);
    }
    if(IsKeyPressed(KEY_LEFT)){
        contadoraPlayer2+=5;
        PlaySound(click);
    }
    if(IsKeyPressed(KEY_RIGHT)){
        contadoraPlayer2-=5;
        PlaySound(click);
    }
    if (contadoraPlayer2 < 0){
        contadoraPlayer2 = 0;
        PlaySound(click);
    }
    if (contadoraPlayer2 > 25){
        contadoraPlayer2 = 25;
        PlaySound(click);
    }
    return contadoraPlayer2;
}





void atualizarTelaFaseDeJogo1Dual(float larguraTotal, float alturaDoMapa, float tempo){
    UpdateMusicStream(musicaJogo);

    BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(0,0,larguraTotal,alturaDoMapa,WHITE);

        char tempoPrintavel[4];
        char zero[2];

        snprintf(tempoPrintavel,sizeof(tempoPrintavel),"%.0f",tempo);
        snprintf(zero,sizeof(zero),"0");

        DrawText(tempoPrintavel,(larguraTotal/2)-45,alturaDoMapa,100,RED);
        DrawText(zero,40,alturaDoMapa+50,100,BLUE);
        DrawText(zero,larguraTotal-90,alturaDoMapa+50,100,GREEN);
    EndDrawing();
}


void atualizarTelaFaseDeJogo2Dual(float larguraTotal, float alturaDoMapa, float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes, Color *cores){
    UpdateMusicStream(musicaJogo);

    char contadora1Printavel[4];
    char contadora2Printavel[4];

    BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, larguraTotal, alturaDoMapa, WHITE);

        if(tamanho > 0 && posicoes != NULL && cores != NULL){
            for (int i = 0; i < tamanho; i++){
                pintarQuadrados(larguraTotal, alturaDoMapa, posicoes[i], cores[i]);
            }
        }

        snprintf(contadora1Printavel, sizeof(contadora1Printavel), "%.0f", contadoraPlayer1);
        snprintf(contadora2Printavel, sizeof(contadora2Printavel), "%.0f", contadoraPlayer2);

        DrawText(contadora1Printavel, 40, alturaDoMapa + 50, 100, BLUE);
        DrawText(contadora2Printavel, 1300, alturaDoMapa + 50, 100, RED);

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



void pintarQuadrados(float larguraTotal, float alturaDoMapa, int pos, Color coresSorteadas){
    float largura = larguraTotal / 5;
    float altura = alturaDoMapa / 5;
    int linha = pos / 5;
    int coluna = pos % 5;
    Rectangle retangulo = {coluna * largura, linha * altura, largura, altura, coresSorteadas};
    DrawRectangle(coluna * largura, linha * altura, largura, altura, coresSorteadas);
    DrawRectangleLinesEx(retangulo, 2, DARKGRAY);
}


void atualizarTelaFaseDeJogo1Singal(float larguraTotal, float alturaDoMapa, float tempo){
    UpdateMusicStream(musicaJogo);

    BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(0,0,larguraTotal,alturaDoMapa,WHITE);
        
        char tempoPrintavel[4];
        char zero[2];
        snprintf(tempoPrintavel,sizeof(tempoPrintavel),"%.0f",tempo);
        snprintf(zero,sizeof(zero),"0");
        
        DrawText(tempoPrintavel,(larguraTotal/2)-45,alturaDoMapa,100,RED);
        DrawText(zero,40,alturaDoMapa+50,100,BLUE);
        
    EndDrawing();
}


void atualizarTelaFaseDeJogo2Singal(float larguraTotal, float alturaDoMapa, float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes, Color coresSorteadas[]){
    UpdateMusicStream(musicaJogo);

    BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(0,0,larguraTotal,alturaDoMapa,WHITE);

        for (int i = 0; i < tamanho; i++){
            pintarQuadrados(larguraTotal,alturaDoMapa,posicoes[i],coresSorteadas[i]);
        }

        char contadora1Printavel[4];
        snprintf(contadora1Printavel,sizeof(contadora1Printavel),"%.0f", contadoraPlayer1);

        DrawText(contadora1Printavel,40,alturaDoMapa+50,100,BLUE);
        DrawText("Quantos quadrados há?", 500, 800, 40, DARKGRAY);

    EndDrawing();
}


void desenharQuadradosDaRodada(float larguraTotal, float alturaDoMapa, int tamanho, int *posicoes, Color coresSorteadas[]) {
    
    DrawRectangle(0, 0, larguraTotal, alturaDoMapa, WHITE);

    for (int i = 0; i < tamanho; i++) {
        pintarQuadrados(larguraTotal, alturaDoMapa, posicoes[i], coresSorteadas[i]);
    }
}



void jogoSinglePlayer(Usuario jogadores[], float larguraTotal, float alturaDoMapa, Texture2D imagemRodape){
    PlayMusicStream(musicaJogo);
    
    int contaRodada = 0;
    float contadoraPlayer1 = 0;
    int tamanho;
    int *posicoes = NULL;
    Color *coresSorteadas = NULL;
    float tempo = 0;
    float tempoDoJogo;
    int qtdQuadrados;
    int qtdCoresUsadas;
    int turnoDeJogo = 1;

    Color cor[] = { GREEN, YELLOW, RED };

    while (jogadores[0].vida > 0 && !WindowShouldClose()){

        
        UpdateMusicStream(musicaJogo);

        if (turnoDeJogo == 1){

            atualizarTelaFaseDeJogo1Singal(larguraTotal, alturaDoMapa, tempo);
            tempo += GetFrameTime();

            if (tempo >= 5){

                if (contaRodada <= 5){
                    qtdQuadrados = 10;
                    tamanho = GetRandomValue(1, qtdQuadrados);
                    qtdCoresUsadas = 1;
                    tempoDoJogo = 3;
                } 
                else if (contaRodada <= 9){
                    qtdQuadrados = 15;
                    tamanho = GetRandomValue(7, qtdQuadrados);
                    qtdCoresUsadas = 2;
                    tempoDoJogo = 2;
                } 
                else {
                    qtdQuadrados = 25;
                    tamanho = GetRandomValue(15, qtdQuadrados);
                    qtdCoresUsadas = 3;
                    tempoDoJogo = 1;
                }

                turnoDeJogo = 2;
                contadoraPlayer1 = 0;
                tempo = 0;

                posicoes = sortearPosEQtd(tamanho);
                coresSorteadas = malloc(tamanho * sizeof(Color));

                for (int i = 0; i < tamanho; i++){
                    coresSorteadas[i] = cor[ GetRandomValue(0, qtdCoresUsadas - 1)];
                }
            }
        }

        else if (turnoDeJogo == 2){

            contadoraPlayer1 = controleContadoraPlayer1(contadoraPlayer1);
            tempo+= GetFrameTime();

            if (tempo <  tempoDoJogo){
                atualizarTelaFaseDeJogo2Singal(larguraTotal, alturaDoMapa, contadoraPlayer1, 0, tamanho, posicoes, coresSorteadas);
            } 
            else {

                atualizarTelaFaseDeJogo2Singal(larguraTotal, alturaDoMapa, contadoraPlayer1, 0, 0, NULL, NULL);

                if (IsKeyPressed(KEY_F)){

                    int diferenca1 = (int)(contadoraPlayer1 + 0.5) - tamanho;

                    if (diferenca1 == 0){
                        jogadores[0].pontuacao += 100;
                    } 
                    else if (diferenca1 == 1 || diferenca1 == -1){
                        jogadores[0].pontuacao += 50;
                        jogadores[0].vida--;
                    } 
                    else {
                        jogadores[0].vida--;
                    }

                    tempo = 0;
                    turnoDeJogo = 1;
                    
                    mostraAcerto(1,contadoraPlayer1,0,tamanho,jogadores,imagemRodape,larguraTotal,alturaDoMapa,posicoes,coresSorteadas, contaRodada+1);

                    contaRodada++;

                    free(posicoes);
                    free(coresSorteadas);

                    posicoes = NULL;
                    coresSorteadas = NULL;
                }
            }
        }
    }
    StopMusicStream(musicaJogo);
}





void jogoDualPlayer(Usuario jogadores[], float larguraTotal, float alturaDoMapa, Texture2D imagemRodape){
    PlayMusicStream(musicaJogo);
    Color cor[] = { GREEN, YELLOW, RED };
    Color *coresSorteadas = NULL;

    bool podeJogar1 = true;
    bool podeJogar2 = true;

    float contadoraPlayer1 = 0;
    float contadoraPlayer2 = 0;

    int tamanho;
    int *posicoes = NULL;

    float tempo = 0;
    int turnoDeJogo = 1;
    int contaRodada = 0;
    float tempoDoJogo;

    int qtdQuadrados;
    int qtdCoresUsadas;

    while (jogadores[0].vida > 0 && jogadores[1].vida > 0 && !WindowShouldClose()){

        
        UpdateMusicStream(musicaJogo);

        if (turnoDeJogo == 1){

            atualizarTelaFaseDeJogo1Dual(larguraTotal, alturaDoMapa, tempo);
            tempo += GetFrameTime();

            if (tempo >= 5){

                if (contaRodada <= 5){
                    qtdQuadrados = 10;
                    tamanho = GetRandomValue(1, qtdQuadrados);
                    qtdCoresUsadas = 1;
                    tempoDoJogo = 3;
                }
                else if (contaRodada <= 9){
                    qtdQuadrados = 15;
                    tamanho = GetRandomValue(7, qtdQuadrados);
                    qtdCoresUsadas = 2; 
                    tempoDoJogo = 2;
                }
                else{
                    qtdQuadrados = 25;
                    tamanho = GetRandomValue(15, qtdQuadrados);
                    qtdCoresUsadas = 3; 
                    tempoDoJogo = 1;
                }

                posicoes = sortearPosEQtd(tamanho);

                coresSorteadas = malloc(tamanho * sizeof(Color));

                for (int i = 0; i < tamanho; i++){
                    coresSorteadas[i] = cor[ GetRandomValue(0, qtdCoresUsadas - 1) ];
                }

                turnoDeJogo = 2;
                contadoraPlayer1 = 0;
                contadoraPlayer2 = 0;
                tempo = 0;
            }
        }

        else if (turnoDeJogo == 2){

            if (podeJogar1) contadoraPlayer1 = controleContadoraPlayer1(contadoraPlayer1);
            if (podeJogar2) contadoraPlayer2 = controleContadoraPlayer2(contadoraPlayer2);

            tempo += GetFrameTime();

            if (tempo < tempoDoJogo){
                atualizarTelaFaseDeJogo2Dual(larguraTotal, alturaDoMapa, contadoraPlayer1, contadoraPlayer2, tamanho, posicoes, coresSorteadas);
            }
            else {

                atualizarTelaFaseDeJogo2Dual(larguraTotal, alturaDoMapa, contadoraPlayer1, contadoraPlayer2, 0, NULL, NULL);

                if (IsKeyPressed(KEY_F) && podeJogar1){
                    int dif1 = (int)(contadoraPlayer1 + 0.5) - tamanho;

                    if (dif1 == 0){
                        jogadores[0].pontuacao += 100;
                    }
                    else if (dif1 == 1 || dif1 == -1){
                        jogadores[0].pontuacao += 50;
                        jogadores[0].vida--;
                    }
                    else{
                        jogadores[0].vida--;
                    }

                    podeJogar1 = false;
                }

                if (IsKeyPressed(KEY_ENTER) && podeJogar2){
                    int dif2 = (int)(contadoraPlayer2 + 0.5) - tamanho;

                    if (dif2 == 0){
                        jogadores[1].pontuacao += 100;
                    }
                    else if (dif2 == 1 || dif2 == -1){
                        jogadores[1].pontuacao += 50;
                        jogadores[1].vida--;
                    }
                    else{
                        jogadores[1].vida--;
                    }

                    podeJogar2 = false;
                }

                if (!podeJogar1 && !podeJogar2){


                    tempo = 0;
                    turnoDeJogo = 1;
                    podeJogar1 = true;
                    podeJogar2 = true;
                    
                    mostraAcerto(2,contadoraPlayer1,contadoraPlayer2 ,tamanho,jogadores,imagemRodape,larguraTotal,alturaDoMapa,posicoes,coresSorteadas, contaRodada+1);

                    contaRodada++;
                    
                    free(posicoes);
                    free(coresSorteadas);
                    posicoes = NULL;
                    coresSorteadas = NULL;

                }
            }
        }
    }
    StopMusicStream(musicaJogo);
}























int main(){
    int escolha = 10;
    int quantidadePlayer;
    const int larguraTela = 1440;
    const int alturaTela = 900;
    float alturaDoMapa = (alturaTela*3)/4;

    InitAudioDevice();
    InitWindow(larguraTela, alturaTela, "Projeto de programação");

    click = LoadSound("click.mp3");
    beep  = LoadSound("beep.mp3");
    SetSoundVolume(beep, 0.1f);

    musicaMenu = LoadMusicStream("musicaMenu.mp3");
    musicaDerrota = LoadMusicStream("derrota.mp3");
    musicaVitoria = LoadMusicStream("musicaVitoria.mp3");
    musicaJogo = LoadMusicStream("musicaJogo.mp3");
    Texture2D imagemRodape = LoadTexture("imagemRodape.png");

    SetTargetFPS(60);

    Usuario *rank = malloc(10*sizeof(Usuario));
    Usuario *jogadores = malloc(2*sizeof(Usuario));

    inicializaVetor(rank, 10);
    inicializaVetor(jogadores, 2);
    carregarArquivo(rank);

    while(escolha != 0 && !WindowShouldClose()){

        escolha = menu();

        switch(escolha){

            case 0:
                while(!WindowShouldClose()){
                    UpdateMusicStream(musicaDerrota);
                    BeginDrawing();
                        ClearBackground(BLACK);
                        DrawText("Arregou! FIM DE JOGO!", 40, 180, 60, MAROON);
                    EndDrawing();
                }
                break;

            case 1:
                quantidadePlayer = menuMultijogador();
                menuJogador(jogadores, quantidadePlayer);
                menuComoJogar(quantidadePlayer);

                if (quantidadePlayer == 1){
                    jogoSinglePlayer(jogadores, larguraTela, alturaDoMapa, imagemRodape);
                }
                else{
                    jogoDualPlayer(jogadores, larguraTela, alturaDoMapa, imagemRodape);
                }

                menuDerrota(quantidadePlayer, jogadores);
                comparaComUsuarios(jogadores, rank);
                inicializaVetor(jogadores, 2);
                break;

            case 2:
                desenhaPlacar(rank);
                break;
        }
    }

    free(rank);
    free(jogadores);

    
    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(musicaDerrota);
    UnloadMusicStream(musicaVitoria);
    UnloadMusicStream(musicaJogo);  

    CloseAudioDevice();
}
