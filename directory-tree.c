#include <stdio.h>
#include <stdlib.h>

typedef struct DIRECTORY{
	char name[20];
	struct DIRECTORY *left;
	struct DIRECTORY *right;
	struct DIRECTORY *pre;
}DIRECTORY;

void mkdir(DIRECTORY *parent, char *name);
DIRECTORY *chdir(DIRECTORY *current, char *name);
void dir(DIRECTORY *dir);
void count(DIRECTORY *current);
void rmdir(DIRECTORY ** parent, char *name);
void free_directory(DIRECTORY *node);

int main(int argc, char *argv[]) {

	char name[20] = "root directory";
	DIRECTORY *root;
	create_directory(&root, name);

	DIRECTORY *current_directory = root;
	
	int exit = 0;
	char command[10], command_line[35];	
	
	while(exit == 0){
		name[0] = "";
		printf("%s >",current_directory->name);
		gets(command_line);
		sscanf(command_line,"%s %s",command, name);
		
		//scanf("%s %s",komut,name);
		
		if(!strcmp(command,"mkdir")) mkdir(current_directory,name);
		else if(!strcmp(command,"chdir")) current_directory = chdir(current_directory,name);
		else if(!strcmp(command,"rmdir")) rmdir(&current_directory,name);
		else if(!strcmp(command,"dir")) dir(current_directory);
		else if(!strcmp(command,"count")) count(current_directory);
		else if(!strcmp(command,"q")) exit = 1;
		else printf(">invalid command\n");	
	}
	
	while(current_directory->pre != NULL){
		current_directory = chdir(current_directory,"..");
	}
	free_directory(current_directory);

	return 0;
}

void create_directory(DIRECTORY **node, char *name){
	*node = (DIRECTORY*)malloc(1*sizeof(DIRECTORY));
	if(*node==NULL){
		exit(1);
	}
	(*node)->left = NULL;
	(*node)->right = NULL;
	(*node)->pre = NULL;
	strcpy((*node)->name,name);
}

void mkdir(DIRECTORY *parent, char *name){
	
	if(!strcmp(name,"")) return;
	
	if(parent->left == NULL){
		
		create_directory(&(parent->left),name);	
		parent->left->pre = parent;
	}
	else{
		parent = parent->left;
		while(parent->right != NULL){
			if(!strcmp(name, parent->name)){
				printf(">%s directory is alredy exist!\n",name);
				return;
			}			
			parent = parent->right;
		}
		if(!strcmp(name, parent->name)){
			printf(">%s directory is alredy exist!\n",name);
			return;
		}
		create_directory(&(parent->right), name);
		parent->right->pre = parent;
	}
	
}

DIRECTORY *chdir(DIRECTORY *parent, char *name){
	
	if(!strcmp(name,"..") && parent->pre!=NULL){
		while(parent->pre->right == parent){
			parent = parent->pre;
		}
		parent = parent->pre;
	}
	else{
		parent = parent->left;
		while(parent->right != NULL && strcmp(parent->name,name)){
			parent = parent->right;
		}
		if(!strcmp(name,parent->name)){
			parent = parent;
		}
		else printf(">can’t change directory. The directory does not exist!\n");	
	}
	return parent;
}



void rmdir(DIRECTORY ** parent, char *name){
	
	DIRECTORY *tmp = *parent;
	
	tmp = tmp->left;
	while(tmp->right != NULL && strcmp(tmp->name,name)){
		tmp = tmp->right;
	}

	if(!strcmp(name,tmp->name)){
		if(tmp->left != NULL){
			printf(">can’t remove directory. The directory must be empty\n");
			return;
		}
		
		if(tmp->pre->left == tmp){
			if(tmp->right == NULL){
				tmp->pre->left = NULL;
			}
			else{
				tmp->pre->left = tmp->right;
				tmp->right->pre = tmp->pre;				
			}

		}
		else if(tmp->right==NULL){
			tmp->pre->right = NULL;
		}
		else{
			tmp->pre->right = tmp->right;
			tmp->right->pre = tmp->pre;
		}
	free(tmp);
	}
	else printf(">can’t remove directory. The directory does not exist!\n");
		
	
}

void dir(DIRECTORY *parent){
	
	if(parent->left == NULL){
		printf(">there is no directory!\n");
	}
	else{
		parent = parent->left;
		printf(">");
		while(parent != NULL){
			printf("\t%s\n",parent->name);
			parent=parent->right;
		}
	}
}

void count(DIRECTORY *parent){
	int c = 0;
	
	if(parent->left == NULL){
		printf(">there is no directory!\n");
	}
	else{
		parent = parent->left;
		while(parent != NULL){
			parent=parent->right;
			c++;
		}		
	}
	printf(">%d\n",c);
}

void free_directory(DIRECTORY *node){
	if(node->left != NULL) free_directory(node->left);
	if(node->right != NULL) free_directory(node->right);
	free(node);
}
