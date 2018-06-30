#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// esse codigo da certo com a cata2, normal1 no cata1 da 67 r
typedef struct Pixel{  //Estrutura que define os pixeis RGB da imagem
	 int r,g,b; //
}PIXEL;

typedef struct {
	char tipo[2];
	int largura;
	int altura;
	int intensidade;
	PIXEL **pixelMap;
}PPM;

int pa, pl;

int escalaDeCinza(int re, int ge,  int be){
	int pixelCinza;
    pixelCinza = (re + ge + be) / 3;

    return pixelCinza;
}

int contaSobel(int g1, int g2){

	float somaG1_G2 = pow(g1,2) + pow(g2,2);
	float raizDaSoma = sqrt(somaG1_G2);

	return (int)raizDaSoma;
}

void loadPPM(PPM *imagem) {
	char nomeDaImagem[10];
	scanf("%s", nomeDaImagem);

	FILE * imgEntrada = fopen(nomeDaImagem, "r"); //Abre a imagem para leitura;

	if(imgEntrada == NULL){
        printf("Não foi possivel econtrar a imagem.\n");
        exit(1); // alterar isso aqui
    }

	//LEITURA DO CABECALHO
	fscanf(imgEntrada,"%s", imagem->tipo); //leitura do formato da imagem
	fscanf(imgEntrada,"%d %d %d", &imagem->largura, &imagem->altura, &imagem->intensidade); //leitura daaltura, largura e tonalidade da imagem	


	//LEITURA DOS PIXELS DA IMAGEM DE ENTRADA	
	imagem->pixelMap = (PIXEL **) malloc(imagem->altura * sizeof(PIXEL*)); //aloca dinamicamente a altura da imagem que esta sendo lida
 	for (int i = 0; i < imagem->altura; ++i)
	{	
		//aloca dinamicamente a largura da imagem que esta sendo lida
		imagem->pixelMap[i] = (PIXEL *) malloc(imagem->largura * sizeof(PIXEL));
		
		for (int j = 0; j < imagem->largura; ++j)
		{
			fscanf(imgEntrada, "%d %d %d", &imagem->pixelMap[i][j].r, &imagem->pixelMap[i][j].g, &imagem->pixelMap[i][j].b);  
		}
	}

    fclose(imgEntrada);
}

int ** loadCinza(PPM *imagem) {
	
	// CRIA UMA NOVA MATRIZ DE INTEIROS PARA ARMAZENAR OS VALORES EM ESCALA DE CINZA
	int**imgCinza =  (int**)malloc(imagem->altura * sizeof(int*)); //Aloca um Vetor de Ponteiros
 
	for (int i = 0; i < imagem->altura; ++i)
	{	
		imgCinza[i] = (int*) malloc(imagem->largura * sizeof(int)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
		
		for (int j = 0; j < imagem->largura; ++j)
		{
			imgCinza[i][j]  =  escalaDeCinza(imagem->pixelMap[i][j].r, imagem->pixelMap[i][j].g, imagem->pixelMap[i][j].b);	
		}
	}
	return imgCinza;
}

void filtroGaussiano(PPM *imagem, int ** imgCinza) {
		int mask[25] =	{2, 4, 5, 4, 2,
					 4, 9, 12, 9 ,4,
					 5, 12, 15, 12, 5,
					 4, 9, 12, 9, 4,
					 2, 4, 5, 4, 2};

	int somatorio;
	int x;

	for (int i = 2; i < imagem->altura-2; ++i)
	{
		for (int j = 2; j < imagem->largura-2; ++j)
		{
			
			somatorio = 0;
			x = 0;

			for (int k = i-2; k < i+3; ++k)
			{
				for (int l = j-2; l < j+3; ++l)
				{
					somatorio += imgCinza[k][l] *  mask[x];				
					x++;
				}
			}

			imgCinza[i][j] = somatorio / 159.0;
			
		}

	}
}

int** filtroSobel(PPM *imagem, int ** imgCinza) {
	// CRIA UMA NOVA MATRIZ DE INTEIROS PARA ARMAZENAR OS VALORES DE SOBEL

	int **SOBEL =  (int**)malloc(imagem->altura * sizeof(int*)); //Aloca um Vetor de Ponteiros
 
	for (int i = 0; i < imagem->altura; ++i)
	{	
		SOBEL[i] = (int*) malloc(imagem->largura * sizeof(int)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
	} 

 	int g1;
 	int g2;

 	// mascaras de Sobel
	int maskX[9] = {1, 0, -1,   2, 0, -2,   1,  0, -1};

	int maskY[9] = {1, 2,  1,   0, 0,  0,  -1, -2, -1};
	

	for (int i = 1; i < imagem->altura-1; ++i)
	{
		for (int j = 1; j < imagem->largura-1; ++j)
		{

			//calcula g1
			g1=0;
			int m = 0;

			for (int k = i-1; k < i+2; ++k)
			{
				for (int l = j-1; l < j+2 ; ++l)
				{
					g1 += imgCinza[k][l] * maskX[m];
					m++;
				}
			}
		
			//calcula g2
			g2=0;
			int n = 0;

			for (int k = i-1; k < i+2; ++k)
			{
				for (int l = j-1; l < j+2 ; ++l)
				{
					g2 += imgCinza[k][l] * maskY[n];
				 	n++;
				}
			}


			// atribue o valor final do pixel a nova matriz de sobel
			SOBEL[i][j] = 	20   *  contaSobel(g1,g2);
			
			if (SOBEL[i][j] > 255)
			{
				SOBEL[i][j] = 255;
			}
		}
	}

	return SOBEL;
}

int** magnitude(PPM *imagem, int ** SOBEL) {
	int ** mag =  (int**)malloc(imagem->altura * sizeof(int*)); //Aloca um Vetor de Ponteiros
 
	for (int i = 0; i < imagem->altura; ++i)
	{	
		mag[i] = (int*) malloc(imagem->largura * sizeof(int)); //Aloca um Vetor de Inteiros para cada posição do Vetor de Ponteiros.
	} 

	//	calcula a porcetagens que deve ser cortado nas bordas 19% da altura e 26% da largura, considerando o conjunto de 4 imagens desse projeto
	pa = imagem->altura * 0.19;
	pl = imagem->largura * 0.26;

	for (int i = pa; i <imagem->altura-pa; ++i)
	{
		for (int j = pl; j < imagem->largura-pl; ++j)
		{
	
			if (SOBEL[i][j] < SOBEL[i-1][j] || SOBEL[i][j] < SOBEL[i+1][j])
			{
				mag[i][j] = 0;
			}
			else{
				mag[i][j] = SOBEL[i][j];
			}
			/////define 255
			if (mag[i][j] > 255 )
			{
				mag[i][j] = 255;
			}
			//mag[i][j] = SOBEL[i][j];
		}
	}



	// calculo da media dos valores da matriz para fazer a binzarizacao//  
	int media = 0;
	int cont=0;

	for (int i = pa; i < imagem->altura-pa; ++i)
	{
		for (int j = pl; j < imagem->largura-pl; ++j)
		{
			
			if (mag[i][j] > 0)
			{
				media += mag[i][j];
				cont++;
			}

		}
	}

	media /= cont;
	printf("media: %d\n",  media );

	//BINARIZACAO usando o valor da media da matriz **mag para fazer as compacoes
	for (int i = pa; i < imagem->altura-pa; ++i)
	{
		for (int j = pl; j < imagem->largura-pl; ++j)
		{
			
			if (mag[i][j] >= media)
			{
				mag[i][j] = 255;
			}
			else{
				mag[i][j] = 0;
			}

		}
	}
	return mag;
}

void hough(PPM *imagem, int ** mag) {
	// alocacao de uma matriz 3d
	
	 int ***matriz3d = (int***) malloc(imagem->altura*sizeof(int**));

	  	for (int i=0; i<imagem->altura; i++) {
		    
		    matriz3d[i] = (int**) malloc(imagem->largura*sizeof(int*));

		    	for (int j=0; j<imagem->largura; j++) {
		    
			   		matriz3d[i][j] = (int*) malloc(300*sizeof(int));
		    	
		     		for (int k=0; k<300; k++) {
		    
		        		matriz3d[i][j][k] = 0;
		        		//printf("%d\n", k );
		        		//printf("%d\n", matriz3d[i][j][k] );
		      		}
		   		}
	 	}


	float PI =3.14;
	int a, b;
	//ideal: 70 - 200
	int rmin = 140;
	int rmax = 150;
		
	
		for (int r = rmin; r < rmax; ++r)
		{
			for (int x = pa; x < imagem->altura-pa; ++x)
			{
				for (int y = pl; y < imagem->largura-pl; ++y)
				{	
					if (mag[x][y] > 0){
						
						for (int t = 0; t < 361; ++t)
						{	
							a = x - (r * cos(t * PI / 180));
							
							b = y - (r * sin(t * PI / 180));

							
							if (a< 1 || a > imagem->altura-1 || b < 1 || b > imagem->largura-1)
							{
								break;									
							}
								
							matriz3d[a][b][r]++;
					
						}
					}
				}
			}
			printf("Aplicando o raio %d\n", r );
		} 	


		// variaves que armazem o ponto central(m,n) do circulo e o seu raio (raio)
		int m,n;
		int raio;		
	  	
	  	//essava varial seve para comparar qual a o pixel que recebeu mais votos
		int maior = 0;
		
		// econtra o maior circulo        			
	  	for (int k = rmin; k < rmax; k++)
	  	{
	  		for (int i = pa; i < imagem->altura-pa; ++i)
	  		{
	  			for (int j = pl; j < imagem->largura-pl; ++j)
	  			{
	  				if (matriz3d[i][j][k] > maior && matriz3d[i][j][k] <= 400 )
	  				{	
	  		
	  					maior = matriz3d[i][j][k];
	  					m = i;
	  					n = j;
	  					raio = k;
	  					printf("raio %d maior%d\n", raio, maior );
	  				}
	  			}
	  		}
	  	}	

	  	
	 

	 	// segmenta os valores da pupipla na imagem colorida

		int compara; // variavel para comparar se o pixel do momento esta na area da pupila
			
		int contPixels = 0; //conta os pixels que tem na pupila
 
		for (int i = 0; i < imagem->altura; ++i)
	  	{
	  		for (int j = 0; j < imagem->largura; ++j)
	  		{
	  			
	  			int a = i-m;
	  			int b = j-n;

	  			compara = sqrt(pow(a,2) + pow(b,2)); // formula do circulo
	  			
	  			if (compara <= raio)
	  			{	
	  				contPixels++;
	  			}
	  			else{
	  				// zera o que nao faz parte do circulo
	  				imagem->pixelMap[i][j].r = 0;
	  				imagem->pixelMap[i][j].g = 0;
	  				imagem->pixelMap[i][j].b = 0;
	  			}
	  			
	  		}
	  	}
	  	
	  	// valores do rgb de um pixel com catarata
	  	int red 	= 140;		
		int green 	= 145;
		int blue 	= 150;

		int range = 60; //intervalo dos pixels com catarata

		int contCatarata = 0;

		for (int i = 0; i < imagem->altura; ++i)
	  	{
	  		for (int j = 0; j < imagem->largura; ++j)
	  		{
	  			// esse if acessa o valor do pixel que que tem cor, ou seja, do circulo
	  			if (imagem->pixelMap[i][j].r !=0 )
	  			{	//esse if verifica se o pixel do momento atende os vallores de um pixel com catarata
	  				if ((imagem->pixelMap[i][j].r <= red + range 		&&	 imagem->pixelMap[i][j].r >= red - range)			&&
	  					(imagem->pixelMap[i][j].g <= green + range 	&&	 imagem->pixelMap[i][j].g >= green - range)		&&
	  					(imagem->pixelMap[i][j].b <= blue + range 	&&	 imagem->pixelMap[i][j].b >= blue - range) 		)
	  			{
	  				contCatarata++;

	  				imagem->pixelMap[i][j].r = 255; //pinta os pixels com catarata de vermelho
	  				imagem->pixelMap[i][j].g = 0;
	  				imagem->pixelMap[i][j].b = 0;
	  			}		
	  			
	  			}
	  		}
	  	}


	 	//calculo do dianostico
	  	int porcetagemFinal = (contCatarata * 100) / contPixels;
	  	
	  	FILE *arqDiagnostico;
	
		arqDiagnostico = fopen("dianostico.txt", "w");
		if(arqDiagnostico == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	
	  	// tem que colocar isso aqui em um arquivo
	  	if (porcetagemFinal < 60)
	  	{
	  		fprintf(arqDiagnostico,"Foi encontrado apenas %d de comprometimento da pupila, logo nao possui catarata :D\n", porcetagemFinal);
	  	}
	  	else{
	  		fprintf(arqDiagnostico,"A pupila está %d compometida, logo possui catarata. =/\n", porcetagemFinal);
	  	}
	fclose(arqDiagnostico);
}

int main(int argc, char const *argv[])
{

	FILE *imgSaida;
	PPM imagem;
	int ** imgCinza; 
	int **SOBEL;
	int ** mag;

	loadPPM(&imagem);  	
	imgCinza = loadCinza(&imagem);


	//FILTRO GAUSSIANO
	filtroGaussiano(&imagem, imgCinza);

	//FILTRO DE SOBEL
	SOBEL = filtroSobel(&imagem, imgCinza);
	


	//CALCULO DA MAGNITUDE
	mag = magnitude(&imagem, SOBEL);
	


	//Transformada de Hough e calculo do diagnostico
	hough(&imagem, mag);
	
	 	
	
	
	imgSaida = fopen("ts3.ppm", "w");
	
	// impressao do cabecalho da imagem
    fprintf(imgSaida, "%s\n%d %d\n%d\n", imagem.tipo, imagem.largura, imagem.altura, imagem.intensidade); 

    //impressao da matriz da imagems
	for(int i = 0; i < imagem.altura; i++){

		for(int j = 0; j < imagem.largura; j++){
			// esse monte de printf eh foi pra ficar vendo o que tava acontecendo com os filtros
		 	//fprintf(imgSaida, "%d %d %d\n", jon[i][j], jon[i][j], jon[i][j]);
		 	//fprintf(imgSaida, "%d %d %d\n", mag[i][j], mag[i][j], mag[i][j]);
		 	//fprintf(imgSaida, "%d %d %d\n", SOBEL[i][j], SOBEL[i][j], SOBEL[i][j]);
			fprintf(imgSaida, "%d %d %d\n", imagem.pixelMap[i][j].r, imagem.pixelMap[i][j].g, imagem.pixelMap[i][j].b );
		}

	}
	
	printf("Malfeito feito \n");

	return 0;
}