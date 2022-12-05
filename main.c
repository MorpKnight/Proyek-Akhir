#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <conio.h>

typedef struct BENDA{
    char nama[20], waktu_nyala[20], waktu_mati[20];
    int temperatur, count;
} RUMAH;

int main(){
    int i, n, j, a, malloccheck, uppercase, mainmenu, back_menu, pilihan;
    char time_now[20], removename[20];
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
        printf("1. Register device\n");
        printf("2. See device list\n");
        printf("3. Remove device\n");
        printf("4. See automation\n");
        printf("5. Exit\n");
        printf("Pilihan anda: ");
        scanf("%d", &pilihan);
        mainmenu += 1;

    while(pilihan != 4){
        switch(pilihan){
            case 1:
                system("cls");
                printf("Masukkan jumlah device yang ingin anda register: ");
                scanf("%d", &n);
                if(malloccheck == 0){
                    benda = (RUMAH*)malloc(n*sizeof(RUMAH));
                    malloccheck = 1;
                }
                else{
                    benda = (RUMAH*)realloc(benda, n*sizeof(RUMAH));
                }
                for(i=0; i<n; i++){
                    printf("Masukkan nama device: ");
                    scanf("%s", benda[i].nama);
                    for(a=0; a<strlen(benda[i].nama); a++){
                        uppercase = benda[i].nama[a];
                        if(uppercase >= 97 && uppercase <= 122){
                            benda[i].nama[a] = uppercase - 32;
                        }
                    }
                    printf("Masukkan temperatur yang diinginkan: ");
                    scanf("%d", &benda[i].temperatur);
                    printf("Masukkan waktu nyala: ");
                    scanf("%s", benda[i].waktu_nyala);
                    printf("Masukkan waktu mati: ");
                    scanf("%s", benda[i].waktu_mati);
                    benda[i].count = 0;
                }
                goto menu;
                break;
            case 2:
                system("cls");
                printf("Device yang terdaftar: \n");
                for(i=0; i<n; i++){
                    printf("%s\n", benda[i].nama);
                }
                //input spacebar to back to menu
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
                        printf("%s\n", benda[i].nama);
                    }
                    printf("Masukkan nama device yang ingin dihapus: ");
                    scanf("%s", removename);
                    for(a=0; a<strlen(removename); a++){
                        uppercase = removename[a];
                        if(uppercase >= 97 && uppercase <= 122){
                            removename[a] = uppercase - 32;
                        }
                    }
                    for(i=0; i<n; i++){
                        if(strcmp(removename, benda[i].nama) == 0){
                            for(j=i; j<n; j++){
                                strcpy(benda[j].nama, benda[j+1].nama);
                                benda[j].temperatur = benda[j+1].temperatur;
                                strcpy(benda[j].waktu_nyala, benda[j+1].waktu_nyala);
                                strcpy(benda[j].waktu_mati, benda[j+1].waktu_mati);
                            }
                            n -= 1;
                            benda = (RUMAH*)realloc(benda, n*sizeof(RUMAH));
                            printf("Device berhasil dihapus\n");
                            printf("Tekan spasi untuk kembali ke menu\n");
                            back_menu = getch();
                            if(back_menu == 32){
                                goto menu;
                            }
                        }
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
                printf("Terima kasih telah menggunakan program ini\n");
                free(benda);
                exit(0);
                break;
            default:
                printf("Pilihan anda tidak ada\n");
                goto menu;
                break;
        }
    }
    return 0;
}