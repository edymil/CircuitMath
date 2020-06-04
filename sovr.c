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
	printf("Not enough memory for allocation\n");
	exit(1); 
}
for (i=0; i<r; i++){
	current[i] = (float*) malloc(r*sizeof(float));
	if (current == NULL) {
		printf("Not enough memory for allocation\n");
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
		printf("\nThere was an error while calculating currents\n");
		k=1;
		break;
	}
}
if(k==0)
	printf("\nComputed currents fit the law of nodes\n");

/*ora per avere nel link giusto il valore della corrente devo togliere tutti i valori negativi da current*/
for(i=0; i<=r-1; i++)
	for(j=0; j<=r-1; j++)
		if(current[i][j]<=0)
			current[i][j]=0;


/*STAMPA FILE ESTERNO*/
FILE *fp;
fp = fopen ("current.dat","w");

/*Scrivo su file*/
fprintf(fp,"The current matrix stores the currents between nodes in the circuit.\n");
fprintf(fp,"\nEntry with (row, column) indexes (A, B) identifies a current flowing from node A to node B, while the corresponding (B, A) entry is empty.\n");
fprintf(fp,"Current matrix:\n");

for(i=0; i<=r-1; i++){
	for(j=0; j<r-1; j++)
		fprintf(fp, "%f\t", current[i][j]);
	fprintf(fp, "%f\n", current[i][r-1]);
}	

if (!fp)
	printf ("\nError in writing file current.dat\n");
else
	printf("\nFile current.dat has been written\n");


return current;

					
	
}


