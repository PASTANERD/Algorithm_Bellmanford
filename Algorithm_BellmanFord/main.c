#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_LEN 20
#define DST_LEN 10
//#define NIL
#define INF 10000 // maximum integer number

// vertex
typedef struct _vertex{
    int id;
    int value;
    struct _vertex *next;
    
} vertex;

//edge
typedef struct _edge{
    int u;
    int v;
    int weight;
}edge;

// graph
typedef struct _graph{
    int V;
    int E;
    int **wgt_all;
    char *content[MAX_LEN];
    struct _vertex **Vertex; // Vertex List
    struct _edge **Edge;     // Edge List
}graph;


void LoadData(void **argument, graph *G);
void AddLink(vertex *Adj, int id);
void LinkCheck(vertex *Adj);

void INIT_SINGLE_SOURCE(graph *G, vertex *s);
void Relax(vertex *u, vertex *v, int w);

void BellMan_Ford(graph *G, vertex *s);

void TestGraph(graph *Graph);

char **path;
int flag = 0;
int distance[MAX_LEN][MAX_LEN];
clock_t start ,end;



int main(int argc, const char* argv[]){
    
    graph G;
    LoadData(*(argv+1), &G);
    printf("Testing Graph now...\n");
    TestGraph(&G);
    printf("\n\n");
    
    
    
    
    printf("BellMan-Ford Algorithm is processing now...\n");
    start = clock();
    for(int i = 0; i < G.V ; i++){
        BellMan_Ford(&G, *(G.Vertex+i));
        if(flag == 0) {
            printf("The Graph has negative weighted edge.\n");
            printf("from %d graph doesn't work\n", (*(G.Vertex+i))->id);
        }
    }
    end = clock();
    printf("It took %.5f seconds to compute shortest path between cities with Bellman Ford algorithm as follows.\n", ((float)(end-start))/1000);
    printf("Bellman Ford Algorithm result ::\n");
    printf("\t\t\t");
    for(int i = 0; i< G.V ; i++){
        printf("\t%s",G.content[i]);
    }
    printf("\n");
    for(int i = 0 ; i < G.V ; i++){
        printf("%10s\t\t", G.content[i]);
        for(int j = 0 ; j < G.V ; j++){
            printf("%4d \t", distance[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
    
    
}



void BellMan_Ford(graph *G, vertex *s){
    INIT_SINGLE_SOURCE(G, s);
    vertex *u;
    vertex *v;
    int w;
    for(int i = 0; i < G->V-1 ; i++){
        for(int j = 0; j < G->E ; j++){
            u = (*(G->Vertex+(*(G->Edge+j))->u));
            v = (*(G->Vertex+(*(G->Edge+j))->v));
            w = (*(G->Edge+j))->weight;
            Relax(u, v, w);
        }
    }
    for(int i = 0; i < G->E ; i++){
        u = (*(G->Vertex+(*(G->Edge+i))->u));
        v = (*(G->Vertex+(*(G->Edge+i))->v));
        w = (*(G->Edge+i))->weight;
        if(v->value > (u->value + w)){
            flag = 0;
        }
    }
    
    for(int i = 0 ; i < G->V ; i++){
        distance[s->id][i] = (*(G->Vertex+i))->value;
    }
    
    flag = 1;
}



void INIT_SINGLE_SOURCE(graph *G, vertex *s){
    for(int i = 0 ; i < G->V ; i++){
        (*(G->Vertex+i))->value = INF;
        path[s->id][i] = '@';
    }
    s->value = 0;
}


void Relax(vertex *u, vertex *v, int w){
    if(v->value > u->value + w){
        v->value = u->value + w;
        *(*(path+u->id)+v->id) = (u->id)+48;
    }
}


void AddLink(vertex *Adj, int id) {
    vertex * finder = Adj;
    while (finder->next != NULL) {
        finder = finder->next;
    }
    
    /* now we can add a new variable */
    finder->next = malloc(sizeof(*finder->next));
    finder->next->id = id;
    finder->next->value = INF;
    finder->next->next = NULL;
}

void LinkCheck(vertex *Adj){
    vertex * seeker = Adj;
    while (seeker->next != NULL) {
        seeker = seeker->next;
        printf("%d", seeker->id);
    }
}


void TestGraph(graph *G){
    
    printf("Adjacency Check : \n");
    for(int i = 0; i < G->V ; i++){
        printf("Adj[%d] = {", G->Vertex[i]->id);
        LinkCheck(*(G->Vertex+i));
        printf("}\n");
    }
    printf("\n");
    printf("Edge Check : \n");
    for(int i = 0; i < G->E ; i++){
        printf("w(%d, %d) = %d ",(*(G->Edge+i))->u, (*(G->Edge+i))->v, (*(G->Edge+i))->weight);
        printf("\n");
    }
    printf("Graph Loading Completed.\n");
    printf("\n");
    
    
    printf("Weight Matrix::\n");
    for(int i = 0 ; i < G->V ; i++){
        for(int j = 0 ; j < G->V ; j++){
            printf("%5d\t", G->wgt_all[i][j]);
        }
        printf("\n");
    }
    
    
}

void LoadData(void **argument, graph *G){
    int *buffer_Edge[MAX_LEN];
    int *buffer_Weight[MAX_LEN];
    
    for (int i = 0; i < MAX_LEN ; i++){
        *(buffer_Edge+i) = (int*)malloc(sizeof(*(buffer_Edge+i))*MAX_LEN);
        *(buffer_Weight+i) = (int*)malloc(sizeof(*(buffer_Weight+i))*MAX_LEN);
        *(G->content+i) = (char*)malloc(sizeof(*(G->content+i))*MAX_LEN);
        for(int j = 0; j < MAX_LEN ; j++){
            *(*(buffer_Edge+i)+j) = 0;
            *(*(buffer_Weight+i)+j) = 0;
        }
    }
    
    FILE *stream;
    
    int i = -1;     // index of row prevent first row of data
    int j = -1;      // index of column
    int k = 0;      // for weight of edges
    
    int count = 0;  // count edges;
    int rowSize = 0; // check the length of column
    
    char temp;   // each character of data
    char str_dump[DST_LEN];
    
    
    // Read input file
    if((stream = fopen((char*)argument, "r")) == NULL){
        printf("file doens't exist\n");
        exit(1);
    }
    
    
    else {
        printf("Loading data file...\n");
        
        
        while((temp = fgetc(stream)) != EOF){ //read file until it is end
            if((temp >= 48 && temp<= 57)){   // read the data only integer value
                do{ // if it is decimal?
                    str_dump[k] = temp; // put it to dump
                    k++;
                    temp = fgetc(stream);
                }while((temp != 9) && (temp != 32) && (temp != 13)); // next column?
                str_dump[k] = '\0';
                *(*(buffer_Weight+i)+j) = atoi(str_dump);  // put dump to weight as integer
                *(*(buffer_Edge+i)+j) = 1; // from v[i] to v[j] has edge
                if(*(*(buffer_Weight+i)+j) == 0){
                    *(*(buffer_Edge+i)+j) = 0;
                }
                else count++;
                j++;     // count index of column
                k = 0; // reset count number for weight;
                
            }
            else if (temp >= 65 && temp <= 90){ // read the data INF node or not
                do{ // put character into string
                    str_dump[k] = temp; // put it to dump
                    k++;
                    temp = fgetc(stream);
                }while((temp != 9) && (temp != 32) && (temp != 13)); // next column?
                
                str_dump[k] = '\0';
                if(i == -1){
                    strcpy((*(G->content+j+1)), str_dump);
                }
                if(strcmp(str_dump, "INF") == 3){
                    *(*(buffer_Weight+i)+j) = INF;  // put dump to weight as integer
                    
                }
                k = 0;
                j++;
            }
            
            if(temp == 10){ // new line
                j = -1;      // reset index of column
                i++;        // bufferacency of next vertex
            }
            
            str_dump[0] = '\0';
        }
    }
    
    rowSize = i;
    
    G->V = rowSize;
    G->E = count;
    int flag_box = 0;
    do{
        flag_box = 0;
        G->Vertex = (vertex**)malloc(sizeof(**G->Vertex)*rowSize);
        k = 0;
        for (i = 0; i < G->V ; i++){
            G->Vertex[i] = (vertex*)malloc(sizeof(*(G->Vertex[i])));
            G->Vertex[i]->id = i;
            G->Vertex[i]->value = INF;
            G->Vertex[i]->next = NULL;
            
            for(int j = 0; j < G->V ; j++){
                if((*(*(buffer_Edge+i)+j))!= 0) {
                    AddLink(*(G->Vertex+i), j);
                    k++;
                }
            }
            if(G->Vertex[i]->id != i) flag_box++;
            k =0;
            
        }
    }while(flag_box != 0);
    
    k = 0;
    
    
    path = (char**)malloc(sizeof(char)*rowSize);
    
    do{
        flag_box = 0;
        G->Edge = (edge**)malloc(sizeof(edge)*count);
        for (i = 0; i < rowSize ; i++){
            path[i] = (char*)malloc(sizeof(char)*rowSize);
            
            for(j = 0; j < rowSize ; j++){
                if((*(*(buffer_Edge+i)+j))!= 0){
                    G->Edge[k] = (edge*)malloc(sizeof(edge));
                    G->Edge[k]->u = i;
                    G->Edge[k]->v = j;
                    G->Edge[k]->weight = *(*(buffer_Weight+i)+j);
                    
                    if(G->Edge[k]->u > G->V || G->Edge[k]->v > G->V || G->Edge[k]->weight > INF){
                        flag_box++;
                    }
                    k++;
                }
            }
        }
        k = 0;
    }while(flag_box);
    
    flag_box = 0;
    
    do{
        flag_box = 0;
        G->wgt_all = (int**)malloc(sizeof(int)*G->V);
        for(i = 0 ; i < G->V ; i++){
            G->wgt_all[i] = (int*)malloc(sizeof(int)*G->V);
        }
        for(i = 0 ; i < G->V ; i++){
            G->wgt_all[i] = (int*)malloc(sizeof(int)*G->V);
            for(j = 0 ; j < G->V ; j++){
                G->wgt_all[i][j] = buffer_Weight[i][j];
                if(G->wgt_all[i][j] == 0){
                    G->wgt_all[i][j] = INF;
                    if(i == j){
                        G->wgt_all[i][j] = 0;
                    }
                }
                if(G->wgt_all[i][j] > INF){
                    flag_box++;
                }
                
            }
        }
    }while(flag_box != 0);
    
    
    for (i = 0; i < MAX_LEN ; i++){
        free(*(buffer_Edge+i));
        free(*(buffer_Weight+i));
    }
    
}


