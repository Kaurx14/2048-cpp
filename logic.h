#ifndef CLI_H
#define CLI_H
#include <vector>

//const int SUURUS = 4;

void looRuudustik(std::vector<std::vector<int>>& ruudustik, int suurus);

void kuvaRuudustik(std::vector<std::vector<int>>& ruudustik);

void liigutaRuudustikku(std::vector<std::vector<int>>& ruudustik, int suund, bool* liigutas, int* punktid);

void lisaNumber(std::vector<std::vector<int>>& ruudustik);

bool kasLabi(std::vector<std::vector<int>>& ruudustik);

//void cliMang();

#endif //CLI_H
