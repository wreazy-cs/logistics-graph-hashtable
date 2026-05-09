/* ============================================================
 * main.c
 * Lojistik Yonetim Sistemi - Hibrit Graf + Hash Table
 * ------------------------------------------------------------
 * Hash : Tip B (Polynomial Rolling) + Zincirleme
 * Graf : Komsuluk listesi + DFS gezinme
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/package.h"
#include "../include/hash_table.h"
#include "../include/graph.h"

static void print_menu(void)
{
    printf("\n========== LOJISTIK YONETIM SISTEMI ==========\n");
    printf(" 1. Sehir ekle\n");
    printf(" 2. Rota (yol) ekle\n");
    printf(" 3. Paket ekle\n");
    printf(" 4. Paket ara (ID ile)\n");
    printf(" 5. Paket sil\n");
    printf(" 6. Tum paketleri listele\n");
    printf(" 7. Grafi yazdir\n");
    printf(" 8. DFS ile graf gezinme\n");
    printf(" 9. BELLEK IZLEME RAPORU\n");
    printf("10. DEMO veri yukle (5+ paket, 6 sehir)\n");
    printf(" 0. Cikis\n");
    printf("Secim: ");
}

static void read_line(char* buf, int sz)
{
    if (fgets(buf, sz, stdin) == NULL) { buf[0] = '\0'; return; }
    buf[strcspn(buf, "\r\n")] = '\0';
}

static void load_demo(Graph* g, HashTable* ht)
{
    printf("\n>>> DEMO veri yukleniyor...\n");

    /* Sehirler */
    add_city(g, "Istanbul");
    add_city(g, "Ankara");
    add_city(g, "Izmir");
    add_city(g, "Bursa");
    add_city(g, "Konya");
    add_city(g, "Antalya");

    /* Rotalar */
    add_route(g, "Istanbul", "Ankara",  450);
    add_route(g, "Istanbul", "Bursa",   150);
    add_route(g, "Ankara",   "Konya",   260);
    add_route(g, "Konya",    "Antalya", 290);
    add_route(g, "Izmir",    "Antalya", 450);
    add_route(g, "Bursa",    "Izmir",   330);

    /* Paketler (>= 5) */
    hash_insert(ht, "PKT001",
        create_package("PKT001","Ahmet","Mehmet","Istanbul","Ankara",2.5f,"Yolda"));
    hash_insert(ht, "PKT002",
        create_package("PKT002","Ayse","Fatma","Izmir","Antalya",1.2f,"Hazirlaniyor"));
    hash_insert(ht, "PKT003",
        create_package("PKT003","Ali","Veli","Bursa","Konya",5.0f,"Yolda"));
    hash_insert(ht, "PKT004",
        create_package("PKT004","Zeynep","Hasan","Ankara","Izmir",0.8f,"Teslim Edildi"));
    hash_insert(ht, "PKT005",
        create_package("PKT005","Murat","Selin","Antalya","Istanbul",3.3f,"Yolda"));
    hash_insert(ht, "PKT006",
        create_package("PKT006","Emre","Deniz","Konya","Bursa",1.7f,"Yolda"));
    hash_insert(ht, "PKT007",
        create_package("PKT007","Cem","Ece","Istanbul","Izmir",4.4f,"Hazirlaniyor"));
    /* Asagidaki iki kayit MUTLAKA cakisma uretir (linear probing tetiklenir): */
    hash_insert(ht, "PKT016",
        create_package("PKT016","Burak","Sevim","Bursa","Antalya",2.1f,"Yolda"));
    hash_insert(ht, "PKT018",
        create_package("PKT018","Kaan","Pinar","Ankara","Bursa",3.8f,"Yolda"));

    printf(">>> Demo veri yuklendi.\n");
}

int main(void)
{
    Graph*      graph = create_graph();
    HashTable*  ht    = create_hash_table();
    int         choice;
    char buf1[64], buf2[64], buf3[64], buf4[64], buf5[64], num[16];
    int   distance;
    float weight;
    Package* p;

    printf("\n=========================================================\n");
    printf("  LOJISTIK YONETIM SISTEMI  -  Graf + Hash Table (Hibrit)\n");
    printf("  Hash      : Tip B (Polynomial Rolling)\n");
    printf("  Cakisma   : Zincirleme (Chaining)\n");
    printf("  Gezinme   : DFS\n");
    printf("=========================================================\n");

    for (;;) {
        print_menu();
        if (!fgets(num, sizeof(num), stdin)) break;
        choice = atoi(num);

        switch (choice) {
            case 1:
                printf("Sehir adi: "); read_line(buf1, sizeof(buf1));
                add_city(graph, buf1);
                break;
            case 2:
                printf("Baslangic sehri: "); read_line(buf1, sizeof(buf1));
                printf("Hedef sehir    : "); read_line(buf2, sizeof(buf2));
                printf("Mesafe (km)    : "); read_line(num, sizeof(num));
                distance = atoi(num);
                add_route(graph, buf1, buf2, distance);
                break;
            case 3:
                printf("Paket ID    : "); read_line(buf1, sizeof(buf1));
                printf("Gonderici   : "); read_line(buf2, sizeof(buf2));
                printf("Alici       : "); read_line(buf3, sizeof(buf3));
                printf("Cikis sehri : "); read_line(buf4, sizeof(buf4));
                printf("Hedef sehir : "); read_line(buf5, sizeof(buf5));
                printf("Kilo (kg)   : "); read_line(num,  sizeof(num));
                weight = (float)atof(num);
                hash_insert(ht, buf1,
                    create_package(buf1, buf2, buf3, buf4, buf5, weight, "Yolda"));
                break;
            case 4:
                printf("Aranacak Paket ID: "); read_line(buf1, sizeof(buf1));
                p = hash_search(ht, buf1);
                if (p) { printf("\n>>> PAKET BULUNDU:\n"); print_package(p); }
                else   printf("Paket bulunamadi.\n");
                break;
            case 5:
                printf("Silinecek Paket ID: "); read_line(buf1, sizeof(buf1));
                if (hash_delete(ht, buf1) == 0) printf("Silindi.\n");
                else                            printf("Bulunamadi.\n");
                break;
            case 6: hash_print(ht);  break;
            case 7: print_graph(graph); break;
            case 8:
                printf("Baslangic sehri: "); read_line(buf1, sizeof(buf1));
                dfs_traverse(graph, buf1);
                break;
            case 9:
                hash_print_memory_report(ht);
                print_graph_memory_report(graph);
                break;
            case 10: load_demo(graph, ht); break;
            case 0:
                free_hash_table(ht);
                free_graph(graph);
                printf("Cikis yapildi.\n");
                return 0;
            default:
                printf("Gecersiz secim.\n");
        }
    }

    free_hash_table(ht);
    free_graph(graph);
    return 0;
}
