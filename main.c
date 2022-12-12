#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <conio.h>

// Mendefinisikan struct BENDA yang
// dapat digunakan dengan kata kunci RUMAH
typedef struct BENDA {
    char nama[20], waktu_nyala[20], waktu_mati[20];
    int atribut, state;
    float kwh;
} RUMAH;

//Function definition
void register_device(RUMAH *benda, int n);
void show_device(RUMAH *benda, int n, int malloccheck);
void remove_device(RUMAH *benda, int *n, int *malloccheck);
void help();
void credit();
void print_smart_home();

int main() {
	
	// Pendeklarasian variabel
    int i, n;
    int back_menu, pilihan, malloccheck, mainmenu, totaldevice;
    float totalkwh;
    char time_now[50], c;
    RUMAH *benda;
    
    // Buat jam lokal di dalam program
    time_t s;
    struct tm* current_time;

    malloccheck = 0;
    mainmenu = 0;
	
	// Menyambut user saat baru menjalankan program
    print_smart_home();
    printf("\n");
	
	// Membuat label statement untuk menu utama
    menu:
		// Jika menu utama sudah dibuka sebelumnya
        // maka hapus semua yang ada di layar sekarang
        if(mainmenu == 1) {
            system("cls");
        }
        
        // Output pilihan menu utama untuk user
        printf("Program ini adalah untuk mengatur device di rumah anda\n");
        printf("Silakan pilih menu dibawah ini\n");
        printf("1. Daftar perangkat\n");
        printf("2. Lihat perangkat yang terdaftar\n");
        printf("3. Hapus perangkat\n");
        printf("4. Mode monitoring\n");
        printf("5. Total kwh\n");
        printf("6. Help/Informasi mengenai program\n");
        printf("7. Credit\n");
        printf("8. Exit\n");
        printf("Pilihan anda: ");
        scanf("%d", &pilihan);
        mainmenu = 1;
	
	//Selama user tidak pilih menu Exit maka program akan berjalan
    while(pilihan != 8) {
        switch(pilihan) {
        	
        	// Jika user memilih menu Daftar perangkat
            case 1:
                system("cls"); // Hapus menu utama dari layar
                
                // Meminta input banyaknya device dari user
                printf("Masukkan jumlah device yang ingin anda daftar: ");
                scanf("%d", &n);
                
                // Jika benda belum dialokasikan ke dalam memori maka dibuat
                if(malloccheck == 0) {
                    benda = (RUMAH*) calloc(n, sizeof(RUMAH));
                    totaldevice = n;
                    malloccheck = 1;
                }
                // Jika sudah maka jumlah device sekarang ditambah
                // dan ukuran benda diubah
                else {
                    n = n + totaldevice;
                    benda = (RUMAH*) realloc(benda, (totaldevice + n) * sizeof(RUMAH));
                }
                
                register_device(benda, n); // Memanggil fungsi register_device()
                goto menu; // Jika user sudah selesai maka balik ke menu utama
                break;
                
            // Jika user memilih menu Lihat perangkat yang terdaftar
            case 2:
                show_device(benda, n, malloccheck); // Memanggil fungsi show_device()
                
                // Meminta user klik spasi untuk balik ke menu utama
                back_menu = getch();
                if(back_menu == 32){
                    goto menu;
                }
                break;
                
            // Jika user memilih menu Hapus perangkat
            case 3:
                remove_device(benda, &n, &malloccheck); // Memanggil fungsi remove_device()
                
                // Jika tidak ada perangkat yang terdaftar maka
                // bebaskan memori benda
                if(n == 0){
                    free(benda);
                    malloccheck = 0;
                } 
				// Jika ada perangkat yang terdaftar
				// maka ukuran benda dialokasikan ulang
				else {
                    benda = (RUMAH*) realloc(benda, n * sizeof(RUMAH));
                }
                
                // Meminta user klik spasi untuk kembali ke menu utama
                printf("Tekan spasi untuk kembali ke menu\n");
                back_menu = getch();
                if(back_menu == 32){
                    goto menu;
                }
                break;
                
            // Jika user memilih mode monitoring 
            case 4:
                system("cls"); // Hapus menu utama dari layar
                
                // Jika tidak ada perangkat yang terdaftar
                if(malloccheck == 0) {
                    printf("Tidak ada device yang terdaftar\n");
                    printf("Tekan spasi untuk kembali ke menu\n");
                    
                    // Meminta user klik spasi untuk kembali ke menu utama;
					back_menu = getch();
                    if(back_menu == 32) {
                        goto menu;
                    }
                }
                // Jika ada perangkat yang terdaftar
				else {
                    printf("Press spacebar to quit\n");
                    while(1) {
                    	// Output hari, tanggal, dan waktu untuk user
                        s = time(NULL);
                        current_time = localtime(&s);
                        strftime(time_now, sizeof(time_now), "%a, %d %b %Y %H:%M", current_time);
                        printf("%s\r", time_now);
                        
                        clock_t start_time = clock();
                        
                        // Cek waktu menyala dan mati setiap perangkat
                        for(i=0; i<n; i++){
                        	
                        	// Jika sudah waktunya perangkat ke-i untuk menyala maka beri tahu user
                            if(strcmp(time_now, benda[i].waktu_nyala) == 0){
                                if(benda[i].state == 0){
                                    printf("%s nyala\n", benda[i].nama);
                                    benda[i].state = 1; // Menyalakan perangkat
                                }
                            }
                            
                            // Jika sudah waktunya perangkat ke-i mati maka beri tahu user
                            if(strcmp(time_now, benda[i].waktu_mati) == 0){
                                if(benda[i].state == 1){
                                    printf("%s mati\n", benda[i].nama);
                                    benda[i].state = 0; // Mematikan penrangkat
                                }
                            }
                        }
                        
                        // Program akan mengecek saat user klik spasi
                        while(clock() < start_time + 1 * CLOCKS_PER_SEC){
                            if(kbhit()){
                                back_menu = _getch();
                                if(back_menu == 32){
                                    goto menu;
                                }
                            }
                        }

                        sleep(1); // delay 1 second
                    }
                }
                break;
                
            // Jika user memilih user total kwh
            case 5:
                system("cls"); // Hapus menu utama dari layar;
                
                // Jika tidak ada perangkat yang terdaftar
                if(malloccheck == 0) {
                    printf("Tidak ada device yang terdaftar\n");
                    
                    // Meminta user klik spasi untuk kembali ke menu utama
                    printf("Tekan spasi untuk kembali ke menu\n");
                    back_menu = getch();
                    if(back_menu == 32) {
                        goto menu;
                    }
                }
                // Jika ada perangkat yang terdaftar
				else {
					// Jumlahkan semua kwh dari perangkat yang terdaftar
                    for(i=0; i<n; i++) {
                        totalkwh += benda[i].kwh;
                    }
                    
                    // Output besar pemakaian kwh dari semua perangkat user yang terdaftar
                    printf("Total pemakaian watt per jamnya: %.2f kwh\n", totalkwh);
                    printf("Pemakaian diatas adalah pemakaian diluar pemakaian yang tidak terdaftar\n");
                    printf("Tekan spasi untuk kembali ke menu\n");
                    
                    // Meminta user klik spasi untuk kembali ke menu utama
                    back_menu = getch();
                    if(back_menu == 32) {
                        goto menu;
                    }
                }
                break;
                
            // Jika user memilih menu Help
            case 6:
                help(); // Memanggil fungsi help()
                
                // Meminta user klik spasi untuk kembali ke menu
                back_menu = getch();
                if(back_menu == 32) {
                    goto menu;
                }
            
            // Jika user memilih menu Credit
            case 7:
                credit(); // Memanggil fungsi credit()
                
                // Meminta user klik spasi untuk kembali ke menu utama
                back_menu = getch();
                if(back_menu == 32 ){
                    goto menu;
                }
                break;
                
            // Jika user memilih menu Exit maka berhenti
            case 8:
                break; 
                
            // Jika user tidak memilih menu yang ada di pilihan menu utama
            default:
            	// Hapus menu utama dari layar
                system("cls");
                
                // Memberi tahu user pilihannya tidak ada dan
                // meminta user klik spasi untuk kembali ke menu utama
                printf("Pilihan anda tidak ada\n");
                printf("Tekan spasi untuk kembali ke menu\n");
                back_menu = getch();
                if(back_menu == 32) {
                    goto menu;
                }
                break;
        }
    }
    
    // Jika user memilih menu Exit maka program berhenti
    endprogram:
        system("cls"); // Hapus menu utama dari layar
        
        // Mengucapkan terima kasih kepada user karena
        // telah menggunakan program ini
        printf("Terima kasih telah menggunakan program ini\n");
        if(malloccheck == 1) {
            free(benda); // Bebaskan benda jika masih ada memori
        }
    return 0;
}

// Function Prototype

// Fungsi untuk mendaftarkan perangkat
void register_device(RUMAH *benda, int n) {
	
	// Pendeklarasian variabel
    int i, device;
    char template;
    
    // Memasukkan perangkat sebanyak yang diminta user
    for(i = 0; i < n; i++) {
    	
    	// Label statement untuk menu pendaftaran
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

        switch(device) {
        	
        	// Jika user memilih menu lampu
            case 1:
				
				// Tanya user apakah ingin menggunakan template yang ada untuk lampu
                printf("Mau pakai template lampu? (y/n)\n");
                printf("Kecerahan lamput (default): 300 LUX\n");
                printf("Waktu menyala (default): 17:00\n");
                printf("Waktu mati (default): 07:00\n");
                printf("Pilihan: ");
                scanf("%s", &template);
                
                // Jika user ingin menggunakan template maka
				// atribut disamakan dengan template
                if(template == 'y' || template == 'Y') {
                    strcpy(benda[i].nama, "Lampu");
                    benda[i].atribut = 300;
                    strcpy(benda[i].waktu_nyala, "17:00");
                    strcpy(benda[i].waktu_mati, "07:00");
                }
                
                // Jika user ingin mengatur lampunya sendiri maka
                // meminta user memasukkan masing-masing atribut lampu
				else if(template == 'n' || template == 'N') {
                    strcpy(benda[i].nama, "Lampu");
                    printf("Masukkan kecerahan lampu: ");
                    scanf("%d", &benda[i].atribut);
                    printf("Masukkan waktu lampu menyala: ");
                    scanf("%s", benda[i].waktu_nyala);
                    printf("Masukkan waktu lampu mati: ");
                    scanf("%s", benda[i].waktu_mati);
                }
				
				// Jika user tidak memilih keduanya maka kembali ke menu pendaftaran
				else {
                    printf("Pilihan tidak ada\n");
                    goto daftardevice;
                }
                break;
                
            // Jika user memilih menu kipas angin
            case 2:
                strcpy(benda[i].nama, "Kipas angin"); // Benda diberi nama kipas angin
                
                // Meminta input kecepatan kipas dari user
                printf("Masukkan kecepatan kipas angin (1-3): ");
                scanf("%d", &benda[i].atribut);
                
                // Jika user memberi input kecepetan tidak sesuai pilihan
                // maka beri tahu user pilihannya tidak ada dan kembali
                // ke menu pendaftaran
                if(benda[i].atribut < 1 || benda[i].atribut > 3) {
                    printf("Kecepatan kipas angin tidak ada\n");
                    goto daftardevice;
                }
                
                //Meminta user memasukkan waktu menyala dan mati dari kipas
                printf("Masukkan waktu kipas angin menyala: ");
                scanf("%s", benda[i].waktu_nyala);
                printf("Masukkan waktu kipas angin mati: ");
                scanf("%s", benda[i].waktu_mati);
                break;
                
            //Jika user memilih menu AC
            case 3:
                strcpy(benda[i].nama, "AC"); // Benda diberi nama AC
                
                // Meminta user memasukkan temperatur AC
                // dan juga waktu menyala dan waktu mati dari AC
                printf("Masukkan temperatur AC: ");
                scanf("%d", &benda[i].atribut);
                printf("Masukkan waktu AC menyala: ");
                scanf("%s", benda[i].waktu_nyala);
                printf("Masukkan waktu AC mati: ");
                scanf("%s", benda[i].waktu_mati);
                break;
                
            // Jika user memilih menu TV
            case 4:
                strcpy(benda[i].nama, "TV"); // Benda diberi nama TV
                
                // Meminta user untuk memasukkan 
				// waktu menyala dan waktu mati dari TV
                printf("Masukkan waktu TV menyala: ");
                scanf("%s", benda[i].waktu_nyala);
                printf("Masukkan waktu TV mati: ");
                scanf("%s", benda[i].waktu_mati);
                break;
                
            // Jika user memilih menu Pemanas Air
            case 5:
                strcpy(benda[i].nama, "Pemanas Air"); // Benda diberi nama Pemanas air
                
				// Meminta user untuk memasukkan
				// waktu menyala dan waktu mati dari Pemanas air
				printf("Masukkan waktu pemanas air menyala: ");
                scanf("%s", benda[i].waktu_nyala);
                printf("Masukkan waktu pemanas air mati: ");
                scanf("%s", benda[i].waktu_mati);
                break;
                
            // Jika user memilih menu Daftar manual
            case 6:
            	
            	// Meminta user untuk memasukkan nama, waktu 
            	// menyala, dan waktu mati dari perangkat
                printf("Masukkan nama device: ");
                scanf("%s", benda[i].nama);
                printf("Masukkan waktu device menyala: ");
                scanf("%s", benda[i].waktu_nyala);
                printf("Masukkan waktu device mati: ");
                scanf("%s", benda[i].waktu_mati);
                break;
            
            // Jika user memilih menu Kembali ke menu utama
            // maka pendaftaran perangkat berhenti
            case 7:
                break;
                
            // Jika user memasukkan input yang tidak termasuk pilihan
            // maka beri tahu user dan kembali ke menu pendaftaran
            default:
                printf("Pilihan tidak ada\n");
                goto daftardevice;
        }
        
        // Meminta user memasukkan besar watt dari perangkat
        // lalu mengatur kondisi perangkat sedang mati
        printf("Berapa watt perangkat tersebut: ");
        scanf("%f", &benda[i].kwh);
        benda[i].state = 0;
    }
}

// Fungsi untuk menunjukkan semua perangkat yang terdaftar
void show_device(RUMAH *benda, int n, int malloccheck) {
    int i;
    system("cls"); // Hapus menu utama dari layar
    
    printf("Daftar device yang terdaftar\n");
    
    // Jika tidak ada perangkat yang terdaftar
    // maka beri tahu user tidak ada
    if(malloccheck == 0) {
        printf("Belum ada device yang terdaftar\n");
    }
    // Jika ada perangkat yang terdaftar maka
    // output semua ke layar
    else {
        for(i = 0; i < n; i++) {
            printf("%d. %s\n", i+1, benda[i].nama);
        }
    }
    printf("\nTekan spasi untuk kembali ke menu utama\n");
}

// Fungsi untuk menghapus perangkat yang sudah terdaftar
void remove_device(RUMAH *benda, int *n, int *malloccheck) {
	
	// Pendeklarasian variabel
    int i, j, device;
    system("cls"); // Menghapus menu utama dari layar
    printf("Daftar device yang terdaftar\n");
    
    // Jika tidak ada perangkat yang terdaftar 
    // maka beri tahu user
    if(*malloccheck == 0) {
        printf("Belum ada device yang terdaftar\n");
    }
    // Jika ada perangkat yang terdaftar maka output semuanya dan
    // meminta user memilih perangkat mana yang dihapus
	else {
        for(i = 0; i < *n; i++) {
            printf("%d. %s\n", i+1, benda[i].nama);
        }
        printf("0. Kembali ke menu utama\n");
        printf("Silakan pilih device yang ingin dihapus: ");
        scanf("%d", &device);
		
		// Jika user tidak memilih menu Kembali ke menu utama
        if(device != 0) {
        	// Menghapus perangkat dari list yang terdaftar
            for(i = device-1; i < *n; i++) {
                strcpy(benda[i].nama, benda[i+1].nama);
                benda[i].atribut = benda[i+1].atribut;
                benda[i].kwh = benda[i+1].kwh;
                strcpy(benda[i].waktu_nyala, benda[i+1].waktu_nyala);
                strcpy(benda[i].waktu_mati, benda[i+1].waktu_mati);
                benda[i].state = benda[i+1].state;
            }
            *n = *n - 1;
            printf("Device berhasil dihapus\n");
        }
    }
}

// Fungsi untuk membantu user mengenal program SMART HOME
void help(){
    system("cls"); // Menghapus menu utama dari layar
    
    // Menjelaskan tujuan program SMART HOME
    printf("Program ini merupakan program yang berfungsi untuk mengatur waktu nyala dan mati suatu device\n");
    printf("Mengapa demikian? Tujuan dari program ini sebenarnya adalah untuk menghemat penggunaan listrik yang\n");
    printf("digunakan, sehingga dapat menekan emisi CO2 yang dihasilkan oleh pembangkit listrik.\n");
    printf("Dalam penggunaan energi listrik yang dihasilkan bukan dari sumber terbarukan, seperti batubara dan sejenisnya.\n");
    printf("Dapat menghasilkan emisi karbon senilai 0.3878 kg CO2/kWh. Oleh karena itu, automasi perangkat listrik bisa\n");
    printf("menjadi salah satu solusi untuk menjawab permasalahan tersebut.\n");
    printf("\n\n");
    
    // Menjelaskan masing-masing menu dari menu utama pada program SMART HOME
    printf("Penjelasan menu menu terkait yang terdapat pada program ini:\n");
    printf("1. Daftar perangkat\n");
    printf("Menu ini digunakan untuk mendaftarkan device yang ingin diatur waktu nyala dan mati nya.\n");
    printf("2. Lihat daftar perangkat\n");
    printf("Menu ini digunakan untuk melihat daftar device yang telah terdaftar.\n");
    printf("3. Hapus perangkat\n");
    printf("Menu ini digunakan untuk menghapus device yang telah terdaftar.\n");
    printf("4. Mode monitoring\n");
    printf("Menu ini digunakan untuk memonitoring device yang telah terdaftar.\n");
    printf("5. Total kwh\n");
    printf("Menu ini digunakan untuk melihat total kwh yang digunakan oleh device yang telah terdaftar.\n");
    printf("6. Help\n");
    printf("Menu ini digunakan untuk melihat informasi tentang program ini.\n");
    printf("7. Credits\n");
    printf("Menu ini digunakan untuk melihat informasi tentang pembuat program ini.\n");
    printf("8. Exit\n");
    printf("Menu ini digunakan untuk keluar dari program.\n");
    printf("\n\n");
    printf("Tekan spasi untuk kembali ke menu\n");
}

// Fungsi ini untuk mengeluarkan output dari 
// orang-orang di balik program SMART HOME
void credit(){
    system("cls"); // Hapus menu utama dari layar
    
    // Output orang-orang di balik program SMART HOME
    printf("Kelompok 5 ProgDas 2\n");
    printf("1. Giovan Christoffel Sihombing (2206816084)\n");
    printf("2. Adhelia Putri Maylani (2206814816)\n");
    printf("3. Darren Adam Dewantoro (2206816600)\n");
    printf("4. Annisa Ardelia Setiawan (2206059471)\n");
    printf("\n");
    printf("Tekan spasi untuk kembali ke menu\n");
}

// Fungsi ini untuk mengeluarkan output WeLCOME to SMART HOME
// untuk user yang baru datang
void print_smart_home(){
    printf("db   d8b   db d88888b db       .o88b.  .d88b.  .88b  d88. d88888b \n");
    printf("88   I8I   88 88'     88      d8P  Y8 .8P  Y8. 88'YbdP`88 88'     \n");
    printf("88   I8I   88 88ooooo 88      8P      88    88 88  88  88 88ooooo \n");
    printf("Y8   I8I   88 88~~~~~ 88      8b      88    88 88  88  88 88~~~~~ \n");
    printf("`8b d8'8b d8' 88.     88booo. Y8b  d8 `8b  d8' 88  88  88 88.     \n");
    printf(" `8b8' `8d8'  Y88888P Y88888P  `Y88P'  `Y88P'  YP  YP  YP Y88888P  TO\n");
    printf("                                                                  \n");
    printf("  ******** ****     ****     **     *******   **********       **      **   *******   ****     **** ********\n");
    printf(" **////// /**/**   **/**    ****   /**////** /////**///       /**     /**  **/////** /**/**   **/**/**///// \n");
    printf("/**       /**//** ** /**   **//**  /**   /**     /**          /**     /** **     //**/**//** ** /**/**      \n");
    printf("/*********/** //***  /**  **  //** /*******      /**          /**********/**      /**/** //***  /**/******* \n");
    printf("////////**/**  //*   /** **********/**///**      /**          /**//////**/**      /**/**  //*   /**/**////  \n");
    printf("       /**/**   /    /**/**//////**/**  //**     /**          /**     /**//**     ** /**   /    /**/**      \n");
    printf(" ******** /**        /**/**     /**/**   //**    /**          /**     /** //*******  /**        /**/********\n");
    printf("////////  //         // //      // //     //     //           //      //   ///////   //         // ////////  by Kelompok 5 ProgDas 2\n");
}
