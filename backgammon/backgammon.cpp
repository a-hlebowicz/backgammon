#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#define BANDA_DODATNICH 26
#define BANDA_UJEMNYCH 27
#define DWOR_DODATNICH 28
#define DWOR_UJEMNYCH 29
#define DUBLET 4	
#define TAK 1
#define NIE 0	
#define DODATNI 1
#define UJEMNY -1
#define ROZMIAR_PLANSZY 24
#define BICIE 2
#define WPROWADZENIE_DO_DOMU 3
#define LICZBA_PIONKOW 15
#define PIERWSZE_POLE_DOMU_DODATNICH 19
#define PIERWSZE_POLE_DOMU_UJEMNYCH 6
#define WPROWADZENIE_DO_DOMU 3
#define DOM_D_PELNY 30
#define DOM_U_PELNY 31
#define POCZATEK_LEWA 40
#define POCZATEK_GORA 2
#define DLUGOSC_PLANSZY 18
#define SZEROKOSC_PLANSZY 38
#define POLE_STARTOWE_DODATNI 0
#define POLE_STARTOWE_UJEMNY 25
#define POLE_12 12
#define DO_GORY 1
#define DO_DOLU -1
#define TEXTW 15
#define KOMUNIKAT 25
#define BICIE_PARAMETRY_PIONEK 1
#define BICIE_PARAMETRY_KOSTKA 2
#define BRAK_RUCHOW -1
#define DO_DOMU 1
#define ZWYKLY 3

void conio2ustawienia() {
	settitle("backgammon");
	_setcursortype(_NOCURSOR);
	textbackground(0);
	clrscr();
	textcolor(7);
}
void zeruj(int* plansza) {
	for (int i = 0; 33 > i; i++)plansza[i] = 0;
}
void ustaw_plansze(int* plansza) {
	//biale
	plansza[1] = 2;
	plansza[12] = 5;
	plansza[17] = 3;
	plansza[19] = 5;
	//czerwone
	plansza[6] = -5;
	plansza[24] = -2;
	plansza[13] = -5;
	plansza[8] = -3;

}
void komunikat() {
	gotoxy(1, 6);
	cputs("test");
	getch();
}
int poleStartowe(int gracz) {
	if (gracz == DODATNI)return POLE_STARTOWE_DODATNI;
	if (gracz == UJEMNY)return POLE_STARTOWE_UJEMNY;
}
void rzuc_kostkami(int kostka[]) {

	kostka[1] = rand() % 6 + 1;
	kostka[2] = rand() % 6 + 1;
	kostka[3] = kostka[1] + kostka[2];
	kostka[DUBLET] = NIE;
	if (kostka[1] == kostka[2]) {
		kostka[DUBLET] = TAK;
		kostka[0] = DUBLET;
	}
	kostka[0] = 0;

}
int kto_startuje(int kostka[]) {

	do {
		rzuc_kostkami(kostka);
	} while (kostka[1] == kostka[2]);

	if (kostka[1] > kostka[2])return DODATNI; //zaczyna gracz 1
	else return UJEMNY; //zaczyna gracz 2
}
//ruch
int czyBandaZajeta(int* plansza, int gracz) {
	if (gracz == DODATNI) if (plansza[BANDA_DODATNICH] == 0)return NIE;
	if (gracz == UJEMNY) if (plansza[BANDA_UJEMNYCH] == 0)return NIE;
	return TAK;
}
int czyDomPelny(int* plansza, int gracz) {
	if (gracz == DODATNI) {
		for (int i = 1; PIERWSZE_POLE_DOMU_DODATNICH > i; i++)if (plansza[i] > 0) {
			plansza[DOM_D_PELNY] = NIE;
			return NIE;
		}
		if (plansza[BANDA_DODATNICH] > 0) {
			plansza[DOM_D_PELNY] = NIE;
			return NIE;
		}
		plansza[DOM_D_PELNY] = TAK;
		return TAK;
	}
	if (gracz == UJEMNY) {
		for (int i = ROZMIAR_PLANSZY; PIERWSZE_POLE_DOMU_UJEMNYCH < i; i--)if (plansza[i] < 0) {
			plansza[DOM_U_PELNY] = NIE;
			return NIE;
		}
		if (plansza[BANDA_UJEMNYCH] > 0) {
			plansza[DOM_U_PELNY] = NIE;
			return NIE;
		}
		plansza[DOM_U_PELNY] = TAK;
		return TAK;
	}
}
int czyRuchMozliwy(int* plansza, int kostka, int gracz, int pionek) {
	if (kostka == 0)return NIE;
	if (gracz == DODATNI) {
		if (pionek + kostka > ROZMIAR_PLANSZY)if (plansza[DOM_D_PELNY] == TAK)return WPROWADZENIE_DO_DOMU; else return NIE;
		if (plansza[pionek + kostka] >= 0)return TAK;
		if (plansza[pionek + kostka] == -1)return BICIE;
		if (plansza[pionek + kostka] < -1)return NIE;
	}

	if (gracz == UJEMNY) {
		if (pionek - kostka < 1)if (plansza[DOM_U_PELNY] == TAK)return WPROWADZENIE_DO_DOMU; else return NIE;
		if (plansza[pionek - kostka] <= 0)return TAK;
		if (plansza[pionek - kostka] == 1)return BICIE;
		if (plansza[pionek - kostka] > 1)return NIE;
	}
}
int czyRuchMozliwyDlaJakiejkolwiekKostki(int* plansza, int kostka[], int gracz, int pionek) {
	if (kostka[0] == DUBLET) {
		if (czyRuchMozliwy(plansza, kostka[1], gracz, pionek) > 0)return TAK;
	}
	else if (czyRuchMozliwy(plansza, kostka[1], gracz, pionek) + czyRuchMozliwy(plansza, kostka[2], gracz, pionek) > 0)return TAK;
	else return NIE;
}
int czyRuchMozliwyDlaKostki3(int* plansza, int kostka[], int gracz, int pionek) {
	if (kostka[1] == 0 || kostka[2] == 0)return NIE;
	if (czyRuchMozliwy(plansza, kostka[1], gracz, pionek) == TAK)return TAK;
	if (czyRuchMozliwy(plansza, kostka[2], gracz, pionek) == TAK)return TAK;
	else return NIE;
}
int czyJakisRuchMozliwy(int* plansza, int kostka[], int gracz, int banda) {
	if (banda == TAK) {
		return czyRuchMozliwyDlaJakiejkolwiekKostki(plansza, kostka, gracz, poleStartowe(gracz));
	}
	for (int i = 1; ROZMIAR_PLANSZY >= i; i++) { //przejdz po calej planszy
		if ((gracz == DODATNI && plansza[i] > 0) || (gracz == UJEMNY && plansza[i] < 0))if (czyRuchMozliwyDlaJakiejkolwiekKostki(plansza, kostka, gracz, i) > 0)return TAK;
	}
	return NIE;
}
int czyBicieNajblizejDomu(int pionek, int gracz_ktory_bije, int obecnie_najblizszy) {
	if (gracz_ktory_bije == DODATNI) {
		if (pionek <= obecnie_najblizszy || obecnie_najblizszy == 0) return TAK;
		else return NIE;
	}
	if (gracz_ktory_bije == UJEMNY) {
		if (pionek >= obecnie_najblizszy) return TAK;
		else return NIE;
	}
}
int czyWymuszanieBiciaK12(int* plansza, int kostka[], int gracz, int bicie_parametry[], int max_i, int numer_kostki, int pionek) { //czyWymuszanieBiciaDlaKostki12
	int indexBitego;
	if (czyRuchMozliwy(plansza, kostka[numer_kostki], gracz, pionek) == BICIE) {
		if (gracz == DODATNI)indexBitego = pionek + kostka[numer_kostki];
		if (gracz == UJEMNY)indexBitego = pionek - kostka[numer_kostki];
		if (czyBicieNajblizejDomu(indexBitego, gracz, max_i) == TAK) {
			max_i = indexBitego;
			bicie_parametry[BICIE_PARAMETRY_PIONEK] = pionek;
			bicie_parametry[BICIE_PARAMETRY_KOSTKA] = numer_kostki;
		}
	}
	return max_i;
}
int czyWymuszanieBiciaK3(int* plansza, int kostka[], int gracz, int bicie_parametry[], int max_i, int pionek) {
	int indexBitego = 0;
	if (czyRuchMozliwyDlaKostki3(plansza, kostka, gracz, pionek) == TAK && czyRuchMozliwy(plansza, kostka[3], gracz, pionek) == BICIE) {
		if (gracz == DODATNI)indexBitego = pionek + kostka[3];
		if (gracz == UJEMNY)indexBitego = pionek - kostka[3];
		if (czyBicieNajblizejDomu(pionek, gracz, max_i) == TAK) {
			max_i = indexBitego;
			bicie_parametry[BICIE_PARAMETRY_PIONEK] = pionek;
			bicie_parametry[BICIE_PARAMETRY_KOSTKA] = 3;
		}
	}
	return max_i;
}
int czyWymuszanie(int* plansza, int kostka[], int gracz, int b_p[], int max_i, int pionek) {
	max_i = czyWymuszanieBiciaK12(plansza, kostka, gracz, b_p, max_i, 1, pionek);
	max_i = czyWymuszanieBiciaK12(plansza, kostka, gracz, b_p, max_i, 2, pionek);
	max_i = czyWymuszanieBiciaK3(plansza, kostka, gracz, b_p, max_i, pionek);
	return max_i;
}
int czyWymuszanieBicia(int* plansza, int kostka[], int gracz, int b_p[], int banda) {
	int  max_i = 0;
	if (banda == TAK) max_i = czyWymuszanie(plansza, kostka, gracz, b_p, max_i, poleStartowe(gracz));
	else {
		for (int i = 1; ROZMIAR_PLANSZY >= i; i++) { //przejdz po calej planszy	
			if ((plansza[i] > 0 && gracz == DODATNI) || (plansza[i] < 0 && gracz == UJEMNY)) max_i = czyWymuszanie(plansza, kostka, gracz, b_p, max_i, i);
		}
	}
	if (max_i != 0) {
		return TAK;
	}
	else return NIE;
}
//przesuniecie
void przesuniecieDODATNI(int* plansza, int wartosc_kostki, int gracz, int pionek, int wynik) {
	if (wynik == TAK)plansza[pionek + wartosc_kostki]++;
	if (wynik == BICIE) {
		plansza[pionek + wartosc_kostki] = 1;
		plansza[BANDA_UJEMNYCH]++;
	}
	if (wynik == WPROWADZENIE_DO_DOMU)plansza[DWOR_DODATNICH]++;
	//odjecie pionka z pola startowego
	if (pionek == 0)plansza[BANDA_DODATNICH]--;
	else plansza[pionek]--;
}
void przesuniecieUJEMNY(int* plansza, int wartosc_kostki, int gracz, int pionek, int wynik) {
	if (wynik == TAK)plansza[pionek - wartosc_kostki]--;
	if (wynik == BICIE) {
		plansza[pionek - wartosc_kostki] = -1;
		plansza[BANDA_DODATNICH]++;
	}
	if (wynik == WPROWADZENIE_DO_DOMU)plansza[DWOR_UJEMNYCH]++;
	//odjecie pionka z pola startowego
	if (pionek == ROZMIAR_PLANSZY + 1)plansza[BANDA_UJEMNYCH]--;
	else plansza[pionek]++;
}
void przesuniecie(int* plansza, int wartosc_kostki, int gracz, int pionek) {
	int wynik = czyRuchMozliwy(plansza, wartosc_kostki, gracz, pionek);
	if (gracz == DODATNI) {
		przesuniecieDODATNI(plansza, wartosc_kostki, gracz, pionek, wynik);
	}
	if (gracz == UJEMNY) {
		przesuniecieUJEMNY(plansza, wartosc_kostki, gracz, pionek, wynik);
	}

}
//zapis
void podajNazwe(char* nazwa_pliku) {
	scanf("%100s", nazwa_pliku);
}
void wczytaj(int* plansza) {
	int gracz;
	char nazwa_pliku[101];
	gotoxy(1, 5);
	cputs("wczytywanie pliku>");
	podajNazwe(nazwa_pliku);
	FILE* plik;
	plik = fopen(nazwa_pliku, "r");
	if (plik == NULL) {
		gotoxy(1, 6);
		cputs("B³¹d otwarcia pliku do zapisu");
		return;
	}
	for (int i = 0; ROZMIAR_PLANSZY + 9 > i; i++) {
		fscanf(plik, "%d\n", &plansza[i]);
	}
	fclose(plik);
}
void zapisz(int* plansza, int gracz) {
	char nazwa_pliku[101];
	gotoxy(1, 5);
	cputs("zapisywanie pliku>");
	podajNazwe(nazwa_pliku);
	FILE* plik;
	plik = fopen(nazwa_pliku, "w");
	if (plik == NULL) {
		perror("B³¹d otwarcia pliku do zapisu");
		return;
	}
	fprintf(plik, "%d\n", gracz);
	/*for (int i = 0; 5 > i; i++) {
		fprintf(plik, "%d", kostka[i]);
	}*/
	for (int i = 1; ROZMIAR_PLANSZY + 9 > i; i++) {
		fprintf(plik, "%d\n", plansza[i]);
	}
	fclose(plik);
}
//rysowanie
void rysujMenu() {
	clrscr();
	gotoxy(1, 1);
	cputs("Aleksander Hlebowicz s197700");
	gotoxy(1, 2);
	cputs("g = graj");
	gotoxy(1, 3);
	cputs("w = wczytaj");
	gotoxy(1, 4);
	cputs("q = wyjdz");
}
void rysujMenuGry(int kostka[], int gracz) {

	gotoxy(1, TEXTW);
	cputs("kostka  [");
	putch(kostka[1] + '0');
	cputs(",");
	putch(kostka[2] + '0');
	cputs("] ");

	gotoxy(1, TEXTW + 1);
	cputs("rusza sie gracz ");
	if (gracz == DODATNI)cputs("BIALY (x)");
	if (gracz == UJEMNY)cputs("CZERWONY (o)");

}
void rysujRamkeRogi() {
	gotoxy(POCZATEK_LEWA - 1, POCZATEK_GORA - 1);
	putch('+');
	gotoxy(POCZATEK_LEWA - 1, POCZATEK_GORA + DLUGOSC_PLANSZY + 1);
	putch('+');
	gotoxy(POCZATEK_LEWA + SZEROKOSC_PLANSZY + 1, POCZATEK_GORA - 1);
	putch('+');
	gotoxy(POCZATEK_LEWA + SZEROKOSC_PLANSZY + 1, POCZATEK_GORA + DLUGOSC_PLANSZY + 1);
	putch('+');
}
void rysujRamkeBoki() {
	for (int k = POCZATEK_LEWA; SZEROKOSC_PLANSZY + POCZATEK_LEWA + 2 >= k; k++) {
		gotoxy(k, POCZATEK_GORA - 1);
		putch('-');
		gotoxy(k, POCZATEK_GORA + DLUGOSC_PLANSZY + 1);
		putch('-');

		gotoxy(k, POCZATEK_GORA + 1);
		putch('-');
		gotoxy(k, POCZATEK_GORA + DLUGOSC_PLANSZY - 1);
		putch('-');

		gotoxy(k, POCZATEK_GORA + DLUGOSC_PLANSZY / 2);
		putch('-');
	}
	for (int w = POCZATEK_GORA; POCZATEK_GORA + DLUGOSC_PLANSZY >= w; w++) {
		gotoxy(POCZATEK_LEWA - 1, w);
		putch('|');
		gotoxy(POCZATEK_LEWA + SZEROKOSC_PLANSZY + 1, w);
		putch('|');

		gotoxy(POCZATEK_LEWA + 18, w);
		putch('|');
		gotoxy(POCZATEK_LEWA + 20, w);
		putch('|');
	}
}
void rysujDwor() {
	int k = POCZATEK_LEWA + SZEROKOSC_PLANSZY;
	gotoxy(k + 2, POCZATEK_GORA - 1);
	putchar('-');
	gotoxy(k + 2, POCZATEK_GORA + DLUGOSC_PLANSZY + 1);
	putchar('-');
	gotoxy(k + 3, POCZATEK_GORA - 1);
	putchar('+');
	gotoxy(k + 3, POCZATEK_GORA + DLUGOSC_PLANSZY + 1);
	putchar('+');
	for (int w = POCZATEK_GORA; POCZATEK_GORA + DLUGOSC_PLANSZY >= w; w++) {
		gotoxy(k + 3, w);
		putchar('|');
	}
}
void rysujRamke() {
	rysujRamkeRogi();
	rysujRamkeBoki();
	rysujDwor();
}
void rysujIndexDol() {
	int index = 0, swiadek = 0, w = POCZATEK_GORA + DLUGOSC_PLANSZY;
	char indexchar;
	for (int k = SZEROKOSC_PLANSZY + POCZATEK_LEWA - 2; POCZATEK_LEWA - 2 < k; k += -3) {
		index++;
		if (index == 7 and swiadek == 0) {
			swiadek = 1;
			index--;
		}
		else if (index > 9) {
			gotoxy(k + 1, w);
			indexchar = (index / 10) + '0';
			putch(indexchar);
			gotoxy(k + 2, w);
			indexchar = (index % 10) + '0';
			putch(indexchar);
		}
		else if (index <= 9) {
			gotoxy(k + 1, w);
			indexchar = index + '0';
			putch(indexchar);
		}



	}
}
void rysujIndexGora() {
	int index = 12, swiadek = 0, w = POCZATEK_GORA;
	char indexchar;
	for (int k = POCZATEK_LEWA; SZEROKOSC_PLANSZY + POCZATEK_LEWA > k; k += 3) {
		index++;
		if (index == 19 and swiadek == 0) {
			swiadek = 1;
			index--;
		}

		else if (index > 9) {
			gotoxy(k + 1, w);
			indexchar = (index / 10) + '0';
			putch(indexchar);
			gotoxy(k + 2, w);
			indexchar = (index % 10) + '0';
			putch(indexchar);
		}


	}
}
void rysujDodatki() {
	int k = POCZATEK_LEWA + SZEROKOSC_PLANSZY / 2, w = POCZATEK_GORA + DLUGOSC_PLANSZY / 2;
	gotoxy(k - 1, w);
	putch('B');
	gotoxy(k, w);
	putch('A');
	gotoxy(k + 1, w);
	putch('R');
	k = POCZATEK_LEWA + SZEROKOSC_PLANSZY + 2;
	gotoxy(k - 1, w);
	putch('E');
	gotoxy(k, w);
	putch('N');
	gotoxy(k + 1, w);
	putch('D');
}
void rysujIndex() {
	rysujIndexGora();
	rysujIndexDol();
	rysujDodatki();
}
int zamianaIndexKolumny(int index) {
	if (index <= 6 && index > 0) {
		return (POCZATEK_LEWA + SZEROKOSC_PLANSZY - 1) - 3 * --index;
	}
	if (index <= 12) {
		return (POCZATEK_LEWA + SZEROKOSC_PLANSZY / 2) - 3 * (index - 6);
	}
	if (index <= 18) {
		return (POCZATEK_LEWA + 1) + 3 * (--index - 12);
	}
	if (index <= 24) {
		return (POCZATEK_LEWA + SZEROKOSC_PLANSZY / 2) + 3 * (index - 18);
	}
}
int zamianaIndexWiersza(int index) {
	if (index <= 12) {
		return (POCZATEK_GORA + DLUGOSC_PLANSZY - 2);
	}
	else {
		return (POCZATEK_GORA + 2);
	}
}
void rysujPionkiDol(int pole) {
	int k = zamianaIndexKolumny(pole);
	int w = zamianaIndexWiersza(pole);
	gotoxy(k, w);
	putchar('x');
}
void rysujStosIkony(char ikona) {
	if (ikona == 'o') {
		textcolor(RED);
		putch('o');
		textcolor(LIGHTGRAY);
	}
	if (ikona == 'x') {
		textcolor(WHITE);
		putch('x');
		textcolor(LIGHTGRAY);
	}
}
void rysujStos(int* plansza, int index, int k, int w, int kierunek) {
	for (int i = 0; abs(plansza[index]) > i; i++) {
		if (i > 5)break;
		gotoxy(k, w - (i * kierunek));
		if (index == BANDA_UJEMNYCH || index == DWOR_UJEMNYCH) {
			if (plansza[index] > 0) rysujStosIkony('o');//na bandzie s¹ liczone normalnie

		}
		else {
			if (plansza[index] < 0) rysujStosIkony('o');
			if (plansza[index] > 0) rysujStosIkony('x');
		}
	}
	if (abs(plansza[index]) > 5) {
		if (kierunek > 0)gotoxy(k, w - 5);
		else if (abs(plansza[index]) > 9)gotoxy(k, w + 6);
		else gotoxy(k, w + 5);
		putch(abs(plansza[index]) % 10 + '0');
		if (abs(plansza[index]) > 9) {
			if (kierunek > 0)gotoxy(k, w - 6);
			else gotoxy(k, w + 5);
			putch(abs(plansza[index]) / 10 + '0');
		}
	}
}
void rysujPlanszaStos(int* plansza, int index, int k, int w) {
	if (index <= POLE_12) {
		int kierunek = DO_GORY;
		rysujStos(plansza, index, k, w, kierunek);
	}
	else {
		int kierunek = DO_DOLU;
		rysujStos(plansza, index, k, w, kierunek);
	}
}
void rysujBande(int* plansza) {
	int k, w;

	k = POCZATEK_LEWA + SZEROKOSC_PLANSZY / 2;
	w = POCZATEK_GORA + 2;
	rysujStos(plansza, BANDA_DODATNICH, k, w, DO_DOLU);
	w = POCZATEK_GORA + DLUGOSC_PLANSZY - 2;
	rysujStos(plansza, BANDA_UJEMNYCH, k, w, DO_GORY);
}
void rysujDwor(int* plansza) {
	int k, w;

	k = POCZATEK_LEWA + SZEROKOSC_PLANSZY + 2;
	w = POCZATEK_GORA + 2;
	rysujStos(plansza, DWOR_DODATNICH, k, w, DO_DOLU);
	w = POCZATEK_GORA + DLUGOSC_PLANSZY - 2;
	rysujStos(plansza, DWOR_UJEMNYCH, k, w, DO_GORY);
}
void rysujPlansza(int* plansza) {

	for (int i = 1; ROZMIAR_PLANSZY >= i; i++) {
		if (plansza[i] != 0) {
			int k = zamianaIndexKolumny(i);
			int w = zamianaIndexWiersza(i);
			rysujPlanszaStos(plansza, i, k, w);
		}
	}
	rysujBande(plansza);
	rysujDwor(plansza);
}
void rysujGra(int* plansza, int kostka[], int gracz) {
	clrscr();
	rysujMenuGry(kostka, gracz);
	rysujRamke();
	rysujIndex();
	rysujPlansza(plansza);
}
//wypisywanie
int ktoWygrywa(int* plansza) {
	if (plansza[DWOR_DODATNICH] == LICZBA_PIONKOW)return DODATNI;
	if (plansza[DWOR_UJEMNYCH] == LICZBA_PIONKOW)return UJEMNY;
	return NIE;
}
void wygranaRysuj(int gracz, int wynik, int* plansza) {
	int x = SZEROKOSC_PLANSZY + POCZATEK_LEWA + 5;
	int y = 2;
	gotoxy(x, y);
	y++;
	cputs("KONIEC GRY");
	gotoxy(x, y);
	y++;
	cputs("WYGRYWA GRACZ >");
	if (gracz == DODATNI)cputs("BIALY");
	else cputs("CZERWONY");
	gotoxy(x, y);
	y++;
	cputs("Z WYNIKIEM > ");
	putch(wynik + '0');
	gotoxy(x, y);
	y++;
	cputs("nacisnij dowolny przycisk ");
	rysujPlansza(plansza);
	getch();
}
void wygrana(int* plansza, int gracz) {
	int wynik = 0;
	if (gracz == DODATNI) {
		wynik++;
		if (plansza[DWOR_UJEMNYCH] == 0)wynik++;
		if (plansza[BANDA_UJEMNYCH] > 0)wynik = 3;
	}
	if (gracz == UJEMNY) {
		wynik++;
		if (plansza[DWOR_DODATNICH] == 0)wynik++;
		if (plansza[BANDA_DODATNICH] > 0)wynik = 3;
	}
	wygranaRysuj(gracz, wynik, plansza);
}
int ustawWszystko(int* plansza, int kostka[]) {
	rzuc_kostkami(kostka);
	zeruj(plansza);
	ustaw_plansze(plansza);
	return kto_startuje(kostka);
}
void kolejkaPrzepada() {
	gotoxy(1, KOMUNIKAT - 1);
	cputs("kolejka przepada");
	gotoxy(1, KOMUNIKAT);
	cputs("nacisnij dowolny przycisk aby kontynuowac");
	getch();
}
void czyscKomunikat() {
	gotoxy(1, KOMUNIKAT);
	clreol();
	gotoxy(1, KOMUNIKAT + 1);
	clreol();
	gotoxy(1, KOMUNIKAT + 2);
	clreol();
	gotoxy(1, KOMUNIKAT + 3);
	clreol();
	gotoxy(1, KOMUNIKAT + 4);
	clreol();
	gotoxy(1, KOMUNIKAT - 1); //zastrzezenia
	clreol();
}
int koniecRuchu() {
	int przycisk;
	czyscKomunikat();
	gotoxy(1, KOMUNIKAT);
	cputs("nacisnij dowolny przycisk aby kontynuowac");
	gotoxy(1, KOMUNIKAT + 1);
	cputs("nacisnij m aby wrocic do menu");
	gotoxy(1, KOMUNIKAT + 2);
	cputs("nacisnij s aby zapisac");
	przycisk = getch();
	return przycisk;
}
void wyborRuchuLegenda() {
	czyscKomunikat();
	cputs("wczytywanie przyciskow");
	gotoxy(1, KOMUNIKAT + 1);
	cputs("1 = ruch pierwsza kostka");
	gotoxy(1, KOMUNIKAT + 2);
	cputs("2 = ruch druga kostka");
	gotoxy(1, KOMUNIKAT + 3);
	cputs("3 = ruch dwoma naraz");
}
void wyborRuchuLegenda2(int kostka1, int kostka2, int kostka3) {
	int i = 1;
	czyscKomunikat();
	gotoxy(1, KOMUNIKAT);
	cputs("wczytywanie przyciskow");
	gotoxy(1, KOMUNIKAT + i);
	if (kostka1 >= TAK) {
		cputs("1 = ruch pierwsza kostka");
		i++;
	}
	gotoxy(1, KOMUNIKAT + i);
	if (kostka2 >= TAK) {
		cputs("2 = ruch druga kostka");
		i++;
	}
	gotoxy(1, KOMUNIKAT + i);
	if (kostka3 >= TAK) {
		cputs("3 = ruch dwoma naraz");
		i++;
	}
}
void wyborRuchuDubletLegenda(int ile_mozliwych_ruchow) {
	int i = 0;
	czyscKomunikat();
	if (ile_mozliwych_ruchow > 0) {
		gotoxy(1, KOMUNIKAT);
		cputs("1 = ruch pojedynczy");
		i++;
	}
	if (ile_mozliwych_ruchow > 1) {
		gotoxy(1, KOMUNIKAT + i);
		cputs("2 = ruch podwojny");
		i++;
	}
	if (ile_mozliwych_ruchow > 2) {
		gotoxy(1, KOMUNIKAT + i);
		cputs("3 = ruch potrojny");
		i++;
	}
	if (ile_mozliwych_ruchow > 3) {
		gotoxy(1, KOMUNIKAT + i);
		cputs("4 = ruch poczworny");
		i++;
	}
}
void rysujRuchDubletKostka(int kostka[]) {
	gotoxy(1, TEXTW);
	cputs("kostka  [");
	putch(kostka[1] + '0');
	cputs(",DUBLET]");
}
void automatycznyRuch() {
	gotoxy(1, KOMUNIKAT + 1);
	cputs("ruch bedzie wykonany automatycznie");
	gotoxy(1, KOMUNIKAT + 2);
	cputs("nacisnij dowolny przycisk aby kontynuowac");
	getch();
}
void wypiszWybranegoPionka(int pionek) {
	if (pionek == BRAK_RUCHOW)return;
	if (pionek == 0 || pionek == ROZMIAR_PLANSZY + 1) {
		gotoxy(1, TEXTW + 2);
		cputs("                   ");
		gotoxy(1, TEXTW + 2);
		cputs("Ruch z bandy");
	}
	else {
		gotoxy(1, TEXTW + 2);
		cputs("                   ");
		gotoxy(1, TEXTW + 2);
		cputs("wybrany pionek >");
		if (pionek > 9)putch(pionek / 10 + '0');
		putch(pionek % 10 + '0');
	}
}
void wypiszKostke(int ktora_kostka) {
	gotoxy(1, TEXTW + 3);
	cputs("wybrana kostka>");
	putch(ktora_kostka + '0');
}
void wypiszKostkeDublet(int ile_kostek) {
	gotoxy(1, TEXTW + 3);
	cputs("wybrana liczba ruchow>");
	putch(ile_kostek + '0');
}
//wybor pionka
int wczytajPionkaIleZnakow(char txt[]) {
	int przycisk, liczba_znakow = 0;
	gotoxy(1, KOMUNIKAT);
	clreol();
	cputs("wczytywanie przyciskow");
	while (1) {
		przycisk = getch();
		if (przycisk == 0x0d)return liczba_znakow;
		if (liczba_znakow < 3)txt[liczba_znakow] = przycisk;
		liczba_znakow++;
		gotoxy(1, KOMUNIKAT + 1);
		clreol();
		cputs("wczytano tyle znakow >");
		putchar(liczba_znakow + '0');
	}
}
int wczytajPionka() {
	int przycisk, pionek = 0, liczba_znakow = 0;
	char txt[5];
	while (1) {
		liczba_znakow = wczytajPionkaIleZnakow(txt);
		czyscKomunikat();
		if (liczba_znakow > 2) {
			gotoxy(1, KOMUNIKAT - 1);
			cputs("za duzo znakow");
		}
		else {
			if (liczba_znakow == 1)pionek = int(txt[0] - '0');
			if (liczba_znakow == 2) {
				pionek = int(txt[0] - '0') * 10;
				pionek = pionek + int(txt[1] - '0');
			}
			if (pionek > 0 && pionek < POLE_STARTOWE_UJEMNY) return pionek;
			else {
				gotoxy(1, KOMUNIKAT - 1);
				cputs("nie ma takiego pola");
			}
		}
	}
}
int wybierzPionka(int* plansza, int kostka[], int gracz) {
	int pionek;
	while (1) {
		pionek = wczytajPionka();
		//wiemy ze pionek jest liczba od 1 do 24	
		if ((gracz == DODATNI && plansza[pionek] <= 0) || (gracz == UJEMNY && plansza[pionek] >= 0)) {
			gotoxy(1, KOMUNIKAT - 1);
			cputs("tu nie ma twojego pionka");
		}
		else if (czyRuchMozliwyDlaJakiejkolwiekKostki(plansza, kostka, gracz, pionek) == NIE) {
			gotoxy(1, KOMUNIKAT - 1);
			cputs("ten pionek nie moze sie ruszyc");
		}
		else return pionek;
	}
}
int ruchPoczatek(int* plansza, int kostka[], int gracz) {

	int ktora_kostka, banda, pionek, przycisk;
	banda = czyBandaZajeta(plansza, gracz);
	if (czyJakisRuchMozliwy(plansza, kostka, gracz, banda) == NIE) {
		kolejkaPrzepada();
		return BRAK_RUCHOW;
	}
	else {
		gotoxy(1, KOMUNIKAT);
		cputs("wybierz pole z ktorego chcesz sie ruszyc");

		while (1) {

			if (banda == TAK) {
				gotoxy(1, KOMUNIKAT - 1);
				cputs("ruch z bandy");
				pionek = poleStartowe(gracz);
			}
			else {
				pionek = wybierzPionka(plansza, kostka, gracz);
			}
			return pionek;

		}
	}
}
//wybor ruchu
void WR_komunikat1() {
	gotoxy(1, KOMUNIKAT - 1);
	cputs("nie ma takiej kostki       ");
}
void WR_komunikat2() {
	gotoxy(1, KOMUNIKAT - 1);
	cputs("ten ruch jest nie mozliwy   ");
}
int wyborRuchu2(int* plansza, int kostka[], int gracz, int pionek) {
	int k_kost, kos1 = NIE, kos2 = NIE, kos3 = NIE; //k_kost= ktora_kostka
	kos1 = czyRuchMozliwy(plansza, kostka[1], gracz, pionek);
	kos2 = czyRuchMozliwy(plansza, kostka[2], gracz, pionek);
	if (czyRuchMozliwyDlaKostki3(plansza, kostka, gracz, pionek) == TAK)kos3 = czyRuchMozliwy(plansza, kostka[3], gracz, pionek);
	wyborRuchuLegenda2(kos1, kos2, kos3);
	while (1) {
		k_kost = getch() - '0';
		if (k_kost < 1 || k_kost>3) WR_komunikat1();
		else {
			if (k_kost == 1 && kos1 >= TAK)return k_kost;
			if (k_kost == 2 && kos2 >= TAK)return k_kost;
			if (k_kost == 3 && kos3 >= TAK)return k_kost;
			WR_komunikat2();
		}
	}
}
void wyborRuchuDubletRuchy(int* plansza, int kostka[], int gracz, int pionek, int ruchy[]) {
	ruchy[1] = czyRuchMozliwy(plansza, kostka[1], gracz, pionek);
	ruchy[2] = czyRuchMozliwy(plansza, kostka[2], gracz, pionek);
	ruchy[3] = czyRuchMozliwy(plansza, kostka[3], gracz, pionek);
	ruchy[4] = czyRuchMozliwy(plansza, kostka[4], gracz, pionek);
}
int DubletIleRuch(int* plansza, int kostka[], int gracz, int pionek) {
	int ile_kostek = 1, ruchy[5];
	wyborRuchuDubletRuchy(plansza, kostka, gracz, pionek, ruchy);
	if (ruchy[ile_kostek] >= TAK && kostka[ile_kostek] != 0) {
		ile_kostek++;
		if (ruchy[ile_kostek - 1] == TAK && ruchy[ile_kostek] >= TAK && kostka[ile_kostek] != 0) {
			ile_kostek++;
			if (ruchy[ile_kostek - 1] == TAK && ruchy[ile_kostek] >= TAK && kostka[ile_kostek] != 0) {
				ile_kostek++;
				if (ruchy[ile_kostek - 1] == TAK && ruchy[ile_kostek] >= TAK && kostka[ile_kostek] != 0) ile_kostek++;  //czy poprzedni nie by³ biciem && czy ten jest mozliwy && czy kostka jest dostepna
			}
		}
	}
	ile_kostek--;
	return ile_kostek;
}
int czyWymuszanieBiciaDublet(int* plansza, int kostka[], int gracz, int b_p[], int banda) {
	int  max_i = 0, S = poleStartowe(gracz);
	if (banda == TAK) for (int k_kost = 1; k_kost <= 4 && k_kost <= DubletIleRuch(plansza, kostka, gracz, S); k_kost++) {
		max_i = czyWymuszanieBiciaK12(plansza, kostka, gracz, b_p, max_i, k_kost, S);
	}
	else for (int i = 1; ROZMIAR_PLANSZY >= i; i++) { //przejdz po calej planszy	
		if ((plansza[i] > 0 && gracz == DODATNI) || (plansza[i] < 0 && gracz == UJEMNY)) {
			for (int k_kost = 1; k_kost <= 4 && k_kost <= DubletIleRuch(plansza, kostka, gracz, i); k_kost++) {
				max_i = czyWymuszanieBiciaK12(plansza, kostka, gracz, b_p, max_i, k_kost, i);
			}
		}
	}
	if (max_i != 0)return TAK;
	else return NIE;
}
int wyborRuchuDublet(int* plansza, int kostka[], int gracz, int pionek) {
	int ile_kostek, ile_mozliwych_ruchow = DubletIleRuch(plansza, kostka, gracz, pionek);
	wyborRuchuDubletLegenda(ile_mozliwych_ruchow);
	while (1) {
		ile_kostek = getch() - '0';
		if (ile_kostek < 1 || ile_kostek>5) {
			gotoxy(1, KOMUNIKAT - 1);
			cputs("nie ma takiej kostki       ");
		}
		else {
			if (ile_kostek <= ile_mozliwych_ruchow)return ile_kostek;
			gotoxy(1, KOMUNIKAT - 1);
			cputs("ten ruch jest nie mozliwy   ");
		}
	}

}
//ruch
void ruchDubletKoniec(int kostka[], int ile_kostek) { //ile kostek=iloma kostkami sie ruszylismy
	int ile_jest = 0;
	for (int i = 0; 4 > i; i++) {
		if (kostka[i + 1] != 0)ile_jest++;
	}
	for (int i = ile_jest; ile_jest - ile_kostek < i; i--)kostka[i] = 0; //odejmuje z kostki wykonane ruchy 

}
void ruchDublet(int* plansza, int kostka[], int gracz) {
	int pionek, ile_kostek;
	rysujRuchDubletKostka(kostka);
	pionek = ruchPoczatek(plansza, kostka, gracz);
	if (pionek == BRAK_RUCHOW) {
		kostka[1] == 0;
		return;
	}
	wypiszWybranegoPionka(pionek);
	ile_kostek = wyborRuchuDublet(plansza, kostka, gracz, pionek);
	wypiszKostkeDublet(ile_kostek);

	przesuniecie(plansza, ile_kostek * kostka[1], gracz, pionek);
	rysujGra(plansza, kostka, gracz);
	rysujRuchDubletKostka(kostka);
	ruchDubletKoniec(kostka, ile_kostek);
}
void ruch1(int* plansza, int kostka[], int gracz) {
	int pionek, ktora_kostka;

	pionek = ruchPoczatek(plansza, kostka, gracz);

	if (pionek == BRAK_RUCHOW)return;
	wypiszWybranegoPionka(pionek);
	ktora_kostka = wyborRuchu2(plansza, kostka, gracz, pionek);
	wypiszKostke(ktora_kostka);

	przesuniecie(plansza, kostka[ktora_kostka], gracz, pionek);
	kostka[ktora_kostka] = 0;
}
//sciaganie z domu
int sciaganieZDomuPrzesuniecie(int* plansza, int kostka[], int gracz, int ktora_kostka, int pionek) {
	if (ktora_kostka == 3 && czyRuchMozliwyDlaKostki3(plansza, kostka, gracz, pionek) == NIE)return NIE;
	przesuniecie(plansza, kostka[ktora_kostka], gracz, pionek);
	automatycznyRuch();
	kostka[ktora_kostka] = 0;
	return TAK;
}
int sciaganieZDomuRuszanie(int* plansza, int kostka[], int gracz, int ktora_kostka, int i) {
	int pionek;
	if (gracz == DODATNI) {
		if (plansza[ROZMIAR_PLANSZY + 1 - kostka[ktora_kostka] + i] > 0) {
			pionek = ROZMIAR_PLANSZY + 1 - kostka[ktora_kostka] + i;
			return sciaganieZDomuPrzesuniecie(plansza, kostka, gracz, ktora_kostka, pionek);
		}
	}
	if (gracz == UJEMNY) {
		if (plansza[0 + kostka[ktora_kostka] - i] < 0) {
			pionek = 0 + kostka[ktora_kostka] - i;
			return sciaganieZDomuPrzesuniecie(plansza, kostka, gracz, ktora_kostka, pionek);
		}
	}
	return NIE;
}
void sciaganieZDomuRuch(int* plansza, int kostka[], int gracz) {
	int pionek, ktora_kostka, banda = czyBandaZajeta(plansza, gracz);
	if (czyJakisRuchMozliwy(plansza, kostka, gracz, banda) == NIE) {
		kolejkaPrzepada();
		return;
	}
	for (ktora_kostka = 1; 3 >= ktora_kostka; ktora_kostka++) {
		for (int i = 0; kostka[ktora_kostka] > i; i++) {
			if (sciaganieZDomuRuszanie(plansza, kostka, gracz, ktora_kostka, i) == TAK)return;
		}

	}
	ruch1(plansza, kostka, gracz);
}
int sciaganieZDomuDubletPrzesuniecie(int* plansza, int kostka[], int gracz, int ktora_kostka, int pionek) {
	if (DubletIleRuch(plansza, kostka, gracz, pionek) < ktora_kostka)return NIE;
	przesuniecie(plansza, kostka[ktora_kostka], gracz, pionek);
	automatycznyRuch();
	ruchDubletKoniec(kostka, ktora_kostka);
	return TAK;
}
int sciaganieZDomuDubletRuszanie(int* plansza, int kostka[], int gracz, int ktora_kostka, int i) {
	int pionek;
	if (gracz == DODATNI) {
		if (plansza[ROZMIAR_PLANSZY + 1 - kostka[ktora_kostka] + i] > 0) {
			pionek = ROZMIAR_PLANSZY + 1 - kostka[ktora_kostka] + i;
			return sciaganieZDomuDubletPrzesuniecie(plansza, kostka, gracz, ktora_kostka, pionek);
		}
	}
	if (gracz == UJEMNY) {
		if (plansza[0 + kostka[ktora_kostka] - i] < 0) {
			pionek = 0 + kostka[ktora_kostka] - i;
			return sciaganieZDomuDubletPrzesuniecie(plansza, kostka, gracz, ktora_kostka, pionek);
		}
	}
	return NIE;
}
void sciaganieZDomuDubletRuch(int* plansza, int kostka[], int gracz) {
	int pionek, ktora_kostka, banda = czyBandaZajeta(plansza, gracz);
	if (czyJakisRuchMozliwy(plansza, kostka, gracz, banda) == NIE) {
		kolejkaPrzepada();
		return;
	}
	for (ktora_kostka = 1; 4 >= ktora_kostka && kostka[ktora_kostka] != 0; ktora_kostka++) {
		for (int i = 0; kostka[ktora_kostka] > i; i++) {
			if (sciaganieZDomuDubletRuszanie(plansza, kostka, gracz, ktora_kostka, i) == TAK)return;
		}
	}
	ruchDublet(plansza, kostka, gracz);
}
//bicie
void ruchZBiciem(int* plansza, int kostka[], int gracz, int bicie_parametry[]) {
	int pionek, ktora_kostka;
	pionek = bicie_parametry[BICIE_PARAMETRY_PIONEK];
	ktora_kostka = bicie_parametry[BICIE_PARAMETRY_KOSTKA];
	wypiszWybranegoPionka(pionek);
	wypiszKostke(ktora_kostka);
	przesuniecie(plansza, kostka[ktora_kostka], gracz, pionek);
	kostka[ktora_kostka] = 0;
	automatycznyRuch();
}
void ruchZBiciemDublet(int* plansza, int kostka[], int gracz, int bicie_parametry[]) {
	int pionek, ktora_kostka;
	pionek = bicie_parametry[BICIE_PARAMETRY_PIONEK];
	ktora_kostka = bicie_parametry[BICIE_PARAMETRY_KOSTKA];
	wypiszWybranegoPionka(pionek);
	wypiszKostke(ktora_kostka);
	przesuniecie(plansza, kostka[ktora_kostka], gracz, pionek);
	ruchDubletKoniec(kostka, ktora_kostka);
	automatycznyRuch();
}
//gra
int jakiRuch(int* plansza, int kostka[], int gracz, int bicie_parametry[]) {
	int czy_dom_pelny = czyDomPelny(plansza, gracz);
	if (czyJakisRuchMozliwy(plansza, kostka, gracz, czyBandaZajeta(plansza, gracz)) == NIE) {
		kolejkaPrzepada();
		return BRAK_RUCHOW;
	}
	if (czyWymuszanieBicia(plansza, kostka, gracz, bicie_parametry, czyBandaZajeta(plansza, gracz)) == TAK)return BICIE;
	if (czy_dom_pelny == TAK)return DO_DOMU;
	return ZWYKLY;
}
int jakiRuchDublet(int* plansza, int kostka[], int gracz, int bicie_parametry[]) {
	int czy_dom_pelny = czyDomPelny(plansza, gracz);
	if (czyJakisRuchMozliwy(plansza, kostka, gracz, czyBandaZajeta(plansza, gracz)) == NIE) {
		kolejkaPrzepada();
		return BRAK_RUCHOW;
	}
	if (czyWymuszanieBiciaDublet(plansza, kostka, gracz, bicie_parametry, czyBandaZajeta(plansza, gracz)) == TAK)return BICIE;
	if (czy_dom_pelny == TAK)return DO_DOMU;
	return ZWYKLY;
}
void startDublet(int* plansza, int stara_kos[], int gracz, int bicie_parametry[]) {
	int kostka[] = { DUBLET,stara_kos[1], stara_kos[1] * 2 , stara_kos[1] * 3 , stara_kos[1] * 4 };
	int pionek, ile_kostek, ruch;
	while (1) {
		ruch = jakiRuchDublet(plansza, kostka, gracz, bicie_parametry);
		if (ruch == BRAK_RUCHOW)return;
		if (ruch == DO_DOMU)sciaganieZDomuDubletRuch(plansza, kostka, gracz);
		if (ruch == BICIE)ruchZBiciemDublet(plansza, kostka, gracz, bicie_parametry);
		if (ruch == ZWYKLY)ruchDublet(plansza, kostka, gracz);
		rysujGra(plansza, kostka, gracz); //nwm
		rysujRuchDubletKostka(kostka);
		if (kostka[1] == 0)return;
	}
}
void startRuch(int* plansza, int kostka[], int gracz, int bicie_parametry[]) {
	int ruch = jakiRuch(plansza, kostka, gracz, bicie_parametry);
	if (ruch == BRAK_RUCHOW)return;
	if (ruch == DO_DOMU)sciaganieZDomuRuch(plansza, kostka, gracz);
	if (ruch == BICIE)ruchZBiciem(plansza, kostka, gracz, bicie_parametry);
	if (ruch == ZWYKLY) ruch1(plansza, kostka, gracz);
}
void start(int* plansza, int kostka[], int gracz) {
	int bicie_parametry[] = { 0, 0, 0, 0 };
	int ruch;

	rysujGra(plansza, kostka, gracz);
	if (kostka[DUBLET] == NIE) {
		startRuch(plansza, kostka, gracz, bicie_parametry);
		rysujGra(plansza, kostka, gracz);
		if (kostka[3] == 0)return;//koniec
		startRuch(plansza, kostka, gracz, bicie_parametry);
		rysujGra(plansza, kostka, gracz);
	}
	else {
		startDublet(plansza, kostka, gracz, bicie_parametry); //dodaj sciaganie do dubletu
	}
}
char graj(int* plansza, int kostka[], int gracz) {
	start(plansza, kostka, gracz);

	rzuc_kostkami(kostka);
	return koniecRuchu();
}
void gra(int* plansza, int kostka[], int gracz) {
	int przycisk;
	do {
		rysujMenu();
		przycisk = getch();
		if (przycisk == 'g') {
			do {
				przycisk = graj(plansza, kostka, gracz);
				gracz *= -1;
				if (przycisk == 's') {
					zapisz(plansza, gracz);
					break;
				}
				if (ktoWygrywa(plansza) != NIE) {
					wygrana(plansza, ktoWygrywa(plansza));
					ustawWszystko(plansza, kostka);
					break;
				}
			} while (przycisk != 'm');//powrot do men
		}
		if (przycisk == 'w') {
			wczytaj(plansza);
			gracz = plansza[0];
		}
	} while (przycisk != 'q');
}
int main() {
	int plansza[40], kostka[5], gracz;
	srand((int)time(NULL));
#ifndef __cplusplus
	Conio2_Init();
#endif
	conio2ustawienia();
	gracz = ustawWszystko(plansza, kostka); //kto zaczyna
	plansza[0] = gracz;
	gra(plansza, kostka, gracz);
	_setcursortype(_NORMALCURSOR);

	return 0;
}
