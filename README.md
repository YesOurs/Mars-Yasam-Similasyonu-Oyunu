# 🚀 Mars Kolonisi Yaşam Destek Simülatörü

Bu proje, **Algoritma ve Programlama** dersi kapsamında C programlama dili kullanılarak geliştirilmiş, metin tabanlı (karakter kontrollü) bir kaynak yönetimi ve hayatta kalma simülasyonudur. 

Oyuncu, Mars'taki küçük bir araştırma istasyonunda tek başına kalmış bir mühendisi yönetmekte ve kurtarma ekibi ulaşana kadar **15. güne** hayatta kalarak ulaşmaya çalışmaktadır.

---

## 📋 İçindekiler
- [Özellikler ve Oyun Mekanikleri](#-özellikler-ve-oyun-mekanikleri)
- [Zorluk Seviyeleri](#-zorluk-seviyeleri)
- [Komut Sistemi](#-komut-sistemi)
- [Teknik Mimari ve Algoritmik Yapı](#-teknik-mimari-ve-algoritmik-yapı)
- [Kurulum ve Çalıştırma](#-kurulum-ve-çalıştırma)
- [Puanlama Formülü](#-puanlama-formülü)

---

## ⚙️ Özellikler ve Oyun Mekanikleri

Simülatör, oyuncunun verdiği tek karakterlik komutlara göre istasyonun ve mühendisin hayati parametrelerini gerçek zamanlı olarak takip eder:

* **Takip Edilen Durum Değişkenleri:** Sağlık (`health`), Enerji (`energy`), Oksijen (`oxygen`), Su (`water`), Yedek Parça (`spare_part`), Modül Hasarı (`module_damage`), Hava Kilidi Durumu (`lock`), Gün (`day`) ve Tur (`round`).
* **Dinamik Zaman Akışı:** Geçerli her aksiyon tur sayacını `1` artırır. Her **3 turda bir** (`round % 3 == 0`) yeni bir gün başlar. Gün başında yaşam destek tüketimi sebebiyle oksijen `8` birim azalır; eğer modül hasarı `70`'in üzerindeyse mühendis ek olarak `5` sağlık kaybeder.
* **Olasılık ve Rastgelelik:** Keşif ve kum fırtınası olaylarında `srand(time(NULL))` ve `rand() % 100` kullanılarak dinamik zar/eşik karşılaştırmaları yapılır ve sonuçlar ekrana raporlanır.
* **Merkezi Sınırlandırma (Clamping):** Her tur sonunda sağlık, enerji, oksijen ve modül hasarı değerleri `[0, 100]` aralığında kalacak şekilde denetlenir.

---

## 🎚️ Zorluk Seviyeleri

Oyun başlangıcında `do-while` doğrulama döngüsü ve `switch-case` yapısı ile oyuncudan bir zorluk seviyesi seçmesi istenir. Seçilen seviyeye göre başlangıç kaynakları ve kum fırtınası süresi değişmektedir:

| Zorluk Seviyesi | Komut | Sağlık / Enerji / Oksijen | Su (Birim) | Yedek Parça | Başlangıç Hasarı | Fırtına Turu |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **Kolay** | `K` / `k` | %100 / %100 / %100 | 6 | 3 | %0 | 3 Tur |
| **Normal** | `N` / `n` | %100 / %100 / %100 | 5 | 2 | %20 | 5 Tur |
| **Zor** | `Z` / `z` | %80 / %80 / %80 | 3 | 1 | %40 | 7 Tur |

---

## 🎮 Komut Sistemi

Komut sistemi büyük/küçük harf duyarsız (*case-insensitive*) olarak `switch-case` **fall-through** yapısı ile tasarlanmıştır. Kaynak yetersizliği nedeniyle iptal edilen komutlar, `D` komutu ve geçersiz girişler (`default`) `continue` yapısı sayesinde tur harcamaz.

| Komut | İşlev | Teknik Detay ve Etki |
| :---: | :--- | :--- |
| **`G` / `g`** | **Keşif Görevi** | `-15 Enerji`, `-10 Oksijen`. Enerji ve oksijen durumuna göre eşik değeri (`45` veya `40`) belirlenir. Zar sonucuna göre `1-9` arası su veya yedek parça bulunur ya da eli boş dönülür. Zar `< 10` veya modül hasarı `> 70` ise karakter yaralanır (`-10 Sağlık`). |
| **`O` / `o`** | **Oksijen Üretimi** | `-10 Enerji`. Elektroliz ile her `2` birim su, `3` birim oksijene dönüştürülür. Tek sayı girilirse modülüs (`%`) kontrolü ile `1` birim su depoda bırakılır. |
| **`T` / `t`** | **Modül Tamiri** | Hasar seviyesine göre kademeli `if - else if - else` yapısı çalışır:<br>• **Düşük Hasar (1-40):** `4` parça harcar, `15` hasar onarır.<br>• **Orta Hasar (41-79):** `5` parça harcar, `20` hasar onarır.<br>• **Yüksek Hasar (80-100):** `6` parça harcar, `25` hasar onarır. |
| **`U` / `u`** | **Uyu / Dinlen** | `+20 Enerji`, `-5 Oksijen`. İç içe `if` (*nested if*) kontrolü ile oksijen `< 20` ise sağlık `5` azalır, aksi halde `5` artar. Enerji tam doluyken (`100`) uyunamaz. |
| **`K` / `k`** | **Kum Fırtınası** | `for` döngüsü ile zorluk seviyesine göre `3`, `5` veya `7` tur simüle edilir. Çift numaralı turlarda (`i % 2 == 0`) rüzgar şiddetlenir ve hasarlar iki katına çıkar. Hava kilidi mühürlüyse (`lock == 1`) oksijen sızıntısı `continue` ile atlanır. Yaşam değerleri sıfırlanırsa döngü `break` ile sonlandırılır. |
| **`H` / `h`** | **Hava Kilidi Şifresi** | `do-while` döngüsü ile `3` deneme hakkı verilir. Şifre o günün sayısından dinamik olarak türetilir (`'A' + (day % 26)`). Her yanlış tahminde `-5 Enerji` düşer ve alfabetik yönlendirme (*İleriye/Geriye git*) yapılır. Başarılı olursa `lock = 1` olur, 3 hak biterse modül hasarı `20` artar. |
| **`D` / `d`** | **Durum Raporu** | Tüm sistem değerlerini ve kritik eşik uyarılarını (`oksijen < 35`, `sağlık < 35`, `hasar > 60`) görüntüler. Tüm değerler güvenli aralıktaysa `&&` operatörü ile `"TUM SISTEMLER NORMAL!"` mesajı verilir. **Tur harcamaz.** |
| **`Q` / `q`** | **Çıkış** | Simülasyonu sonlandırır ve oyun sonu sonuç/puan raporunu ekrana yazdırır. |

---

## 🛠️ Teknik Mimari ve Algoritmik Yapı

Projede C dilinin temel kontrol yapıları ve operatörleri aşağıdaki amaçlarla kullanılmıştır:

1. **Aritmetik ve Modülüs (`%`) Operatörleri:** Su-oksijen dönüşümünde çift sayı kontrolü (`i - (i % 2)`), kum fırtınasında çift tur çarpanı (`i % 2 == 0`), Caesar şifreleme benzeri dinamik harf üretimi (`'A' + (day % 26)`) ve 3 turda bir gün geçiş kontrolü (`round % 3 == 0`).
2. **Mantıksal Operatörler (`&&`, `||`, `!=`):** Keşif eşiği belirleme, durum raporunda sistem normalliği kontrolü, hava kilidi `do-while` döngü koşulu (`guess != password && chance > 0`) ve ana oyun döngüsünün sonlanma koşulları.
3. **Akış Kontrolü (`break` ve `continue`):** Kum fırtınasında mühürlü kilit durumunda sızıntıyı atlamak için `continue`, ölüm durumunda fırtınayı erken bitirmek için `break` kullanılmıştır. Ayrıca ana döngüde geçersiz veya yetersiz kaynaklı komutların gün sayacını etkilememesi için `continue` ile döngü adımı korunmuştur.

---

## 💻 Kurulum ve Çalıştırma

Projeyi kendi bilgisayarınızda derlemek ve çalıştırmak için bir C derleyicisine (GCC, Clang vb.) sahip olmanız yeterlidir.

1. **Depoyu klonlayın:**
   ```bash
   git clone https://github.com/YesOurs/Mars-Yasam-Similasyonu-Oyunu.git
   cd Mars-Yasam-Similasyonu-Oyunu
   ```

2. **Kodu derleyin (GCC örneği):**
   ```bash
   gcc main.c -o mars_simulator
   ```

3. **Uygulamayı çalıştırın:**
   * **Windows:**
     ```bash
     .\mars_simulator.exe
     ```
   * **Linux / macOS:**
     ```bash
     ./mars_simulator
     ```

---

## 🏆 Puanlama Formülü

Simülasyon hangi koşulla biterse bitsin (15. güne ulaşma, sağlık/oksijen tükenmesi veya `Q` ile çıkış), oyuncunun performansı aşağıdaki aritmetik formülle hesaplanarak sonuç ekranında gösterilir:

$$\text{Puan} = (\text{Gün} \times 100) + \text{Sağlık} + \text{Oksijen} - \text{Modül Hasarı}$$
