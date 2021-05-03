#include <iostream>
#include <vector>
using namespace std;

int from0 = 48; //ASCII numer dziesiątkowy na 0

int index=0;
int charVal=0;

//Źródło: Wikipedia(https://pl.wikipedia.org/wiki/Kod_Hamminga)
int H1[4][12] ={{1,1,1,0,1,1,1,0, 1,0,0,0},
                {1,1,0,1,1,0,0,1, 0,1,0,0},
                {1,0,1,1,0,1,0,0, 0,0,1,0},
                {0,1,1,1,0,0,1,1, 0,0,0,1}};

//Autor: Bartosz Durys 229869
int H2[8][16] = {
        {1,1,1,1,0,0,0,0, 1,0,0,0,0,0,0,0},
        {1,1,0,0,1,1,0,0, 0,1,0,0,0,0,0,0},
        {1,0,1,0,1,0,1,0, 0,0,1,0,0,0,0,0},
        {0,1,0,1,0,1,1,0, 0,0,0,1,0,0,0,0},
        {1,1,1,0,1,0,0,1, 0,0,0,0,1,0,0,0},
        {1,0,0,1,0,1,0,1, 0,0,0,0,0,1,0,0},
        {0,1,1,1,1,0,1,1, 0,0,0,0,0,0,1,0},
        {1,1,1,0,0,1,1,1, 0,0,0,0,0,0,0,1}};

//Autor: Bartosz Durys 229869
int H3[11][19] = {
        {1,1,1,1,1,1,1,1, 1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,1,1,1, 0,1,0,0,0,0,0,0,0,0,0},
        {1,1,0,0,0,0,0,1, 0,0,1,0,0,0,0,0,0,0,0},
        {0,1,0,1,1,0,1,0, 0,0,0,1,0,0,0,0,0,0,0},
        {0,1,1,0,1,1,0,0, 0,0,0,0,1,0,0,0,0,0,0},
        {1,0,0,1,0,1,1,0, 0,0,0,0,0,1,0,0,0,0,0},
        {1,1,0,1,1,1,0,1, 0,0,0,0,0,0,1,0,0,0,0},
        {0,1,1,1,0,0,1,1, 0,0,0,0,0,0,0,1,0,0,0},
        {1,0,1,1,1,0,0,0, 0,0,0,0,0,0,0,0,1,0,0},
        {0,0,1,1,0,1,0,1, 0,0,0,0,0,0,0,0,0,1,0},
        {1,1,1,0,0,1,1,0, 0,0,0,0,0,0,0,0,0,0,1}};

int getHVal(int n, int m, int mode){
    if(mode==1){
        return H1[n][m];
    }
    else if(mode==2){
        return H2[n][m];
    }
    else{
        return H3[n][m];
    }
}

void charToBin(unsigned char znak, int *messBin){
    for (int i = 7; i > -1; i--) { // w tej petli zamieniamy nasz pobrany znak z pliku na postac binarna
        messBin[i] = znak%2;
        znak /= 2;
    }
}

//wiadomosc, bity parzystosci, liczba bitow parzystosci(wierszy), długość zakodowanej wiadomości(kolumn), macierz H
void obliczBityParzystosci(int *messBin, int *bity_parzystosci, int n, int m, int mode){

    for (int i=0;i<n;i++){ // w tej pêtli obliczamy bity parzystoœci dla naszej wiadomosci
        for(int k=0;k<m;k++){
            bity_parzystosci[i]+=messBin[k]*getHVal(i,k,mode);
         }
         bity_parzystosci[i]=bity_parzystosci[i]%2;
    }
}

void zapiszBity(int *messBin, int len, FILE *file){
    for(int i=0; i<len; i++){
        fputc(messBin[i] + from0, file);
    }
}

void zapiszBityJakoZnak(int *messBin, int len, FILE *file){
    for(int i=0; i<len; i++){
        index++;
        charVal = charVal*2 + messBin[i];
        if(index==8){
            fputc(charVal, file);
            charVal=0;
            index=0;
        }
    }
}

void zapiszPozostale(FILE *file){
    if(index>0){
        while(index!=8){
            charVal*=2;
            index++;
        }
        fputwc(charVal, file);
        charVal=0;
        index=0;
    }
}

void dlugoscBitowParzystosci(int &dlugosc, int mode){
    if(mode==1){
        dlugosc = 4;
    }else if(mode==2){
        dlugosc = 8;

    }else{
        dlugosc = 11;
    }
}

void binToChar(char &slowo, int *zakodowana_wiadomosc){
    int a = 128;
    slowo = 0;
    for (int i = 0; i <8; i++) {
        slowo += a * zakodowana_wiadomosc[i]; // zmieniamy z formy bitowej na kod asci
        a /= 2;
    }
}

//funkcja kodujaca wiadomosc z pliku
void kodowanie(FILE *wiadomosc,FILE *zakodowana, int mode){ //funkcja, która koduje zawartosc pliku z nasza wiadomoscia (znakiem)
    int znak;
    int messBin[8],bity_parzystosci[11];
    int messLen=8, parrityBitsLen;

    dlugoscBitowParzystosci(parrityBitsLen, mode);

    // odczyt z pliku
    while((znak=fgetc(wiadomosc))!=EOF){
        fill_n(bity_parzystosci, 11, 0);

        charToBin(znak, messBin);

        obliczBityParzystosci(messBin, bity_parzystosci, parrityBitsLen, messLen, mode);

        // zapisujemy znak w postaci binarnej do pliku
        zapiszBityJakoZnak(messBin, messLen, zakodowana);
        // zapisujemy bity parzystosci do pliku
        zapiszBityJakoZnak(bity_parzystosci, parrityBitsLen, zakodowana);
    }
    zapiszPozostale(zakodowana);
}

bool blad1bit(int dlugosc_zakod, int dlugosc_parz, int mode, int *tab_bledow, int *zakodowana_wiadomosc){
    for (int i = 0; i <dlugosc_zakod; i++) {
        for (int j = 0; j <dlugosc_parz; j++) {
            if (getHVal(j,i,mode) != tab_bledow[j]) { // szukamy takiej samej jak wektor błedu, kolumnmny W w maierzy  H
                break;
            }
            if (j == dlugosc_parz-1) {
               zakodowana_wiadomosc[i] = !zakodowana_wiadomosc[i]; //zmieniamy błędny bit na poprawny
                return true;
            }
        }
    }
    return false;
}

bool blad2bit(int dlugosc_zakod, int dlugosc_parz, int mode, int *tab_bledow, int *zakodowana_wiadomosc){
    int bledy=0; // jeśli błedy=1 to mamy 2 błedy, jesli błedy=0 to mamy 1 bład
    for(int i=0;i<dlugosc_zakod-1;i++){
        for (int j=i+1;j<dlugosc_zakod;j++){ // liczymy ile jest błedow
            bledy=1;
            for(int k=0;k<dlugosc_parz;k++){ // sprawdz czy suma dwoch dowolnych kolumn spelnia rownosc
                if(tab_bledow[k]!=getHVal(k,i,mode)^getHVal(k,j,mode)){
                    bledy=0;
                    break;
                }
            }
            if (bledy == 1) { // 2 błedy bitowe
                zakodowana_wiadomosc[i] = !zakodowana_wiadomosc[i]; // Zamieniamy błędny bit
                zakodowana_wiadomosc[j] = !zakodowana_wiadomosc[j]; // Zamieniamy błędny bit
                return true;
            }
        }
    }
    return false;
}

bool blad3bit(int dlugosc_zakod, int dlugosc_parz, int mode, int *tab_bledow, int *zakodowana_wiadomosc){
    int bledy=0; // jeśli błedy=1 to mamy 2 błedy, jesli błedy=0 to mamy 1 bład
    for(int i=0;i<dlugosc_zakod-2;i++){
        for(int j=i+1;j<dlugosc_zakod-1;j++){ // liczymy ile jest błedow
            for(int k=j+1;k<dlugosc_zakod;k++){
                bledy=1;
                for(int l=0;l<dlugosc_parz;l++){ // sprawdz czy suma dwoch dowolnych kolumn spelnia rownosc
                    if(tab_bledow[l]!=getHVal(l,i,mode)^getHVal(l,j,mode)^getHVal(l,k,mode)){
                        bledy=0;
                        break;
                    }
                }
                if (bledy == 1) { // 3 błedy bitowe
                    zakodowana_wiadomosc[i] = !zakodowana_wiadomosc[i]; // Zamieniamy błędny bit
                    zakodowana_wiadomosc[j] = !zakodowana_wiadomosc[j]; // Zamieniamy błędny bit
                    zakodowana_wiadomosc[k] = !zakodowana_wiadomosc[k]; // Zamieniamy błędny bit
                    return true;
                }
            }
        }
    }
    return false;
}

// funckja wykrywajaca i poprawiajaca bledy (1, 2 lub 3 bity)
void popraw(FILE *zakodowana,FILE *zdekodowana,FILE *file, int mode){
    int dlugosc_zakod, dlugosc_parz;
    dlugoscBitowParzystosci(dlugosc_parz, mode);
    dlugosc_zakod = 8 + dlugosc_parz;
        int *zakodowana_wiadomosc= new int[dlugosc_zakod]; //tablica na bity zakodowanej wiadomości
    int *tab_bledow=new int[dlugosc_parz];  // tablica z błędami z wiersza // tablica z bledami z wiersza
    int znak, temp_mode;
    int ile=0, ile_znakow=1;
    bool czy_blednie=false, res;

    int zakodowanyBajt[8];
    vector<int> allBits;

    while((znak=fgetc(zakodowana))!=EOF){
        charToBin(znak, zakodowanyBajt);
        for(int i=0; i<8; i++){
            allBits.push_back(zakodowanyBajt[i]);
        }
    }

    for(int i=0; (i*dlugosc_zakod)<(allBits.size()-1); i++){
        for(int j=i*dlugosc_zakod; j<(i*dlugosc_zakod+dlugosc_zakod); j++){
            zakodowana_wiadomosc[ile] = allBits[j];
            ile++;
        }
        for(int j=0; j<dlugosc_parz;j++){
            tab_bledow[j]=0;
            for(int k=0; k<dlugosc_zakod;k++){
                tab_bledow[j]+=zakodowana_wiadomosc[k]*getHVal(j,k,mode); // mnożenie macierzy T oraz H
            }
            tab_bledow[j]%=2;

            if(tab_bledow[j]==1){
                czy_blednie=true;
            }
        }

        if(czy_blednie){
            temp_mode = mode;

            if(temp_mode>2){
                res = blad3bit(dlugosc_zakod, dlugosc_parz, mode, tab_bledow, zakodowana_wiadomosc);
                temp_mode = res ? 0 : mode;
            }
            if(temp_mode>1){
                res = blad2bit(dlugosc_zakod, dlugosc_parz, mode, tab_bledow, zakodowana_wiadomosc);
                temp_mode = res ? 0 : mode;
            }
            if(temp_mode>0){
                blad1bit(dlugosc_zakod, dlugosc_parz, mode, tab_bledow, zakodowana_wiadomosc);
            }
        }

        ile = 0;
        ile_znakow++;
        czy_blednie = false;

        zapiszBity(zakodowana_wiadomosc,dlugosc_zakod,file);
        fputc('\n', file);
        char slowo = 0;
        binToChar(slowo, zakodowana_wiadomosc);
        fputc(slowo, zdekodowana); // zapisujemy do 2 plikow - w postaci litery i w postaci binarnej(dla pewnosci czy dobrze dziala)
    }
}

int main() {
    cout<<"Copyrights: \n"
    "Filip Hajek - 229891 \n"
    "Bartosz Durys - 229869\n";
    int wybor;
    while(true){
        cout<<"Wybierz opcje: \n"
        "1- Kodowanie wiadomosci\n"
        "2- Dekodowanie wiadomosci\n"
        "3- Wyjdz z programu\n";
        cin>> wybor;
        if(wybor==1){
            FILE *mess=fopen("wiadomosc.txt.","r");
            FILE *code = fopen("zakodowana.bin", "wb");

            while(true)
            {
                cout<<"Ile bledow ma poprawiac? \n"
                "1- 1-bit\n"
                "2- 2-bit\n"
                "3- 3-bit\n";
                cin>> wybor;
                if(wybor>0 && wybor<4)
                {
                    kodowanie(mess, code, wybor);
                    break;
                }
                else
                {
                    cout<<"BLAD"<<endl;
                }
            }

            fclose(mess);
            fclose(code);
            cout<<"Zakodowano wiadomosc!"<<endl<<endl;
        }
        else if(wybor==2){
            FILE *code = fopen("zakodowana.bin", "rb");
            FILE *decode = fopen("zdekodowana.txt", "w");
            FILE *check = fopen("sprawdzenie.txt", "w");

            while(true)
            {
                cout<<"Ile bledow ma poprawiac? \n"
                "1- 1-bit\n"
                "2- 2-bit\n"
                "3- 3-bit\n";
                cin>> wybor;
                if(wybor>0 && wybor<4)
                {
                    popraw(code,decode,check, wybor);
                    break;
                }
                else
                {
                    cout<<"BLAD"<<endl;
                }
            }

            fclose(code);
            fclose(decode);
            fclose(check);
            cout<<"Zdekodowano wiadomosc!"<<endl<<endl;
        }
        else if(wybor==3){
            break;
        }
        else{
            cout<<"BLAD"<<endl;
        }
    }
    return 0;
}
