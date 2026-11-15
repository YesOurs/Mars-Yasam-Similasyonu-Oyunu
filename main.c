#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));

    // Değişkenler
    char difficult, command;
    int health, energy, oxygen, water, module_damage, spare_part, round, sandstorm_round, day, lock;
    day = 1;
    lock = 0;
    round = 0;

    // Oyun başlangıcı ve zorluk seçimi arayüzü
    printf("========================================================\n");
    printf("          MARS KOLONISI YASAM DESTEK SIMULATORU         \n");
    printf("========================================================\n");
    printf("Amac: Kurtarma ekibi gelene kadar 15 gun hayatta kalmak.\n\n");
    printf("Zorluk Seviyesi Secin:\n");
    printf("  [K] Kolay\n");
    printf("  [N] Normal\n");
    printf("  [Z] Zor\n");
    printf("--------------------------------------------------------\n");


    // Zorluk seçimi
    do {
        printf("Seciminiz (K/N/Z): ");
        scanf(" %c", &difficult);
    } while (difficult != 'K' && difficult != 'k' && 
        difficult != 'N' && difficult != 'n' && 
        difficult != 'Z' && difficult != 'z');


    // Zorluk Ayarları
    switch (difficult)
    {
    case 'K':
    case 'k':
        health = energy = oxygen = 100;
        water = 6;
        spare_part = 3;
        module_damage = 0;
        sandstorm_round = 3;
        break;
    case 'N':
    case 'n':
        health = energy = oxygen = 100;
        water = 5;
        spare_part = 2;
        module_damage = 20;
        sandstorm_round = 5;
        break;
    case 'Z':
    case 'z':
        health = energy = oxygen = 80;
        water = 3;
        spare_part = 1;
        module_damage = 40;
        sandstorm_round = 7;
        break;
    default:
        printf("Zorluk Yapisinde bir hata meydana geldi.\n");
        break;
    }

    // Ana oyun döngüsü
    do {
        // Oyun arayüzü
        printf("\n========================================================\n");
        printf(" GUN: %-2d / 15  |  TUR: %-3d  |  HAVA KILIDI: %s\n", day, round, (lock == 1) ? "MUHURLU" : "ACIK");
        printf("--------------------------------------------------------\n");
        printf(" SAGLIK : %%%-3d |  OKSIJEN : %%%-3d |  ENERJI    : %%%-3d\n", health, oxygen, energy);
        printf(" SU     : %-3d  |  PARCA   : %-3d  |  MODUL HSR : %%%-3d\n", water, spare_part, module_damage);
        printf("========================================================\n");
        printf(" KOMUTLAR:\n");
        printf("  [G] Kesif Gorevi       [O] Oksijen Uret\n");
        printf("  [T] Modul Tamiri       [U] Uyu / Dinlen\n");
        printf("  [K] Kum Firtinasi      [H] Hava Kilidi Sifresi\n");
        printf("  [D] Durum Raporu       [Q] Cikis\n");
        printf("--------------------------------------------------------\n");
        printf("Komut giriniz: ");
        scanf(" %c", &command);

        // Komut menüsü
        switch (command) {
            case 'g':
            case 'G':
                if(energy < 15){
                    printf("Kesfe cikacak enerjin yok.\n");
                    continue;
                }
                // [G] Keşif görevi işlemleri
                int border;
                int random_discovery_number = rand() % 100;
                int random_material_number = (rand() % 9) + 1;
                energy -= 15;
                oxygen -= 10;
                
                if (energy > 40 && oxygen > 40) {
                    border = 45;
                } else {
                    border = 40;
                }

                printf("Zar: %d / Esik: %d -> %s\n", random_discovery_number, border, (random_discovery_number >= border) ? "BASARILI" : "BASARISIZ");

                if (random_discovery_number < border) {
                    printf("Birsey bulamadin.\n");
                }
                else if (random_discovery_number >= border && random_discovery_number <= 70) {
                    printf("%d adet yedek parca buldun!\n", random_material_number);
                    spare_part = spare_part + random_material_number;
                }
                else {
                    printf("%d litre su buldun!\n", random_material_number);
                    water = water + random_material_number;
                }
                if(random_discovery_number < 10 || module_damage > 70){
                    printf("Kesif sirasinde yaralandin!\n");
                    health -= 10;
                }
                round++;
                break;

            case 'o':
            case 'O':
                if(water < 2){
                    printf("Yeteri kadar su yok.\n");
                    continue;
                }
                else if(energy <= 10){
                    printf("Yeterince enerjin yok.\n");
                    continue;
                }
                // [O] Oksijen üretme işlemleri
                int i, j;
                printf("2 litre su 3 mol oksijene cevrilir\n");
                printf("Elektroliz edilecek su miktari: ");
                scanf("%d", &i);
                if((i > water || i < 2)){
                    printf("Paneldeki su miktarindan fazlasi girilemez.\n");
                    continue;
                }
                j = i - (i % 2);
                water = water - j;
                oxygen = oxygen + 3 * j / 2;
                energy = energy - 10;
                printf("%d mol oksijen uretildi!\n", 3 * j / 2);
                
                round++;
                break;

            case 't':
            case 'T':
                // [T] Modül tamir işlemleri
                int notification;
                if(module_damage == 0){
                    printf("Onarilacak bir modul hasari yok.\n");
                    continue;
                }
                else if(module_damage <= 40 && spare_part >= 4){
                    printf("Tehdit seviyesi dusuk modul hasari onarildi!\n");
                    spare_part -= 4;
                    module_damage -= 15;
                }
                else if(module_damage > 40 && module_damage < 80 && spare_part >= 5){
                    printf("Tehdit seviyesi orta modul hasari onarildi!\n");
                    spare_part -= 5;
                    module_damage -= 20;
                }
                else if(module_damage >= 80 && module_damage <= 100 && spare_part >= 6){
                    printf("Tehdit seviyesi yuksek modul hasari onarildi!\n");
                    spare_part -= 6;
                    module_damage -= 25;
                }
                else {
                    if(module_damage <= 40){notification = 4;}
                    else if(module_damage > 40 && module_damage < 80){notification = 5;}
                    else if(module_damage >= 80 && module_damage <= 100){notification = 6;}
                    printf("Yeterli parcaniz yok! (Gerekli parca sayisi: %d)\n", notification);
                    continue;
                }
                round++;
                break;

            case 'u':
            case 'U':
                // [U] Uyku ayarları
                if(energy == 100){
                    printf("Enerjin doluyken uyuyamazsin.\n");
                    continue;
                }
                else {
                    if(oxygen < 20){
                        health -= 5;
                        energy += 20;
                    }
                    else {
                        health += 5;
                        energy += 20;
                    }
                }

                oxygen -= 5;
                round++;
                break;

            case 'k':
            case 'K':
                // [K] Kum fırtınası
                int damaged_rounds = 0;
                int lost_health = 0;
                int lost_oxygen = 0;
                int storm_border = module_damage < 50 ? 40 : 65;

                printf("\n>> [ALARM] Kum firtinasi basladi! Toplam %d tur surecek.\n", sandstorm_round);

                for (int i = 1; i <= sandstorm_round; i++) {
                    int random_storm_number = rand() % 100;
                    int critical_damage = 1;

                    printf("\n--- [Firtina Turu %d/%d] ---\n", i, sandstorm_round);

                    if (i % 2 == 0) {
                        critical_damage = 2;
                        printf(">> [SIDDETLI RUZGAR] Cift numarali tur! Hasar etkileri 2 katina cikti.\n");
                    }

                    // Zorunlu zar
                    printf("Zar: %d / Esik: %d -> %s\n", random_storm_number, storm_border, (random_storm_number >= storm_border) ? "KORUNMA BASARILI" : "HASAR ALINDI");

                    if (random_storm_number >= storm_border) {
                        printf(">> Firtinanin bu dalgasindan zararsiz korundun.\n");
                    }
                    else if (random_storm_number < 20) {
                        if (lock == 1) {
                            printf(">> [MUHURLU KILIT] Hava kilidi muhurlu oldugu icin oksijen sizintisi engellendi!\n");
                            continue; 
                        }
                        int oxygen_loss = 6 * critical_damage;
                        oxygen -= oxygen_loss;
                        lost_oxygen += oxygen_loss;
                        damaged_rounds++;
                        printf(">> [SIZINTI] Oksijen sizintisi yasandi! (-%d Oksijen)\n", oxygen_loss);
                    }
                    else if (random_storm_number >= 20 && random_storm_number < 35) {
                        int health_loss = 5 * critical_damage;
                        health -= health_loss;
                        lost_health += health_loss;
                        damaged_rounds++;
                        printf(">> [YARALANMA] Savrulan parcalar yaralanmana sebep oldu! (-%d Saglik)\n", health_loss);
                    }
                    else {
                        int damage_increase = 8 * critical_damage;
                        module_damage += damage_increase;
                        damaged_rounds++;
                        printf(">> [MODUL HASARI] Dis govde firtinadan hasar aldi! (+%d Modul Hasari)\n", damage_increase);
                    }

                    if (health <= 0 || oxygen <= 0) {
                        printf("\n>> [KRITIK] Yasamsal degerler tukendi! Firtina dongusu sonlandiriliyor...\n");
                        break;
                    }
                }

                // Döngü sonunda fırtına özeti
                printf("\n---------------- [ KUM FIRTINASI OZETI ] ----------------\n");
                printf(" Hasar Alinan Tur Sayisi   : %d / %d\n", damaged_rounds, sandstorm_round);
                printf(" Toplam Kaybedilen Saglik  : %d\n", lost_health);
                printf(" Toplam Kaybedilen Oksijen : %d\n", lost_oxygen);
                printf("---------------------------------------------------------\n");

                round++;
                break;

            case 'h':
            case 'H':
                // [H] Hava kilidi şifresi
                if (lock == 1){
                    printf("Hava kilidi muhurlu.\n");
                    continue;
                }
                else if(energy < 10){
                    printf("Yeterince enerjin yok.\n");
                    continue;
                }

                char password = 'A' + (day % 26);
                char guess;
                int chance = 3;

                printf("Sifre, alfabede A harfinden bugunun gun sayisi kadar ilerideki harftir.\n");

                do {
                    printf("Sifre tahmini giriniz (Kalan Hak: %d): ", chance);
                    scanf(" %c", &guess);

                    // Küçük harfi büyük harfe çevirme
                    if(guess >= 'a' && guess <= 'z'){
                        guess -= 32;
                    }
                    // Tahmin kontrolü
                    if(guess == password){
                        lock = 1;
                        printf("TEBRIKLER! Hava kilidi muhurlendi.\n");
                    }
                    else {
                        energy -= 5;
                        chance -= 1;
                    }
                    // Oyuncuyu yönlendirme
                    if(guess > password){
                        printf("Geriye git.\n");
                    }
                    else if(guess < password){
                        printf("Ileriye git.\n");
                    }

                } while (guess != password && chance > 0);

                if(guess != password){
                    module_damage += 20;
                    printf("Haklariniz bitti. (20 Modul Hasari)\n");
                }

                round++;
                break;

            case 'd':
            case 'D':
                // [D] Durum raporu sayfası
                printf("\n------------------- [ DURUM RAPORU ] -------------------\n");
                printf(" Saglik: %%%d | Enerji: %%%d | Oksijen: %%%d | Hasar: %%%d\n", health, energy, oxygen, module_damage);
                printf(" Su Miktari: %d birim | Yedek Parca: %d adet\n", water, spare_part);
                printf("--------------------------------------------------------\n");
                if(oxygen < 35){
                    printf("[!] UYARI: Oksijen seviyesi kritik!\n");
                }
                if(health < 35){
                    printf("[!] UYARI: Saglik seviyesi kritik!\n");
                }
                if(module_damage > 60){
                    printf("[!] UYARI: Hasar seviyesi kritik!\n");
                }
                if(module_damage <= 60 && health >= 35 && oxygen >= 35){
                    printf("[OK] TUM SISTEMLER NORMAL!\n");
                }
                printf("--------------------------------------------------------\n");
                continue;

            case 'q':
            case 'Q':
                // [Q] Çıkış yolu
                printf("\n>> [SISTEM] Simulasyondan cikis yapiliyor...\n");
                continue;

            default:
                printf("\n>> [HATA] Tanimsiz komut girdiniz! Lutfen gecerli bir komut secin.\n");
                continue;
    }

    // Tur geçiş kontrolcüsü
    if(round > 0 && round % 3 == 0) {
        if(module_damage > 70){
            health -= 5;
        }
        day++;
        oxygen -= 8;
    }

    // Limit denetleyicileri
    if (health > 100) {health = 100;}
    if (health < 0)   {health = 0;}
    if (energy > 100) {energy = 100;}
    if (energy < 0)   {energy = 0;}
    if (oxygen > 100) {oxygen = 100;}
    if (oxygen < 0)   {oxygen = 0;}
    if (module_damage > 100) {module_damage = 100;}
    if (module_damage < 0)   {module_damage = 0;}

    } while (command != 'q' && command != 'Q' && health > 0 && oxygen > 0 && day < 15);

    int point = (day * 100) + health + oxygen - module_damage;

    printf("\n########################################################\n");
    printf("                   SIMULASYON SONA ERDI                 \n");
    printf("########################################################\n");
    printf(" Hayatta Kalinan Gun : %d\n", day);
    printf(" Oynanan Tur         : %d\n", round);
    printf("--------------------------------------------------------\n");
    printf(" SON DURUM DEGERLERI:\n");
    printf("  - Saglik       : %%%d\n", health);
    printf("  - Oksijen      : %%%d\n", oxygen);
    printf("  - Enerji       : %%%d\n", energy);
    printf("  - Modul Hasari : %%%d\n", module_damage);
    printf("  - Kalan Su     : %d birim\n", water);
    printf("  - Kalan Parca  : %d adet\n", spare_part);
    printf("--------------------------------------------------------\n");
    printf(" TOPLAM PUAN         : %d\n", point);
    printf("########################################################\n");

    return 0;
}
