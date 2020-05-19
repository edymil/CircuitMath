#include <stdio.h>
#include <stdlib.h>
/*#include "nr.h"*/
/*#include "nrutil.h"*/

#define MAXSTR 80
#define ERROR 1.0e-3

int** algorithm(int r, int** circuit, int* m);
void verso(int r,int c, int** circuit);
float** creatematrix(int m, int r, int** mcurrent, float** res, int c);
void createb(float* b, int r, int c, int m, float** volt, int** mcurrent);
void mprove(float** a,float** alud, int n, int* indx, float* b, float* x);
float** sovr(int c, int r, int** mcurrent, float* x, int m);
void tension(int r, float** res, float** current, float** volt);
void lubksb(float** a,int n,int* indx,float* b);
void ludcmp(float** a,int n,int* indx, float* d);

main()
{
/*RAPPRESENTAZIONE DEL CIRCUITO NEL PROGRAMMA*/
/*creazione vettore*/
int i, j, r, *z, y;
char dummy[MAXSTR];
FILE *fp;

int** circuit;

if ((fp = fopen("MatriceGiulio.dat","r")) == NULL)
printf("\nNon è stato possbile aprire il file dati!\n");
else
printf("\nFile dati caricato correttamente!\n");
	
fgets(dummy,MAXSTR,fp);    /*legge la prima riga testuale*/
fgets(dummy,MAXSTR,fp);    /*legge la seconda riga testuale*/
fscanf(fp,"%d ",&r);       /*legge la terza riga (dimensioni matrice)*/ 

circuit = (int**)malloc(r*sizeof(int*));
if (circuit == NULL) {
	printf("Non ho abbastanza memoria perl'allocazione\n");
	exit(1); 
}
for(i=0; i<r; i++){
	circuit[i] = (int*)malloc(r*sizeof(int));
	if (circuit == NULL) {
		printf("Non ho abbastanza memoria perl'allocazione\n");
		exit(1); 
	}
}


fgets(dummy,MAXSTR,fp);          /*legge la quarta riga testuale*/ 
for(i=0; i<=r-1; i++)            /*mi scansiona i valori numerici e li assegna a circuit*/
	for(j=0; j<=r-1; j++)
		fscanf(fp,"%d ",&circuit[i][j]);


/*RICERCA MAGLIE INDIPENDENTI: scritta function algorithm*/
int **mcurrent, m, c;
mcurrent = algorithm(r, circuit, &m);

c = r*m+m+1;

/*ora creo indm come matrice e la riempio con inmdp*/
/*int mcurrent[r][c];
for(i=0; i<=r-1; i++)            
	for(j=0; j<=c-1; j++)
		mcurrent[i][j]=*(*(mcurrentp+i)+j);*/
/*printf("\n Checkpoint before! \n");*/
/*VERSO DELLE CORRENTI*/
/*creo current come maglie indipendenti solo che lati con  verso (valori + o meno 1)*/
verso(r, c, mcurrent); /*uso la function per trovare il verso*/
/*printf("\n Checkpoint after! \n");*/
/*for(i=0; i<=r-1; i++){
	for(j=1; j<r-1+1; j++)
		printf("%d\t", mcurrent[i][j]);
	printf("%d\n", mcurrent[i][r-1+1]);
}*/
/*creazione matrice resistenze */
float** res;

res = (float**)malloc(r*sizeof(float*));
if (res == NULL) {
	printf("Non ho abbastanza memoria perl'allocazione\n");
	exit(1); 
}
for(i=0; i<r; i++){
	res[i] = (float*)malloc(r*sizeof(float));
	if (res == NULL) {
		printf("Non ho abbastanza memoria perl'allocazione\n");
		exit(1); 
	}
}

fgets(dummy,MAXSTR,fp);          /*legge la riga testuale "matrice delle resistenze"*/ 
for(i=0; i<=r-1; i++)            /*mi scansiona i valori numerici e li assegna a res*/
	for(j=0; j<=r-1; j++)
		fscanf(fp,"%f ",&res[i][j]);

/* creo la matrice A per il metodo delle maglie*/
float **a;
a = creatematrix(m, r, mcurrent, res, c);	
/*stampa totale*/
/*for(i=0; i<=r-1; i++){
	for(j=0; j<m-1; j++)
		printf("%f", res[i][j]);
	printf("%f\n", res[i][r-1]);
}*/

/*la copio col nome d, sarà quella che sarà scomposta*/
float **d;
d = creatematrix(m, r, mcurrent, res, c);

/*inizalizzo ciò che mi serve per la prossima function*/
float s;
int* perm;
perm = (int*)malloc(m*sizeof(int));
if (perm == NULL) {
	printf("Non ho abbastanza memoria perl'allocazione\n");
	exit(1); 
}

/* decomposizione */
ludcmp(d,m-1,perm,&s);

/*fine programma*/
/*mi creo la matrice del voltaggio e la inizializzo*/
float** volt;

volt = (float**)malloc(r*sizeof(float*));
if (volt == NULL) {
	printf("Non ho abbastanza memoria perl'allocazione\n");
	exit(1); 
}
for(i=0; i<r; i++){
	volt[i] = (float*)malloc(r*sizeof(float));
	if (volt == NULL) {
		printf("Non ho abbastanza memoria perl'allocazione\n");
		exit(1); 
	}
}

fgets(dummy,MAXSTR,fp);       /*legge la riga 'matrice del voltaggio'*/
for(i=0; i<=r-1; i++)            /*mi scansiona i valori numerici e li assegna a res*/
	for(j=0; j<=r-1; j++)
		fscanf(fp,"%f ",&volt[i][j]);

/*inizializzo per ax=b vettore b con voltaggio in ogni maglia*/
float *b;
b = (float*)malloc(m*sizeof(float));
if (b == NULL) {
	printf("Non ho abbastanza memoria perl'allocazione\n");
	exit(1); 
}
createb(b, r, c, m, volt, mcurrent);

/*for(i=0; i<=m-1;i++)
printf("%f\n", b[i]);*/


/*mi salvo b per le prossime volte*/
float *x;
x = (float*)malloc(m*sizeof(float));
if (x == NULL) {
	printf("Non ho abbastanza memoria perl'allocazione\n");
	exit(1); 
}
createb(x, r, c, m, volt, mcurrent);

/* risolvo le equazioni */
lubksb(d,m-1,perm,x);
/*for(i=0;i<=m-1;i++)
printf("%f\n", x[i]);*/
/*verifico che i risultati siano corretti*/
float *p;
p = (float*)malloc(m*sizeof(float));
if (p == NULL) {
	printf("Non ho abbastanza memoria perl'allocazione\n");
	exit(1); 
}
j=0;
while(j==0){
	for (i=0;i<=m-1;i++) {
		for (j=0;j<=m-1;j++)
			p[i] += (a[i][j]*x[j]);
		if (p[i]-b[i]<=ERROR && p[i]-b[i]>=-ERROR){
			printf("\nIl %do risultato relativo al sistema di equazioni del metodo delle maglie è corretto entro la terza cifra decimale.\n", i+1);
			j=1;
		}
		else
			mprove(a,d,m-1,perm,b,x);
	
	}
}
/*for(i=0; i<=m-1; i++)
printf("%f\n", x[i]);*/
/*PRINCIPIO DI SOVRAPPOSIZIONE*/
float** current;

current= sovr(c, r, mcurrent, x, m);

/*TENSIONI*/
tension(r, res, current, volt);





/*stampa totale*/
/*for(i=0; i<=m-1; i++){
	for(j=0; j<m-1; j++)
		printf("%f", a[i][j]);
	printf("%f\n", a[i][m-1]);
}*/

free(circuit);
free(mcurrent);
free(res);
free(a);
free(d);
free(perm);
free(volt);
free(b);
free(x);
free(p);
free(current);


/*fine programma*/
}

					


