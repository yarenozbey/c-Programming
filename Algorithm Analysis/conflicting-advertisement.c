#include <stdio.h>
#include <stdlib.h>
//#typedef N 10

typedef struct AD {
	int StartTime;
	int Duration;
	int Value;
	int Order;
}AD;

void mergeSort(struct AD *, int , int );
void print_2D(int **,int );

int main(int argc, char *argv[]) {
	int i=0, j=0, k; // index için kullanýlacak degiskenler
	int N; // reklam sayýsý
	AD* ads = malloc(50 * sizeof(struct AD));
	
	// Verilerin dosyadan okunmasý
	char filename[11] = "Sample.txt";
	FILE *data=fopen(filename,"r+"); 
	if(!data){
		printf("file could not be opened");
		return 0;
	}
	while(!feof(data)){
		fscanf(data,"%d%*c%d%*c%d",&ads[i].StartTime, &ads[i].Duration, &ads[i].Value);
		ads[i].Order = i++;
	}		
	fclose(data);
	
	// Reklamlarýn bitis zamanýna gore sýralanmasý
	N = i;	
	mergeSort(ads, 0, N-1);
	
	char sortedfile[11] = "sorted.txt";
	FILE *fw=fopen(sortedfile,"w+"); 	
	if(fw){
		while(j<i){
			fprintf(fw,"%d %d %d\n",ads[j].StartTime, ads[j].Duration, ads[j].Value);
			j++;
		}
		fclose(fw);
	}	

	// Cakýsan reklamlarýn tutuldugu matris
    int **ad_num=(int **)malloc((N+1)*sizeof(int*));
    for(i=0;i<N+1;i++)
        ad_num[i]=(int *)malloc((N+1)*sizeof(int));	
	
	for(i=0;i<N+1;i++){
		for(j=0;j<N+1;j++){
			ad_num[i][j] = 0;
		}
	}
	
	int profit[N+1];	// maksimum kazanc	
	profit[0] = 0;
	profit[1]= ads[0].Value;
	ad_num[1][1] = 1;
	j = 2;
	
	while(j < N+1)
	{
		i = j-1;
		while(ads[i-1].StartTime+ads[i-1].Duration > ads[j-1].StartTime && i>0){ // cakýsma olmayan ilk reklamý bulur.
			i--;
		}
		if(i==0){ // saglanýrsa kendisinden onceki butun reklamlarla cakýsýyor.
			if(profit[0] + ads[j-1].Value > profit[j-1]){
				profit[j] = profit[0] + ads[j-1].Value;
				ad_num[j][j] = 1;
			}
			else{
				profit[j] = profit[j-1];
				for(k=0;k<j;k++){
					ad_num[j][k] = ad_num[j-1][k];					
				}
			}
		}
		else{

			if(profit[i] + ads[j-1].Value > profit[j-1]){
				profit[j] = profit[i] + ads[j-1].Value;
				for(k=0;k<j;k++){
					ad_num[j][k] = ad_num[i][k];					
				}				
				ad_num[j][j] = 1;
			}
			else{
				profit[j] = profit[j-1];
				for(k=0;k<j;k++){
					ad_num[j][k] = ad_num[j-1][k];					
				}								
			}
		}
		j++;
		printf("\n\nAD_NUM MATRIX(step %d):",j);
		print_2D(ad_num, N+1);
	}
	
	printf("\n**********\n");
	printf("Total profit: %d\nAds to be posted: \n",profit[N]);
	for(i=1;i<N+1;i++){
		if(ad_num[N][i] == 1){
			printf(" %d",ads[i-1].Order);			
		}
	}
	
	
	for(i = 0; i < N+1; i++)
    	free(ad_num[i]);
	free(ad_num);
	free(ads);
	return 0;
}

//print 2D array
void print_2D(int **arr,int n)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
    	printf("\n");
    	for (j = 0; j < n; j++)
        {
            printf("%d ", arr[i][j]);
        }
    }

}


//Merge Sort
void merge(struct AD *arr, int l,int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;	

	AD* L = malloc(n1 * sizeof(struct AD));
	AD* R = malloc(n2 * sizeof(struct AD));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
	
	i=0; j=0; k=l;
	
	while (i < n1 && j < n2){
		if (L[i].StartTime+L[i].Duration <= R[j].StartTime+R[j].Duration){
			arr[k] = L[i];
			i++;
		}
		else{
			arr[k] = R[j];
			j++;
		}
		k++;
	}
	
	while(i < n1){
		arr[k] = L[i];
		i++;
		k++;
	}
	
	while(j < n2){
		arr[k] = R[j];
		j++;
		k++;
	}	
	
	free(L); free(R);
}


void mergeSort(struct AD *arr, int l, int r)
{
	if(l < r){
        int m = l + (r - l) / 2;
        
        mergeSort(arr, l ,m);
        mergeSort(arr, m+1, r);
        
        merge(arr, l, m, r);
	}
}

