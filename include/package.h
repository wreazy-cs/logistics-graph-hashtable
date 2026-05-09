/* ============================================================
 * package.h
 * Paket (Kargo) veri yapisinin tanimi
 * ============================================================ */
#ifndef PACKAGE_H
#define PACKAGE_H

typedef struct Package {
    char id[32];            /* Paket numarasi (anahtar)        */
    char sender[64];        /* Gonderici                       */
    char receiver[64];      /* Alici                           */
    char origin_city[32];   /* Cikis sehri                     */
    char dest_city[32];     /* Varis sehri                     */
    float weight;           /* Agirlik (kg)                    */
    char status[32];        /* Yolda / Teslim Edildi / vb.     */
} Package;

Package* create_package(const char* id, const char* sender, const char* receiver,
                        const char* origin, const char* dest,
                        float weight, const char* status);
void print_package(const Package* p);
void free_package(Package* p);

#endif
