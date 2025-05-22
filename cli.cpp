#include "cli.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <Windows.h>
#include <conio.h>
#include <vector>
using namespace std;

void looRuudustik(vector<vector<int>>& ruudustik, int suurus) {
    // Loo tühi ruudustik
    ruudustik.clear();
    for (int i = 0; i < suurus; i++) {
        vector<int> rida;
        for (int j = 0; j < suurus; j++) {
            rida.push_back(0);
        }
        ruudustik.push_back(rida);
    }

    // Initsialiseeri 2 juhuslikku ruutu väärtusega "2"
    int rida = rand() % ruudustik.size();
    int veerg = rand() % ruudustik.size();
    ruudustik[rida][veerg] = 2;

    // Initsialiseeri ka teine ruut, kontrollides, et see ruut on tühi
    do {
        rida = rand() % ruudustik.size();
        veerg = rand() % ruudustik.size();
    } while (ruudustik[rida][veerg] != 0);
    ruudustik[rida][veerg] = 2;
}

void kuvaRuudustik(vector<vector<int>>& ruudustik) {
    cout << "+----+----+----+----+\n";
    for (int i = 0; i < ruudustik.size(); ++i) {
        cout << "|";
        for (int j = 0; j < ruudustik.size(); ++j) {
            if (ruudustik[i][j] == 0)
                cout << "    |";
            else
                cout << setw(4) << ruudustik[i][j] << "|";
        }
        cout << "\n+----+----+----+----+\n";
    }
}

void liigutaRuudustikku(vector<vector<int>>& ruudustik, int suund, bool* liigutas) {
    //Liiguta üles
    if (suund == 0) {
        for (int i = 0; i < ruudustik.size(); ++i) {
            for (int j = 0; j < ruudustik.size(); ++j) {
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
        for (int i = ruudustik.size() - 1; i >= 0; --i) {
            for (int j = 0; j < ruudustik.size(); ++j) {
                if (ruudustik[i][j] != 0) {
                    int vaadeldav = i;
                    for (int uus = i+1; uus < ruudustik.size(); uus++) {
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
        for (int i = 0; i < ruudustik.size(); ++i) {
            for (int j = 0; j < ruudustik.size(); ++j) {
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
        for (int i = 0; i < ruudustik.size(); ++i) {
            for (int j = ruudustik.size() - 1; j >= 0; --j) {
                if (ruudustik[i][j] != 0) {
                    int vaadeldav = j;
                    for (int uus = j+1; uus < ruudustik.size(); uus++) {
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
}

void lisaNumber(vector<vector<int>>& ruudustik) {
    int rida, veerg = 0;
    do {
        rida = rand() % ruudustik.size();
        veerg = rand() % ruudustik.size();
    } while (ruudustik[rida][veerg] != 0);
    //õiges mängus on võimalik et uus ruut on ka 4
    int uusnr = rand() % 10 < 7 ? 2 : 4; //hetkel 70 : 30 võimalus 2 ja 4 nelja vahel
    ruudustik[rida][veerg] = uusnr;
}

bool kasLabi(vector<vector<int>>& ruudustik) {
    for (int i = 0; i < ruudustik.size(); ++i) {
        for (int j = 0; j < ruudustik.size(); ++j) {
            if (ruudustik[i][j] != 0) {
                if (j != ruudustik.size() - 1) {
                    if (ruudustik[i][j] == ruudustik[i][j+1]) {
                        return false;
                    }
                }
                if (i != ruudustik.size() - 1) {
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

void cliMang() {
    srand(static_cast<unsigned int>(time(0)));

    vector<vector<int>> ruudustik;
    looRuudustik(ruudustik, 4);
    system("cls");
    kuvaRuudustik(ruudustik);

    bool running = true;
    while (running) {
        if (_kbhit()) {
            char key = _getch();
            //cout << static_cast<int>(key) << endl;
            if (key == 27) { // ESC key
                running = false;
            } else //if (key == 0 || key == 224) { // Nooled on 0 või 224
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
}
