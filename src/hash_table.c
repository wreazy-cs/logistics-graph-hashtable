/* ============================================================
 * hash_table.c
 * Tip B Hash + Acik Adresleme (Linear Probing)
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hash_table.h"

/* ============================================================
 *  HASH FONKSIYONU - TIP B (Polynomial Rolling Hash)
 * ------------------------------------------------------------
 *  String anahtarlar icin yaygin kullanilan, dengeli dagilim
 *  veren bir hash. Karakterleri sira ile p tabanina gore
 *  carparak biriktirir, sonunda mod m alinir.
 *
 *  Formul:
 *      h(s) = ( s[0]*p^0 + s[1]*p^1 + ... + s[n-1]*p^(n-1) ) mod m
 *
 *  p = 31  (asal, alfabe genisliginden buyuk)
 *  m = TABLE_SIZE (13)
 *
 *  Ornek: "PKT001" -> 'P'(80) + 'K'(75)*31 + 'T'(84)*961 + ...
 * ============================================================ */
unsigned int hash_function_B(const char* key)
{
    unsigned int hash  = 0;
    unsigned int p_pow = 1;
    const unsigned int p = 31;
    int i;

    for (i = 0; key[i] != '\0'; i++) {
        hash  = (hash  + (unsigned char)key[i] * p_pow) % TABLE_SIZE;
        p_pow = (p_pow * p) % TABLE_SIZE;
    }
    return hash;
}

HashTable* create_hash_table(void)
{
    int i;
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) return NULL;

    for (i = 0; i < TABLE_SIZE; i++) {
        ht->slots[i].key[0]  = '\0';
        ht->slots[i].package = NULL;
        ht->slots[i].state   = SLOT_EMPTY;
    }
    ht->item_count      = 0;
    ht->collision_count = 0;
    ht->probe_total     = 0;

    printf("  [malloc] HashTable olusturuldu @ %p (size=%d, ALL slots EMPTY)\n",
           (void*)ht, TABLE_SIZE);
    return ht;
}

/* ============================================================
 *  CAKISMA COZUMU - ACIK ADRESLEME / LINEAR PROBING
 * ------------------------------------------------------------
 *  Hedef slot dolu ise bir sonraki slota bakilir, gerekirse
 *  bir sonrakine, ta ki bos (EMPTY veya DELETED) bir yer
 *  bulununcaya kadar.
 *
 *      probe(i) = (h(key) + i) mod m       (i = 0,1,2,...)
 *
 *  Avantaj : Pointer/dinamik liste yok, cache-dostu, sade.
 *  Dikkat  : Tablo dolarsa insert basarisiz olur. Yuk faktoru
 *            < 0.7 tutulmasi tavsiye edilir.
 * ============================================================ */
int hash_insert(HashTable* ht, const char* key, Package* pkg)
{
    unsigned int home, idx;
    int i;

    if (!ht || !key || !pkg) return -1;
    if (ht->item_count >= TABLE_SIZE) {
        printf("  [!] Tablo DOLU. Insert yapilamaz.\n");
        return -1;
    }

    home = hash_function_B(key);

    /* Linear probe */
    for (i = 0; i < TABLE_SIZE; i++) {
        idx = (home + i) % TABLE_SIZE;
        ht->probe_total++;

        if (ht->slots[idx].state == SLOT_OCCUPIED) {
            /* Ayni anahtar tekrar mi geldi? */
            if (strcmp(ht->slots[idx].key, key) == 0) {
                ht->slots[idx].package = pkg;
                printf("  [~] Anahtar guncellendi: '%s' (slot %u)\n", key, idx);
                return 0;
            }
            /* Cakisma: bir sonraki slota gec */
            ht->collision_count++;
            printf("  [!] CAKISMA: key='%s' home=%u dolu (slot %u)"
                   " -> probe %d\n", key, home, idx, i + 1);
            continue;
        }

        /* SLOT_EMPTY veya SLOT_DELETED -> burayi kullan */
        strncpy(ht->slots[idx].key, key, sizeof(ht->slots[idx].key) - 1);
        ht->slots[idx].key[sizeof(ht->slots[idx].key) - 1] = '\0';
        ht->slots[idx].package = pkg;
        ht->slots[idx].state   = SLOT_OCCUPIED;
        ht->item_count++;

        if (idx == home)
            printf("  [+] Slot[%u] dolduruldu (probe yok) | key='%s'\n", idx, key);
        else
            printf("  [+] Slot[%u] dolduruldu (home=%u, %d adim probe) | key='%s'\n",
                   idx, home, i, key);
        return 0;
    }
    return -1;
}

Package* hash_search(HashTable* ht, const char* key)
{
    unsigned int home, idx;
    int i;

    if (!ht || !key) return NULL;
    home = hash_function_B(key);

    printf("  [arama] '%s' -> home=%u\n", key, home);
    for (i = 0; i < TABLE_SIZE; i++) {
        idx = (home + i) % TABLE_SIZE;
        printf("          probe %d: slot[%u] state=%d\n",
               i, idx, ht->slots[idx].state);

        if (ht->slots[idx].state == SLOT_EMPTY) {
            return NULL;            /* Hic kullanilmamis -> yok */
        }
        if (ht->slots[idx].state == SLOT_OCCUPIED &&
            strcmp(ht->slots[idx].key, key) == 0) {
            return ht->slots[idx].package;
        }
        /* DELETED veya farkli anahtar -> devam */
    }
    return NULL;
}

int hash_delete(HashTable* ht, const char* key)
{
    unsigned int home, idx;
    int i;

    if (!ht || !key) return -1;
    home = hash_function_B(key);

    for (i = 0; i < TABLE_SIZE; i++) {
        idx = (home + i) % TABLE_SIZE;
        if (ht->slots[idx].state == SLOT_EMPTY) return -1;
        if (ht->slots[idx].state == SLOT_OCCUPIED &&
            strcmp(ht->slots[idx].key, key) == 0) {
            if (ht->slots[idx].package) free_package(ht->slots[idx].package);
            ht->slots[idx].package = NULL;
            ht->slots[idx].state   = SLOT_DELETED;  /* tombstone */
            ht->item_count--;
            printf("  [free]   Slot[%u] DELETED isaretlendi (key='%s')\n",
                   idx, key);
            return 0;
        }
    }
    return -1;
}

void hash_print(const HashTable* ht)
{
    int i;
    static const char* state_str[] = { "EMPTY   ", "OCCUPIED", "DELETED " };

    printf("\n--- HASH TABLOSU (Tip B + Acik Adresleme) ---\n");
    printf("idx | state    | key       | hash(key)\n");
    printf("----+----------+-----------+-----------\n");
    for (i = 0; i < TABLE_SIZE; i++) {
        printf(" %2d | %s | ", i, state_str[ht->slots[i].state]);
        if (ht->slots[i].state == SLOT_OCCUPIED)
            printf("%-9s | h=%u\n",
                   ht->slots[i].key, hash_function_B(ht->slots[i].key));
        else
            printf("%-9s | -\n", "");
    }
    printf("Toplam kayit: %d/%d | Cakisma: %d | Toplam probe: %d\n",
           ht->item_count, TABLE_SIZE,
           ht->collision_count, ht->probe_total);
}

void hash_print_memory_report(const HashTable* ht)
{
    int i;

    printf("\n========================================================\n");
    printf("        BELLEK IZLEME RAPORU - HASH TABLE                 \n");
    printf("        (Tip B + Acik Adresleme / Linear Probing)         \n");
    printf("========================================================\n");
    printf(" HashTable adresi    : %p\n", (void*)ht);
    printf(" slots[] adresi      : %p\n", (void*)ht->slots);
    printf(" Bir slot boyutu     : %lu byte\n",
           (unsigned long)sizeof(HashSlot));
    printf(" Toplam tablo boyutu : %lu byte\n",
           (unsigned long)sizeof(HashTable));
    printf(" Yuk faktoru         : %.2f (%d/%d)\n",
           (double)ht->item_count / TABLE_SIZE,
           ht->item_count, TABLE_SIZE);
    printf(" Cakisma sayisi      : %d\n", ht->collision_count);
    printf(" Toplam probe sayisi : %d\n", ht->probe_total);

    printf("\n--- SLOT ADRES TABLOSU ---\n");
    printf(" idx | adres            | state    | key       | pkg adres\n");
    printf("-----+------------------+----------+-----------+-----------------\n");
    for (i = 0; i < TABLE_SIZE; i++) {
        const char* st = ht->slots[i].state == SLOT_EMPTY    ? "EMPTY   " :
                         ht->slots[i].state == SLOT_OCCUPIED ? "OCCUPIED" :
                                                               "DELETED ";
        printf(" %2d  | %p | %s | %-9s | %p\n",
               i, (void*)&ht->slots[i], st,
               ht->slots[i].state == SLOT_OCCUPIED ? ht->slots[i].key : "-",
               (void*)ht->slots[i].package);
    }
    printf("--------------------------------------------------------\n");
    printf(" NOT: Acik adreslemede dinamik node yoktur. slots[] dizisi\n");
    printf(" tek bir bitisik bellek bloku olarak HashTable icindedir.\n");
    printf("--------------------------------------------------------\n");
}

void free_hash_table(HashTable* ht)
{
    int i;
    if (!ht) return;
    for (i = 0; i < TABLE_SIZE; i++) {
        if (ht->slots[i].state == SLOT_OCCUPIED && ht->slots[i].package)
            free_package(ht->slots[i].package);
    }
    printf("  [free]   HashTable serbest birakildi @ %p\n", (void*)ht);
    free(ht);
}
