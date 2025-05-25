#include <iostream>
#include "logic.h"
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <fstream>
#include <algorithm>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace std;
using namespace ftxui;

//----
//terminali suuruse koodi viide: https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#endif // Windows/Linux

void get_terminal_size(int& width, int& height) {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = (int)(csbi.srWindow.Right-csbi.srWindow.Left+1);
    height = (int)(csbi.srWindow.Bottom-csbi.srWindow.Top+1);
#elif defined(__linux__)
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    width = (int)(w.ws_col);
    height = (int)(w.ws_row);
#endif // Windows/Linux
}
//----

//Kasutusel FTXUI teek
//https://github.com/ArthurSonzogni/FTXUI

//Abimeetodid TUI loomiseks
Color getColor(int number, int varviskeem) {
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

    if (varviskeem == 1) {
        //värviskeem 2
        switch (index) {
            case 1: return Color::Cyan;
            case 2: return Color::BlueLight;
            case 3: return Color::Blue;
            case 4: return Color::MagentaLight;
            case 5: return Color::Magenta;
            case 6: return Color::RedLight;
            case 7: return Color::Red;
            case 8: return Color::DarkRed;
            case 9: return Color::OrangeRed1;
            case 10: return Color::Orange1;
            case 11: return Color::Yellow;
            default: return Color::Gold1;
        }
    } else if (varviskeem == 2) {
        //värviskeem 3
        switch (index) {
            case 1: return Color::Red;
            case 2: return Color::RedLight;
            case 3: return Color::OrangeRed1;
            case 4: return Color::Orange1;
            case 5: return Color::Yellow;
            case 6: return Color::PaleGreen3;
            case 7: return Color::CyanLight;
            case 8: return Color::Turquoise2;
            case 9: return Color::Cyan;
            case 10: return Color::BlueLight;
            case 11: return Color::Blue;
            default: return Color::DeepPink1;
        }
    } else {
        //tavaline värviskeem
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
            case 11: return Color::OrangeRed1;
            default: return Color::Red;
        }
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

Element joonistaRuudustik(vector<vector<int>>& ruudustik, int varviskeem) {
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
            Color ruuduVärv = getColor(ruudustik[i][j], varviskeem);
            cols.push_back(värviRuut(ruut, ruuduVärv));
        }
        rows.push_back(cols);
    }
    auto grid = gridbox(rows);

    return grid;
}

void salvestaSkoor(const string& nimi, int skoor, int lauasuurus) {
    ofstream file("skoorid.txt", std::ios::app);
    if (file.is_open()) {
        file << nimi << " " << skoor << " " << lauasuurus << "\n";
        file.flush();
        file.close();

        ifstream readCheck("skoorid.txt");
        readCheck.close();
    }
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

    int varviskeem = 0;

    //---------------
    //Mänguväli
    //---------------
    InputOption nimiInputOption;
    nimiInputOption.on_enter = [&] {
        if (kasutajaNimi.empty()) {
            kasutajaNimi = "Mängija";
        }
        salvestaSkoor(kasutajaNimi, punktid, suurus);
        küsiNime = false;
        küsiUutMängu = true;
        screen.PostEvent(Event::Custom); // Force refresh
    };
    auto nimiInput = Input(&kasutajaNimi, "", nimiInputOption);
    nimiInput |= CatchEvent([&](Event event) {
        return event.is_character() && kasutajaNimi.size() > 19;
    });

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
        salvestaSkoor(kasutajaNimi, punktid, suurus);
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

    auto manguRenderer = Renderer([&] {
        vector<Element> vali = {joonistaRuudustik(ruudustik, varviskeem) | hcenter};

        if (küsiSalvestamist) {
            vali.push_back(vbox({
                separator(),
                text("Mäng läbi! Sinu skoor: " + to_string(punktid)),
                filler(),
                text("Kas soovid salvestada oma tulemuse?"),
                hbox({
                    salvestaJah->Render(),
                    text(" "),
                    salvestaEi->Render(),
                })
            }));
        }

        if (küsiNime) {
            vali.push_back( vbox({
                separator(),
                text("Sisesta oma nimi:"),
                nimiInput->Render(),
            }));
        }

        if (küsiUutMängu) {
            vali.push_back( vbox({
                separator(),
                text("Kas soovid alustada uut mängu?"),
                hbox({
                    uusMängJah->Render(),
                    text(" "),
                    uusMängEi->Render(),
                })
            }));
        }

        return vbox(vali);
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

    //---------------
    //Seaded
    //---------------
    string suurusString = "4";
    InputOption suurusInputOption;
    suurusInputOption.on_enter = [&] {
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
    };
    suurusInputOption.on_change = [&] {
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

        }
    };
    auto suuruseMuutmine = Input(&suurusString, "", suurusInputOption);
    suuruseMuutmine |= CatchEvent([&](Event event) {
        return event.is_character() && !std::isdigit(event.character()[0]);
    });
    suuruseMuutmine |= CatchEvent([&](Event event) {
        return event.is_character() && suurusString.size() > 0;
    });

    vector<string> varvid = {"Värviskeem 1", "värviskeem 2","värviskeem 3"};
    auto varviValik = Radiobox(&varvid, &varviskeem);

    auto tagasiNuppSeaded = Button("Tagasi", [&] {
        gamestate = 0;
        if (suurusString.empty()) {
            suurusString = to_string(suurus);
        }
    });

    auto seadedTab = Container::Vertical({
        suuruseMuutmine,
        varviValik,
        tagasiNuppSeaded,
    });
    auto seadedRenderer = Renderer(seadedTab, [&] {
        return vbox({
            text("Seaded:"),
            separator(),
            hbox(hbox(text("Mängulaua suurus: "), suuruseMuutmine->Render() | size(WIDTH, EQUAL, 3) | size(HEIGHT, LESS_THAN, 3))
                | vcenter | size(WIDTH, GREATER_THAN, 23),
                text(to_string(suurus) + "x" + to_string(suurus)) | vcenter | border),
            hbox(text("Värviskeem: "), varviValik->Render()),
            separator(),
            tagasiNuppSeaded->Render() | size(WIDTH, EQUAL, 6)
        });
    });

    //---------------
    // Edetabel
    //---------------
    auto tagasiNuppEdetabel = Button("Tagasi", [&]{gamestate = 0;});
    auto edetabelRenderer = Renderer(tagasiNuppEdetabel, [&] {
        std::vector<Element> skoorid;
        skoorid.push_back(hbox(text("Nimi                      ") | flex, text("Skoor") | flex, text("Mängulaua suurus") | flex));
        skoorid.push_back(separator());

        vector<tuple<string, int, int>> allScores;

        string filename = "skoorid.txt";
        ifstream file(filename);
        file.close();

        file.open(filename, ios::in);

        if (file.is_open()) {
            string nimi;
            int skoor;
            int lauasuurus;
            while (file >> nimi >> skoor >> lauasuurus) {
                allScores.push_back({nimi, skoor, lauasuurus});
            }
            file.close();

            // Sorteeri tulemused paremusjärjestuses
            sort(allScores.begin(), allScores.end(),
                [](const tuple<string, int, int>& a, const tuple<string, int, int>& b) {
                    return get<1>(a) > get<1>(b);
                });

            for (auto& [nimi, skoor, lauasuurus] : allScores) {
                while (nimi.size() <= 20) {
                    nimi += " ";
                }
                skoorid.push_back(hbox(text(nimi) | flex, text(to_string(skoor)) | flex, text(to_string(lauasuurus) + "x" + to_string(lauasuurus)) | flex));
            }
        }

        if (skoorid.size() <= 2) {
            skoorid.push_back(text("Edetabelis pole veel tulemusi!"));
        }

        return vbox({
            hbox(text("Edetabel:")),
            separator(),
            vbox(skoorid),
            separator(),
            tagasiNuppEdetabel->Render() | size(WIDTH, EQUAL, 6),
        });
    });

    //---------------
    //Peamenüü
    //---------------
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
    option.entries_option.animated_colors.foreground.enabled = true;
    option.entries_option.animated_colors.background.enabled = true;
    option.entries_option.animated_colors.background.active = Color::DarkGreen;
    option.entries_option.animated_colors.background.inactive = Color::Default;
    option.entries_option.animated_colors.foreground.active = Color::White;
    option.entries_option.animated_colors.foreground.inactive = Color::GrayDark;
    auto mainMenu = Menu(&valikud, &selected, option)  | size(WIDTH, EQUAL, 12) | center;

    auto mainMenuRenderer = Renderer([&] {
        return vbox();
    });

    //kõikide osade konteiner
    //lisa siia uusi osasid
    auto tab_container = Container::Tab(
        {
            mainMenu,
            manguRenderer,
            seadedRenderer,
            edetabelRenderer,
            skoorRenderer,
        },
        &gamestate
    );

    //---------------
    //Põhikompoment ja renderdamine
    //---------------
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

    auto main_renderer = Renderer(main_component, [&] {
        auto tiitel = vbox();
        if (gamestate == 0) {
            tiitel = vbox({
                text("  _______  ________  ___   ___  ________                 ________  ________  ________   ") | color(Color::Green),
                text(" /  ___  \\|\\   __  \\|\\  \\ |\\  \\|\\   __  \\               |\\   ____\\|\\   __  \\|\\   __  \\  ") | color(Color::Yellow),
                text("/__/|_/  /\\ \\  \\|\\  \\ \\  \\\\_\\  \\ \\  \\|\\  \\  ____________\\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\ ") | color(Color::PaleGreen3),
                text("|__|//  / /\\ \\  \\\\\\  \\ \\______  \\ \\   __  \\|\\____________\\ \\  \\    \\ \\   ____\\ \\   ____\\") | color(Color::Aquamarine3),
                text("    /  /_/__\\ \\  \\\\\\  \\|_____|\\  \\ \\  \\|\\  \\|____________|\\ \\  \\____\\ \\  \\___|\\ \\  \\___|") | color(Color::Cyan),
                text("   |\\________\\ \\_______\\     \\ \\__\\ \\_______\\              \\ \\_______\\ \\__\\    \\ \\__\\   ") | color(Color::BlueLight),
                text("    \\|_______|\\|_______|      \\|__|\\|_______|               \\|_______|\\|__|     \\|__|   ") | color(Color::Blue),
                text(""),
                text(""),
            });
        }
        return vbox({
            tiitel | center,
            skoorRenderer->Render(),
            main_component->Render() | frame,
        })  | border;
    });
    //---------------

    srand(static_cast<unsigned int>(time(0)));

    system("cls");

    //Terminali suuruse muutmine, et algses olekus mängu ära mahutada
    int korgus, laius;
    get_terminal_size(laius, korgus);
    int algnekorgus = korgus;
    int algnelaius = laius;
    if (korgus < 35) {
        korgus = 35;
    }
    if (laius < 90) {
        laius = 90;
    }
    string sizeCtrl = "\e[8;" + to_string(korgus) + ";" + to_string(laius) + "t";
    cout << sizeCtrl;

    screen.Loop(main_renderer);

    //taastame algse terminali suuruse peale mängu sulgemist
    system("cls");
    sizeCtrl = "\e[8;" + to_string(algnekorgus) + ";" + to_string(algnelaius) + "t";
    cout << sizeCtrl;

    return 0;
}
