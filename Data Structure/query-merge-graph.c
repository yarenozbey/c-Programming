//18011022 Yaren Ozbey
#include <stdio.h>
#include <stdlib.h>
#define MAX 100

typedef struct NODE{
	char query[20];
	int num_query;
	struct NODE *next;
}NODE;

typedef struct GRAPH{
	int numVertices;
	struct NODE** adjLists;
}GRAPH;

struct NODE* createNode(char *que){
	struct NODE* newNode = (NODE*)malloc(sizeof(struct NODE));
	strcpy(newNode->query,que);
	newNode->num_query=0;
	newNode->next = NULL;
	newNode->num_query = 1;
	return newNode;	
}

struct GRAPH* createGraph(int n){
	int i;
	struct GRAPH* graph = (GRAPH*)malloc(sizeof(struct GRAPH));
	graph->adjLists = (NODE**)malloc(n * sizeof(struct NODE*));
	
	for(i=0;i<n;i++){
		graph->adjLists[i] = NULL;
	}
	graph->numVertices = n;
	
	return graph;
}

void addEdge(struct GRAPH* graph, char *que1, char *que2){
	
	int i=0;
	
	while( strcmp(graph->adjLists[i]->query,que1) )
		i++;
	
	NODE *tmp = graph->adjLists[i];
	while(tmp->next != NULL)
		tmp = tmp->next;
		
	tmp->next = createNode(que2);
	
	i=0;
	while( strcmp(graph->adjLists[i]->query,que2) )
		i++;
		
	tmp = graph->adjLists[i];
		while(tmp->next != NULL)
		tmp = tmp->next;
		
	tmp->next = createNode(que1);
}


struct GRAPH* mergeGraph(GRAPH* graph1, GRAPH* graph2){
	int i, j, k, totalVertices = graph1->numVertices+graph2->numVertices;
	GRAPH *final = createGraph(totalVertices);
	NODE *tmp;
	char *que;

	for(i=0;i<graph1->numVertices;i++){
		que = graph1->adjLists[i]->query;
		final->adjLists[i] = graph1->adjLists[i];
	}
	final->numVertices = graph1->numVertices;
	for(i=0;i<graph2->numVertices;i++){
		que = graph2->adjLists[i]->query;
		j = 0;
		while( final->adjLists[j] != NULL && strcmp(final->adjLists[j]->query,que) ){			
			j++;
		}
		if(final->adjLists[j] == NULL){
			final->adjLists[j] = graph2->adjLists[i];
			final->numVertices++;
		}
		else{
			tmp = final->adjLists[j];
			while(tmp->next != NULL)
				tmp = tmp->next;
			tmp->next = createNode(graph2->adjLists[j]->next->query);
		}
	}
	return final;	
}


// Print the graph
void printGraph(struct GRAPH* graph) {
  int v;
  for (v = 0; v < graph->numVertices; v++) {
    struct NODE* temp = graph->adjLists[v];
    printf("\n Vertex %s\n: ", graph->adjLists[v]->query);
    while (temp) {
      printf("%s -> ", temp->query);
      temp = temp->next;
    }
    printf("\n");
  }
}


void querying(GRAPH* graph, char* que){
	char first[20], second[20], third[20];
	first[0]  = 32, second[0] = 32, third[0] = 32;
	int i, j, firstNum = 0, secondNum = 0, thirdNum = 0;
	NODE *tmp;
	char **related = (char**)malloc(20 * sizeof(char*));
	
	i = 0;
	while( strcmp(graph->adjLists[i]->query, que) )
		i++;
	if(i==0 && graph->adjLists[i]->next == NULL){
		printf("\nTHE IS NO RELATED SEARCH");
		return;
	}
	
	graph->adjLists[i]->num_query++;
	tmp = graph->adjLists[i]->next;
	
	j = 0;
	while(tmp != NULL){
		related[j] = (char*)malloc(20 * sizeof(char));
		strcpy(related[j], tmp->query);
		tmp = tmp->next;
		j++;
	}

		
	i = 0;
	int k;
	while(i<j){
		k=0;
		while(strcmp(related[i], graph->adjLists[k]))
			k++;
		tmp = graph->adjLists[k];
		
		if(tmp->num_query > firstNum){
			strcpy(first, tmp->query);
			firstNum = tmp->num_query;
		}
		else if(tmp->num_query > secondNum){
			strcpy(second, tmp->query);
			secondNum = tmp->num_query;

		}
		else if(tmp->num_query > thirdNum){
			strcpy(third, tmp->query);
			thirdNum = tmp->num_query;
		}		
		i++;	
	}
	
	printf("\n****** MOST RELATED QUERIES *****");
	if(first[0] != 32)
		printf("\n%s",first);
	if(second[0] != 32)
		printf("\n%s",second);
	if(third[0] != 32)
		printf("\n%s",third);		
	
	//graph->adjLists[i]->query;
	

}

int main(int argc, char *argv[]) {

	int i, j,numOfVertices1=4 , numOfVertices2=5, numEdge1=3, numEdge2=4;
	char que1[20], que2[20];
	struct NODE *node;
	
	//printf("Number of vertices of the first graph: ");
	//scanf("%d",&numOfVertices1);
	GRAPH *graph1 = createGraph(numOfVertices1);

	
	//printf("Number of vertices of the second graph: ");
	//scanf("%d",&numOfVertices2);
	GRAPH *graph2 = createGraph(numOfVertices2);

		
	for(i=0;i<numOfVertices1;i++){
		printf("query of %d. node of graph1: ",i+1);
		scanf("%s",que1);
		graph1->adjLists[i] = createNode(que1);
	}
	
	for(i=0;i<numOfVertices2;i++){
		printf("query of %d. node of graph2: ",i+1);
		scanf("%s",que1);
		graph2->adjLists[i] = createNode(que1);
	}
	
	
	printf("For the first graph: \n");
	for(i=0;i<numEdge1;i++){
		printf("%d. edge\nFrom: ",i+1);
		scanf("%s",que1);
		
		printf("to: ");
		scanf("%s",que2);		
		addEdge(graph1, que1, que2);	
		
	}
	
	printf("For the second graph: \n");
	for(i=0;i<numEdge2;i++){
		printf("%d. edge\nFrom: ",i+1);
		scanf("%s",que1);
		
		printf("to: ");
		scanf("%s",que2);
		addEdge(graph2, que1, que2);	
		
	}
	
		
	printGraph(graph1);
	printf("\n*******************\n");
	printGraph(graph2);
	
	GRAPH *merged_graph = mergeGraph(graph1, graph2);
	
	printf("\n********** MERGED GRAPH *********\n");
	printGraph(merged_graph);
	
	char que[20] = "Look Here";
	printf("\nSEARCH STARTED\ntype q to end the search");
	while(que[0] != 'q'){
		printf("\nSTRING TO QUERY: ");
		scanf("%s",que);
		
		querying(merged_graph, que);
	}
	return 0;
}

