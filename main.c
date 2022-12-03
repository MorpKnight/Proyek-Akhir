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

int main(){
    int i, n, *count, fd;
    char time_now[20], temp;
    RUMAH *benda;
    time_t s;
    struct tm* current_time;

    s = time(NULL);
    current_time = localtime(&s);
    printf("%02d:%02d\n", current_time->tm_hour, current_time->tm_min);

    printf("Ada berapa benda yang dapat di-automisasi: ");
    scanf("%d", &n);
    benda = (RUMAH*)malloc(n*sizeof(RUMAH));
    count = (int*)malloc(n*sizeof(int));

    printf("Untuk waktu nyala dan mati, formatnya HH:MM\n");
    for(i=0; i<n; i++){
        printf("Nama benda: ");
        scanf("%s", benda[i].nama);
        printf("Waktu nyala: ");
        scanf("%s", benda[i].waktu_nyala);
        printf("Waktu mati: ");
        scanf("%s", benda[i].waktu_mati);
        printf("Temperatur: ");
        scanf("%d", &benda[i].temperatur);
    }

    //initialize every count
    for(i=0; i<n; i++){
        count[i] = 0;
    }

    while(1){
        s = time(NULL);
        current_time = localtime(&s);
        sprintf(time_now, "%02d:%02d", current_time->tm_hour, current_time->tm_min);
        for(i=0; i<n; i++){
            if(strcmp(time_now, benda[i].waktu_nyala) == 0){
                if(count[i] == 0){
                    printf("Nyala %s\n", benda[i].nama);
                    count[i] = 1;
                }
            } else if(strcmp(time_now, benda[i].waktu_mati) == 0){
                if(count[i] == 1){
                    printf("Mati %s\n", benda[i].nama);
                    count[i] = 0;
                }
            }
        }
        //use nonblocking i/o to scan character to temp
        read(0, &temp, 1);
        if(temp == 'q'){
            break;
        }
        sleep(10);
    }
    free(benda);
    free(count);
    return 0;
}