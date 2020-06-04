#include <stdio.h>
#include <stdlib.h>

int** algorithm(r, circuit, m)

int r;
int **circuit;
int *m;

{
int done, b[r][r], x[r], bc[r][r], k, n, c, nu, i, j, f;
/*BLOCK 1*/
/*inizalizzo b a zero*/
for(i=0; i<=r-1; i++)            
	for(j=0; j<=r-1; j++)
		b[i][j]=0;

for(i=0; i<=r-1; i++) {   
	done=0;       
	for(j=i; j<=r-1; j++)
		/*printf("%d\t%d\t%d\n", i, j, done);*/
		if(circuit[i][j]==1 && done==0){
			b[i][j]=1;
			b[j][i]=1;
			done=1;
		}
}

/*BLOCK 2*/

for(i=0; i<=r-1; i++)           /*bc è la copia di b che verrà modificata (mi conservo b)*/     
	for(j=0; j<=r-1; j++)
		bc[i][j]=b[i][j];

for(i=0; i<=r-1; i++) /*inizializzo x a zero*/
	x[i]=0;
/*compatto le righe per vedere i singoli filoni (es.1-2-3-7-6-5-4 o 1-2-3-7-9-8 o 10-11)*/

for(i=0;i<=r-1; i++)
	for(j=0;j<=r-1; j++)
		if(bc[i][j]==1) 
			for(k=0; k<=r-1; k++)
				if(bc[j][k]==1){
					bc[i][k]=bc[j][k]; /*compatto le righe*/
					bc[j][k]=-bc[j][k];/*segno con -1*/
					
				}
/*ora compatto insieme i filoni che fanno parte di un unico cordone (primi due sopra)*/
n=0;
for(j=0;j<=r-1; j++){          /*mi fisso la colonna e vedo le varie righe*/
	for(i=0;i<=r-1; i++){
		if(bc[i][j]==1 && n==0){
			k=i;
			n=1;
		}
		else if(bc[i][j]==1 && n!=0){
			for(n=0; n<=r-1; n++)
				if(bc[i][n]==1){
					bc[k][n]=bc[i][n]; /*compatto le righe*/
					bc[i][n]=-bc[i][n];/*segno con -1*/
				}
		}
	}
n=0;
}

/*conto quante righe ha la matrice c*/
n=0;
for(i=0;i<=r-1; i++)        
	for(j=0;j<=r-1; j++)
		if(bc[i][j]==1){
			n++;
			break;
		}

/*creo la matrice d e la riempio*/
int d[n][r];
k=0;
for(i=0;i<=r-1; i++)        
	for(j=0;j<=r-1; j++)
		if(bc[i][j]==1){
			for(f=0; f<=r-1; f++)	
				d[k][f]=bc[i][f];
			k++;
			break;
		}

/*controllo correttezza*/
for(j=0;j<=r-1; j++)        
	for(i=0;i<=r-1; i++)
		if(d[i][j]==1)
			continue;
		if(d[i][j]==0 && j==n-1)
			printf("\nError in block 2 while searching for independent meshes\n");


/*BLOCK 3*/
f=0;
for(i=0;i<=n-1; i++)        
	for(j=0;j<=r-1; j++)
		if(d[i][j]==1)
			for(k=0; k<=r-1; k++) /*vado a vedere se trovo un lato che unisce i cordoni nella j-esima colonna di a*/
				if(circuit[k][j]==1 && d[i][k]==0 && f==0) {
					/*printf("k=%d\tj=%d\n", k, j);*/
					b[k][j]=1;
					b[j][k]=1;
					f=1;       /*devo aggiungere un solo lato!*/
				}


/*BLOCK 4*/
/*conto quanti lati ho eliminato per avere il grafo ad albero = numero di maglie indipendenti*/
*m=0;
for(i=0; i<=r-1; i++)
		for(j=0; j<=r-1; j++)	
			if(b[i][j]!=circuit[i][j])
				(*m)++;
*m=*m/2;
/*mi creo la matrice delle maglie indipendenti e la inizializzo come b*/
c=r*(*m)+*m+1;
int **indm;

indm = (int**) malloc(c*sizeof(int*));
if (indm == NULL) {
	printf("Not enough memory for allocation\n");
	exit(1); 
}
for (i=0; i<r; i++){
	indm[i] = (int*) malloc(c*sizeof(int));
	if (indm == NULL) {
		printf("Not enough memory for allocation\n");
		exit(1); 
	}
}

for(j=0; j<=c-r-1; j=j+r+1)
	for(i=0; i<=r-1; i++)
		indm[i][j]=-4;
for(i=0; i<=r-1; i++)
		indm[i][c-1]=-5; /*segno l'ultima colonna con -5, mi servirà nel main per trovare dimensione matrice*/

for(k=1; k<c; k=k+r+1)
	for(i=0; i<=r-1; i++)
		for(j=0; j<=r-1; j++)	
			indm[i][j+k]=b[i][j];


/*aggiungo uno alla volta i lati*/
k=1;
for(i=0; i<=r-1; i++)
		for(j=i+1; j<=r-1; j++)	
			if(circuit[i][j]==1 && b[i][j]==0){
				indm[i][j+k]=1;
				indm[j][i+k]=1;
				k=k+r+1;
			}

/*tolgo i segmenti che sono al di fuori del ciclo (vedere disegno)*/
nu = 0; /*nu è il numero di uno su una riga*/
done=0;
for(k=1; k<c; k=k+r+1){
	while(done==0){
		done=1;
		for(i=0; i<=r-1; i++){
			for(j=0; j<=r-1; j++) /*mi conto quanto uno ci sono su una riga*/
				if(indm[i][j+k]==1)
					nu++;
			if(nu==1)       /*se c'è un solo uno lo rendo nullo*/
				for(j=0; j<=r-1; j++)    /*mi trovo il j dell'1*/
					if(indm[i][j+k]==1){
						indm[i][j+k]=0;
						indm[j][i+k]=0;
						done=0;
					}
			nu=0;
		}
	}
	done=0;	
}

return indm;
/*fine function */
}
