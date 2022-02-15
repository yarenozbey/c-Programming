#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct PERSON {
    char id[10];
    char name[20];
    char surname[20];
    int born;
    char city[20];
} PERSON;

typedef struct HASH {
    char id[10];
    int order;
} HASH;

typedef struct NODE {
    int hashIndex;
    struct PERSON* p;
    struct NODE* next;
} NODE;

int doubleHashing(char*, int, HASH*);
int smallest_prime(int );

void printHash(HASH* hash, int size) {
 	printf("\n*****HASH TABLE*****\n");
    int i;
    for (i = 0; i < size; i++) {
        printf("\n%d. GOZ: %s \tCACHE SIRASI: %d\n", i, hash[i].id, hash[i].order);
    }
}

void printCache(NODE* root) {
 	printf("\n*****CACHE*****\n");
    while (root) {
        printf("%s %s %s\n", root->p->id, root->p->name, root->p->surname);
        root = root->next;
    }
}



struct NODE* createNode(int i, PERSON* p) {
    NODE* newNode = malloc(sizeof(NODE));
    newNode->p = malloc(sizeof(PERSON));
    strcpy(newNode->p->id, p->id);
    strcpy(newNode->p->name, p->name);
    strcpy(newNode->p->surname, p->surname);
    newNode->p->born = p->born;
    strcpy(newNode->p->city, p->city);
    newNode->hashIndex = i;
    newNode->next = NULL;

    return newNode;
}

void insertToHash(int index, PERSON* p, HASH* table) {
    strcpy(table[index].id, p->id);
    table[index].order = 0;
}

//cahce bellekte olmayan bir kisiyi bellege ekler.
void insertToCache(NODE** root, NODE* newNode, HASH* hash, int cache_size) {

    NODE* tmp, *tmp2;
	tmp = (*root);
	int i = 0;
	if(tmp){
		while(tmp->next){
			hash[tmp->hashIndex].order++;
			tmp2 = tmp;
			tmp = tmp->next;
			i++;
		}		
		hash[tmp->hashIndex].order++;
	}

	if(i == cache_size-1){
		printf("\nCache Dolu. %s ID'li kisi cahce ve hash tablosundan silindi.\n",hash[tmp->hashIndex].id);
		hash[tmp->hashIndex].id[1]= '\0';
		hash[tmp->hashIndex].id[0] = -1;
		hash[tmp->hashIndex].order = -1;
		tmp2->next = NULL;
		free(tmp);
	}
	
    newNode->next = *root;
    *root = newNode;
	
	//printCache(*root);	
   
}
//bellkte mevcut kisiyi linkli listenin basýna alýr.
void findInCache(NODE** root, HASH* hash, int i)
{
	NODE *tmp = *root, *tmp2;
	int k;
	
	for(k = 0; k<hash[i].order-1; k++){
		hash[tmp->hashIndex].order++;
		tmp = tmp->next;
	}
	
	hash[tmp->hashIndex].order++;
	tmp2 = tmp->next;
	tmp->next = tmp->next->next;
	tmp2->next = (*root);
	*root = tmp2;
	
	hash[i].order = 0;
}


int main(int argc, char* argv[]) {

    int index, i, N , count = 0, cache_size ;
    char filename[9] = "test.txt";
    FILE* data = fopen(filename, "r+");

    PERSON *person;
    NODE *cache = NULL, *root = NULL,*tmp;
    root = cache;


	printf("Cache size: "); scanf("%d", &cache_size);
	int M = (int)(cache_size / 0.6 + 0.5);
	
//	printf("Hash Table size: "); scanf("%d", &N);

	N = smallest_prime(M);

    person = malloc(sizeof(PERSON));
    HASH hash[N];
    for (i = 0; i < N; i++) {
        hash[i].id[0] = '\0';
        hash[i].order = -1;
    }

    if (!data) {
        printf("file could not be opened");
        return 0;
    }
    while (fscanf(data, " %s %s %s %d %s", person->id, person->name, person->surname, &person->born, person->city) == 5) {
        index = doubleHashing(person->id, N, hash);
   		printf("\n\n############################\n");
        printf("\n\n%s icin hash adresi %d\n", person->id, index);
        
        tmp = root;
        if( !strcmp(hash[index].id, person->id) ){
        	for(i=0; i<hash[index].order; i++){
        		tmp = tmp->next;
			}
        	printf("TC No: %s\nName: %s\nSurname: %s\nBorn: %d\nCity: %s\nLinkli listenin %d. sirasinda\n",
				tmp->p->id, tmp->p->name, tmp->p->surname, tmp->p->born, tmp->p->city, hash[index].order);
        	
        	findInCache(&root, hash, index);
		}
		else{
        	printf("TC No: %s\nName: %s\nSurname: %s\nBorn: %d\nCity: %s\nKisisi cache'de bulunamadi\n",
				person->id, person->name, person->surname, person->born, person->city, hash[index].order);

	        insertToHash(index, person, hash);
	        cache = createNode(index, person);
	        insertToCache(&root, cache, hash, cache_size);		
			printf("%s ID'li kisi Cache ve Hash tablosuna eklendi.\n",person->id);	
		}
        printHash(hash, N);
        printCache(root);
    }
    fclose(data);

    return 0;
}

/*
Horner methodu
*/
int hornersMethod(char* str, int m, int i) {
    int l = strlen(str);
    int h1 = 0, R1 = 31, h2 = 0, h = 0, k;

    for (k = 0; k < l; k++) {
        h1 = h1 + pow(R1, (l - 1 - k) ) * str[k]; 
    }
    h1 = h1 % m;

    for (k = 0; k < l; k++) {
        h2 += str[k];
    }
    h2 = 1 + (h2 % (m - 1));

    h = (h1 + i * h2) % m;
    return h;
}

/*
Horner methodunu kullanarak double hashing yapar.
*/
int doubleHashing(char* str, int m, HASH* table) {
    int l = strlen(str), i = 0, m2 = m - 1, flag = 0;
    int tmp = -1, h;

    while (flag == 0) {
        h = hornersMethod(str, m, i);

        if (table[h].id[0] == '\0' || !strcmp(table[h].id, str)) {
            // bulunan yer bos ise veya ayni id ise index dondurulur.
            return h;
        } else if (table[h].id[0] == -1) {
            // silinmis index tmp alinir 2. while dongusune girilir.
            tmp = h;
            flag = 1;
        } else if (i > m) {
            printf("\n\nHATA HASHING! i = %d\n", i);
            return (-1);
        }
        i++;
    }

    //silinmis yerden sonra aradigimiz id veya null yer bulana kadar doner.
    while (table[h].id[0] != '\0' && strcmp(table[h].id, str)) {
        h = hornersMethod(str, m, i);
        i++;
    }
    if (table[h].id[0] == '\0')
        return tmp;  // null buldugunda tmp doner.
    else
        return h;  // aradigimiz id yerini doner.
}

//Asal sayi bulma fonksiyonlari.
int is_prime(int n)
{
	int i = 5;
	
	if (n<=1) return 0;
	if (n == 2 || n==3 ) return 1;
	
	if( !(n%2) || !(n%3) ) return 0;
	
	while ( sqrt(i) < n+1 && (n%i || n% (i+2)))
		i++;
		
	if(sqrt(i) > n) return 1;
	
	return 0;	
}

int smallest_prime(int n){
	if(n < 3) return 2;
	
	if( n%2 == 0) n++;
	
	while( is_prime(n) == 0) n++;
	
	return n;
}


