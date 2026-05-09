/* ============================================================
 * hash_table.h
 * Hash Tablosu (Tip B Hash + Acik Adresleme / Linear Probing)
 * ------------------------------------------------------------
 * Hash Fonksiyonu : Tip B (Polynomial Rolling Hash)
 * Cakisma Cozumu  : Acik Adresleme (Linear Probing)
 * ============================================================ */
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "package.h"

#define TABLE_SIZE 13   /* Asal sayi - cakismalari azaltir   */

/* Acik adreslemede her slot 3 durumdan birinde olabilir.
 * DELETED durumu (tombstone) silmeden sonra arama zincirinin
 * kopmamasi icin gereklidir.
 */
typedef enum {
    SLOT_EMPTY    = 0,   /* Hic kullanilmadi   */
    SLOT_OCCUPIED = 1,   /* Suan dolu          */
    SLOT_DELETED  = 2    /* Silindi (tombstone)*/
} SlotState;

typedef struct HashSlot {
    char       key[32];
    Package*   package;
    SlotState  state;
} HashSlot;

typedef struct HashTable {
    HashSlot slots[TABLE_SIZE];
    int item_count;
    int collision_count;
    int probe_total;     /* Toplam probe sayisi (insert sirasinda) */
} HashTable;

/* API */
HashTable*   create_hash_table(void);
unsigned int hash_function_B(const char* key);
int          hash_insert(HashTable* ht, const char* key, Package* pkg);
Package*     hash_search(HashTable* ht, const char* key);
int          hash_delete(HashTable* ht, const char* key);
void         hash_print(const HashTable* ht);
void         hash_print_memory_report(const HashTable* ht);
void         free_hash_table(HashTable* ht);

#endif
