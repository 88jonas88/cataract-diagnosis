#ifndef CATARATA_H
#define CATARATA_H
#include "structs.h"
#include <stdio.h>

//funcao para deixar a imagem em tons de cinza
int escalaDeCinza(int re, int ge,  int be);

//funcao para ler a imagem ppm
void loadPPM(PPM *imagem, char * nomeDaImagem) ;

//funcao para 
int ** loadCinza(PPM *imagem);

//funcao para aplicacao do filtro de Gauss
void filtroGaussiano(PPM *imagem, int ** imgCinza);

//funcao para fazer a conta de Sobel
int contaSobel(int g1, int g2);

//funcao para aplicacao do filtro de Sobel
int** filtroSobel(PPM *imagem, int ** imgCinza);

//funcao para 
int** magnitude(PPM *imagem, int ** SOBEL);

//funcao para aplicacao da transformada de hough
void hough(PPM *imagem, int ** mag, char *nomeDoArquivo);

#endif