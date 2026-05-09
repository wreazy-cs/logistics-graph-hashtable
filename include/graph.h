/* ============================================================
 * graph.h
 * Sehirler arasi yollarin tutuldugu Graf yapisi
 * ------------------------------------------------------------
 * Gosterim       : Komsuluk listesi (linked list)
 * Gezinme        : DFS (Depth-First Search, Derinlik Onceligi)
 * Yon            : Yonsuz (cift yonlu kenar)
 * ============================================================ */
#ifndef GRAPH_H
#define GRAPH_H

#define MAX_CITIES    50
#define CITY_NAME_LEN 32

/* Komsuluk listesi dugumu (kenar) */
typedef struct EdgeNode {
    int dest_index;          /* Hedef sehrin indeksi    */
    int distance;            /* Mesafe (km)             */
    struct EdgeNode* next;
} EdgeNode;

/* Sehir (vertex / koselerin biri) */
typedef struct City {
    char name[CITY_NAME_LEN];
    EdgeNode* edges;         /* Bu sehirden cikan kenarlar */
    int visited;             /* DFS bayragi               */
} City;

typedef struct Graph {
    City cities[MAX_CITIES];
    int city_count;
} Graph;

/* API */
Graph* create_graph(void);
int    add_city(Graph* g, const char* name);
int    find_city_index(const Graph* g, const char* name);
int    add_route(Graph* g, const char* from, const char* to, int distance);
void   dfs_traverse(Graph* g, const char* start_city);
void   print_graph(const Graph* g);
void   print_graph_memory_report(const Graph* g);
void   free_graph(Graph* g);

#endif
