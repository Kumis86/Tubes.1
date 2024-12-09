
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include "programAtm.h"

#define MAX_LENGTH 10




// Mengosongkan input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {} 
    if (c != '\n') { // Tambahan: bersihkan jika karakter terakhir bukan newline
        while ((c = getchar()) != '\n' && c != EOF) {} 
    }
}

void clearScreen() {
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

void welcome() {
char opsi_log_reg;
	
	printf("                          SELAMAT DATANG\n");
	printf("                                DI\n");
	printf("                           BANK BERSAMA\n");
	printf("               SILAHKAN PILIH OPSI (LOGIN/REGISTRASI)\n\n");
	printf("1.REGISTRASI                                                  2.LOGIN\n\n");
	printf("MASUKAN OPSI (1/2): ");
	scanf("%c", &opsi_log_reg);
	//syntax dibawah ini berfungsi untuk menghapus newline supaya tidak terjadi error pada input user
	clearInputBuffer();
	
	
	clearScreen();
	
	if (opsi_log_reg == '1'){
		registrasiNasabah();
	} else if (opsi_log_reg == '2'){
		loginNasabah();
	} else {
		printf("SILAHKAN PILIH ANTARA (1/2) \n");
		welcome();
	}
}

// Generate nomor rekening acak
void generateNoRekening(char *noRekening) {
    int i;
    const char charset[] = "0123456789";
    for (i = 0; i < MAX_LENGTH; i++) {
        noRekening[i] = charset[rand() % 10];
    }
    noRekening[MAX_LENGTH] = '\0';
}

// Validasi email sederhana
int isValidEmail(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return (at && dot && at < dot);
}

// Validasi PIN (hanya angka dan panjang <= 6)
int isValidPin(const char *pin) {
    int i;
    if (strlen(pin) > 6) return 0;
    for (i = 0; pin[i] != '\0'; i++) {
        if (!isdigit(pin[i])) return 0;
    }
    return 1;
}

// Tampilan pembatas untuk mempercantik output
void printSeparator() {
    printf("\n========================================\n");
}

// Registrasi nasabah dengan tampilan profesional
void registrasiNasabah() {
    int pilihan;
    Nasabah nasabah;
    char filename[] = "data_nasabah.txt";
    FILE *file;

    printSeparator();
    printf("        *** Registrasi Nasabah ***\n");
    printSeparator();

    // Input nama
    printf("Masukkan Nama: ");
    fgets(nasabah.nama, sizeof(nasabah.nama), stdin);
    nasabah.nama[strcspn(nasabah.nama, "\n")] = 0;

    // Input email
    do {
        printf("Masukkan Email: ");
        fgets(nasabah.email, sizeof(nasabah.email), stdin);
        nasabah.email[strcspn(nasabah.email, "\n")] = 0;
        if (!isValidEmail(nasabah.email)) {
            printf("Error: Email tidak valid! Silakan coba lagi.\n");
        }
    } while (!isValidEmail(nasabah.email));

    // Input PIN
    do {
        printf("Masukkan PIN (maksimal 6 karakter): ");
        fgets(nasabah.pin, sizeof(nasabah.pin), stdin);
        nasabah.pin[strcspn(nasabah.pin, "\n")] = 0;
        if (!isValidPin(nasabah.pin)) {
            printf("Error: PIN harus berupa angka dan maksimal 6 karakter! Silakan coba lagi.\n");
        }
    } while (!isValidPin(nasabah.pin));

    // Input saldo awal
    while (1) {
        printf("Masukkan Saldo Awal: ");
        if (scanf("%d", &nasabah.saldo) == 1 && nasabah.saldo >= 0) {
            clearInputBuffer();
            break;
        }
        printf("Error: Saldo awal tidak valid! Silakan coba lagi.\n");
        clearInputBuffer();
    }

    // Generate nomor rekening
    generateNoRekening(nasabah.no_rekening);

    // Simpan data ke file dengan format yang terstruktur
    file = fopen(filename, "a+");
    if (file == NULL) {
        printf("Error: Gagal membuka file.\n");
        return;
    }
    fprintf(file, "%s;%s;%s;%s;%d\n", 
            nasabah.no_rekening, nasabah.pin, nasabah.nama, nasabah.email, nasabah.saldo);
    fclose(file);

    // Output data registrasi
    printSeparator();
    printf("        *** Registrasi Berhasil ***\n");
    printSeparator();
    printf("Detail Akun Anda:\n");
    printf("Nama             : %s\n", nasabah.nama);
    printf("Email            : %s\n", nasabah.email);
    printf("Saldo Awal       : %d\n", nasabah.saldo);
    printf("No Rekening     : %s\n", nasabah.no_rekening);

    // Opsi setelah registrasi
    printSeparator();
    printf("Ketik 1 untuk masuk ke fitur Login.\n");
    printf("Ketik 2 untuk keluar dari program.\n");
    printf("Masukkan pilihan Anda: ");
    scanf("%d", &pilihan);
    clearInputBuffer();

    if (pilihan == 1) {
        clearScreen();
        if (loginNasabah()) {
            printf("Selamat datang di sistem kami!\n");
        } else {
            printf("Login gagal. Silakan coba lagi.\n");
        }
    } else if (pilihan == 2) {
        printf("Terima kasih telah menggunakan sistem kami.\n");
        exit(0);
    } else {
        printf("Pilihan tidak valid. Program akan keluar.\n");
        exit(0);
    }
}

// Login nasabah
int loginNasabah() {
    char noRekening[MAX_LENGTH + 1];
    char pin[7];
    Nasabah nasabah;
    int pilihan;
    int found = 0;
    int tries = 0;
    FILE *file;

    printSeparator();
    printf("             * Login Nasabah *\n");
    printSeparator();

    while (tries < 3) {
        file = fopen("data_nasabah.txt", "r");
        if (file == NULL) {
            printf("Error: Gagal membuka file.\n");
            return 0;
        }

        // Input data login
        printf("Masukkan No Rekening: ");
        fgets(noRekening, sizeof(noRekening), stdin);
        noRekening[strcspn(noRekening, "\n")] = '\0';
        clearInputBuffer();

        printf("Masukkan PIN: ");
        fgets(pin, sizeof(pin), stdin);
        pin[strcspn(pin, "\n")] = '\0';

        // Cek data di file (sesuaikan dengan format baru)
        while (fscanf(file, "%[^;];%[^;];%[^;];%[^;];%d\n", 
                      nasabah.no_rekening, nasabah.pin, nasabah.nama, nasabah.email, &nasabah.saldo) == 5) {
            // Cocokkan data login
            if (strcmp(noRekening, nasabah.no_rekening) == 0 && strcmp(pin, nasabah.pin) == 0) {
                found = 1;
                break;
            }
        }

        fclose(file);

        if (found) {
            clearScreen();
            printSeparator();
            printf("             * Login Berhasil *\n");
            printSeparator();
            printSeparator();
            printf("Ketik 1 untuk masuk ke Menu utama.\n");
            printf("Ketik 2 untuk keluar dari program.\n");
            printf("Masukkan pilihan Anda: ");
            scanf("%d", &pilihan);
            clearInputBuffer();

            if (pilihan == 1) {
                clearScreen();
                menuUtama(&nasabah); 
            } else if (pilihan == 2) {
                printf("Terima kasih telah menggunakan sistem kami.\n");
                exit(0);
            } else {
                printf("Pilihan tidak valid. Program akan keluar.\n");
                exit(0);
            }
            return 1;
        } else {
            tries++;
            printf("Error: No Rekening atau PIN salah! (%d/3)\n", tries);
        }

        if (tries == 3) {
            printf("\nLogin gagal 3 kali. Silakan verifikasi dengan email.\n");

            char inputEmail[100];
            printf("Masukkan Email untuk Verifikasi: ");
            fgets(inputEmail, sizeof(inputEmail), stdin);
            inputEmail[strcspn(inputEmail, "\n")] = '\0';

            if (strcmp(inputEmail, nasabah.email) == 0) {
                printf("Verifikasi berhasil! Anda dapat mencoba login kembali.\n");
                tries = 0; // Reset percobaan
            } else {
                printf("Error: Email tidak cocok. Program akan keluar.\n");
                exit(0);
            }
        }
    }

    return 0;
}

void cekSaldo(Nasabah *nasabah) {
    printf("Saldo Anda saat ini adalah: Rp%d\n", nasabah->saldo);
    printf("\nTekan enter untuk kembali ke menu utama...");
    system("pause");
     // Kembali ke menu utama setelah menyelesaikan proses tarik tunai
    getchar();
    menuUtama(nasabah);
    
}


// Tarik Tunai
void tarikTunai(Nasabah *nasabah) {
    int pilihan, jumlah = 0;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    
    printf("Pilih jumlah yang ingin ditarik:\n");
    printf("1. Rp 300\n");
    printf("2. Rp 600\n");
    printf("3. Rp 1000\n");
    printf("4. Lainnya (kelipatan 100)\n");
    printf("Masukkan pilihan Anda (1-4): ");
    scanf("%d", &pilihan);
    clearInputBuffer();
    
    switch (pilihan) {
        case 1:
            jumlah = 300;
            break;
        case 2:
            jumlah = 600;
            break;
        case 3:
            jumlah = 1000;
            break;
        case 4:
            printf("Masukkan jumlah yang ingin ditarik (kelipatan 100): ");
            scanf("%d", &jumlah);
            clearInputBuffer();
            if (jumlah % 100 != 0) {
                printf("Jumlah harus dalam kelipatan 100.\n");
                return;
            }
            break;
        default:
            printf("Pilihan tidak valid. Silakan coba lagi.\n");
            return;
    }

    if (jumlah > nasabah->saldo) {
        printf("Saldo Anda tidak mencukupi.\n");
    } else {
        nasabah->saldo -= jumlah;
        system("cls");
        printf("Penarikan berhasil. Sisa saldo Anda: Rp%d\n\n", nasabah->saldo);

        // Update file dengan saldo baru
        updateSaldoNasabah(nasabah);
        
       // Simpan transaksi ke riwayat mutasi dengan tanggal
	    char filename[25];
	    FILE *mutasiFile;
	    snprintf(filename, sizeof(filename), "%s_mutasi.txt", nasabah->no_rekening);
	    mutasiFile = fopen(filename, "a+");
	    if (mutasiFile != NULL) {
	        fprintf(mutasiFile, "%02d-%02d-%04d Tarik Tunai: -Rp%d, Sisa Saldo: Rp%d\n", 
	                local->tm_mday, local->tm_mon + 1, local->tm_year + 1900, jumlah, nasabah->saldo);
	        fclose(mutasiFile);
    }
    }

    // Kembali ke menu utama setelah menyelesaikan proses tarik tunai
    printf("\nTekan enter untuk kembali ke menu utama...");
    getchar();
    menuUtama(nasabah);
   
}


// Transfer Uang
void transferUang(Nasabah *nasabah) {
    char noRekTujuan[MAX_LENGTH + 1];
    int jumlahTransfer, found = 0;
    Nasabah penerima;
    FILE *file;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    // Input nomor rekening tujuan
    printf("Masukkan nomor rekening tujuan: ");
    getchar(); // Tambahkan baris ini 
    fgets(noRekTujuan, sizeof(noRekTujuan), stdin);
    noRekTujuan[strcspn(noRekTujuan, "\n")] = '\0';

    // Input jumlah uang yang akan ditransfer
    printf("\nMasukkan jumlah uang yang akan ditransfer: ");
    if (scanf("%d", &jumlahTransfer) != 1) {
        printf("Error: Input tidak valid. Silakan masukkan angka.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    // Validasi jumlah transfer
    if (jumlahTransfer > nasabah->saldo) {
        printf("Saldo Anda tidak mencukupi.\n");
        return;
    }

    // Buka file data nasabah
    file = fopen("data_nasabah.txt", "r");
    if (file == NULL) {
        printf("Error: Gagal membuka file.\n");
        return;
    }

    // Cari data penerima di file
    while (fscanf(file, "%[^;];%[^;];%[^;];%[^;];%d\n", 
                    penerima.no_rekening, penerima.pin, penerima.nama, penerima.email, &penerima.saldo) == 5) {
        if (strcmp(noRekTujuan, penerima.no_rekening) == 0) {
            found = 1;
            break;
        }
    }

    // Tutup file
    fclose(file);

    // Jika rekening tujuan ditemukan
    if (found) {
        // Kurangi saldo pengirim
        nasabah->saldo -= jumlahTransfer;

        // Tambah saldo penerima
        penerima.saldo += jumlahTransfer;

        // Update data di file (pengirim dan penerima)
        updateSaldoNasabah(nasabah); // Update saldo pengirim
        updateSaldoPenerima(penerima); // Update saldo penerima

        system("cls");
        printf("Transfer berhasil ke rekening %s sebesar Rp%d\n", noRekTujuan, jumlahTransfer);
        printf("Sisa saldo Anda: Rp%d\n", nasabah->saldo);

        // Simpan transaksi ke riwayat mutasi dengan tanggal
	    char filename[25];
	    FILE *mutasiFile;
	    snprintf(filename, sizeof(filename), "%s_mutasi.txt", nasabah->no_rekening);
	    mutasiFile = fopen(filename, "a+");
	    if (mutasiFile != NULL) {
	        fprintf(mutasiFile, "%02d-%02d-%04d Transfer ke %s: -Rp%d, Sisa Saldo: Rp%d\n", 
	                local->tm_mday, local->tm_mon + 1, local->tm_year + 1900, noRekTujuan, jumlahTransfer, nasabah->saldo);
	        fclose(mutasiFile);
	    }

    } else {
        printf("Nomor rekening tujuan tidak ditemukan.\n");
    }

    // Kembali ke menu utama setelah menyelesaikan proses transfer uang
    printf("\nTekan enter untuk kembali ke menu utama...");
    getchar();
    menuUtama(nasabah);
}


// Fungsi untuk update saldo nasabah (baik pengirim maupun penerima)
void updateSaldoNasabah(Nasabah *nasabah) {
    FILE *file, *tempFile;
    Nasabah nasabahTemp;
    char filename[] = "data_nasabah.txt";
    char tempFilename[] = "temp_data_nasabah.txt";

    // Buka file data nasabah dan file sementara
    file = fopen(filename, "r");
    tempFile = fopen(tempFilename, "w");
    if (file == NULL || tempFile == NULL) {
        printf("Error: Gagal membuka file.\n");
        return;
    }

    // Salin data ke file sementara dan update saldo nasabah yang sesuai
    while (fscanf(file, "%[^;];%[^;];%[^;];%[^;];%d\n", 
                    nasabahTemp.no_rekening, nasabahTemp.pin, nasabahTemp.nama, nasabahTemp.email, &nasabahTemp.saldo) == 5) {
        if (strcmp(nasabah->no_rekening, nasabahTemp.no_rekening) == 0) {
            nasabahTemp.saldo = nasabah->saldo; // Update saldo
        }
        fprintf(tempFile, "%s;%s;%s;%s;%d\n", 
                nasabahTemp.no_rekening, nasabahTemp.pin, nasabahTemp.nama, nasabahTemp.email, nasabahTemp.saldo);
    }

    // Tutup file
    fclose(file);
    fclose(tempFile);

    // Hapus file lama dan ganti dengan file sementara
    remove(filename);
    rename(tempFilename, filename);
}

void updateSaldoPenerima(Nasabah penerima) {
    FILE *file, *tempFile;
    Nasabah nasabahTemp;
    char filename[] = "data_nasabah.txt";
    char tempFilename[] = "temp_data_nasabah.txt";

    // Buka file data nasabah dan file sementara
    file = fopen(filename, "r");
    tempFile = fopen(tempFilename, "w");
    if (file == NULL || tempFile == NULL) {
        printf("Error: Gagal membuka file.\n");
        return;
    }

    // Salin data ke file sementara dan update saldo penerima
    while (fscanf(file, "%[^;];%[^;];%[^;];%[^;];%d\n", 
                  nasabahTemp.no_rekening, nasabahTemp.pin, nasabahTemp.nama, nasabahTemp.email, &nasabahTemp.saldo) == 5) {
        if (strcmp(penerima.no_rekening, nasabahTemp.no_rekening) == 0) {
            nasabahTemp.saldo = penerima.saldo;
        }
        fprintf(tempFile, "%s;%s;%s;%s;%d\n", 
                nasabahTemp.no_rekening, nasabahTemp.pin, nasabahTemp.nama, nasabahTemp.email, nasabahTemp.saldo);
    }

    // Tutup file
    fclose(file);
    fclose(tempFile);

    // Hapus file lama dan ganti dengan file sementara
    remove(filename);
    rename(tempFilename, filename);
}

void mutasi(Nasabah *nasabah) {
    char filename[25];
    FILE *mutasiFile;
    char line[256];

    // Membuat nama file mutasi berdasarkan nomor rekening nasabah
    snprintf(filename, sizeof(filename), "%s_mutasi.txt", nasabah->no_rekening);

    // Membuka file mutasi dalam mode read ("r")
    mutasiFile = fopen(filename, "r");

    // Jika file mutasi tidak ditemukan, tampilkan pesan error
    if (mutasiFile == NULL) {
        printf("Tidak Ada Histori Transaksi.\n\n");
        system("pause"); // Jeda sebelum kembali ke menu
        printf("\nTekan enter untuk kembali ke menu utama...");
        getchar();
        menuUtama(nasabah);
        return;
    }

    // Menampilkan judul riwayat mutasi
    printf("\n             === Riwayat Mutasi ===\n\n");

    // Membaca setiap baris dari file mutasi dan menampilkannya
    while (fgets(line, sizeof(line), mutasiFile) != NULL) {
        printf("%s", line);
    }

    // Menutup file mutasi
    fclose(mutasiFile);

    // Kembali ke menu utama setelah menampilkan mutasi
    system("pause"); // Jeda sebelum kembali ke menu
    printf("\n\nTekan enter untuk kembali ke menu utama...");
    getchar();
    menuUtama(nasabah);
}


void pengaturanAkun(Nasabah *nasabah) {
    int pilihan;
    printf("\n=== Pengaturan Akun ===\n");
    printf("1. Lihat Detail Akun\n");
    printf("2. Ubah PIN\n");
    printf("Pilihan: ");
    scanf("%d", &pilihan);
    clearInputBuffer();

    switch (pilihan) {
        case 1:
            printf("Nama: %s\n", nasabah->nama);
            printf("Saldo: Rp%d\n", nasabah->saldo);
            printf("No Rekening: %s\n", nasabah->no_rekening);
            break;

        case 2: {
            char pinBaru[7];
            printf("Masukkan PIN baru (maksimal 6 karakter): ");
            fgets(pinBaru, sizeof(pinBaru), stdin);
            pinBaru[strcspn(pinBaru, "\n")] = '\0';

            if (isValidPin(pinBaru)) {
                strcpy(nasabah->pin, pinBaru);
                printf("PIN berhasil diubah.\n");
            } else {
                printf("PIN tidak valid! Harus berupa angka dan maksimal 6 karakter.\n");
            }
            break;
        }

        default:
            printf("Pilihan tidak valid.\n");
    }
}

int verifikasiEmail(const char *email, const char *verifikasiEmail) {
    return strcmp(email, verifikasiEmail) == 0;
}

void menuUtama(Nasabah *nasabah) {
	system("cls");
    int pilihan;

    printf("                 * Menu Utama *\n\n\n");
    printSeparator();
    printf("1. Cek Saldo                        4. Tarik Tunai\n\n");
    printf("2. Transfer Uang                    5. Mutasi\n\n");
    printf("3. Pengaturan Akun                  6. Keluar\n\n");
    printf("Masukkan pilihan Anda: ");
    scanf("%d", &pilihan);

    switch (pilihan) {
        case 1:
        	system("cls");
            cekSaldo(nasabah); 
            
            break;
        case 2:
        	system("cls");
            transferUang(nasabah); 
            break;
        case 3:
        	system("cls");
            pengaturanAkun(nasabah); 
            break;
        case 4:
        	system("cls");
            tarikTunai(nasabah); 
            break;
        case 5:
        	system("cls");
            // Implementasi modul mutasi
            mutasi(nasabah);
            break;
        case 6:
            exitATM();
            break;
        default:
            printf("Pilihan tidak valid. Silakan coba lagi.\n");
    }
}

void exitATM() {
	system("cls");
    printf("Terima kasih telah menggunakan ATM Kami. Selamat tinggal!\n");
    exit(0);
}

