#ifndef PROGRAMATM_H
#define PROGRAMATM_H

typedef struct {
    char no_rekening[20];
    char nama[50];
    char email[50];
    char pin[10];
    int saldo;
} Nasabah;

void clearScreen();
void clearInputBuffer();
void welcome();
void generateNoRekening(char *noRekening);
void registrasiNasabah();
int loginNasabah();
void menuUtama();
void exitATM();
void cekSaldo(Nasabah *nasabah);
void tarikTunai(Nasabah *nasabah);
void transferUang(Nasabah *nasabah);
void pengaturanAkun(Nasabah *nasabah);
int verifikasiEmail(const char *email, const char *verifikasiEmail);
void updateSaldoPenerima(Nasabah penerima);
void updateSaldoNasabah(Nasabah *nasabah);
void mutasi(Nasabah *nasabah);
#endif
