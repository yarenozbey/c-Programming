#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
void anahtar_kod_bul(char *kod, char *anahtar);
int anahtar_kod_karsilastir(char *kod1, char*kod2);

int main(int argc, char *argv[]) {
	char kod1[100] = "cabc2ssss3uvxyz4sab2kabak4odun1";
	char anahtar1[50];

	char kod2[100] = "cabc2ssss3uvxyz4sab2kabak4adun1";
	char anahtar2[50];
	
	anahtar_kod_bul(kod1, anahtar1);
	anahtar_kod_bul(kod2, anahtar2);
	//anahtar_kod_karsilastir(anahtar1, anahtar2);
	if(!anahtar_kod_karsilastir(anahtar1, anahtar2)){
		printf("\nCasuslar iletisime GECEBILIR!");
	}
	else{
		printf("\nCasuslar iletisime GECEMEZ!");
	}
	
	return 0;
}

void anahtar_kod_bul(char *kod, char *anahtar){
	int len = strlen(kod);
	int i,j,k=0, point=0;
	for(i=0;i<len;i++){
		if(kod[i] <= '9' && kod[i] >= '0'){
			if((i-point) < kod[i]-'0'){
				printf("\n girdiginiz kod hatalidir.");
				return;
			}
			for(j=point;j<(i-(kod[i]-'0'));j++){
				anahtar[k] = kod[j];
				k++;
			}
			point = i+1;
		}
	}
	anahtar[k] = '\0';
	printf("\nAnahtar: %s",anahtar);
}

int anahtar_kod_karsilastir(char *kod1, char*kod2){
	int len1 = strlen(kod1), len2 = strlen(kod2);
	int i=0;
	if(len1 != len2){
		return 1;
	}
	else{
		while(kod1[i]-'0' == kod2[i]-'0' && i<len1){
			i++;
		}
		if(i!=len1) return 1;	
	}
	return 0;

}
