Diagnóstico de Catarata		Projeto ITP/PTP		3ª Unidade

Alunos: Jonas Rocha Araujo	
		Joicy Daliane Silva Oliveira

Professora: Anna Giselle Camara Dantas Ribeiro Rodrigues 

Objetivo: Colocar em prática todo o conhecimento adquirido no semestre, através de processamento de imagem para diagnosticar catarata. 

O que foi feito:
	>>structs.h
	struct PIXEL : Estrutura que define os pixeis RGB da imagem;
	struct PPM : Estrutura que define tipo, largura, altura, intensidade e os pixeis RGB da imagem;

	>>funcoes.h
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

	>>Processamento de um conjunto de parâmetros;
	>>Criação do Makefile para facilitar a compilação do projeto;

Divisão das tarefas(sei lá):
	Jonas : Filtro de Sobel
			FIltro Gaussiano
			Calculo da magniude da imagem
			Binarização
			Transformada de Hough
			Calculo da porcetagem de catarata

	Joicy : Modularização externa e interna do programa
			Leitura e impressão de imagens PPM P3
			Filtro escala de cinza
			Diagnostico final em arquivo
	
O que não foi feito:
	Leitura e manipulação de imagens em outros formatos;
	

O que seria feito diferente:
	Clareza na implementação de modo geral.
	

Como compilar o programa:
	Utilização do comando 'make' no terminal do Linux;

Como executar o programa:
	./catarata -i <input-image> -f <input-image-format> -o <diagnose-file>

	-i <input-image> a imagem de entrada a ser processada
	-f <input-image-format> indica o formato da imagem de entrada
	-o <diagnose-file> indica o nome do arquivo texto contendo o diagnóstico

	
Referências:
	http://disciplinas.ist.utl.pt/~leic-cg.daemon/textos/livro/Formatos%20de%20Imagem.pdf
	http://www.cplusplus.com/reference/