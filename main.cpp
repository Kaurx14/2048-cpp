#include <iostream>
#include "cli.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace std;
using namespace ftxui;

Element make_grid2(int ruudustik[SUURUS][SUURUS]) {
    std::vector<Elements> rows;
    for (int i = 0; i < SUURUS; ++i) {
        std::vector<Element> cols;
        for (int j = 0; j < SUURUS; ++j) {
            string ruut;
            if (ruudustik[i][j] == 0) {
                ruut = " ";
            } else {
                ruut = to_string(ruudustik[i][j]);
            }
            cols.push_back(text(ruut)  | center | size(WIDTH, EQUAL, 10) | size(HEIGHT, EQUAL, 5) | border);
        }
        rows.push_back(cols);
    }
    auto grid = gridbox(rows);

    return grid;
}

int main() {
    auto screen = ScreenInteractive::TerminalOutput();

    int ruudustik[SUURUS][SUURUS];
    looRuudustik(ruudustik);

    auto count = 0;
    bool liigutas = true;
    bool gameover = false;

    auto gridRenderer = Renderer([&] {
        string gameoverText = "";
        if (gameover) {
            gameoverText = "Mäng läbi!";
        }
        return vbox(make_grid2(ruudustik), text(gameoverText));  //
  });

    auto component = Container::Horizontal({gridRenderer});
    component |= CatchEvent([&](Event event) {
        liigutas = false;
        bool handled = false;
        if (event == Event::ArrowUp) {
            liigutaRuudustikku(ruudustik, 0, &liigutas);
            handled = true;
        }
        if (event == Event::ArrowDown) {
            liigutaRuudustikku(ruudustik, 1, &liigutas);
            handled = true;
        }
        if (event == Event::ArrowLeft) {
            liigutaRuudustikku(ruudustik, 2, &liigutas);
            handled = true;
        }
        if (event == Event::ArrowRight) {
            liigutaRuudustikku(ruudustik, 3, &liigutas);
            handled = true;
        }
        if (liigutas) {
            lisaNumber(ruudustik);
            if (kasLabi(ruudustik)) {
                gameover = true;
            }
        }
        if (event == Event::Escape) {
            screen.Exit();
        }

        return handled;
    });

    screen.Loop(component);

    return 0;
}
