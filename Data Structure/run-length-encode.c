//YAREN OZBEY 18011022
#include <stdio.h>
#include <stdlib.h>

typedef struct NODE{
	int color;
	int length;
	struct NODE *next; 
	struct NODE *pre;
}NODE;

typedef struct {
	int width;
	int heigth;
	int max_value;
	int **pixels;
}pgm_information;
void encode(pgm_information info);
pgm_information readInfo();
void RLEtoPGM(char* fileName);
void changeColor(char* fileName);
void change_coordinate(char* fileName);
void histogram(char* fileName);

int main(int argc, char *argv[]) {
	
	//INPUT DOSYASI ISMI input.pgm olmalýdýr!
	pgm_information info = readInfo();
	encode(info);
	RLEtoPGM("test_encoded.txt");
	change_color("test_encoded.txt");
	RLEtoPGM("test_encoded.txt");	
	change_coordinate("test_encoded.txt");
	RLEtoPGM("test_encoded.txt");
	
	histogram("test_encoded.txt");
	
	
	return 0;
}

//Sýkýstýrýlmýs dosyaný histogramýný cýkartýr.
void histogram(char* fileName){
	int width, heigth,i,j,k,found;
	FILE *file = fopen(fileName,"rb");
	fscanf(file,"%d%*c%d%*c",&width, &heigth);
	int s = width*heigth;
	int *pixels, *length;
	pixels = (int*) malloc(sizeof(int)*s);
	length = (int*) malloc(sizeof(int)*s);


	int check=2;
	
	while(!feof(file) && check == 2){
		check = fscanf(file,"%d%*c%d%*c", &length[i], &pixels[i]);
		i++;
	}	
	fclose(file);
	
	int hist[2][i], numOfColors=0;	

	for(j=0;j<i-1;j++){
		k = 0; found = 0;
		while(k<numOfColors){
			if(pixels[j] == hist[0][k]){
				hist[1][k] += length[j];
				found = 1;
			}
			k++;
		}
		if(found == 0){
			hist[0][k] = pixels[j];
			hist[1][k] = length[j];
			numOfColors++;
		}
	}
	
	printf("\n\n******BU RESIMDE %d FARKLI RENK KULLANILMISTIR******\nHISTORGRAM :\n",numOfColors);
	
	for(i=0;i<numOfColors;i++){
		printf("%d RENGI: %d ADET\n", hist[0][i],hist[1][i]);
	}
		
}

//Verilen koordinattaki pikselin deðerini degistirir.
void change_coordinate(char* fileName){
	int x,y,color;
	//printf("\n\n********(%d, %d) KOORDINATINA %d RENGI YAZMA ISLEMI BASLADI********",x,y,color);
	printf("\n\n********KOORDINATTAKI RENGI DEGISTIRME ISLEMI BASLADI********",x,y,color);
	printf("\nX KOORDINATI :"); scanf("%d",&x);
	printf("\nY KOORDINATI :"); scanf("%d",&y);
	printf("\nYENI RENK :"); scanf("%d",&color);
	
	int width, heigth, count=0;
	FILE *file = fopen(fileName,"rb");
	fscanf(file,"%d%*c%d%*c",&width, &heigth);
	if( (x>width || x<0) || (y>heigth || y<0) ){
		printf("\nHATA: GECERSIZ KOORDINAT");
		return;
	}
	
	int s = width*heigth;
	NODE *pixels = (NODE*)malloc(sizeof(NODE));
	NODE  *root = pixels;
	int i=0, j, check=2, distance = 0;
	int coordinate = y*width + x ;
	while(!feof(file)){
		check = fscanf(file,"%d%*c%d%*c", &pixels->length, &pixels->color);
		if(check == 2){				
			pixels->next = (NODE*)malloc(sizeof(NODE));	
			pixels->next->next = NULL;
			pixels->next->pre = pixels;
			pixels = pixels->next;
		}
		else{
		}	
	}

	fclose(file);

	pixels = root;

	if(coordinate == 0 ){
		if(pixels->length == 1){
			if(color == pixels->next->color){
				pixels->next->length++;
				root = pixels->next;
				root->pre = NULL;
				free(pixels);				
			}
			else 
				pixels->color = color;		
		}
		else if(color != pixels->color){
			pixels->length--;
			NODE *new_pixel = (NODE*)malloc(sizeof(NODE));
			new_pixel->color = color;
			new_pixel->length = 1;
			new_pixel->next = pixels;
			pixels->pre = new_pixel;
			root = new_pixel;		
		}
	}
	else{
		while(distance < coordinate && pixels->next != NULL){
			distance += pixels->length;
			pixels = pixels->next;
		}
		
		if(distance == coordinate){
			if(pixels->length == 1){
				if(color == pixels->next->color && color == pixels->pre->color){
					pixels->pre->length += pixels->length+pixels->next->length;
					pixels->pre->next = pixels->next->next;
					pixels->next->pre = pixels->pre;
					free(pixels->next); free(pixels);
				}
				else if(color == pixels->next->color){
					pixels->next->length++;
					pixels->next->pre = pixels->pre;
					pixels->pre->next = pixels->next;
					free(pixels);				
				}
				else if(color == pixels->pre->color){
					pixels->pre->length++;
					pixels->next->pre = pixels->pre;
					pixels->pre->next = pixels->next;
					free(pixels);				
				}
				else 
					pixels->color = color;					
			}
			else if(color == pixels->pre->color){
				pixels->pre->length++;
				pixels->length--;
			}
			else{
				NODE *p = (NODE*)malloc(sizeof(NODE));
				p->color = color;
				p->length = 1;
				pixels->length--;
				p->next = pixels;
				p->pre = pixels->pre;
				pixels->pre->next = p;
				pixels->pre = p;
			}		
		}
		else if(distance > coordinate){
			if(distance - coordinate == 1){
				if(color == pixels->color){
					pixels->length++;
					pixels->pre->length--;
				}
				else if( color != pixels->pre->color){
					pixels->pre->length--;
					NODE *p = (NODE*)malloc(sizeof(NODE));
					p->color = color;
					p->length = 1;
					p->next = pixels;
					p->pre = pixels->pre;
					pixels->pre->next = p;
					pixels->pre = p;
				}
			}
			else if( color != pixels->pre->color){
				NODE *p1, *p2, *p3;
				p1 = (NODE*)malloc(sizeof(NODE));
				p2 = (NODE*)malloc(sizeof(NODE));
				p3 = (NODE*)malloc(sizeof(NODE));
				
				p1->color = pixels->pre->color;
				p1->length = pixels->pre->length - (distance-coordinate);
				p2->color = color;
				p2->length = 1;	
				p3->color = pixels->pre->color;
				p3->length = pixels->pre->length - p1->length -1;
				
				p1->pre = pixels->pre;
				p1->next = p2;
				p2->next = p3;
				p2->pre = p1;
				p3->next = pixels;
				p3->pre = p2;
				if(pixels->pre != root)
					pixels->pre->pre->next = p1;
				else{
					root = p1;
				}
				free(pixels->pre);				
			}						
		}					
	}
	

	pixels = root;
	FILE *tmp = fopen("tmp.txt","wb");
	
	fprintf(tmp,"%d %d\n",width, heigth);
	while(pixels->next != NULL){
		fprintf(tmp,"%d %d ", pixels->length, pixels->color);
		pixels = pixels->next;
	}
	fclose(tmp);
	remove(fileName); rename("tmp.txt",fileName);
	
	printf("\n\n********** (%d, %d) KOORDINATINA %d RENGI YAZILDI **********",x,y,color);



}

void change_color(char* fileName){
	int firstColor,secondColor;
	printf("\n********RENK DEGISTIRME ISLEMI BASLADI********\n");
	printf("\nDEGISTIRMEK ISTEDIGINIZ RENK? :"); scanf("%d",&firstColor);
	printf("\nYENI RENK? :"); scanf("%d",&secondColor);
	int width, heigth;
	FILE *file = fopen(fileName,"rb");
	fscanf(file,"%d%*c%d%*c",&width, &heigth);
	int s = width*heigth;
	int *pixels, *length;
	pixels = (int*) malloc(sizeof(int)*s);
	length = (int*) malloc(sizeof(int)*s);

	int i=0, j, check=2;
	
	while(!feof(file)){
		check = fscanf(file,"%d%*c%d%*c", &length[i], &pixels[i]);
		if(pixels[i] == firstColor){
			pixels[i] = secondColor;
		}
		if(i>0 && pixels[i] == pixels[i-1] && check == 2){
			length[i-1] += length[i];
			i--;	
		}
		i++;
	}	
	fclose(file);
	FILE *tmp = fopen("tmp.txt","wb");
	fprintf(tmp,"%d %d\n",width, heigth);
	for(j=0;j<i-1;j++){
		fprintf(tmp,"%d %d ", length[j], pixels[j]);
	}
	fclose(tmp);
	free(pixels); free(length);
	remove(fileName); rename("tmp.txt",fileName);
	printf("\n%d RENGI YERINE %d YAZILDI\n",firstColor, secondColor);
}


//SIKISTIRILMIS VERIYI ACAR PGM FORMATINDA YAZAR
void RLEtoPGM(char* fileName){
	
	printf("\n\n********DECODE ISLEMI GERCEKLESTIRILIYOR********\n\n");
	int width, heigth, total_pixels=0,j,k;
	//DOSYA ACILMAYA UYGUN OZELLIKTE MI KONTROL EDILIR
	FILE *rle = fopen(fileName,"rb");
	fscanf(rle,"%d%*c%d%*c",&width, &heigth);
	int s = width*heigth;
	int *pixels, *length;
	pixels = (int*) malloc(sizeof(int)*s);
	length = (int*) malloc(sizeof(int)*s);

	int i=0,check=2; 
	while(!feof(rle)){
		check = fscanf(rle,"%d%*c%d%*c", &length[i], &pixels[i]);
		if((pixels[i]>255 || pixels[i]<0) && check==2){
			printf("HATA: RENK ARALIGI 0-255 OLMALIDIR!\n");
			fclose(rle);
			return;
		}
		if(i>0){
			if(pixels[i] == pixels[i-1] && check == 2){
				printf("HATA: YANLIS SIKISTIRILMIS DOSYA\nAYNI RENK ARKA ARKA ARAKYA YAZILAMAZ!\n");
				fclose(rle);
				return;
			}
		}
		if(check==2) total_pixels = total_pixels + length[i];
		i++;
	}

	if(total_pixels != width*heigth){
		printf("HATA: PIKSEL SAYISI ESLESMIYOR!\n");
		fclose(rle);
		return;
	}
	fclose(rle);
	
	//ACILMIS PGM DOSYASI OLUSTURULUR
	FILE *pgm = fopen("test_decoded.pgm","wb");
	fprintf(pgm,"P2\n%d %d\n255\n", width, heigth);
	int newline=0;
	for(j=0;j<i-1;j++){
		for(k=0;k<length[j];k++){
			if(newline==width){	
				fprintf(pgm,"\n");
				printf("\n");
				newline=0;
			}	
			fprintf(pgm,"%d ",pixels[j]);
			printf("%d ",pixels[j]);
			newline++;
		}
	}
	
	fclose(pgm); free(pixels); free(length);
	printf("\n\n********DECODE ISLEMI BASARILI!********\n");
	
	
}

//input.pgm dosyasýný rle formatýnda sýkýstýrýr. pgm_information structý döndürür.
pgm_information readInfo(){
	char c1,c2;
	int i,j;
	
	pgm_information info;
	// PGM DOSYASINI AÇAR VE DOSYA TÜRÜNÜ KONTROL EDER
	FILE *pgm = fopen("input.pgm","rb");
	if (pgm == NULL){
        printf("HATA: DOSYA BULUNAMADI!\n");
    }
    else {
        fscanf(pgm, "%c%c", &c1, &c2);
        if (c1 != 'P' || c2 != '2') {
            printf("HATA: Dosya tipi PGM-P2 DEGIL!\n");
            fclose(pgm);
		}
	}
	//BILGILER OKUNUR 
	printf("\n********input.pgm DOSYASINDAN OKUNAN DEGERLER********\n");
    fscanf(pgm, "%d%*c%d%*c%d%*c", &info.width, &info.heigth, &info.max_value);       
	printf("%d %d", info.width, info.heigth);

	
	info.pixels = (int**)malloc(sizeof(int*)*info.heigth);
	for(i=0;i<info.heigth;i++){
		info.pixels[i] = (int*)malloc(sizeof(int)*info.width);
	}
	

	for(i=0;i<info.heigth;i++){
		printf("\n");
		for(j=0;j<info.width;j++){
			fscanf(pgm, "%d%*c", &info.pixels[i][j]);
			printf("%d ", info.pixels[i][j]);
		}
	}
	printf("\n\n");
	fclose(pgm);
	return info;	
}

//pgm_information structýndaki bilgileri kullanarak sýkýstýrýlmýs dosyayý olusturur.
void encode(pgm_information info){
	//SIKISTIRILMIS BILGI test_encoded.txt DOSYASINA YAZILIR
	printf("\n********ENCODE ISLEMI SONUCU********\n\n");
	FILE *rle = fopen("test_encoded.txt","wb");	
	fprintf(rle,"%d %d\n", info.width, info.heigth);
	int count = 1,i,j;	
	for(i=0;i<info.heigth;i++){
		for(j=0;j<info.width-1;j++){
			if(info.pixels[i][j] == info.pixels[i][j+1])
				count++;
			else{
			fprintf(rle, "%d %d ",count, info.pixels[i][j]);
			printf( "%d %d ",count, info.pixels[i][j]);
			count = 1;
			}
		}
		if(i != info.heigth-1){
			if(info.pixels[i][j] == info.pixels[i+1][0])
				count++;
			else{
				fprintf(rle, "%d %d ",count, info.pixels[i][j]);
				printf( "%d %d ",count, info.pixels[i][j]);
				count = 1;	
			}
		}
		else{
			fprintf(rle, "%d %d ",count, info.pixels[i][j]);
			printf( "%d %d ",count, info.pixels[i][j]);	
		}
	}
	printf("\n");
	fclose(rle);	
}

