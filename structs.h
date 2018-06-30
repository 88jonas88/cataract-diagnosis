#ifndef STRUCTS_H
#define STRUCTS_H

//Estrutura que define os pixeis RGB da imagem
typedef struct{ 
	 int r,g,b; 
}PIXEL;

//Estrutura que define tipo, largura, altura, intensidade e os pixeis RGB da imagem;
typedef struct {
	char tipo[2];
	int largura;
	int altura;
	int intensidade;
	PIXEL **pixelMap;
}PPM;

//Variáveis globais para cortar a imagem depois de aplicar a magnitude
int pa, pl; 

#endif
