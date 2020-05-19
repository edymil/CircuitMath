#include <stdio.h>
#include <stdlib.h>
#define NC 1.0e+10


void tension(r, res, current, volt)
int r;
float **res, **volt;
float **current;


{
int i, j, ii, jj, iii, jjj, k, end;


/*CALCOLO LE CADUTE DI TENSIONE SU OGNI RESISTENZA*/
/*creo la matrice fall che mi salva le cadute di potenziale in ogni punto e lo inizializzo a zero*/
float fall[r][r];

for(i=0; i<=r-1; i++)
	for(j=0; j<=r-1; j++)
		fall[i][j]=0;		


for(i=0; i<=r-1; i++)
	for(j=0; j<=r-1; j++)
		if(res[i][j]!=0 && current[i][j]!=0) /*occhio se cambi*/
			fall[i][j]= current[i][j]*res[i][j];

















/*CALCOLO LA TENSIONE IN UNA MAGLIA CHE HA LA BATTERIA*/
/*cerco dov'è la batteria (l'ultimo che trovo)*/
for(i=0; i<=r-1; i++)
	for(j=0; j<=r-1; j++)
		if(volt[i][j]!=0 && current[i][j]!=0){
			/*printf("%d\t%d\n", i, j);*/
			ii=i;
			jj=j;
		}
/*printf("ii%d\tjj%d\n", ii, jj);*/
/*creo la matrice per le tensioni iniziali e finali in ogni ramo*/
float tensi[r][r], tensf[r][r];

/*inizializzo tutto a NC (not yet calculated)*/
for(i=0; i<=r-1; i++)
	for(j=0; j<=r-1; j++){
		tensf[i][j]=NC;
		tensi[i][j]=NC;
	}

tensf[ii][jj]=volt[ii][jj]-fall[ii][jj]; /*mi calcolo la tensione finale nel ramo con la batteria*/
/*printf("prova\n");*/
iii=ii; /*mi salvo le coordinate del ramo con la batteria*/
jjj=jj;

while(jj!=j){
	i=ii; /*mi salvo il ramo precedente*/
	j=jj;
	for(k=0; k<=r-1; k++)         /*cerco il ramo successivo*/
		if(current[jj][k]!=0 && tensi[jj][k]==NC){
			ii=jj;
			jj=k;
			/*printf("%d\t%d\n", ii+1, jj+1);*/
			break;
		}
	if((ii!=iii || jj!=jjj) && (ii!=i || jj!=j)){
		/*printf("prova\n");*/
		tensi[ii][jj]=tensf[i][j]; /*calcolo i voltaggi*/
		tensf[ii][jj]=tensi[ii][jj]+volt[ii][jj]-fall[ii][jj];
	}
	/*printf("tensi%f\ttensf%f\n", tensi[ii][jj], tensf[ii][jj]);*/
}
/*ora mi concentro sulla tensione iniziale del ramo con la batteria*/
for(i=0; i<=r-1; i++)
	if(current[i][iii]!=0 && tensf[i][iii]!=NC)
		tensi[iii][jjj]=tensf[i][iii];

for(i=0; i<=r-1; i++)
		for(j=0; j<=r-1; j++)
			if(current[i][j]==0){
				tensf[i][j]=0;
				tensi[i][j]=0;
			}

/*printf("prova\n");*/























/*CALCOLO LA TENSIONE NEGLI ALTRI RAMI*/
end=1;
for(i=0; i<=r-1; i++)
	for(j=0; j<=r-1; j++)
		if(tensi[i][j]==NC)
			end=0;


while(end==0){
	/*cerco un ramo in cui sia ancora da calcolare la tensione (l'ultimo che trovo)*/
	for(i=0; i<=r-1; i++)
		for(j=0; j<=r-1; j++)
			if(tensi[i][j]==NC && current[i][j]!=0){
				ii=i; /*ii è il punto da cui parte la corrente*/
				jj=j; /*jj il punto in cui arriva*/
				}
	/*printf("%d\t%d\n", ii+1, jj+1);*/
	/*vedo se questo ramo ha un ramo adiacente in cui sia già stata calcolata la tensione, in caso positivo calcolo la tensione del 		nuovo ramo*/
	k=-1; /*k mi serve ad indicare se ho trovato o meno un ramo vicino con la tensione calcolata*/
	for(j=0; j<=r-1; j++)         
		if(tensi[ii][j]!=NC && tensi[ii][j]!=-NC && current[ii][j]!=0){
			tensi[ii][jj]= tensi[ii][j];
			tensf[ii][jj]= tensi[ii][jj]-fall[ii][jj]+volt[ii][jj];
			k=1;
			for(i=0; i<=r-1; i++)
				for(j=0; j<=r-1; j++)
					if(tensi[i][j]==-NC)
						tensi[i][j]= NC;
			/*printf("a\n");*/
			break;
		}
		else if(tensi[j][ii]!=NC && tensi[j][ii]!=-NC && current[j][ii]!=0){ 
			tensi[ii][jj]= tensf[j][ii];
			tensf[ii][jj]= tensi[ii][jj]-fall[ii][jj]+volt[ii][jj];
			k=1;
			for(i=0; i<=r-1; i++)
				for(j=0; j<=r-1; j++)
					if(tensi[i][j]==-NC)
						tensi[i][j]= NC;
			/*printf("b\n");*/
			break;
		}
		else if(tensi[jj][j]!=NC && tensi[jj][j]!=-NC && current[jj][j]!=0){
			tensf[ii][jj]= tensi[jj][j];
			tensi[ii][jj]= tensf[ii][jj]+fall[ii][jj]-volt[ii][jj];
			k=1;
			for(i=0; i<=r-1; i++)
				for(j=0; j<=r-1; j++)
					if(tensi[i][j]==-NC)
						tensi[i][j]= NC;
			/*printf("c\n");*/
			break;
		}
		else if(tensi[j][jj]!=NC && tensi[j][jj]!=-NC && current[j][jj]!=0){ 
			tensf[ii][jj]= tensf[j][jj];
			tensi[ii][jj]= tensf[ii][jj]+fall[ii][jj]-volt[ii][jj];
			k=1;
			for(i=0; i<=r-1; i++)
				for(j=0; j<=r-1; j++)
					if(tensi[i][j]==-NC)
						tensi[i][j]= NC;
			/*printf("d\n");*/
			break;
		}
		else if(k==-1 && j==r-1){ /*questo ciclo considera il caso in cui non si abbia alcun ramo vicino in cui è già stata calcolata la 							tensione*/
		/*quando trovo un ramo che non ha vicini calcolati lo devo segnalare in modo che, finchè non calcola la tensione in un altro ramo*/
		/*non lo controlli più, questo per evitare loop infiniti. Per far ciò segno con -NC tali rami quando casco nel caso e, */
		/*quando invece casco negli altri casi in cui calcolo un ramo rimetto tutti i -NC a NC */
			tensi[ii][jj]=-NC;
			/*printf("e\n");*/
		}	

	end=1;
	/*stampa totale*/
	/*printf("tensi\n");
	for(i=0; i<=r-1; i++){
		for(j=0; j<r-1; j++)
			printf("%f", tensi[i][j]);
		printf("%f\n", tensi[i][r-1]);
	}
	printf("tensf\n");
	for(i=0; i<=r-1; i++){
		for(j=0; j<r-1; j++)
			printf("%f", tensf[i][j]);
		printf("%f\n", tensf[i][r-1]);
	}*/
	for(i=0; i<=r-1; i++)
		for(j=0; j<=r-1; j++)
			if(tensi[i][j]==NC && current[i][j]!=0){
				end=0;
	/*printf("end%d\n", end);*/
				}
/*fine while*/
}
































/*verifica leggi fisiche*/
/*printf("%f\n", tensi[iii][jjj]);*/
if(tensi[iii][jjj]<=0.0001 && tensi[iii][jjj]>=-0.0001)
	printf("\nI valori ottenuti per le tensioni rispettano le leggi fisiche!\n");

/*STAMPA FILE ESTERNO*/
FILE *fp;
fp = fopen ("tension.dat","w");

/*Scrivo su file*/
fprintf(fp,"Nella matrice delle tensioni iniziali sono salvati i valori delle tensioni all'inizio del ramo. Per inizio si intende rispetto al verso della corrente\n");
for(i=0; i<=r-1; i++){
	for(j=0; j<r-1; j++)
		fprintf(fp, "%f\t", tensi[i][j]);
	fprintf(fp, "%f\n", tensi[i][r-1]);
}	
fprintf(fp,"Nella matrice delle tensioni finali sono salvati i valori delle tensioni alla fine del ramo.\n");
for(i=0; i<=r-1; i++){
	for(j=0; j<r-1; j++)
		fprintf(fp, "%f\t", tensf[i][j]);
	fprintf(fp, "%f\n", tensf[i][r-1]);
}	
fprintf(fp,"Nella matrice delle cadute di potenziale sono salvate le cadute di potenzioni su ogni resistenza.\n");
for(i=0; i<=r-1; i++){
	for(j=0; j<r-1; j++)
		fprintf(fp, "%f\t", fall[i][j]);
	fprintf(fp, "%f\n", fall[i][r-1]);
}	

if (!fp)
	printf ("\nC'è stato un errore nella scrittura del file 'tension.dat'!\n");
else
	printf("\nIl file tension.dat è stato scritto correttamente!\n");






/*fine function*/
}

