#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Untuk usleep()

#define MAX_LANGKAH 50
#define MAX_PANJANG_LANGKAH 255
#define MAX_NAMA_MENU 50
#define TOTAL_MENU_DEFAULT 7

// Daftar menu default
const char *menuDefault[] = {
    "Cheeseburger",
    "Indomie Goreng Spesial",
    "Lasagna",
    "Nasi Goreng Spesial",
    "Pancake Cokelat",
    "Pizza Margherita",
    "Spaghetti Aglio e Olio"
};

// Fungsi untuk membaca langkah memasak dari file
int bacaLangkahDariFile(const char *namaFile, char langkah[][MAX_PANJANG_LANGKAH]) {
    FILE *file = fopen(namaFile, "r");
    if (!file) {
        perror("Gagal membuka file langkah");
        return -1;
    }

    int count = 0;
    while (fgets(langkah[count], MAX_PANJANG_LANGKAH, file)) {
        langkah[count][strcspn(langkah[count], "\n")] = 0; // Hapus newline
        count++;
        if (count >= MAX_LANGKAH) {
            break;
        }
    }

    fclose(file);
    return count;
}

// Fungsi untuk menampilkan langkah-langkah memasak
void langkahMemasakUI(char langkah[][MAX_PANJANG_LANGKAH], int totalLangkah) {
    int pilihan;
    for (int i = 0; i < totalLangkah; i++) {
        printf("\nLangkah %d/%d: %s\n", i + 1, totalLangkah, langkah[i]);
        printf("Apakah Anda melakukannya? (1: Ya, 0: Tidak): ");
        scanf("%d", &pilihan);

        if (pilihan != 1) {
            printf("\nMasakan Anda selesai. Kembali ke menu utama.\n");
            return;
        }
    }
    printf("\nSelamat! Masakan Anda telah selesai dimasak.\n");
}

// Fungsi untuk menampilkan animasi teks
void animasiTeks(const char *teks, int delayMs) {
    for (int i = 0; teks[i] != '\0'; i++) {
        printf("%c", teks[i]);
        fflush(stdout);
        usleep(delayMs * 1000); // Delay dalam mikrodetik
    }
    printf("\n");
}

// Fungsi untuk menampilkan ASCII Art
void tampilkanAsciiArt() {
    printf(" _______  _______  _        _______    _______           _______  _______ \n");
    printf("(  ____ \\(  ____ \\( \\      (  ____ \\  (  ____ \\|\\     /|(  ____ \\(  ____ \\\n");
    printf("| (    \\\/| (    \\\/| (      | (    \\\/  | (    \\\/| )   ( || (    \\\/| (    \\\/\n");
    printf("| (_____ | (__    | |      | (__      | |      | (___) || (__    | (__    \n");
    printf("(_____  )|  __)   | |      |  __)     | |      |  ___  ||  __)   |  __)   \n");
    printf("      ) || (      | |      | (        | |      | (   ) || (      | (      \n");
    printf("/\\____) || (____/\\| (____/\\| )        | (____/\\| )   ( || (____/\\| )      \n");
    printf("\\_______)(_______/(_______/|/         (_______/|/     \\\|(_______/|/         \n");
}

// Fungsi untuk menambahkan menu baru
void tambahMenu(char namaMenu[], char langkahBaru[][MAX_PANJANG_LANGKAH], int totalLangkahBaru) {
    char namaFile[MAX_NAMA_MENU + 4];
    snprintf(namaFile, sizeof(namaFile), "%s.txt", namaMenu);

    FILE *file = fopen(namaFile, "w");
    if (!file) {
        perror("Gagal membuat file langkah");
        return;
    }

    for (int i = 0; i < totalLangkahBaru; i++) {
        fprintf(file, "%s\n", langkahBaru[i]);
    }
    fclose(file);

    printf("\nMenu '%s' berhasil ditambahkan dengan langkah-langkahnya.\n", namaMenu);
}

// Fungsi untuk sorting menu secara alfabetis
void sortMenu(char menu[][MAX_NAMA_MENU], int totalMenu) {
    for (int i = 0; i < totalMenu - 1; i++) {
        for (int j = i + 1; j < totalMenu; j++) {
            if (strcmp(menu[i], menu[j]) > 0) {
                char temp[MAX_NAMA_MENU];
                strcpy(temp, menu[i]);
                strcpy(menu[i], menu[j]);
                strcpy(menu[j], temp);
            }
        }
    }
}

// Fungsi untuk menambahkan menu sementara (di memory)
void tambahMenuSementara(char menuBaru[][MAX_NAMA_MENU], char langkahBaru[][MAX_LANGKAH][MAX_PANJANG_LANGKAH], int langkahBaruCount[], int *totalMenuBaru) {
    char namaMenu[MAX_NAMA_MENU];
    char langkah[MAX_LANGKAH][MAX_PANJANG_LANGKAH];
    int totalLangkahBaru = 0;

    printf("\nMasukkan nama menu baru: ");
    scanf(" %[^\n]", namaMenu);

    printf("\nMasukkan langkah-langkah memasak (ketik 'selesai' untuk mengakhiri):\n");
    while (1) {
        printf("Langkah %d: ", totalLangkahBaru + 1);
        scanf(" %[^\n]", langkah[totalLangkahBaru]);

        if (strcmp(langkah[totalLangkahBaru], "selesai") == 0) {
            break;
        }

        totalLangkahBaru++;
        if (totalLangkahBaru >= MAX_LANGKAH) {
            printf("Mencapai batas maksimal langkah.\n");
            break;
        }
    }

    strcpy(menuBaru[*totalMenuBaru], namaMenu);
    for (int i = 0; i < totalLangkahBaru; i++) {
        strcpy(langkahBaru[*totalMenuBaru][i], langkah[i]);
    }
    langkahBaruCount[*totalMenuBaru] = totalLangkahBaru;
    (*totalMenuBaru)++;

    tambahMenu(namaMenu, langkah, totalLangkahBaru); // Simpan ke file
    sortMenu(menuBaru, *totalMenuBaru); // Sortir menu setelah ditambahkan
}

// Fungsi utama
int main() {
    tampilkanAsciiArt(); // Tampilkan ASCII Art

    char langkah[MAX_LANGKAH][MAX_PANJANG_LANGKAH];
    char menuBaru[MAX_LANGKAH][MAX_NAMA_MENU];
    char langkahBaru[MAX_LANGKAH][MAX_LANGKAH][MAX_PANJANG_LANGKAH];
    int langkahBaruCount[MAX_LANGKAH] = {0};
    int totalMenuBaru = 0;
    int pilihan;

    animasiTeks("Selamat datang di Self Chef dimana kamu adalah Chef nya!", 75);

    while (1) {
        printf("Silahkan memilih apa yang anda inginkan!\n");
        printf("\n=================================\n");
        printf("          MENU UTAMA\n");
        printf("=================================\n");
        printf("1. Lihat Menu\n");
        printf("2. Tambah Menu Baru\n");
        printf("3. Keluar\n");
        printf("=================================\n");
        printf("Pilih opsi (1-3): ");
        scanf("%d", &pilihan);

        if (pilihan == 1) { // Lihat menu
            printf("\nDaftar Menu:\n");
            for (int i = 0; i < TOTAL_MENU_DEFAULT; i++) {
                printf("%d. %s\n", i + 1, menuDefault[i]);
            }
            for (int i = 0; i < totalMenuBaru; i++) {
                printf("%d. %s\n", i + 1 + TOTAL_MENU_DEFAULT, menuBaru[i]);
            }
            printf("\nPilih menu untuk dimasak (0 untuk kembali): ");
            int menuDipilih;
            scanf("%d", &menuDipilih);

            if (menuDipilih > 0 && menuDipilih <= TOTAL_MENU_DEFAULT) {
                // Menu default
                char namaFile[MAX_NAMA_MENU + 4];
                snprintf(namaFile, sizeof(namaFile), "%s.txt", menuDefault[menuDipilih - 1]);

                if (access(namaFile, F_OK) != 0) {
                    printf("File '%s' tidak ditemukan. Pastikan file ada.\n", namaFile);
                    continue;
                }

                int totalLangkah = bacaLangkahDariFile(namaFile, langkah);
                if (totalLangkah > 0) {
                    printf("\nMemasak '%s':\n", menuDefault[menuDipilih - 1]);
                    langkahMemasakUI(langkah, totalLangkah);
                }
            } else if (menuDipilih > TOTAL_MENU_DEFAULT &&
                       menuDipilih <= TOTAL_MENU_DEFAULT + totalMenuBaru) {
                // Menu baru
                int idx = menuDipilih - TOTAL_MENU_DEFAULT - 1;
                printf("\nMemasak '%s':\n", menuBaru[idx]);
                langkahMemasakUI(langkahBaru[idx], langkahBaruCount[idx]);
            }
        } else if (pilihan == 2) { // Tambah menu baru
            tambahMenuSementara(menuBaru, langkahBaru, langkahBaruCount, &totalMenuBaru);
        } else if (pilihan == 3) { // Keluar
            animasiTeks("\nTerima kasih telah memasak di restoran kita!", 75);
            animasiTeks("Sampai jumpa lagi! ??", 100);
            break;
        } else {
            animasiTeks("Pilihan tidak valid. Silakan coba lagi.", 50);
        }
    }

    return 0;
}

