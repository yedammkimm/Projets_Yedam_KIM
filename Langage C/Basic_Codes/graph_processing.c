#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// Nombre maximum de sommets
#define MAXV 100


struct edgenode {
 int y; // le voisin
 struct edgenode *next; // la suite de la liste
};
typedef struct edgenode edgenode;


struct graph {
 edgenode *edges[MAXV]; // tableau de listes d'adjacence
 int degree[MAXV]; // le degré de chaque sommet
 int nvertices;
 int nedges;
 bool directed; // indique si le graphe est orienté
 bool discovered[MAXV]; // Quels sommets sont connus
 bool processed[MAXV]; // Quels sommets sont traités
 int parent[MAXV]; // parent[x] est le père de x dans le parcours
                   // s'il n'y en a pas, c'est -1
};
typedef struct graph graph;




void initialize_graph(graph *g, int n, bool directed) {
 g->directed = directed;
 g->nvertices = n;
 g->nedges = 0;
 for(int i = 0; i < n-1; i++){
   g->parent[i] = -1;
   g->edges[i] = NULL;
   g->discovered[i] = false;
   g->processed[i] = false;
   g->degree[i] = 0;
 }
}


void insert_edge(graph *g, int x, int y) {
   edgenode* e = (edgenode*)malloc(sizeof(edgenode));
   e->y = y;
   e->next = NULL; 
   while(g->edges[x]->next != NULL){
     g->edges[x] = g->edges[x]->next;
 }
 g->edges[x]->next = e;
 if (!g->directed){
   edgenode* e1 = (edgenode*)malloc(sizeof(edgenode));
   e->y = x;
   e->next = NULL; 
   while(g->edges[y]->next != NULL){
     g->edges[y] = g->edges[y]->next;
 }
 g->edges[y]->next = e1;
}
}




void read_graph(graph *g) {
 int n, m, d;
 scanf("%d %d %d", &n, &m, &d);
 initialize_graph(g, n, d);
 g->directed = (d == 1);
 for (int i = 0; i < m; i++) {
   int x, y;
   scanf("%d %d", &x, &y);
   insert_edge(g, x, y);
 }
}


void free_edge_next(edgenode* e){
 if(e != NULL){
   free_edge_next(e->next);
 }
   free(e);
}
void free_edges(graph *g) {
 for (int i = 0; i < g->nvertices -1; i++){
   while(g->edges[i] != NULL){
     free_edge_next(g->edges[i]);
   }   
 }
}


void initialize_search(graph *g) {
   for(int i = 0; i < g->nvertices; i++){
   g->parent[i] = -1;
   g->discovered[i] = false;
   g->processed[i] = false;
 }
}




void process_vertex_early(graph *g, int v) {
 printf("[start] processing vertex %d\n", v);
}


void process_vertex_late(graph *g, int v) {
 printf("[done] processing vertex %d\n", v);
}


void process_edge(graph *g, int x, int y) {
 printf("* processing edge %d --> %d\n", x, y);
}


void dfs(graph *g, int x) {
  // À compléter
}


void all_dfs(graph *g) {
  // À compléter
}


int main(void) {
 graph* g = (graph*)malloc(sizeof(graph));
 read_graph(g);
 printf("The size of the graph: %d",g->nvertices);
 printf("The number of edges: %d",g->nedges);
 free_edges(g);
 free(g);
}






