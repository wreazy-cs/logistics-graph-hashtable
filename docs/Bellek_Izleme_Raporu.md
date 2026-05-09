================================================================
       BELLEK IZLEME RAPORU (RAM)  -  Lojistik Yonetim Sistemi
       Ogrenci : Samet Muhammed Ali ŞAFAK
       Numara  : 1250505013
       Konfigurasyon: Tip B Hash + Acik Adresleme + DFS
================================================================

Bu rapor; demo veri (6 sehir + 9 paket - 2 cakisma uretir)
yuklendikten sonra isletim sirasinda dinamik olarak (malloc ile)
ayrilan bellek bolgelerini, adresleri (0x...) ve pointer
baglantilarini gostermektedir. Asagidaki ham cikti, programin
kendisi tarafindan uretilmistir (printf("%p", ...) cagrilari ile).

================================================================
1) TEKNIK BELLEK TABLOSU
================================================================

| # | Yapi             | Tip            | Boyut        | Adres ornegi     |
|---|------------------|----------------|--------------|------------------|
| 1 | Graph            | struct Graph   | sizeof(Graph)| 0x55774b4942a0   |
| 2 | HashTable        | struct HashTable | 640 byte   | 0x55774b495c20   |
| 3 | City[0] Istanbul | struct City    | sizeof(City) | 0x55774b4942a0   |
| 4 | City[1] Ankara   | struct City    | sizeof(City) | 0x55774b4942d0   |
| 5 | City[2] Izmir    | struct City    | sizeof(City) | 0x55774b494300   |
| 6 | EdgeNode (IST<->ANK) | struct EdgeNode | 16 byte | 0x55774b496ec0   |
| 7 | Package PKT001   | struct Package | sizeof(Package) | 0x55774b497040 |
| 8 | Package PKT002   | struct Package | sizeof(Package) | 0x55774b497150 |
| 9 | Package PKT016 (CAKISTI) | struct Package | sizeof(Package) | 0x55774b4977b0 |

NOT: Adresler, programin her calismasinda farkli olur (ASLR -
     Address Space Layout Randomization). Yukaridaki degerler bir
     ornek calisma surumune aittir.

ONEMLI: Acik adreslemede, Zincirlemenin aksine, "HashNode" gibi
        dinamik bir dugum YOKTUR. Tum slotlar HashTable'in icinde
        slots[TABLE_SIZE] dizisi olarak BITISIK bir bellek
        blokunda yer alir. Bu yuzden cache-dostu calisir.

================================================================
2) POINTER BAGLANTILARI - SEMATIK GORUNUM (HASH TABLE)
================================================================

  HashTable @ 0x55774b495c20
  +------------------------------------------------------------+
  | slots[ 0]  OCCUPIED  key=PKT002    package -> 0x...7150    |
  | slots[ 1]  EMPTY                                           |
  | slots[ 2]  OCCUPIED  key=PKT005    package -> 0x...7480    |
  | slots[ 3]  EMPTY                                           |
  | slots[ 4]  EMPTY                                           |
  | slots[ 5]  OCCUPIED  key=PKT003    package -> 0x...7260    |
  | slots[ 6]  OCCUPIED  key=PKT018    package -> 0x...78c0    | <- cakisma sonrasi probe
  | slots[ 7]  OCCUPIED  key=PKT006    package -> 0x...7590    |
  | slots[ 8]  OCCUPIED  key=PKT001    package -> 0x...7040    | <- home slot
  | slots[ 9]  OCCUPIED  key=PKT016    package -> 0x...77b0    | <- cakisti, +1 probe
  | slots[10]  OCCUPIED  key=PKT004    package -> 0x...7370    |
  | slots[11]  EMPTY                                           |
  | slots[12]  OCCUPIED  key=PKT007    package -> 0x...76a0    |
  +------------------------------------------------------------+

  Bir HashSlot icindeki alanlar:
       +------------------------------+
       | key      : "PKT001"          |
       | package  : 0x55774b497040  --+----> [Package nesnesi]
       | state    : OCCUPIED          |
       +------------------------------+

  CAKISMA ORNEGI - LINEAR PROBING:
       hash("PKT001") = 8       hash("PKT016") = 8  (CAKISMA!)
       PKT001 -> slot[8] (bos, yerlesti)
       PKT016 -> slot[8] dolu -> slot[9] bos -> yerlesti
       Probe formulu:  (8 + i) mod 13,   i = 0,1,2,...

================================================================
3) POINTER BAGLANTILARI - SEMATIK GORUNUM (GRAPH)
================================================================

  Graph @ 0x55774b4942a0
  +-----------------------------------------------------------+
  | cities[0] "Istanbul" edges --> [E:Bursa]   --> [E:Ankara] |
  | cities[1] "Ankara"   edges --> [E:Konya]   --> [E:Istanbul]|
  | cities[2] "Izmir"    edges --> [E:Bursa]   --> [E:Antalya]|
  | cities[3] "Bursa"    edges --> [E:Izmir]   --> [E:Istanbul]|
  | cities[4] "Konya"    edges --> [E:Antalya] --> [E:Ankara] |
  | cities[5] "Antalya"  edges --> [E:Izmir]   --> [E:Konya]  |
  +-----------------------------------------------------------+

  Bir EdgeNode dugumu:
       +---------------------------+
       | dest_index : 1 (Ankara)   |
       | distance   : 450          |
       | next       : 0x...        |
       +---------------------------+

================================================================
4) PROGRAMIN URETTIGI GERCEK CIKTI (HAM)
================================================================

(Asagidaki bolum,  ./lojistik  ->  10 (demo)  ->  9 (rapor)  ->  0
 menusunden alinan gercek calisma ciktisidir.)

----------------------------------------------------------------
  [malloc] Graph olusturuldu @ 0x5574d4f102a0
  [malloc] HashTable olusturuldu @ 0x5574d4f11c20 (size=13, ALL slots EMPTY)

=========================================================
  LOJISTIK YONETIM SISTEMI  -  Graf + Hash Table (Hibrit)
  Hash      : Tip B (Polynomial Rolling)
  Cakisma   : Zincirleme (Chaining)
  Gezinme   : DFS
=========================================================

========== LOJISTIK YONETIM SISTEMI ==========
 1. Sehir ekle
 2. Rota (yol) ekle
 3. Paket ekle
 4. Paket ara (ID ile)
 5. Paket sil
 6. Tum paketleri listele
 7. Grafi yazdir
 8. DFS ile graf gezinme
 9. BELLEK IZLEME RAPORU
10. DEMO veri yukle (5+ paket, 6 sehir)
 0. Cikis
Secim: 
>>> DEMO veri yukleniyor...
  [+] Sehir eklendi: Istanbul (index=0, adres=0x5574d4f102a0)
  [+] Sehir eklendi: Ankara (index=1, adres=0x5574d4f102d0)
  [+] Sehir eklendi: Izmir (index=2, adres=0x5574d4f10300)
  [+] Sehir eklendi: Bursa (index=3, adres=0x5574d4f10330)
  [+] Sehir eklendi: Konya (index=4, adres=0x5574d4f10360)
  [+] Sehir eklendi: Antalya (index=5, adres=0x5574d4f10390)
  [+] Rota: Istanbul <-> Ankara (450 km) | edges @ 0x5574d4f12ec0, 0x5574d4f12ee0
  [+] Rota: Istanbul <-> Bursa (150 km) | edges @ 0x5574d4f12f00, 0x5574d4f12f20
  [+] Rota: Ankara <-> Konya (260 km) | edges @ 0x5574d4f12f40, 0x5574d4f12f60
  [+] Rota: Konya <-> Antalya (290 km) | edges @ 0x5574d4f12f80, 0x5574d4f12fa0
  [+] Rota: Izmir <-> Antalya (450 km) | edges @ 0x5574d4f12fc0, 0x5574d4f12fe0
  [+] Rota: Bursa <-> Izmir (330 km) | edges @ 0x5574d4f13000, 0x5574d4f13020
  [malloc] Package nesnesi olusturuldu @ 0x5574d4f13040 (id=PKT001)
  [+] Slot[8] dolduruldu (probe yok) | key='PKT001'
  [malloc] Package nesnesi olusturuldu @ 0x5574d4f13150 (id=PKT002)
  [+] Slot[0] dolduruldu (probe yok) | key='PKT002'
  [malloc] Package nesnesi olusturuldu @ 0x5574d4f13260 (id=PKT003)
  [+] Slot[5] dolduruldu (probe yok) | key='PKT003'
  [malloc] Package nesnesi olusturuldu @ 0x5574d4f13370 (id=PKT004)
  [+] Slot[10] dolduruldu (probe yok) | key='PKT004'
  [malloc] Package nesnesi olusturuldu @ 0x5574d4f13480 (id=PKT005)
  [+] Slot[2] dolduruldu (probe yok) | key='PKT005'
  [malloc] Package nesnesi olusturuldu @ 0x5574d4f13590 (id=PKT006)
  [+] Slot[7] dolduruldu (probe yok) | key='PKT006'
  [malloc] Package nesnesi olusturuldu @ 0x5574d4f136a0 (id=PKT007)
  [+] Slot[12] dolduruldu (probe yok) | key='PKT007'
  [malloc] Package nesnesi olusturuldu @ 0x5574d4f137b0 (id=PKT016)
  [!] CAKISMA: key='PKT016' home=8 dolu (slot 8) -> probe 1
  [+] Slot[9] dolduruldu (home=8, 1 adim probe) | key='PKT016'
  [malloc] Package nesnesi olusturuldu @ 0x5574d4f138c0 (id=PKT018)
  [!] CAKISMA: key='PKT018' home=5 dolu (slot 5) -> probe 1
  [+] Slot[6] dolduruldu (home=5, 1 adim probe) | key='PKT018'
>>> Demo veri yuklendi.

========== LOJISTIK YONETIM SISTEMI ==========
 1. Sehir ekle
 2. Rota (yol) ekle
 3. Paket ekle
 4. Paket ara (ID ile)
 5. Paket sil
 6. Tum paketleri listele
 7. Grafi yazdir
 8. DFS ile graf gezinme
 9. BELLEK IZLEME RAPORU
10. DEMO veri yukle (5+ paket, 6 sehir)
 0. Cikis
Secim: 
========================================================
        BELLEK IZLEME RAPORU - HASH TABLE                 
        (Tip B + Acik Adresleme / Linear Probing)         
========================================================
 HashTable adresi    : 0x5574d4f11c20
 slots[] adresi      : 0x5574d4f11c20
 Bir slot boyutu     : 48 byte
 Toplam tablo boyutu : 640 byte
 Yuk faktoru         : 0.69 (9/13)
 Cakisma sayisi      : 2
 Toplam probe sayisi : 11

--- SLOT ADRES TABLOSU ---
 idx | adres            | state    | key       | pkg adres
-----+------------------+----------+-----------+-----------------
  0  | 0x5574d4f11c20 | OCCUPIED | PKT002    | 0x5574d4f13150
  1  | 0x5574d4f11c50 | EMPTY    | -         | (nil)
  2  | 0x5574d4f11c80 | OCCUPIED | PKT005    | 0x5574d4f13480
  3  | 0x5574d4f11cb0 | EMPTY    | -         | (nil)
  4  | 0x5574d4f11ce0 | EMPTY    | -         | (nil)
  5  | 0x5574d4f11d10 | OCCUPIED | PKT003    | 0x5574d4f13260
  6  | 0x5574d4f11d40 | OCCUPIED | PKT018    | 0x5574d4f138c0
  7  | 0x5574d4f11d70 | OCCUPIED | PKT006    | 0x5574d4f13590
  8  | 0x5574d4f11da0 | OCCUPIED | PKT001    | 0x5574d4f13040
  9  | 0x5574d4f11dd0 | OCCUPIED | PKT016    | 0x5574d4f137b0
 10  | 0x5574d4f11e00 | OCCUPIED | PKT004    | 0x5574d4f13370
 11  | 0x5574d4f11e30 | EMPTY    | -         | (nil)
 12  | 0x5574d4f11e60 | OCCUPIED | PKT007    | 0x5574d4f136a0
--------------------------------------------------------
 NOT: Acik adreslemede dinamik node yoktur. slots[] dizisi
 tek bir bitisik bellek bloku olarak HashTable icindedir.
--------------------------------------------------------

========================================================
              BELLEK IZLEME RAPORU - GRAPH                
========================================================
 Graph adresi : 0x5574d4f102a0
 Sehir sayisi : 6 / 50

--- VERTEX -> EDGE BAGLANTILARI ---
[0] Istanbul     @ 0x5574d4f102a0 | edges-head=0x5574d4f12f00
    -> Edge @ 0x5574d4f12f00 | dest=Bursa      | 150 km | next=0x5574d4f12ec0
    -> Edge @ 0x5574d4f12ec0 | dest=Ankara     | 450 km | next=(nil)
[1] Ankara       @ 0x5574d4f102d0 | edges-head=0x5574d4f12f40
    -> Edge @ 0x5574d4f12f40 | dest=Konya      | 260 km | next=0x5574d4f12ee0
    -> Edge @ 0x5574d4f12ee0 | dest=Istanbul   | 450 km | next=(nil)
[2] Izmir        @ 0x5574d4f10300 | edges-head=0x5574d4f13020
    -> Edge @ 0x5574d4f13020 | dest=Bursa      | 330 km | next=0x5574d4f12fc0
    -> Edge @ 0x5574d4f12fc0 | dest=Antalya    | 450 km | next=(nil)
[3] Bursa        @ 0x5574d4f10330 | edges-head=0x5574d4f13000
    -> Edge @ 0x5574d4f13000 | dest=Izmir      | 330 km | next=0x5574d4f12f20
    -> Edge @ 0x5574d4f12f20 | dest=Istanbul   | 150 km | next=(nil)
[4] Konya        @ 0x5574d4f10360 | edges-head=0x5574d4f12f80
    -> Edge @ 0x5574d4f12f80 | dest=Antalya    | 290 km | next=0x5574d4f12f60
    -> Edge @ 0x5574d4f12f60 | dest=Ankara     | 260 km | next=(nil)
[5] Antalya      @ 0x5574d4f10390 | edges-head=0x5574d4f12fe0
    -> Edge @ 0x5574d4f12fe0 | dest=Izmir      | 450 km | next=0x5574d4f12fa0
    -> Edge @ 0x5574d4f12fa0 | dest=Konya      | 290 km | next=(nil)

 Toplam kenar (cift yonlu sayim): 12
--------------------------------------------------------

========== LOJISTIK YONETIM SISTEMI ==========
 1. Sehir ekle
 2. Rota (yol) ekle
 3. Paket ekle
 4. Paket ara (ID ile)
 5. Paket sil
 6. Tum paketleri listele
 7. Grafi yazdir
 8. DFS ile graf gezinme
 9. BELLEK IZLEME RAPORU
10. DEMO veri yukle (5+ paket, 6 sehir)
 0. Cikis
Secim:   [free]   Package serbest birakildi @ 0x5574d4f13150
  [free]   Package serbest birakildi @ 0x5574d4f13480
  [free]   Package serbest birakildi @ 0x5574d4f13260
  [free]   Package serbest birakildi @ 0x5574d4f138c0
  [free]   Package serbest birakildi @ 0x5574d4f13590
  [free]   Package serbest birakildi @ 0x5574d4f13040
  [free]   Package serbest birakildi @ 0x5574d4f137b0
  [free]   Package serbest birakildi @ 0x5574d4f13370
  [free]   Package serbest birakildi @ 0x5574d4f136a0
  [free]   HashTable serbest birakildi @ 0x5574d4f11c20
  [free]   Graph serbest birakildi @ 0x5574d4f102a0
Cikis yapildi.

================================================================
5) BELLEK YONETIMI - OZET
================================================================

* Graph, HashTable, Package ve EdgeNode malloc ile heap'te ayrilir.
* HashSlot dizisi HashTable'in icinde TEK BLOK olarak ayrilir.
* free_hash_table() ve free_graph() ile butun bellek serbest birakilir.
* Bellek sizinti yoktur.
