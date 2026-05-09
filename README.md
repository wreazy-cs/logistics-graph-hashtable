# Lojistik Yonetim Sistemi - Hibrit Graf + Hash Table

## Konfigurasyon (Ogrenci numarasi: 1250505013)

| Kural | Numaradaki Rakam | Sonuc |
|---|---|---|
| Hash Fonksiyonu (cift/tek) | son rakam **3** = TEK | **Tip B** (Polynomial Rolling) |
| Cakisma Cozumu (0-4 / 5-9) | son rakam **3** -> 0-4 | **Acik Adresleme** (Linear Probing) |
| Graf Gezinme (sondan 2. cift/tek) | sondan 2. rakam **1** = TEK | **DFS** (Depth-First Search) |

## Klasor yapisi

```
lojistik-odev/
├── include/          (Header dosyalar)
│   ├── package.h
│   ├── hash_table.h
│   └── graph.h
├── src/              (Kaynak kodlar)
│   ├── main.c
│   ├── package.c
│   ├── hash_table.c
│   └── graph.c
├── docs/
│   └── Bellek_Izleme_Raporu.md
├── Makefile
└── README.md
```

## Derleme (Windows + MinGW)

```bat
gcc -Wall -Iinclude src\main.c src\graph.c src\hash_table.c src\package.c -o lojistik.exe
```

## Derleme (Linux / WSL)

```bash
make
```

## Calistirma

```bat
lojistik.exe        :: Windows
./lojistik          # Linux/WSL
```

## Hizli test

Program acildiginda:
- **`10`** -> Demo veri yukle (6 sehir, 9 paket - 2 tanesi CAKISIR)
- **`6`**  -> Hash tablosunu (slotlari) goruntule
- **`9`**  -> Bellek izleme raporu
- **`8`**  -> DFS ile graf gezinme
- **`0`**  -> Cikis

## Sistem mimarisi

```
+--------------+         +-----------------+
|     GRAF     |         |   HASH TABLE    |
| (sehirler &  |         | (paket ID ile   |
|  rotalar)    |         |  hizli sorgu)   |
+--------------+         +-----------------+
         |                       |
         +-----+        +--------+
               |        |
           +--------------+
           |   main.c     |
           +--------------+
```

## Acik Adresleme (Linear Probing) - kisa ozet

- Hedef slot doluysa bir sonrakine bakilir: `probe(i) = (h(key) + i) mod m`
- Slot durumlari: `EMPTY`, `OCCUPIED`, `DELETED` (tombstone)
- Avantaj: pointer/dinamik liste yok, cache-dostu
- Demo'da 9 paketten 2'si cakisir, ortalama probe sayisi cikti olarak gosterilir
