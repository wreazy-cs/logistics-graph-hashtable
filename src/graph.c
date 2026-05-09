/* ============================================================
 * graph.c
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/graph.h"

Graph* create_graph(void)
{
    int i;
    Graph* g = (Graph*)malloc(sizeof(Graph));
    if (!g) return NULL;

    g->city_count = 0;
    for (i = 0; i < MAX_CITIES; i++) {
        g->cities[i].name[0] = '\0';
        g->cities[i].edges   = NULL;
        g->cities[i].visited = 0;
    }
    printf("  [malloc] Graph olusturuldu @ %p\n", (void*)g);
    return g;
}

int find_city_index(const Graph* g, const char* name)
{
    int i;
    for (i = 0; i < g->city_count; i++)
        if (strcmp(g->cities[i].name, name) == 0) return i;
    return -1;
}

int add_city(Graph* g, const char* name)
{
    int idx;
    if (!g || !name) return -1;
    if (find_city_index(g, name) >= 0) {
        printf("  [!] Sehir zaten var: %s\n", name);
        return -1;
    }
    if (g->city_count >= MAX_CITIES) return -1;

    idx = g->city_count++;
    strncpy(g->cities[idx].name, name, CITY_NAME_LEN - 1);
    g->cities[idx].name[CITY_NAME_LEN - 1] = '\0';
    g->cities[idx].edges   = NULL;
    g->cities[idx].visited = 0;

    printf("  [+] Sehir eklendi: %s (index=%d, adres=%p)\n",
           name, idx, (void*)&g->cities[idx]);
    return idx;
}

int add_route(Graph* g, const char* from, const char* to, int distance)
{
    int from_idx, to_idx;
    EdgeNode *e1, *e2;

    if (!g) return -1;
    from_idx = find_city_index(g, from);
    to_idx   = find_city_index(g, to);
    if (from_idx < 0 || to_idx < 0) {
        printf("  [!] Sehir bulunamadi: %s veya %s\n", from, to);
        return -1;
    }

    /* Yonsuz graf -> her iki yone de kenar */
    e1 = (EdgeNode*)malloc(sizeof(EdgeNode));
    e1->dest_index = to_idx;
    e1->distance   = distance;
    e1->next = g->cities[from_idx].edges;
    g->cities[from_idx].edges = e1;

    e2 = (EdgeNode*)malloc(sizeof(EdgeNode));
    e2->dest_index = from_idx;
    e2->distance   = distance;
    e2->next = g->cities[to_idx].edges;
    g->cities[to_idx].edges = e2;

    printf("  [+] Rota: %s <-> %s (%d km) | edges @ %p, %p\n",
           from, to, distance, (void*)e1, (void*)e2);
    return 0;
}

/* ============================================================
 *  GRAF GEZINME - DFS (Depth-First Search)
 * ------------------------------------------------------------
 *  Recursive yapi: gidilebilen en derin dugume in, sonra geri
 *  donerek diger kollari ziyaret et. Stack (yigit) cagrı yigi-
 *  ti uzerinden ortuk olarak kullanilir.
 * ============================================================ */
static void dfs_recursive(Graph* g, int idx, int depth)
{
    EdgeNode* e;
    int i;

    g->cities[idx].visited = 1;
    for (i = 0; i < depth; i++) printf("  ");
    printf("-> %s (idx=%d)\n", g->cities[idx].name, idx);

    e = g->cities[idx].edges;
    while (e) {
        if (!g->cities[e->dest_index].visited)
            dfs_recursive(g, e->dest_index, depth + 1);
        e = e->next;
    }
}

void dfs_traverse(Graph* g, const char* start_city)
{
    int i, start;
    if (!g) return;

    start = find_city_index(g, start_city);
    if (start < 0) {
        printf("  [!] Baslangic sehri bulunamadi: %s\n", start_city);
        return;
    }
    /* Visited bayraklarini sifirla */
    for (i = 0; i < g->city_count; i++) g->cities[i].visited = 0;

    printf("\n--- DFS GEZINME (baslangic: %s) ---\n", start_city);
    dfs_recursive(g, start, 0);
}

void print_graph(const Graph* g)
{
    int i;
    EdgeNode* e;

    printf("\n--- GRAF (Komsuluk Listesi) ---\n");
    for (i = 0; i < g->city_count; i++) {
        printf("%-12s : ", g->cities[i].name);
        e = g->cities[i].edges;
        if (!e) printf("(bos)");
        while (e) {
            printf("-> %s(%dkm) ",
                   g->cities[e->dest_index].name, e->distance);
            e = e->next;
        }
        printf("\n");
    }
}

void print_graph_memory_report(const Graph* g)
{
    int i, edge_total = 0;
    EdgeNode* e;

    printf("\n========================================================\n");
    printf("              BELLEK IZLEME RAPORU - GRAPH                \n");
    printf("========================================================\n");
    printf(" Graph adresi : %p\n", (void*)g);
    printf(" Sehir sayisi : %d / %d\n", g->city_count, MAX_CITIES);

    printf("\n--- VERTEX -> EDGE BAGLANTILARI ---\n");
    for (i = 0; i < g->city_count; i++) {
        printf("[%d] %-12s @ %p | edges-head=%p\n",
               i, g->cities[i].name,
               (void*)&g->cities[i], (void*)g->cities[i].edges);
        e = g->cities[i].edges;
        while (e) {
            printf("    -> Edge @ %p | dest=%-10s | %3d km | next=%p\n",
                   (void*)e, g->cities[e->dest_index].name,
                   e->distance, (void*)e->next);
            e = e->next;
            edge_total++;
        }
    }
    printf("\n Toplam kenar (cift yonlu sayim): %d\n", edge_total);
    printf("--------------------------------------------------------\n");
}

void free_graph(Graph* g)
{
    int i;
    EdgeNode *curr, *next;
    if (!g) return;
    for (i = 0; i < g->city_count; i++) {
        curr = g->cities[i].edges;
        while (curr) {
            next = curr->next;
            free(curr);
            curr = next;
        }
    }
    printf("  [free]   Graph serbest birakildi @ %p\n", (void*)g);
    free(g);
}
