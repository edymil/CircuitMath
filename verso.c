#include <stdio.h>
#include <stdlib.h>

void verso(r, c, mcurrent)
int r, c, **mcurrent;
{
int k, i, j, n, ii, jj, nloop;

for(k=1;k<c-1;k=k+r+1){
	/* count the number of links in each mesh */
	n=0;
	ii = -1; /* initialize ii to -1 to overwrite it just once*/
	for(i=0; i<=r-1; i++)
		for(j=0; j<=r-1; j++){
			if(mcurrent[i][j+k]==1){
				n++;
				/*printf("i=%d\tj=%d\n", i, j);*/
			}
			if(n==1 && ii==-1){ /* store coordinates of the first detected 1*/
				ii=i;
				jj=j;
				/*printf("second i=%d\tj=%d\n", i, j);*/
			}
		}
	n=n/2;
	/*printf("n=%d\n", n);*/
	/*global printout*/
	/*printf("first\n");
	for(i=0; i<=r-1; i++){
		for(j=0; j<r-1; j++)
			printf("%d", mcurrent[i][j+k]);
	printf("%d\n", mcurrent[i][r-1+k]);
	}*/
	mcurrent[jj][ii+k]=-1; /* store a -1 in the symmetric position of the first detected 1 -> set direction of first side*/
	nloop=1; /* first branch of mesh has been directed*/
        /*next, look for the next branch of mesh in row jj*/
	i=jj; /*select row*/
	do{
		for(j=0; j<=r-1; j++) /*check columns searching for a 1*/
			if(mcurrent[i][j+k]==1){
				mcurrent[j][i+k]=-1; /*set the symmetric element to -1*/
		                nloop++;             /*increase the number of directed sides*/
				/*printf("nloop=%d\n", nloop);*/
		                i=j;                 /*select next row*/
				break;
			}
	}while(nloop<n); /*iterate procedure until direction found for all branches*/
	/*global printout*/
	/*printf("after\n");
	for(i=0; i<=r-1; i++){
		for(j=0; j<r-1; j++)
			printf("%d", mcurrent[i][j+k]);
	printf("%d\n", mcurrent[i][r-1+k]);
	}*/
}

/*end function*/	
}	
