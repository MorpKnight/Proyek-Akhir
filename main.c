#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <conio.h>

typedef struct BENDA{
    char nama[20], waktu_nyala[20], waktu_mati[20];
    int temperatur, count, kecepatan, status;
    float kwh;
} RUMAH;

int main(){
    int i, j, k, n, a;
    int back_menu, pilihan, malloccheck, uppercase, mainmenu, device, removedevice, totaldevice;
    float totalkwh;
    char time_now[20];
    RUMAH *benda;
    time_t s;
    struct tm* current_time;

    malloccheck = 0;
    mainmenu = 0;


    menu:
        if(mainmenu == 1){
            system("cls");
        }
        printf("Program ini adalah untuk mengatur device di rumah anda\n");
        printf("Silakan pilih menu dibawah ini\n");
        printf("1. Daftar perangkat\n");
        printf("2. Lihat perangkat yang terdaftar\n");
        printf("3. Hapus perangkat\n");
        printf("4. Mode monitoring\n");
        printf("5. Total kwh\n");
        printf("6. Exit\n");
        printf("Pilihan anda: ");
        scanf("%d", &pilihan);
        mainmenu = 1;

    while(pilihan != 6){
        switch(pilihan){
            case 1:
                system("cls");
                printf("Masukkan jumlah device yang ingin anda daftar: ");
                scanf("%d", &n);
                if(malloccheck == 0){
                    benda = (RUMAH*)calloc(n, sizeof(RUMAH));
                    totaldevice = n;
                    malloccheck = 1;
                }
                else{
                    benda = (RUMAH*)realloc(benda, (totaldevice + n) * sizeof(RUMAH));
                }
                for(i = 0; i < n; i++){
                    daftardevice:
                        printf("Silakan pilih salah satu dari device dibawah ini untuk di register\n");
                        printf("1. Lampu\n");
                        printf("2. Kipas angin\n");
                        printf("3. AC\n");
                        printf("4. TV\n");
                        printf("5. Pemanas Air\n");
                        printf("6. Daftar Manual\n");
                        printf("7. Kembali ke menu utama\n");
                        printf("Silakan pilih: ");
                        scanf("%d", &device);
                        printf("\n");

                    switch(device){
                        case 1:
                            strcpy(benda[i].nama, "Lampu");
                            printf("Masukkan kecerahan lampu: ");
                            scanf("%d", &benda[i].temperatur);
                            printf("Masukkan waktu lampu menyala: ");
                            scanf("%s", benda[i].waktu_nyala);
                            printf("Masukkan waktu lampu mati: ");
                            scanf("%s", benda[i].waktu_mati);
                            break;
                        case 2:
                            strcpy(benda[i].nama, "Kipas angin");
                            printf("Masukkan kecepatan kipas angin (1-3): ");
                            scanf("%d", &benda[i].kecepatan);
                            printf("Masukkan waktu kipas angin menyala: ");
                            scanf("%s", benda[i].waktu_nyala);
                            printf("Masukkan waktu kipas angin mati: ");
                            scanf("%s", benda[i].waktu_mati);
                            break;
                        case 3:
                            strcpy(benda[i].nama, "AC");
                            printf("Masukkan temperatur AC: ");
                            scanf("%d", &benda[i].temperatur);
                            printf("Masukkan waktu AC menyala: ");
                            scanf("%s", benda[i].waktu_nyala);
                            printf("Masukkan waktu AC mati: ");
                            scanf("%s", benda[i].waktu_mati);
                            break;
                        case 4:
                            strcpy(benda[i].nama, "TV");
                            printf("Masukkan waktu TV menyala: ");
                            scanf("%s", benda[i].waktu_nyala);
                            printf("Masukkan waktu TV mati: ");
                            scanf("%s", benda[i].waktu_mati);
                            break;
                        case 5:
                            strcpy(benda[i].nama, "Pemanas Air");
                            printf("Masukkan temperatur air: ");
                            scanf("%d", &benda[i].temperatur);
                            printf("Masukkan waktu pemanas air menyala: ");
                            scanf("%s", benda[i].waktu_nyala);
                            printf("Masukkan waktu pemanas air mati: ");
                            scanf("%s", benda[i].waktu_mati);
                            break;
                        case 6:
                            printf("Masukkan nama device: ");
                            scanf("%[^\n]s", benda[i].nama);
                            printf("Masukkan waktu device menyala: ");
                            scanf("%s", benda[i].waktu_nyala);
                            printf("Masukkan waktu device mati: ");
                            scanf("%s", benda[i].waktu_mati);
                            break;
                        case 7:
                            goto menu;
                        default:
                            printf("Invalid! Silakan ulangi\n");
                            goto daftardevice;
                            break;
                    }
                    printf("Masukkan kwh device: ");
                    scanf("%f", &benda[i].kwh);
                    benda[i].count = 0;
                }
                goto menu;
                break;
            case 2:
                system("cls");
                printf("Device yang terdaftar: \n");
                if(malloccheck == 0){
                    printf("Tidak ada device yang terdaftar\n\n");
                    printf("Silakan register device terlebih dahulu\n");

                    printf("Tekan spasi untuk kembali ke menu\n");
                    back_menu = getch();
                    if(back_menu == 32){
                        goto menu;
                    }
                } else {
                    for(i=0; i<n; i++){
                        printf("%d. %s\n", i+1, benda[i].nama);
                    }
                }
                printf("Tekan spasi untuk kembali ke menu\n");
                back_menu = getch();
                if(back_menu == 32){
                    goto menu;
                }
                break;
            case 3:
                system("cls");
                printf("Device yang ingin dihapus:\n");
                if(malloccheck == 0){
                    printf("Tidak ada device yang terdaftar\n");
                    printf("Tekan spasi untuk kembali ke menu\n");
                    back_menu = getch();
                    if(back_menu == 32){
                        goto menu;
                    }
                }else{
                    for(i=0; i<n; i++){
                        printf("%d. %s\n", i+1, benda[i].nama);
                    }
                    printf("Silakan pilih device yang ingin dihapus: ");
                    scanf("%d", &removedevice);
                    //remove device from pointer struct then realloc benda
                    for(i=removedevice-1; i<n; i++){
                        benda[i] = benda[i+1];
                    }
                    n--;
                    benda = realloc(benda, n*sizeof(RUMAH));
                    printf("Device berhasil dihapus\n");
                    printf("Tekan spasi untuk kembali ke menu\n");
                    back_menu = getch();
                    if(back_menu == 32){
                        goto menu;
                    }
                }
                break;
            case 4:
                system("cls");
                if(malloccheck == 0){
                    printf("Tidak ada device yang terdaftar\n");
                    printf("Tekan spasi untuk kembali ke menu\n");
                    back_menu = getch();
                    if(back_menu == 32){
                        goto menu;
                    }
                }else{
                    printf("Press spacebar to quit\n");
                    while(1){
                        s = time(NULL);
                        current_time = localtime(&s);
                        sprintf(time_now, "%02d:%02d", current_time->tm_hour, current_time->tm_min);
                        
                        clock_t start_time = clock();
                        
                        for(i=0; i<n; i++){
                            if(strcmp(time_now, benda[i].waktu_nyala) == 0){
                                if(benda[i].count == 0){
                                    printf("%s nyala\n", benda[i].nama);
                                    benda[i].count = 1;
                                }
                            }
                            if(strcmp(time_now, benda[i].waktu_mati) == 0){
                                if(benda[i].count == 1){
                                    printf("%s mati\n", benda[i].nama);
                                    benda[i].count = 0;
                                }
                            }
                        }
                        while(clock() < start_time + 1 * CLOCKS_PER_SEC){
                            if(kbhit()){
                                a = _getch();
                                if(a == 32){
                                    goto menu;
                                }
                            }
                        }

                        sleep(1);
                    }
                }
                break;
            case 5:
                system("cls");
                if(malloccheck == 0){
                    printf("Tidak ada device yang terdaftar\n");
                    printf("Tekan spasi untuk kembali ke menu\n");
                    back_menu = getch();
                    if(back_menu == 32){
                        goto menu;
                    }
                }else{
                    for(i=0; i<n; i++){
                        totalkwh += benda[i].kwh;
                    }
                    printf("Total kwh: %.2f\n", totalkwh);
                    printf("Tekan spasi untuk kembali ke menu\n");
                    back_menu = getch();
                    if(back_menu == 32){
                        goto menu;
                    }
                }
                break;
            case 6:
                break;
            default:
                system("cls");
                printf("Pilihan anda tidak ada\n");
                printf("Tekan spasi untuk kembali ke menu\n");
                back_menu = getch();
                if(back_menu == 32){
                    goto menu;
                }
                break;
        }
    }
    endprogram:
        system("cls");
        printf("Terima kasih telah menggunakan program ini\n");
        //check to free memory
        if(malloccheck == 1){
            free(benda);
        }
    return 0;
}