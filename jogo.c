#include "jogo.h"



float controleContadoraPlayer1(float contadoraPlayer1){
    if (IsKeyPressed(KEY_W)) contadoraPlayer1++;
    if (IsKeyPressed(KEY_S)) contadoraPlayer1--;

    
    if (contadoraPlayer1 < 0) contadoraPlayer1 = 0;
    if (contadoraPlayer1 > 25) contadoraPlayer1 = 25;

    return contadoraPlayer1;
}

float controleContadoraPlayer2(float contadoraPlayer2){
    if (IsKeyPressed(KEY_UP)) contadoraPlayer2++;
    if (IsKeyPressed(KEY_DOWN)) contadoraPlayer2--;

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
            }
        } else if(turnoDeJogo == 2){
            contadoraPlayer1 = controleContadoraPlayer1(contadoraPlayer1);
            atualizarTelaFaseDeJogo2Singal(musicaFaseDeJogo, larguraTotal, alturaDoMapa, contadoraPlayer1, 0, tamanho, posicoes);
            tempo += GetFrameTime();
            if(tempo >= 5){
                int diferenca1 = (int)(contadoraPlayer1 + 0.5) - tamanho;
                if(diferenca1 == 0){
                    jogadores[0].pontuacao += 150; 
                } else if(diferenca1 == 1 || diferenca1 == -1){
                    jogadores[0].pontuacao += 50;
                    jogadores[0].vida--;
                } else {
                    jogadores[0].vida--;
                }
                free(posicoes);
                posicoes = NULL;
                tempo = 0;
                turnoDeJogo = 1;
            }
        }
    }

}
