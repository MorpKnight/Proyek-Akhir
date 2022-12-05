#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

typedef struct BENDA{
    char nama[20], waktu_nyala[20], waktu_mati[20];
    int temperatur;
} RUMAH;

void display_time(){
    time_t s;
    struct tm* current_time;
    s = time(NULL);
    current_time = localtime(&s);
    printf("%02d:%02d\n", current_time->tm_hour, current_time->tm_min);
}

void mainmenu(int *pilihan){
    printf("Selamat datang di Home Automation\n");
    printf("Program ini akan membantu anda dalam mengatur keadaan rumah anda, melalui automisasi yang dilakukan terhadap perangkat rumah anda\n");
    printf("\n\nSilakan pilih menu dibawah\n");
    printf("1. Tambahkan perangkat baru\n");
    printf("2. Lihat daftar perangkat\n");
    printf("3. Hapus perangkat\n");
    printf("4. Bantuan\n");
    printf("5. Keluar\n");

    printf(">");
    scanf("%d", pilihan);
}

void add_device(RUMAH *device, int *n, int *count){
    int i;
    display_time();
    printf("Ada berapa perangkat yang ingin anda tambahkan?\n");
    printf(">");
    scanf("%d", n);
    device = (RUMAH*)malloc(*n * sizeof(RUMAH));
    count = (int*)malloc(*n * sizeof(int));

    printf("Format waktu adalah HH:MM\n");
    for(i = 0; i < *n; i++){
        printf("Masukkan nama perangkat ke-%d\n", i+1);
        printf("Nama benda: ");
        scanf("%s", &(device+i)->nama);
        printf("Waktu nyala: ");
        scanf("%s", &(device+i)->waktu_nyala);
        printf("Waktu mati: ");
        scanf("%s", &(device+i)->waktu_mati);
        printf("Temperatur: ");
        scanf("%d", &(device+i)->temperatur);
        printf("\n");
    }

}

void check_device(RUMAH *device, int *n, int *count){
    int i;
    time_t s;
    char waktu[20];
    struct tm* current_time;

    for(i = 0; i < *n; i++){
        count[i] = 0;
    }
    //printf device
    printf("Daftar perangkat yang ada\n");
    for(i = 0; i < *n; i++){
        printf("Nama benda: %s\n", device[i].nama);
        printf("Waktu nyala: %s\n", device[i].waktu_nyala);
        printf("Waktu mati: %s\n", device[i].waktu_mati);
        printf("Temperatur: %d\n", device[i].temperatur);
        printf("\n");
    }


    // while(1){
    //     s = time(NULL);
    //     current_time = localtime(&s);
    //     sprintf(waktu, "%02d:%02d", current_time->tm_hour, current_time->tm_min);
    //     for(i = 0; i < *n; i++){
    //         printf("Nama benda: %s\n", (device+i)->nama);
    //     }
    //     printf("Waktu sekarang: %s\n", waktu);
    //     sleep(1);
    // }
}

int main(){
    int i, n;
    int *count, *pilihan;
    char time_now[20], temp;
    struct tm* current_time;
    RUMAH *benda;
    time_t s;
    mainmenu(pilihan);
    add_device(benda, &n, count);
    check_device(benda, &n, count);
    
    printf("Terima kasih telah menggunakan program ini\n");

    free(benda);
    free(count);
    return 0;
}