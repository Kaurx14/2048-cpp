#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <Windows.h>
#include <conio.h>
using namespace std;

const int SUURUS = 4;

void looRuudustik(int ruudustik[SUURUS][SUURUS]) {
    // Loo tühi ruudustik
    for (int i = 0; i < SUURUS; i++) {
        for (int j = 0; j < SUURUS; j++) {
            ruudustik[i][j] = 0;
        }
    }

    // Initsialiseeri 2 juhuslikku ruutu väärtusega "2"
    int rida = rand() % SUURUS;
    int veerg = rand() % SUURUS;
    ruudustik[rida][veerg] = 2;

    // Initsialiseeri ka teine ruut, kontrollides, et see ruut on tühi
    do {
        rida = rand() % SUURUS;
        veerg = rand() % SUURUS;
    } while (ruudustik[rida][veerg] != 0);
    ruudustik[rida][veerg] = 2;
}

void kuvaRuudustik(int ruudustik[SUURUS][SUURUS]) {
    cout << "+----+----+----+----+\n";
    for (int i = 0; i < SUURUS; ++i) {
        cout << "|";
        for (int j = 0; j < SUURUS; ++j) {
            if (ruudustik[i][j] == 0)
                cout << "    |";
            else
                cout << setw(4) << ruudustik[i][j] << "|";
        }
        cout << "\n+----+----+----+----+\n";
    }
}

void liigutaRuudustikku(int ruudustik[SUURUS][SUURUS], int suund, bool* liigutas) {

    //Liiguta üles
    if (suund == 0) {
        for (int i = 0; i < SUURUS; ++i) {
            for (int j = 0; j < SUURUS; ++j) {
                if (ruudustik[i][j] != 0) {
                    int vaadeldav = i;
                    for (int uus = i-1; uus >= 0; uus--) {
                        if (ruudustik[uus][j] == 0) {
                            //tühi ruut
                            ruudustik[uus][j] = ruudustik[vaadeldav][j];
                            ruudustik[vaadeldav][j] = 0;
                            vaadeldav -= 1;
                            *liigutas = true;
                        } else {
                            //ei ole tühi ruut
                            if (ruudustik[uus][j] == ruudustik[vaadeldav][j]) {
                                //on sama number, liidame
                                ruudustik[uus][j] *= 2;
                                ruudustik[vaadeldav][j] = 0;
                                *liigutas = true;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    //Liiguta alla
    if (suund == 1) {
        for (int i = SUURUS - 1; i >= 0; --i) {
            for (int j = 0; j < SUURUS; ++j) {
                if (ruudustik[i][j] != 0) {
                    int vaadeldav = i;
                    for (int uus = i+1; uus < SUURUS; uus++) {
                        if (ruudustik[uus][j] == 0) {
                            //tühi ruut
                            ruudustik[uus][j] = ruudustik[vaadeldav][j];
                            ruudustik[vaadeldav][j] = 0;
                            vaadeldav += 1;
                            *liigutas = true;
                        } else {
                            //ei ole tühi ruut
                            if (ruudustik[uus][j] == ruudustik[vaadeldav][j]) {
                                //on sama number, liidame
                                ruudustik[uus][j] *= 2;
                                ruudustik[vaadeldav][j] = 0;
                                *liigutas = true;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    //Liiguta vasakule
    if (suund == 2) {
        for (int i = 0; i < SUURUS; ++i) {
            for (int j = 0; j < SUURUS; ++j) {
                if (ruudustik[i][j] != 0) {
                    int vaadeldav = j;
                    for (int uus = j-1; uus >= 0; uus--) {
                        if (ruudustik[i][uus] == 0) {
                            //tühi ruut
                            ruudustik[i][uus] = ruudustik[i][vaadeldav];
                            ruudustik[i][vaadeldav] = 0;
                            vaadeldav -= 1;
                            *liigutas = true;
                        } else {
                            //ei ole tühi ruut
                            if (ruudustik[i][uus] == ruudustik[i][vaadeldav]) {
                                //on sama number, liidame
                                ruudustik[i][uus] *= 2;
                                ruudustik[i][vaadeldav] = 0;
                                *liigutas = true;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    //Liiguta paremale
    if (suund == 3) {
        for (int i = 0; i < SUURUS; ++i) {
            for (int j = SUURUS - 1; j >= 0; --j) {
                if (ruudustik[i][j] != 0) {
                    int vaadeldav = j;
                    for (int uus = j+1; uus < SUURUS; uus++) {
                        if (ruudustik[i][uus] == 0) {
                            //tühi ruut
                            ruudustik[i][uus] = ruudustik[i][vaadeldav];
                            ruudustik[i][vaadeldav] = 0;
                            vaadeldav += 1;
                            *liigutas = true;
                        } else {
                            //ei ole tühi ruut
                            if (ruudustik[i][uus] == ruudustik[i][vaadeldav]) {
                                //on sama number, liidame
                                ruudustik[i][uus] *= 2;
                                ruudustik[i][vaadeldav] = 0;
                                *liigutas = true;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    /*
    // Liiguta ruute üles
    if (suund == 0) {
        for (int j = 0; j < SUURUS; ++j) {
            for (int i = 0; i < SUURUS - 1; ++i) {
                if (ruudustik[i][j] == 0) {
                    ruudustik[i][j] = ruudustik[i + 1][j];
                    ruudustik[i + 1][j] = 0;
                    *liigutas = true;
                }
            }
        }
    }

    // Liiguta ruute alla
    if (suund == 1) {
        for (int j = 0; j < SUURUS; ++j) {
            for (int i = SUURUS - 1; i > 0; --i) {
                if (ruudustik[i][j] == 0) {
                    ruudustik[i][j] = ruudustik[i - 1][j];
                    ruudustik[i - 1][j] = 0;
                    *liigutas = true;
                }
            }
        }
    }

    // Liiguta ruute vasakule
    if (suund == 2) {
        for (int i = 0; i < SUURUS; ++i) {
            for (int j = 0; j < SUURUS - 1; ++j) {
                if (ruudustik[i][j] == 0) {
                    ruudustik[i][j] = ruudustik[i][j + 1];
                    ruudustik[i][j + 1] = 0;
                    *liigutas = true;
                }
            }   
        }
    }

    // Liiguta ruute paremale
    if (suund == 3) {
        for (int i = 0; i < SUURUS; ++i) {  
            for (int j = SUURUS - 1; j > 0; --j) {
                if (ruudustik[i][j] == 0) {
                    ruudustik[i][j] = ruudustik[i][j - 1];
                    ruudustik[i][j - 1] = 0;
                    *liigutas = true;
                }
            }
        }
    }
    */
}

void lisaNumber(int ruudustik[SUURUS][SUURUS]) {
    int rida, veerg = 0;
    do {
        rida = rand() % SUURUS;
        veerg = rand() % SUURUS;
    } while (ruudustik[rida][veerg] != 0);
    //õiges mängus on võimalik et uus ruut on ka 4
    int uusnr = rand() % 10 < 7 ? 2 : 4; //hetkel 70 : 30 võimalus 2 ja 4 nelja vahel
    ruudustik[rida][veerg] = uusnr;
}

bool kasLabi(int ruudustik[SUURUS][SUURUS]) {
    for (int i = 0; i < SUURUS; ++i) {
        for (int j = 0; j < SUURUS; ++j) {
            if (ruudustik[i][j] != 0) {
                if (j != SUURUS - 1) {
                    if (ruudustik[i][j] == ruudustik[i][j+1]) {
                        return false;
                    }
                }
                if (i != SUURUS - 1) {
                    if (ruudustik[i][j] == ruudustik[i+1][j]) {
                        return false;
                    }
                }
            }
            else {
                return false;
            }
        }
    }
    return true;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int ruudustik[SUURUS][SUURUS];
    looRuudustik(ruudustik);
    kuvaRuudustik(ruudustik);
    
    bool running = true;
    while (running) {
        if (_kbhit()) {
            char key = _getch();
            //cout << static_cast<int>(key) << endl;
            if (key == 27) { // ESC key
                running = false;
            } else //if (key == 0 || key == 224 || key == -32) { // Nooled on 0 või 224
                //või minul -32. kuid seda if-i siin pole vist üldse vaja
            {
                bool liigutas = false;
                key = _getch(); // Annab key väärtuse
                switch (key) {
                    case 72: // Üles
                        liigutaRuudustikku(ruudustik, 0, &liigutas);
                        break;
                    case 80: // Alla
                        liigutaRuudustikku(ruudustik, 1, &liigutas);
                        break;
                    case 75: // Vasak
                        liigutaRuudustikku(ruudustik, 2, &liigutas);
                        break;
                    case 77: // Parem
                        liigutaRuudustikku(ruudustik, 3, &liigutas);
                        break;
                }
                if (liigutas) {
                    lisaNumber(ruudustik);
                    system("cls");
                    kuvaRuudustik(ruudustik);
                    if (kasLabi(ruudustik)) {
                        cout << "Mäng läbi!\n";
                        break;
                    }
                }
            }
        }
    }
    //et kohe kinni ei viska, kui mäng saab läbi
    //kui mäng pani ESC kaudu kinni, siis läheb siit otse läbi
    while (running) {
        if (_kbhit()) {
            break;
        }
    }
    return 0;
}
