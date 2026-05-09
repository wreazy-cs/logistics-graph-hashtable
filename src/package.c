/* ============================================================
 * package.c
 * Paket olusturma / yazdirma / silme
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/package.h"

Package* create_package(const char* id, const char* sender, const char* receiver,
                        const char* origin, const char* dest,
                        float weight, const char* status)
{
    Package* p = (Package*)malloc(sizeof(Package));
    if (!p) return NULL;

    strncpy(p->id,          id,       sizeof(p->id)         - 1); p->id[sizeof(p->id)-1] = '\0';
    strncpy(p->sender,      sender,   sizeof(p->sender)     - 1); p->sender[sizeof(p->sender)-1] = '\0';
    strncpy(p->receiver,    receiver, sizeof(p->receiver)   - 1); p->receiver[sizeof(p->receiver)-1] = '\0';
    strncpy(p->origin_city, origin,   sizeof(p->origin_city)- 1); p->origin_city[sizeof(p->origin_city)-1] = '\0';
    strncpy(p->dest_city,   dest,     sizeof(p->dest_city)  - 1); p->dest_city[sizeof(p->dest_city)-1] = '\0';
    strncpy(p->status,      status,   sizeof(p->status)     - 1); p->status[sizeof(p->status)-1] = '\0';
    p->weight = weight;

    printf("  [malloc] Package nesnesi olusturuldu @ %p (id=%s)\n", (void*)p, p->id);
    return p;
}

void print_package(const Package* p)
{
    if (!p) { printf("  (paket bos)\n"); return; }
    printf("  +-------------------------------+\n");
    printf("  | Paket ID  : %-17s |\n", p->id);
    printf("  | Gonderici : %-17s |\n", p->sender);
    printf("  | Alici     : %-17s |\n", p->receiver);
    printf("  | Rota      : %s -> %s\n",  p->origin_city, p->dest_city);
    printf("  | Kilo      : %.2f kg\n",   p->weight);
    printf("  | Durum     : %s\n",        p->status);
    printf("  +-------------------------------+\n");
}

void free_package(Package* p)
{
    if (p) {
        printf("  [free]   Package serbest birakildi @ %p\n", (void*)p);
        free(p);
    }
}
