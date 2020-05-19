void createb(b, r, c, m, volt, mcurrent)

int r, c, m, **mcurrent;
float **volt, *b;

{

int i, j, k;

/*inizializzo b a zero*/
for(i=0;i<=m-1; i++)
	b[i]=0;


/*ora inserisco i valori in b*/
for(k=1;k<c-1;k=k+r+1)
	for(i=0;i<=r-1; i++)
		for(j=0;j<=r-1; j++)
			if(mcurrent[i][j+k]==1 && volt[i][j]!=0)
				b[k/r]=b[k/r]+volt[i][j];


/*fine function*/
}
