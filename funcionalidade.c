#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


float controleContadoraPlayer1(float contadora1);
float controleContadoraPlayer2(float contadora2);
void atualizarTelaFaseDeJogo1(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float tempo);
void atualizarTelaFaseDeJogo2(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float contadoraPlayer1, float contadoraPlayer2);
void desenharMapa(float larguraTotal, float alturaDoMapa);
int main(){
    float larguraTotal = 1440.0;
    float alturaTotal = 900.0;
    float alturaDoMapa = (alturaTotal*3)/4;
    float turnoDeJogo = 1;
    InitWindow(larguraTotal,alturaTotal,"jogassoDaMulesta");
    InitAudioDevice();
    Music musicaFaseDeJogo = LoadMusicStream("musica1.mp3");
    SetTargetFPS(60);
    PlayMusicStream(musicaFaseDeJogo);
    float tempo = 0;
    float contadoraPlayer1 = 0;
    float contadoraPlayer2 = 0;
    while(!WindowShouldClose()){
        if (turnoDeJogo == 1){
            atualizarTelaFaseDeJogo1(musicaFaseDeJogo,larguraTotal,alturaDoMapa,tempo);
            tempo += GetFrameTime();
            if (tempo >=5 ){
                tempo = 0;
                turnoDeJogo = 2;
                contadoraPlayer1 = 0;
                contadoraPlayer2 = 0;
            }
        }
        else if (turnoDeJogo == 2){
            contadoraPlayer1 = controleContadoraPlayer1(contadoraPlayer1);
            contadoraPlayer2 = controleContadoraPlayer2(contadoraPlayer2);
            atualizarTelaFaseDeJogo2(musicaFaseDeJogo,larguraTotal,alturaDoMapa,contadoraPlayer1,contadoraPlayer2);
            tempo += GetFrameTime();
            if (tempo>=5){
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
        DrawText(tempoPrintavel,(larguraTotal/2)-45,alturaDoMapa,100,RED);
        DrawText(zero,40,alturaDoMapa+50,100,BLUE);
        DrawText(zero,larguraTotal-90,alturaDoMapa+50,100,GREEN);
    EndDrawing();
}

void atualizarTelaFaseDeJogo2(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float contadoraPlayer1, float contadoraPlayer2){
    UpdateMusicStream(musicaFaseDeJogo);

    BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(0,0,larguraTotal,alturaDoMapa,WHITE);
        desenharMapa(larguraTotal,alturaDoMapa);
        char contadora1Printavel[4];
        char contadora2Printavel[4];
        snprintf(contadora1Printavel,sizeof(contadora1Printavel),"%.0f",contadoraPlayer1);
        snprintf(contadora2Printavel,sizeof(contadora2Printavel),"%.0f",contadoraPlayer2);
        DrawText(contadora1Printavel,40,alturaDoMapa+50,100,BLUE);
        DrawText(contadora2Printavel,larguraTotal-90,alturaDoMapa+50,100,GREEN);
    EndDrawing();
}