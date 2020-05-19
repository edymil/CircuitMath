#include <stdio.h>
#include <stdlib.h>

void verso(r, c, mcurrent)
int r, c, **mcurrent;
{
int k, i, j, n, ii, jj, nloop;

for(k=1;k<c-1;k=k+r+1){
	/* conto quanto link ha ogni maglia (n)*/
	n=0;
	ii = -1; /*inizializzo ii a -1 per sovrascriverla una sola volta*/
	for(i=0; i<=r-1; i++)
		for(j=0; j<=r-1; j++){
			if(mcurrent[i][j+k]==1){
				n++;
				/*printf("i=%d\tj=%d\n", i, j);*/
			}
			if(n==1 && ii==-1){ /*mi salvo le coordinate del primo 1 che trovo*/
				ii=i;
				jj=j;
				/*printf("secondo i=%d\tj=%d\n", i, j);*/
			}
		}
	n=n/2;
	/*printf("n=%d\n", n);*/
	/*stampa totale*/
	/*printf("prima\n");
	for(i=0; i<=r-1; i++){
		for(j=0; j<r-1; j++)
			printf("%d", mcurrent[i][j+k]);
	printf("%d\n", mcurrent[i][r-1+k]);
	}*/
	mcurrent[jj][ii+k]=-1; /*pongo a -1 il simmetrico del primo uno che ho trovato -> oriento il primo lato*/
	nloop=1; /* ho orientato il primo lato della maglia*/
        /*ora cerco il lato successivo della maglia nella riga jj*/
	i=jj; /*fisso la riga*/
	do{
		for(j=0; j<=r-1; j++) /*controllo le colonne in cerca di un 1*/
			if(mcurrent[i][j+k]==1){
				mcurrent[j][i+k]=-1; /*pongo a -1 il simmetrico*/
		                nloop++;             /*mi segno che ho orientato un altro lato*/
				/*printf("nloop=%d\n", nloop);*/
		                i=j;                 /*ricordo la prossima riga da guardare*/
				break;
			}
	}while(nloop<n); /*itero il procedimento finché non ho orientato tutti i rami*/
	/*stampa totale*/
	/*printf("dopo\n");
	for(i=0; i<=r-1; i++){
		for(j=0; j<r-1; j++)
			printf("%d", mcurrent[i][j+k]);
	printf("%d\n", mcurrent[i][r-1+k]);
	}*/
}

/*fine function*/	
}	
