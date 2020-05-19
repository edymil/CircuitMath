#include <stdio.h>
#include <stdlib.h>

float** creatematrix(m, r, mcurrent, res, c)

int m, r, c, **mcurrent;
float **res;

{

int k, kk, ii, jj, i, j;

float** a;

a = (float**) malloc(m*sizeof(float*));
if (a == NULL) {
	printf("Non ho abbastanza memoria perl'allocazione\n");
	exit(1); 
}
for (i=0; i<m; i++){
	a[i] = (float*) malloc(m*sizeof(float));
	if (a == NULL) {
		printf("Non ho abbastanza memoria perl'allocazione\n");
		exit(1); 
	}
}

for(i=0; i<=m-1; i++)
	for(j=0; j<=m-1; j++)
		a[i][j]=0;

/*elementi sulla diagonale*/
for(k=1;k<c-1;k=k+r+1)
	for(i=0; i<=r-1; i++)
		for(j=0; j<=r-1; j++)
			if(mcurrent[i][j+k]==1 && res[i][j]!=0)
				a[k/r][k/r]=a[k/r][k/r] + res[i][j];			
/*elementi sopra la diagonale*/
for(i=0; i<m-1; i++)                
	for(j=i+1; j<=m-1; j++){
		k  =1+(r+1)*i; /*shift riga    *//*questi shift mi permettono di vedere se ci sono sovrapposizioni per le   */
		kk =1+(r+1)*j; /*shift colonna*/ /*maglie che riguardano la casella di a di cui vogliamo calcolare il valore*/
		for(ii=0; ii<=r-1; ii++)
			for(jj=ii; jj<=r-1; jj++)
				if(mcurrent[ii][jj+k]!=0 && mcurrent[ii][jj+kk]!=0)  /*se le maglie si sovrappongono*/
					a[i][j]=a[i][j]+(mcurrent[ii][jj+k]*mcurrent[ii][jj+kk])*res[ii][jj];
	}


/*elementi sotto la diagonale*/
for(i=0; i<=m-1; i++)                
	for(j=0; j<i; j++)
		a[i][j]=a[j][i];

return a;


/*fine function*/
}

