#ifndef CLI_H
#define CLI_H

const int SUURUS = 4;

void looRuudustik(int ruudustik[SUURUS][SUURUS]);

void kuvaRuudustik(int ruudustik[SUURUS][SUURUS]);

void liigutaRuudustikku(int ruudustik[SUURUS][SUURUS], int suund, bool* liigutas);

void lisaNumber(int ruudustik[SUURUS][SUURUS]);

bool kasLabi(int ruudustik[SUURUS][SUURUS]);

void cliMang();

#endif //CLI_H
