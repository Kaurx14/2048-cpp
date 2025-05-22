#include <iostream>
#include "cli.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace std;
using namespace ftxui;

//https://github.com/ArthurSonzogni/FTXUI?tab=readme-ov-file

Element make_grid2(vector<vector<int>>& ruudustik) {
    std::vector<Elements> rows;
    for (int i = 0; i < ruudustik.size(); ++i) {
        std::vector<Element> cols;
        for (int j = 0; j < ruudustik.size(); ++j) {
            string ruut;
            if (ruudustik[i][j] == 0) {
                ruut = " ";
            } else {
                ruut = to_string(ruudustik[i][j]);
            }
            cols.push_back(text(ruut)  | center | size(WIDTH, EQUAL, 7) | size(HEIGHT, EQUAL, 3) | border);
        }
        rows.push_back(cols);
    }
    auto grid = gridbox(rows);

    return grid;
}

int main() {
    auto screen = ScreenInteractive::TerminalOutput();

    vector<vector<int>> ruudustik;
    int suurus = 4;

    int gamestate = 0;

    bool liigutas = false;
    bool gameover = false;

    //mänguväli
    auto gridRenderer = Renderer([&] {
        string gameoverText = "";
        if (gameover) {
            gameoverText = "Mäng läbi!";
        }
        return vbox(make_grid2(ruudustik), text(gameoverText));  //
    });

    //seaded
    //TODO implemeteeri veel seadeid, nt värviskeemi muutmine
    string suurusString = "4";
    auto suuruseMuutmine = Input(&suurusString, "");
    suuruseMuutmine |= CatchEvent([&](Event event) {
        return event.is_character() && !std::isdigit(event.character()[0]);
    });
    suuruseMuutmine |= CatchEvent([&](Event event) {
        return event.is_character() && suurusString.size() > 0;
    });
    auto seadedKinnita = Button("Kinnita", [&] {
        try {
            int uusSuurus = stoi(suurusString);
            if (uusSuurus < 2) {
                uusSuurus = 2;
            }
            if (uusSuurus > 9) {
                uusSuurus = 9;
            }
            suurusString = to_string(uusSuurus);
            suurus = uusSuurus;
        } catch (invalid_argument& e) {
            suurusString = "4";
            suurus = 4;
        }
    });
    auto seadedTab = Container::Vertical({
        suuruseMuutmine,
        seadedKinnita,
    });
    auto seadedRenderer = Renderer(seadedTab, [&] {
        return vbox({
            hbox(text("Mängulaua suurus: "), suuruseMuutmine->Render()),
            text("muud valikud TODO"),
            seadedKinnita->Render(),
            separator(),
            text("debug asjad"),
            text(suurusString),
            text(to_string(suurus)),
        });
    });

    //peamenüü
    vector<string> valikud = {"Mängi", "Seaded", "Sulge"};
    int selected = 0;
    MenuOption option = MenuOption::Vertical();
    option.on_enter = [&]{
        if (selected == 2) {
            screen.Exit();
        }
        if (selected == 0) {
            liigutas = false;
            gameover = false;
            looRuudustik(ruudustik, suurus);
        }
        gamestate = selected+1;
    };
    auto mainMenu = Menu(&valikud, &selected, option);

    //kõikide osade konteiner
    //lisa siia uusi osasid
    auto tab_container = Container::Tab(
        {
            mainMenu,
            gridRenderer,
            seadedRenderer,
        },
        &gamestate
    );

    //põhi komponent
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

    //TODO parem välimus, kehtib ka eelmiste osade kohta
    auto main_renderer = Renderer(main_component, [&] {
        return vbox({
            text("debug info"),
            hbox(text("selected = "), text(std::to_string(selected))),
            hbox(text("gamestate = "), text(std::to_string(gamestate))),
            separator(),
            hbox(text("ESC viib tagasi peamenüüsse")),
            separator(),
            main_component->Render() | frame,
        }) |
            border;
    });

    cout << "\e[8;40;100t"; //Muudab algse terminali suurust

    screen.Loop(main_renderer);

    return 0;
}
