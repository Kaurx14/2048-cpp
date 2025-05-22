#include <iostream>
#include "cli.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace std;
using namespace ftxui;

//https://github.com/ArthurSonzogni/FTXUI?tab=readme-ov-file

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

    int gamestate = 0;

    bool liigutas = false;
    bool gameover = false;

    auto gridRenderer = Renderer([&] {
        string gameoverText = "";
        if (gameover) {
            gameoverText = "Mäng läbi!";
        }
        return vbox(make_grid2(ruudustik), text(gameoverText));  //
    });

    vector<string> valikud = {"Mängi", "Seaded", "Sulge"};
    int selected = 0;
    MenuOption option = MenuOption::Vertical();
    option.on_enter = [&]{
        if (selected == 2) {
            screen.Exit();
        }
        if (selected == 1) {
            liigutas = false;
            gameover = false;
            looRuudustik(ruudustik);
        }
        gamestate = selected+1;
    };
    auto mainMenu = Menu(&valikud,
    &selected, option);

    //TODO
    auto seaded = Container::Horizontal({
        Renderer([&] {
            return text("TODO");
        })
    });

    int state = 0;
    auto tab_container = Container::Tab(
        {
            mainMenu,
            gridRenderer,
            seaded,
        },
        &gamestate
    );

    auto main_component = Container::Vertical({tab_container});
    main_component |= CatchEvent([&](Event event) {
        bool handled = false;
        if (gamestate == 1) {
            liigutas = false;
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
                gamestate = 0;
            }
        }
        if (gamestate == 2) {
            if (event == Event::Escape) {
                gamestate = 0;
            }
        }
        return handled;
    });

    auto main_renderer = Renderer(main_component, [&] {
        return vbox({
            hbox(text("selected = "), text(std::to_string(selected))),
            hbox(text("gamestate = "), text(std::to_string(gamestate))),
            hbox(text("ESC viib tagasi main menusse")),
            separator(),
            main_component->Render() | frame,
        }) |
            border;
    });

    cout << "\e[8;40;100t"; //Muudab algse terminai suurust

    screen.Loop(main_renderer);

    return 0;
}
