#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//C:\Users\Eric Felix\Projeto2PI1

float controleContadoraPlayer1(float contadora1);
float controleContadoraPlayer2(float contadora2);
void atualizarTelaFaseDeJogo1(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float tempo);
void atualizarTelaFaseDeJogo2(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes);
void desenharMapa(float larguraTotal, float alturaDoMapa);
int *sortearPosEQtd(int tamanho);
void pintarQuadrados(float larguraTotal, float alturaDoMapa, int pos);

int main(){
    float larguraTotal = 1440;
    float alturaTotal = 900;
    float alturaDoMapa = (alturaTotal*3)/4;
    float turnoDeJogo = 1;
    int tamanho;
    int *posicoes = malloc(sizeof(int));
    InitWindow(larguraTotal,alturaTotal,"jogassoDaMulesta");
    InitAudioDevice();
    Music musicaFaseDeJogo = LoadMusicStream("musica1.mp3");
    SetTargetFPS(60);
    PlayMusicStream(musicaFaseDeJogo);
    float tempo = 0;
    float contadoraPlayer1 = 0;
    float contadoraPlayer2 = 0;
    int qtdQuadrados = 25;
    while(!WindowShouldClose()){
        if (turnoDeJogo == 1){
            atualizarTelaFaseDeJogo1(musicaFaseDeJogo,larguraTotal,alturaDoMapa,tempo);
            tempo += GetFrameTime();
            if (tempo >=5 ){
                turnoDeJogo = 2;
                contadoraPlayer1 = 0;
                contadoraPlayer2 = 0;
                tamanho = GetRandomValue(1,qtdQuadrados);
                posicoes = sortearPosEQtd(tamanho);
                tempo = 0;
            }
        }
        else if (turnoDeJogo == 2){
            contadoraPlayer1 = controleContadoraPlayer1(contadoraPlayer1);
            contadoraPlayer2 = controleContadoraPlayer2(contadoraPlayer2);
            atualizarTelaFaseDeJogo2(musicaFaseDeJogo,larguraTotal,alturaDoMapa,contadoraPlayer1,contadoraPlayer2,tamanho,posicoes);
            tempo += GetFrameTime();
            if (tempo>=3){
                free(posicoes);
                tempo = 0;
                turnoDeJogo = 1;
                contadoraPlayer1 = 0;
                contadoraPlayer2 = 0;
            }
        }
    }

    CloseWindow();
}

//eh soh ler man
float controleContadoraPlayer1(float contadoraPlayer1){
    if (IsKeyPressed(KEY_W)) contadoraPlayer1++;
    if (IsKeyPressed(KEY_S)) contadoraPlayer1--;
    return contadoraPlayer1;
}

float controleContadoraPlayer2(float contadoraPlayer2){
    if (IsKeyPressed(KEY_UP)) contadoraPlayer2++;
    if (IsKeyPressed(KEY_DOWN)) contadoraPlayer2--;
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

//como se fosse a main dos desenhos, e meio que eh mesmo
void atualizarTelaFaseDeJogo1(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float tempo){
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

void atualizarTelaFaseDeJogo2(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float contadoraPlayer1, float contadoraPlayer2, int tamanho,int *posicoes){
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
    for (int i = 0; i < 7; i++){
        selecionadas[i] = todas[i];
    }

    free(todas);
    return selecionadas;
}



void pintarQuadrados(float larguraTotal, float alturaDoMapa, int pos){
    if (pos == 0) DrawRectangle(0,0,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 1) DrawRectangle(larguraTotal/5,0,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 2) DrawRectangle((larguraTotal*2)/5,0,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 3) DrawRectangle((larguraTotal*3)/5,0,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 4) DrawRectangle((larguraTotal*4)/5,0,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 5) DrawRectangle(0,alturaDoMapa/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 6) DrawRectangle(larguraTotal/5,alturaDoMapa/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 7) DrawRectangle((larguraTotal*2)/5,alturaDoMapa/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 8) DrawRectangle((larguraTotal*3)/5,alturaDoMapa/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 9) DrawRectangle((larguraTotal*4)/5,alturaDoMapa/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 10) DrawRectangle(0,(alturaDoMapa*2)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 11) DrawRectangle(larguraTotal/5,(alturaDoMapa*2)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 12) DrawRectangle((larguraTotal*2)/5,(alturaDoMapa*2)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 13) DrawRectangle((larguraTotal*3)/5,(alturaDoMapa*2),larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 14) DrawRectangle((larguraTotal*4)/5,(alturaDoMapa*2),larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 15) DrawRectangle(0,(alturaDoMapa*3)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 16) DrawRectangle(larguraTotal/5,(alturaDoMapa*3)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 17) DrawRectangle((larguraTotal*2)/5,(alturaDoMapa*3)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 18) DrawRectangle((larguraTotal*3)/5,(alturaDoMapa*3)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 19) DrawRectangle((larguraTotal*4)/5,(alturaDoMapa*3)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 20) DrawRectangle(0,(alturaDoMapa*4)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 21) DrawRectangle(larguraTotal/5,(alturaDoMapa*4)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 22) DrawRectangle((larguraTotal*2)/5,(alturaDoMapa*4)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 23) DrawRectangle((larguraTotal*3)/5,(alturaDoMapa*4)/5,larguraTotal/5,alturaDoMapa/5,RED);
    else if (pos == 24) DrawRectangle((larguraTotal*4)/5,(alturaDoMapa*4)/5,larguraTotal/5,alturaDoMapa/5,RED);

}