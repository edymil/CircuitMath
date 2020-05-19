#include <stdio.h>
#include <stdlib.h>

float** sovr(c, r, mcurrent, x, m)
int c, r, m, **mcurrent;
float *x;
{
int i, j, k, n;
/*creo una matrice temp in cui copio i valori di mcurrent con tipo float*/
float temp[r][c], sum;

/*moltiplico la matrice temp per i valori delle correnti di maglia in ogni singola maglia*/
for(k=1;k<c-1;k=k+r+1)
	for(i=0; i<=r-1; i++)
		for(j=0; j<=r-1; j++)
			if(mcurrent[i][j+k]==1)
				temp[i][j+k]=x[k/r];
			else if (mcurrent[i][j+k]==-1)
				temp[i][j+k]=-x[k/r];
			else
				temp[i][j+k]=0;

/*nella parte riguardante la prima maglia in mcurrent sommo i valori riguardanti le altre maglie*/
for(i=0; i<=r-1; i++)
	for(j=1; j<=r; j++)
		for(k=1; k<=m-1; k++)
			temp[i][j]=temp[i][j]+temp[i][j+k*(r+1)];

/*creo current che è la matrice delle correnti, lo alloco e la copio dalla prima parte di mcurrent*/
float **current;

current = (float**) malloc(r*r*sizeof(float));
if (current == NULL) {
	printf("Non ho abbastanza memoria perl'allocazione\n");
	exit(1); 
}
for (i=0; i<r; i++){
	current[i] = (float*) malloc(r*sizeof(float));
	if (current == NULL) {
		printf("Non ho abbastanza memoria perl'allocazione\n");
		exit(1); 
	}
}

for(i=0; i<=r-1; i++)
	for(j=0; j<=r-1; j++)
		current[i][j]=temp[i][j+1];


/*verifico che i risultati siano corretti applicando la legge dei nodi (e "degli angoli")*/
/*si nota che la legge dei nodi è verificata se la somma dei termini su ogni riga è nullo (precisione sulla quatra cifra decimale)*/
k=0;
for(i=0; i<=r-1; i++){
	sum=0;
	for(j=0; j<=r-1; j++){
		sum=sum+current[i][j];
	}
	if(sum>=0.0001 && sum<=-0.0001){
		printf("\nC'è stato un errore nel calcolo delle correnti!\n");
		k=1;
		break;
	}
}
if(k==0)
	printf("\nI valori ottenuti per le correnti rispettano la legge dei nodi!\n");

/*ora per avere nel link giusto il valore della corrente devo togliere tutti i valori negativi da current*/
for(i=0; i<=r-1; i++)
	for(j=0; j<=r-1; j++)
		if(current[i][j]<=0)
			current[i][j]=0;


/*STAMPA FILE ESTERNO*/
FILE *fp;
fp = fopen ("current.dat","w");

/*Scrivo su file*/
fprintf(fp,"Nella matrice delle correnti sono salvati i valori della corrente tra i vari punti del circuito.\n");
fprintf(fp,"\nSe la corrente va dal punto A al punto B nella matrice è individuata nella casella con indici AB per riga e colonna, mentre la casella BA è vuota.\n");
fprintf(fp,"Ecco la matrice:\n");

for(i=0; i<=r-1; i++){
	for(j=0; j<r-1; j++)
		fprintf(fp, "%f\t", current[i][j]);
	fprintf(fp, "%f\n", current[i][r-1]);
}	

if (!fp)
	printf ("\nC'è stato un errore nella scrittura del file 'corrent.dat'!\n");
else
	printf("\nIl file current.dat è stato scritto correttamente!\n");


return current;

					
	
}


