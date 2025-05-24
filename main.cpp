#include <iostream>
#include "cli.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <fstream>
#include <algorithm>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace std;
using namespace ftxui;

//https://github.com/ArthurSonzogni/FTXUI?tab=readme-ov-file

Color getColor(int number) {
    if (number == 0) {
        return Color::Black;
    }
    
    // Arvuta number log2 järgi
    // 2 -> 1, 4 -> 2, 8 -> 3, 16 -> 4, jne
    int index = 0;
    int temp = number;
    while (temp > 1) {
        temp /= 2;
        index++;
    }

    switch (index) {
        case 1: return Color::Yellow;
        case 2: return Color::Green;
        case 3: return Color::CadetBlueBis;
        case 4: return Color::Cyan;
        case 5: return Color::Magenta;
        case 6: return Color::RedLight;
        case 7: return Color::GreenLight;
        case 8: return Color::BlueLight;
        case 9: return Color::CyanLight;
        case 10: return Color::MagentaLight;
        case 11: return Color::Aquamarine1;
        default: return Color::Red;
    }
}

Element värviRuut(const std::string& content, const Color& bg_color) {
    return vbox({
        filler(),
        hbox({
            filler(),
            text(content),
            filler(),
        }),
        filler(),
    }) | bgcolor(bg_color) | color(Color::White) | size(WIDTH, EQUAL, 7) | size(HEIGHT, EQUAL, 3) | border;
}

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
            // Värv väärtuse järgi
            Color ruuduVärv = getColor(ruudustik[i][j]);
            cols.push_back(värviRuut(ruut, ruuduVärv));
        }
        rows.push_back(cols);
    }
    auto grid = gridbox(rows);

    return grid;
}

void salvestaSkoor(const string& nimi, int skoor) {
    ofstream file("skoorid.txt", std::ios::app);
    if (file.is_open()) {
        file << nimi << " " << skoor << "\n";
        file.flush();
        file.close();

        ifstream readCheck("skoorid.txt");
        readCheck.close();
    }
}

Element kuvaSkoorid() {
    ifstream file("skoorid.txt");
    if (!file.is_open()) {
        return text("Edetabelit ei saa kuvada!");
    }
    
    std::vector<Element> skoorid;
    skoorid.push_back(hbox(text("Nimi") | flex, text("Skoor") | flex));
    skoorid.push_back(separator());
    
    string nimi;
    int skoor;
    while (file >> nimi >> skoor) {
        skoorid.push_back(hbox(text(nimi) | flex, text(to_string(skoor)) | flex));
    }
    
    if (skoorid.empty()) {
        skoorid.push_back(text("Edetabelis pole veel tulemusi!"));
    }
    
    return vbox(skoorid);
}

int main() {
    auto screen = ScreenInteractive::TerminalOutput();

    vector<vector<int>> ruudustik;
    int suurus = 4;

    int gamestate = 0;
    int punktid = 0;

    bool liigutas = false;
    bool gameover = false;
    string kasutajaNimi = "";
    bool küsiNime = false;
    bool küsiUutMängu = false;
    bool küsiSalvestamist = false;

    auto nimiInput = Input(&kasutajaNimi, "");
    
    // Nupud mängu lõpetuseks
    auto salvestaJah = Button("Jah", [&] {
        küsiSalvestamist = false;
        küsiNime = true;
        screen.PostEvent(Event::Custom);  // Force refresh
    });
    
    auto salvestaEi = Button("Ei", [&] {
        küsiSalvestamist = false;
        küsiUutMängu = true;
        screen.PostEvent(Event::Custom);  // Force refresh
    });
    
    // Kinnita nimi
    auto nimiKinnita = Button("Salvesta", [&] {
        if (kasutajaNimi.empty()) {
            kasutajaNimi = "Mängija";
        }
        salvestaSkoor(kasutajaNimi, punktid);
        küsiNime = false;
        küsiUutMängu = true;
        screen.PostEvent(Event::Custom); // Refresh
    });
    
    auto uusMängJah = Button("Jah", [&] {
        punktid = 0;
        liigutas = false;
        gameover = false;
        küsiUutMängu = false;
        looRuudustik(ruudustik, suurus);
        screen.PostEvent(Event::Custom); // Refresh
    });
    
    auto uusMängEi = Button("Ei", [&] {
        punktid = 0;
        gameover = false;
        küsiUutMängu = false;
        gamestate = 0;
        screen.PostEvent(Event::Custom);  // refresh
    });

    auto salvestamiseValik = Container::Horizontal({
        salvestaJah,
        salvestaEi
    });

    auto nimiSisestus = Container::Vertical({
        nimiInput,
        nimiKinnita
    });

    auto uusMängValik = Container::Horizontal({
        uusMängJah,
        uusMängEi
    });

    auto lõpetaMäng = [&]() {
        gameover = true;
        küsiSalvestamist = true;
        kasutajaNimi = "";
        screen.PostEvent(Event::Custom); // refresh
    };

    //mänguväli
    auto gridRenderer = Renderer([&] {
        if (küsiSalvestamist) {
            return vbox({
                text("Mäng läbi! Sinu skoor: " + to_string(punktid)),
                separator(),
                text("Kas soovid salvestada oma tulemuse?"),
                hbox({
                    salvestaJah->Render(),
                    text(" "),
                    salvestaEi->Render(),
                })
            });
        }
        
        if (küsiNime) {
            return vbox({
                text("Sisesta oma nimi:"),
                nimiInput->Render(),
                nimiKinnita->Render()
            });
        }
        
        if (küsiUutMängu) {
            return vbox({
                text("Kas soovid alustada uut mängu?"),
                hbox({
                    uusMängJah->Render(),
                    text(" "),
                    uusMängEi->Render(),
                })
            });
        }
        
        string gameoverText = "";
        if (gameover) {
            gameoverText = "Mäng läbi!";
        }
        return vbox(make_grid2(ruudustik), text(gameoverText));
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

    auto edetabelRenderer = Renderer([&] {
        std::vector<Element> skoorid;
        skoorid.push_back(hbox(text("Nimi") | flex, text("Skoor") | flex));
        skoorid.push_back(separator());

        vector<pair<string, int>> allScores;

        string filename = "skoorid.txt";
        ifstream file(filename);
        file.close();

        file.open(filename, ios::in);
        
        if (file.is_open()) {
            string nimi;
            int skoor;
            while (file >> nimi >> skoor) {
                allScores.push_back({nimi, skoor});
            }
            file.close();

            // Sorteeri tulemused paremusjärjestuses
            sort(allScores.begin(), allScores.end(), 
                [](const pair<string, int>& a, const pair<string, int>& b) {
                    return a.second > b.second;
                });

            for (const auto& [nimi, skoor] : allScores) {
                skoorid.push_back(hbox(text(nimi) | flex, text(to_string(skoor)) | flex));
            }
        }
        
        if (skoorid.size() <= 2) {
            skoorid.push_back(text("Edetabelis pole veel tulemusi!"));
        }
        
        return vbox({
            hbox(text("Edetabel:")),
            separator(),
            vbox(skoorid)
        });
    });

    auto skoorRenderer = Renderer([&] {
        if (gamestate == 1) {
            return vbox({
                hbox(text("Punktid: " + to_string(punktid))),
                separator()
            });
        }

        return vbox({});
    });

    //peamenüü
    vector<string> valikud = {"Mängi", "Seaded", "Edetabel", "Sulge"};
    int selected = 0;
    MenuOption option = MenuOption::Vertical();
    option.on_enter = [&]{
        if (selected == 3) {
            screen.Exit();
        }
        if (selected == 0) {
            liigutas = false;
            gameover = false;
            looRuudustik(ruudustik, suurus);
        }
        if (selected == 2) {
            screen.PostEvent(Event::Custom);
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
            edetabelRenderer,
            skoorRenderer,
        },
        &gamestate
    );

    //põhi komponent
    auto main_component = Container::Vertical({tab_container});
    
    main_component |= CatchEvent([&](Event event) {
        bool handled = false;
        
        // Mängu lõpu inputid
        if (gamestate == 1) {
            if (küsiSalvestamist && salvestamiseValik->OnEvent(event)) {
                return true;
            }
            
            if (küsiNime && nimiSisestus->OnEvent(event)) {
                return true;
            }
            
            if (küsiUutMängu && uusMängValik->OnEvent(event)) {
                return true;
            }

            if (!gameover) {
                liigutas = false;
                if (event == Event::ArrowUp) {
                    liigutaRuudustikku(ruudustik, 0, &liigutas, &punktid);
                    handled = true;
                }
                if (event == Event::ArrowDown) {
                   liigutaRuudustikku(ruudustik, 1, &liigutas, &punktid);
                    handled = true;
                }
                if (event == Event::ArrowLeft) {
                    liigutaRuudustikku(ruudustik, 2, &liigutas, &punktid);
                    handled = true;
                }
                if (event == Event::ArrowRight) {
                    liigutaRuudustikku(ruudustik, 3, &liigutas, &punktid);
                    handled = true;
                }
                if (liigutas) {
                   lisaNumber(ruudustik);
                   if (kasLabi(ruudustik)) {
                        lõpetaMäng();
                    }
                }
                if (event == Event::Escape) {
                    lõpetaMäng();
                    return true;
                }
            }
        }
        if (gamestate == 2) {
            if (event == Event::Escape) {
                gamestate = 0;
            }
        }
        if (gamestate == 3) {
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
            skoorRenderer->Render(),
            main_component->Render() | frame,
        }) |
            border;
    });

    cout << "\e[8;40;100t"; //Muudab algse terminali suurust

    screen.Loop(main_renderer);

    return 0;
}
