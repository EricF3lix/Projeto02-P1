#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED

#include "raylib.h"


typedef struct usuario {
    char nome[20];
    int vida;
    int pontuacao;
} Usuario;


float controleContadoraPlayer1(float contadora1);
float controleContadoraPlayer2(float contadora2);
void atualizarTelaFaseDeJogo1Dual(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float tempo);
void atualizarTelaFaseDeJogo2Dual(Music musicaFaseDeJogo,float larguraTotal, float alturaDoMapa,float contadoraPlayer1, float contadoraPlayer2, int tamanho, int *posicoes);
void desenharMapa(float larguraTotal, float alturaDoMapa);
int *sortearPosEQtd(int tamanho);
void pintarQuadrados(float larguraTotal, float alturaDoMapa, int pos);
void jogoSinglePlayer(Usuario jogadores[], Music musicaFaseDeJogo, float larguraTotal, float alturaDoMapa);

#endif // JOGO_H_INCLUDED

