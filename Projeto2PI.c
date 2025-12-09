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

//============================ Variáveis globais (apenas os sons e músicas) ============================

Sound click;
Sound beep;

Music musicaMenu;
Music musicaDerrota;
Music musicaVitoria;
Music musicaJogo;
//=======================================================================================================
//============================ Declaração das funções ============================

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
int menuSairDoJogo();
void mostraAcerto(int quantidadePlayer,int contadoraPlayer1,int contadoraPlayer2,int tamanho,Usuario jogadores[], float larguraTotal,float alturaDoMapa,int *posicoes,Color coresSorteadas[], int rodadaAtual, int contaRodada);

void inicializaVetor(Usuario vetor[], int tamanho);
float controleContadoraPlayer1(float contadora1);
float controleContadoraPlayer2(float contadora2);

void atualizarTelaFaseDeJogo1Dual(float larguraTotal, float alturaDoMapa, float tempo);
void atualizarTelaFaseDeJogo2Dual(float larguraTotal, float alturaDoMapa, float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes, Color *cores, int contaRodada);
void jogoDualPlayer(Usuario jogadores[], float larguraTotal, float alturaDoMapa);

void atualizarTelaFaseDeJogo1Singal(float larguraTotal, float alturaDoMapa, float tempo);
void atualizarTelaFaseDeJogo2Singal(float larguraTotal, float alturaDoMapa, float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes, Color coresSorteadas[], int contaRodada);
void jogoSinglePlayer(Usuario jogadores[], float larguraTotal, float alturaDoMapa);

int *sortearPosEQtd(int tamanho);
void pintarQuadrados(float larguraTotal, float alturaDoMapa, int pos, Color cor, int contaRodada);
void desenharQuadradosDaRodada(float larguraTotal, float alturaDoMapa, int tamanho, int *posicoes, Color coresSorteadas[], int contaRodada);

// ==========================================================================================================



// ============== TUDO QUE MEXE COM O PLACAR ==============================================

void desenhaPlacar(Usuario rank[]){

    PlayMusicStream(musicaVitoria);


    Rectangle botaoVoltar = {20, 20, 120, 50};
    int parar = 1;
    Texture2D imagemFundo = LoadTexture("assets/imagemfundo.png");
    while(parar!=0 && !WindowShouldClose()){
        UpdateMusicStream(musicaVitoria);
        Vector2 mouse = GetMousePosition();
        bool voltar = CheckCollisionPointRec(mouse, botaoVoltar);

        BeginDrawing();

            DrawTexture(imagemFundo, 0, 0, WHITE);

            DrawText("RANK TOP 10 DO DEU A LOUCA NO QUADRADO!", 220, 30, 40, MAROON);

            int eixoY = 120;
            char jogador[100];

            for(int i = 0 ; i < 10 ; i++){
                snprintf(jogador, sizeof(jogador), "%dº - %s %d", i+1, rank[i].nome, rank[i].pontuacao);

                DrawRectangle(300, eixoY - 5, 840, 40, GRAY);

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
            PlaySound(beep);
            parar = 0;
        }
    }


    StopMusicStream(musicaVitoria);
    UnloadTexture(imagemFundo);

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
    FILE *arquivo = fopen("assets/PlacarLideranca.txt", "r");
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
    FILE *arquivo = fopen("assets/PlacarLideranca.txt", "w");
    if(arquivo){
        fprintf(arquivo, "RANK TOP 10 DO DEU A LOUCA NO QUADRADO!\n");
        for(int i = 0 ; i < 10 ; i++){
            fprintf(arquivo, "%s %d\n", rank[i].nome, rank[i].pontuacao);
        }
        fclose(arquivo);

    }
}

// ==========================================================================================================

// =============================================== TODOS OS MENUS DO JOGO ====================================

int menuSairDoJogo() {
    PlayMusicStream(musicaDerrota);
    Texture2D imagemFim = LoadTexture("assets/imagemFim.png");

    int opcao = -1;


    Rectangle botaoSair   = { 560, 700, 320, 65 };
    Rectangle botaoVoltar = { 560, 785, 320, 65 };

    while (opcao == -1 && !WindowShouldClose()) {
        UpdateMusicStream(musicaDerrota);
        Vector2 mouse = GetMousePosition();

        bool sair   = CheckCollisionPointRec(mouse, botaoSair);
        bool voltar = CheckCollisionPointRec(mouse, botaoVoltar);

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(imagemFim, 0, 0, WHITE);

            DrawText("ARREGOU?", 470, 20, 80, MAROON);
            DrawText("Obrigado por ter jogado!", 310, 100, 60, RAYWHITE);

            DrawText("Feito por", 1250, 800, 20, RAYWHITE);
            DrawText("Eric Felix", 1250, 840, 20, RAYWHITE);
            DrawText("Rodrigo de Alvino", 1250, 860, 20, RAYWHITE);

            DrawRectangleRec(botaoSair, sair ? LIGHTGRAY : GRAY);
            DrawText("SAIR DO JOGO", botaoSair.x + 45, botaoSair.y + 18, 32, BLACK);


            DrawRectangleRec(botaoVoltar, voltar ? LIGHTGRAY : GRAY);
            DrawText("VOLTAR AO JOGO", botaoVoltar.x + 20, botaoVoltar.y + 18, 32, BLACK);

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (sair) {
                PlaySound(beep);
                opcao = 0;
            }
            else if (voltar) {
                PlaySound(beep);
                opcao = 1;
            }
        }
    }
    StopMusicStream(musicaDerrota);
    UnloadTexture(imagemFim);
    return opcao;
}










void menuDerrota(int quantidadePlayer, Usuario jogador[]) {


    int space = 1;
    Texture2D imagemFundo = LoadTexture("assets/imagemfundo.png");
    while (space != 0 && !WindowShouldClose()) {
        UpdateMusicStream(musicaMenu);

        BeginDrawing();
            DrawTexture(imagemFundo, 0, 0, WHITE);
            DrawRectangleRounded((Rectangle){300, 120, 800, 450}, 0.2f, 20, LIGHTGRAY);

            DrawRectangleRoundedLines((Rectangle){300, 120, 800, 450}, 0.2f, 20, GRAY);

            DrawText("DERROTA!", 550, 145, 60, RED);

            char texto[200];
            snprintf(texto, sizeof(texto),
                     "%s | Pontuação: %d",
                     jogador[0].nome, jogador[0].pontuacao);

            DrawText(texto, 350, 260, 40, BLACK);

            if (quantidadePlayer == 2) {
                snprintf(texto, sizeof(texto),
                         "%s | Pontuação: %d",
                         jogador[1].nome, jogador[1].pontuacao);

                DrawText(texto, 350, 330, 40, BLACK);
            }

            DrawText("Pressione ESPAÇO para continuar",400, 520, 35,RED);

        EndDrawing();

        if (IsKeyPressed(KEY_SPACE)) {
            PlaySound(beep);
            space = 0;
        }
    }
    UnloadTexture(imagemFundo);
}







void menuComoJogar(int quantidadePlayer) {

    int space = 1;
    Texture2D imagemFundo = LoadTexture("assets/imagemfundo.png");
    while (space!=0 && !WindowShouldClose()) {

        UpdateMusicStream(musicaMenu);

        BeginDrawing();
            DrawTexture(imagemFundo, 0, 0, WHITE);


            DrawRectangleRounded((Rectangle){120, 40, 1200, 700}, 0.10f, 12, LIGHTGRAY);

            DrawRectangleRoundedLines((Rectangle){120, 40, 1200, 700}, 0.10f, 12,GRAY);


        DrawText("COMO JOGAR", 520, 100, 50, DARKBLUE);


        DrawText("JOGADOR 1", 260, 180, 35, MAROON);

        DrawText("W  - Aumentar contadora", 260, 230, 30, BLACK);
        DrawText("S  - Diminuir contadora", 260, 270, 30, BLACK);
        DrawText("E  - Aumentar +5",        260, 310, 30, BLACK);
        DrawText("Q  - Diminuir -5",        260, 350, 30, BLACK);
        DrawText("F  - Confirmar resposta", 260, 390, 30, BLACK);


        if (quantidadePlayer == 2) {

            DrawText("JOGADOR 2", 760, 180, 35, DARKGREEN);

            DrawText("UP - Aumentar contadora",      760, 230, 30, BLACK);
            DrawText("DOWN - Diminuir contadora",      760, 270, 30, BLACK);
            DrawText("RIGHT - Aumentar +5",              760, 310, 30, BLACK);
            DrawText("LEFT - Diminuir -5",              760, 350, 30, BLACK);
            DrawText("ENTER - Confirmar resposta", 760, 390, 30, BLACK);
        }


        DrawText("Pressione ESPAÇO para continuar", 360, 630, 32, DARKGRAY);

        EndDrawing();

        if (IsKeyPressed(KEY_SPACE)) {
            PlaySound(beep);
            space = 0;
        }
    }
    UnloadTexture(imagemFundo);
}









void menuJogador(Usuario jogadores[], int quantidadePlayer) {

    Rectangle caixa1 = { 500, 260, 450, 70 };
    Rectangle caixa2 = { 500, 430, 450, 70 };

    bool ativa1 = false;
    bool ativa2 = false;

    int len1 = 0;
    int len2 = 0;
    Texture2D imagemFundo = LoadTexture("assets/imagemfundo.png");
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


        if (IsKeyPressed(KEY_SPACE)) {
            if (quantidadePlayer == 1 && len1 > 0) break;
            if (quantidadePlayer == 2 && len1 > 0 && len2 > 0) break;

            PlaySound(beep);
        }


        BeginDrawing();
            DrawTexture(imagemFundo, 0, 0, WHITE);

            DrawText("DIGITE OS NOMES DOS JOGADORES", 300, 120, 50, MAROON);


            DrawText("Jogador 1:", 500, 215, 32, BLACK);

            DrawRectangleRounded(caixa1, 0.3f, 20, LIGHTGRAY);
            DrawRectangleRoundedLines(caixa1, 0.3f, 20, GRAY);

            DrawText(jogadores[0].nome, caixa1.x + 15, caixa1.y + 20, 34, BLACK);

            if (ativa1)
                DrawText("_",
                    caixa1.x + 15 + MeasureText(jogadores[0].nome, 34),
                    caixa1.y + 20, 34, BLACK);


            if (quantidadePlayer == 2) {
                DrawText("Jogador 2:", 500, 385, 32, BLACK);

                DrawRectangleRounded(caixa2, 0.3f, 20, LIGHTGRAY);
                DrawRectangleRoundedLines(caixa2, 0.3f, 20, GRAY);

                DrawText(jogadores[1].nome, caixa2.x + 15, caixa2.y + 20, 34, BLACK);

                if (ativa2)
                    DrawText("_",
                        caixa2.x + 15 + MeasureText(jogadores[1].nome, 34),
                        caixa2.y + 20, 34, BLACK);
            }

            DrawText("Pressione ESPAÇO para continuar",
                     420, 720, 36, DARKGRAY);

        EndDrawing();
    }
    UnloadTexture(imagemFundo);
}






int menuMultijogador() {
    const char *texto[] = {"Você deseja jogar",
                           "SINGLE PLAYER",
                           "OU",
                           "DUAL PLAYER"};

    Rectangle botao1Player = {570, 500, 300, 70};
    Rectangle botao2Player = {570, 600, 300, 70};
    Texture2D imagemFundo = LoadTexture("assets/imagemfundo.png");
    while (!WindowShouldClose()){
        UpdateMusicStream(musicaMenu);
        Vector2 mouse = GetMousePosition();
        bool player1 = CheckCollisionPointRec(mouse, botao1Player);
        bool player2 = CheckCollisionPointRec(mouse, botao2Player);

        BeginDrawing();
            DrawTexture(imagemFundo, 0, 0, WHITE);

            DrawText(texto[0], 450, 80, 60, BLACK);

            DrawText(texto[1], 530, 180, 40, MAROON);
            DrawText(texto[2], 680, 240, 40, BLACK);
            DrawText(texto[3], 540, 300, 45, MAROON);

            DrawRectangleRec(botao1Player, player1 ? LIGHTGRAY : GRAY);
            DrawText("SINGLE PLAYER", botao1Player.x + 30, botao1Player.y + 22, 30, BLACK);

            DrawRectangleRec(botao2Player, player2 ? LIGHTGRAY : GRAY);
            DrawText("DUAL PLAYER", botao2Player.x + 40, botao2Player.y + 22, 30, BLACK);

        EndDrawing();

        if (player1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(beep);
            UnloadTexture(imagemFundo);
            return 1;
        }
        if (player2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(beep);
            UnloadTexture(imagemFundo);
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
    int opcao = -1;
    Rectangle botaoJogar  = { 570, 500, 300, 70 };
    Rectangle botaoPlacar = { 570, 600, 300, 70 };
    Rectangle botaoSair   = { 570, 700, 300, 70 };
    Texture2D imagemFundo = LoadTexture("assets/imagemfundo.png");
    while (opcao == -1 && !WindowShouldClose()) {
        UpdateMusicStream(musicaMenu);

        Vector2 mouse = GetMousePosition();

        bool jogar  = CheckCollisionPointRec(mouse, botaoJogar);
        bool placar = CheckCollisionPointRec(mouse, botaoPlacar);
        bool sair   = CheckCollisionPointRec(mouse, botaoSair);

        BeginDrawing();
            DrawTexture(imagemFundo, 0, 0, WHITE);

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
            opcao = 1;



        }
        if (placar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            PlaySound(beep);
            opcao = 2;



        }
        if (sair && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(beep);
            opcao = 0;



        }
    }
    UnloadTexture(imagemFundo);
    return opcao;
}



// ===================================================================================================

//  ======================================= OPERAÇÕES USADAS NO JOGO =====================================

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
        contadoraPlayer2-=5;
        PlaySound(click);
    }
    if(IsKeyPressed(KEY_RIGHT)){
        contadoraPlayer2+=5;
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



void pintarQuadrados(float larguraTotal, float alturaDoMapa, int pos, Color coresSorteadas, int contaRodada){
    float largura = larguraTotal / 5;
    float altura = alturaDoMapa / 5;
    int linha = pos / 5;
    int coluna = pos % 5;


    if (contaRodada < 10){
        Rectangle retangulo = {coluna * largura, linha * altura, largura, altura};
        DrawRectangle(coluna * largura, linha * altura, largura, altura, coresSorteadas);
        DrawRectangleLinesEx(retangulo, 2, DARKGRAY);
    }
    else{
        Rectangle retangulo = {coluna * largura, linha * altura, largura+12, altura+12};
        DrawRectangle(coluna * largura, linha * altura, largura+12, altura+12, coresSorteadas);
        DrawRectangleLinesEx(retangulo, 2, DARKGRAY);

    }

}








void desenharQuadradosDaRodada(float larguraTotal, float alturaDoMapa, int tamanho, int *posicoes, Color coresSorteadas[], int contaRodada) {

    DrawRectangle(0, 0, larguraTotal, alturaDoMapa, WHITE);

    for (int i = 0; i < tamanho; i++) {
        pintarQuadrados(larguraTotal, alturaDoMapa, posicoes[i], coresSorteadas[i], contaRodada);
    }
}

void mostraAcerto(int quantidadePlayer,int contadoraPlayer1,int contadoraPlayer2,int tamanho,Usuario jogadores[], float larguraTotal,float alturaDoMapa,int *posicoes,Color coresSorteadas[], int rodadaAtual, int contaRodada) {
    float tempo = 0;
    char texto[200];
    Texture2D imagemFundo = LoadTexture("assets/imagemfundo.png");
    while (tempo < 3 && !WindowShouldClose()) {

        UpdateMusicStream(musicaJogo);
        tempo += GetFrameTime();

        BeginDrawing();
            DrawTexture(imagemFundo, 0, 0, WHITE);


            desenharQuadradosDaRodada(larguraTotal, alturaDoMapa, tamanho, posicoes, coresSorteadas, contaRodada);


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
    UnloadTexture(imagemFundo);
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

void atualizarTelaFaseDeJogo2Singal(float larguraTotal, float alturaDoMapa, float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes, Color coresSorteadas[], int contaRodada){
    UpdateMusicStream(musicaJogo);

    BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(0,0,larguraTotal,alturaDoMapa,WHITE);

        for (int i = 0; i < tamanho; i++){
            pintarQuadrados(larguraTotal,alturaDoMapa,posicoes[i],coresSorteadas[i], contaRodada);
        }

        char contadora1Printavel[4];
        snprintf(contadora1Printavel,sizeof(contadora1Printavel),"%.0f", contadoraPlayer1);

        DrawText(contadora1Printavel,40,alturaDoMapa+50,100,BLUE);
        DrawText("Quantos quadrados há?", 500, 800, 40, DARKGRAY);

    EndDrawing();
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


void atualizarTelaFaseDeJogo2Dual(float larguraTotal, float alturaDoMapa, float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes, Color *cores, int contaRodada){
    UpdateMusicStream(musicaJogo);

    char contadora1Printavel[4];
    char contadora2Printavel[4];

    BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, larguraTotal, alturaDoMapa, WHITE);

        if(tamanho > 0 && posicoes != NULL && cores != NULL){
            for (int i = 0; i < tamanho; i++){
                pintarQuadrados(larguraTotal, alturaDoMapa, posicoes[i], cores[i], contaRodada);
            }
        }

        snprintf(contadora1Printavel, sizeof(contadora1Printavel), "%.0f", contadoraPlayer1);
        snprintf(contadora2Printavel, sizeof(contadora2Printavel), "%.0f", contadoraPlayer2);

        DrawText(contadora1Printavel, 40, alturaDoMapa + 50, 100, BLUE);
        DrawText(contadora2Printavel, 1300, alturaDoMapa + 50, 100, RED);

        DrawText("Quantos quadrados há?", 500, 800, 40, DARKGRAY);

    EndDrawing();
}
// ===================================================================================================


// ============================ FUNÇÃO RESPONSÁVEL PELO JOGO SINGLE PLAYER ===========================

void jogoSinglePlayer(Usuario jogadores[], float larguraTotal, float alturaDoMapa){
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

                if (contaRodada < 3){
                    qtdQuadrados = 10;
                    tamanho = GetRandomValue(1, qtdQuadrados);
                    qtdCoresUsadas = 1;
                    tempoDoJogo = 3;
                }
                else if (contaRodada < 7){
                    qtdQuadrados = 15;
                    tamanho = GetRandomValue(7, qtdQuadrados);
                    qtdCoresUsadas = 2;
                    tempoDoJogo = 2;
                }
                else if (contaRodada < 10) {
                    qtdQuadrados = 25;
                    tamanho = GetRandomValue(15, qtdQuadrados);
                    qtdCoresUsadas = 3;
                    tempoDoJogo = 1;
                }
                else {
                    qtdQuadrados = 25;
                    tamanho = GetRandomValue(15, qtdQuadrados);
                    qtdCoresUsadas = 3;
                    tempoDoJogo = 0.5;
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
                atualizarTelaFaseDeJogo2Singal(larguraTotal, alturaDoMapa, contadoraPlayer1, 0, tamanho, posicoes, coresSorteadas, contaRodada);
            }
            else {

                atualizarTelaFaseDeJogo2Singal(larguraTotal, alturaDoMapa, contadoraPlayer1, 0, 0, NULL, NULL, 0);

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

                    mostraAcerto(1,contadoraPlayer1,0,tamanho,jogadores, larguraTotal,alturaDoMapa,posicoes,coresSorteadas, contaRodada+1, contaRodada);

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

// ==============================================================================================

// ================= FUNÇÃO RESPONSÁVEL PELO JOGO DUAL PLAYER =====================================
void jogoDualPlayer(Usuario jogadores[], float larguraTotal, float alturaDoMapa){
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

                if (contaRodada < 3){
                    qtdQuadrados = 10;
                    tamanho = GetRandomValue(1, qtdQuadrados);
                    qtdCoresUsadas = 1;
                    tempoDoJogo = 3;
                }
                else if (contaRodada < 7){
                    qtdQuadrados = 15;
                    tamanho = GetRandomValue(7, qtdQuadrados);
                    qtdCoresUsadas = 2;
                    tempoDoJogo = 2;
                }
                else if (contaRodada < 10){
                    qtdQuadrados = 25;
                    tamanho = GetRandomValue(15, qtdQuadrados);
                    qtdCoresUsadas = 3;
                    tempoDoJogo = 1;
                }
                else {
                    qtdQuadrados = 25;
                    tamanho = GetRandomValue(15, qtdQuadrados);
                    qtdCoresUsadas = 3;
                    tempoDoJogo = 0.5;

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
                atualizarTelaFaseDeJogo2Dual(larguraTotal, alturaDoMapa, contadoraPlayer1, contadoraPlayer2, tamanho, posicoes, coresSorteadas, contaRodada);
            }
            else {

                atualizarTelaFaseDeJogo2Dual(larguraTotal, alturaDoMapa, contadoraPlayer1, contadoraPlayer2, 0, NULL, NULL, 0);

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

                    mostraAcerto(2,contadoraPlayer1,contadoraPlayer2 ,tamanho,jogadores, larguraTotal,alturaDoMapa,posicoes,coresSorteadas, contaRodada+1, contaRodada);

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

//=====================================================================

int main(){
    int escolha = 10;
    int quantidadePlayer;
    const int larguraTela = 1440;
    const int alturaTela = 900;
    float alturaDoMapa = (alturaTela*3)/4;

    InitAudioDevice();
    InitWindow(larguraTela, alturaTela, "Projeto de programação");

    click = LoadSound("assets/click.wav");
    beep  = LoadSound("assets/beep.wav");
    SetSoundVolume(beep, 0.7f);


    musicaMenu = LoadMusicStream("assets/musicaMenu.mp3");
    musicaDerrota = LoadMusicStream("assets/fimJogo.mp3");
    musicaVitoria = LoadMusicStream("assets/musicaVitoria.mp3");
    musicaJogo = LoadMusicStream("assets/musicaJogo.mp3");
    SetMusicVolume(musicaVitoria, 0.3f);
    

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
                int opcao = menuSairDoJogo();
                if(opcao == 0){
                    escolha = 0;
                }
                else {
                    escolha = -1;
                }
                break;
            case 1:
                quantidadePlayer = menuMultijogador();
                menuJogador(jogadores, quantidadePlayer);
                menuComoJogar(quantidadePlayer);

                if (quantidadePlayer == 1){
                    jogoSinglePlayer(jogadores, larguraTela, alturaDoMapa);
                }
                else{
                    jogoDualPlayer(jogadores, larguraTela, alturaDoMapa);
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
