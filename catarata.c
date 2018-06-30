#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>

#include "funcoes.h"
#include "structs.h"


int main(int argc, char *argv[]) {
  int i;
  char *argv_imagem, *argv_output;
  //verificando se os argumentos passados pelo usuário são válidos
	if ( (argc == 7)||(argc == 6) ){
    	if( !(strcmp(argv[4], "ppm") == 0 || ( argc == 2 && strcmp(argv[4], "ppm" ) == 0) ) ){
      		printf("\nFormato de imagem digitado não suportado.\nEsperado: ppm\nDuvidas? Consulte o README.\n\n");
      		exit(1);
    	}
    	if ( strcmp(argv[1], "-i") == 0 && strcmp(argv[4], "ppm") == 0 ){
      	argv_imagem = argv[2];
      	argv_output = argv[6];
      	FILE *imgSaida;
    
        PPM imagem;
        int ** imgCinza; 
        int **SOBEL;
        int ** mag;

        loadPPM(&imagem, argv_imagem);   
        imgCinza = loadCinza(&imagem);


        //FILTRO GAUSSIANO
        filtroGaussiano(&imagem, imgCinza);

        //FILTRO DE SOBEL
        SOBEL = filtroSobel(&imagem, imgCinza);
  
        //CALCULO DA MAGNITUDE
        mag = magnitude(&imagem, SOBEL);
    
        //Transformada de Hough e calculo do diagnostico
        hough(&imagem, mag, argv_output);
        
      } 
	
  } else {
	    printf("\nERRO -> Parametro(s) informado(s) nao confere(m) com o(s) esperado(s).\n");
	    printf("Digitados:\n");
	    for(i = 1; i < argc; i++){
	      printf(" %s ", argv[i]);
	    }
	    printf("\nEsperando:\n");
	    printf("./catarata -i <input-image> -f <input-image-format> -o <diagnose-file>\n");
	    printf("Consulte o arquivo README para mais informacoes e exemplos.\n\n");
	    exit(1);
	 }

  return 0;
}