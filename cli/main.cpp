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

void liigutaRuudustikku(int ruudustik[SUURUS][SUURUS], int suund) {
    bool liigutas = false;

    // Liiguta ruute üles
    if (suund == 0) {
        for (int j = 0; j < SUURUS; ++j) {
            for (int i = 0; i < SUURUS - 1; ++i) {
                if (ruudustik[i][j] == 0) {
                    ruudustik[i][j] = ruudustik[i + 1][j];
                    ruudustik[i + 1][j] = 0;
                    liigutas = true;
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
                    liigutas = true;
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
                    liigutas = true;
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
                    liigutas = true;
                }
            }
        }
    }
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
            if (key == 27) { // ESC key
                running = false;
            } else if (key == 0 || key == 224) { // Nooled on 0 või 224
                key = _getch(); // Annab key väärtuse
                switch (key) {
                    case 72: // Üles
                        liigutaRuudustikku(ruudustik, 0);
                        system("cls");
                        kuvaRuudustik(ruudustik);
                        break;
                    case 80: // Alla
                        liigutaRuudustikku(ruudustik, 1);
                        system("cls");
                        kuvaRuudustik(ruudustik);
                        break;
                    case 75: // Vasak
                        liigutaRuudustikku(ruudustik, 2);
                        system("cls");
                        kuvaRuudustik(ruudustik);
                        break;
                    case 77: // Parem
                        liigutaRuudustikku(ruudustik, 3);
                        system("cls");
                        kuvaRuudustik(ruudustik);
                        break;
                }
            }
        }
    }
    return 0;
}
