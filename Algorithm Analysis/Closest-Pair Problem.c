#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct POINT {
	int x;
	int y;
}POINT;

typedef struct PAIR {
	struct POINT p1;
	struct POINT p2;
	float d;
}PAIR; // values of the closest pair of ponints

void mergeSortX(struct POINT *, int , int );
struct PAIR* closest_pair(struct POINT *,  struct PAIR *, struct PAIR *, int );

int main(int argc, char *argv[]) {
	struct POINT* points =  malloc(50 * sizeof(struct POINT));
	int i=0, j=0;
	float median; // meidaan index of the array
	char filename[11] = "sample.txt";
	FILE *data=fopen(filename,"r+"); 
	if(!data){
		printf("file could not be opened");
		return 0;
	}
	while(!feof(data)){
		fscanf(data,"%d%*c%d",&points[i].x,&points[i].y);
		i++;
	}		
	fclose(data);	

	mergeSortX(points,0,i-1);

	// Writes sorted array to 'sorted.txt' file
	char sortedfile[11] = "sorted.txt";
	FILE *fw=fopen(sortedfile,"w+"); 	
	if(fw){
		while(j<i){
			fprintf(fw,"%d %d\n",points[j].x,points[j].y);
			j++;
		}
			fclose(fw);
	}

	
	PAIR *pair =  malloc(sizeof(struct PAIR)); // the closest pair's adress
	PAIR *RLmin =  malloc(sizeof(struct PAIR));// min value of between the right and left of array  ( d = min {dl,dr} )
	pair->d = INFINITY;
	RLmin->d = INFINITY;
	
	closest_pair(points, pair, RLmin, i); // recursive function
	printf("\n**** MIN DIST: %f ****",pair->d);
	printf("\n p1 x:%d\tp1 y:%d\n",pair->p1.x,pair->p1.y);
	printf("\n p2 x:%d\tp2 y:%d\n",pair->p2.x,pair->p2.y);
	
	free(RLmin); free(pair);

	return 0;
}

float distance(int a, int b) // calculates distince of two point
{
	float d;
	
	d = (float)sqrt(a*a + b*b);
	
	return d;
}

void merge_y(struct POINT *arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    struct POINT* L =  malloc(n1 * sizeof(struct POINT));
	struct POINT* R =  malloc(n2 * sizeof(struct POINT));	
  
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
  
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].y <= R[j].y) {
           	arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
  
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L); free(R);
}

void mergeSortY(struct POINT *arr, int l, int r) // merge sort by Y
{
    if (l < r) {

        int m = l + (r - l) / 2;

        mergeSortY(arr, l, m);
        mergeSortY(arr, m + 1, r);
  
        merge_y(arr, l, m, r);
    }
}

// find points which have min distance. use brute force approach if size of the array grater than three
// else consider possibility of one of points in the left and right side of array and the other in the right side of array.
void min_dist(struct POINT *arr, int n, struct PAIR *pair)
{
	float mind, d; // minimum distance and distance. if d < mind -> mind = d
	int i = 0, j=0, m;
	struct POINT* rec =  malloc(n * sizeof(struct POINT)); // array of the possible points
	
	if (n<4){
		mind = INFINITY;
		while(i<n-1){
			d = distance(arr[i].x-arr[i+1].x, arr[i].y-arr[i+1].y);
			if(d<mind){
				mind = d;
				pair->p1 = arr[i];
				pair->p2 = arr[i+1];
				pair->d = mind;				
			}
			i++;
		}
		if(n==3){
			d = distance(arr[i].x-arr[0].x, arr[i].y-arr[0].y);
			if(d<mind){
				mind = d;
				pair->p1 = arr[i];
				pair->p2 = arr[0];
				pair->d = mind;				
			}
		}
	}
	else{
		m = n/2-1;
		int x = (int)m-pair->d;
		int y = (int)m+pair->d;
		if (x<0) x=0;
		if (y>n) y=n;
		for(i=x;i<y;i++){
				rec[j] = arr[i];
				j++;
		}
		
		int n1 = j;
		mergeSortY(rec, 0, j-1);
		
		i = 0;
		j=i+1;
		while(i<n1){
			j=i+1;
			while(rec[j].y-rec[i].y < pair->d ){
				d = distance(rec[i].x-rec[j].x, rec[i].y-rec[j].y);
				if( d < pair->d){
					pair->p1 = rec[i];
					pair->p2 = rec[j];
					pair->d = d;
				}
				j++;
			}
			i++;
		}
	}
	free(rec);
}

struct PAIR* closest_pair(struct POINT *arr, struct PAIR *pair, struct PAIR *RLmin, int n) // find points which have the min distance recursively
{
	int i, j;
    int n1 = n/2; // size of left side of array
    int n2 = n - n1; // size of right side of array

    struct POINT* L =  malloc(n1 * sizeof(struct POINT));
	struct POINT* R =  malloc(n2 * sizeof(struct POINT));
	struct PAIR* pL = malloc(sizeof(struct PAIR)); //min pair of the left side
	struct PAIR* pR = malloc(sizeof(struct PAIR)); //min pair of the right side
	
	for(i=0;i<n1;i++){
		L[i] = arr[i];	// left side of array
	}
	for(j=0;j<n2;j++){
		R[j] = arr[i+j]; // right side of array
	}
	
	
	
	if (n>3){
 		*pL = *closest_pair(L, pair, RLmin, n1);
		*pR = *closest_pair(R, pair, RLmin, n2);
		
		if( pL->d >= pR->d){
			*RLmin = *pR;
		}
		else{
			*RLmin = *pL;
		}
		
		if(RLmin->d < pair->d)
			*pair = *RLmin;
	}

	min_dist(arr, n, pair);
	
	free(L); free(R); 
	free(pL); free(pR);

	return pair;
	
}

void merge_x(struct POINT *arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
  
    /* create temp arrays */
    //struct POINT L[n1], R[n2];
    struct POINT* L =  malloc(n1 * sizeof(struct POINT));
	struct POINT* R =  malloc(n2 * sizeof(struct POINT));	
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].x <= R[j].x) {
           	arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
  
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
  
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L); free(R);
}

void mergeSortX(struct POINT *arr, int l, int r) // merge sort by X
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
  
        // Sort first and second halves
        mergeSortX(arr, l, m);
        mergeSortX(arr, m + 1, r);
  
        merge_x(arr, l, m, r);
    }
}

