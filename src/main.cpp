#include <cstdlib>
#include <ctime>
#include <Windows.h>

#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "Assets.h"
#include "TextureManager.h"

#include "StageManager.h"

/* START PROGRAMU */

/* Wyswietlaniem zajmuje sie instancja obiektu StageManager */
/* Podzielilam wyswietlanie na poszczegolne funkcje aby zachowac jako-taka czytelnosc kodu */

int main()
{
    /* Ukrycie konsoli, jesli nie jestem w trybie odrobaczania */

#ifdef NDEBUG
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
#endif

    srand(time (NULL));
    auto window = sf::RenderWindow( { W_WIDTH, W_HEIGHT }, "Automat skonczony" );

    /* ladowanie potrzebnych tekstur */
    TextureManager texture_manager;
    texture_manager.load_font(ComicMono_ttf, ComicMono_ttf_len); // czcionka
    texture_manager.load_texture(arrow_png, arrow_png_len); //0 - strzalka
    texture_manager.load_texture(reactions_png, reactions_png_len ); //1 - wszystkie wizualne reakcje

    sf::Image icon;
    icon.loadFromMemory(fav_png, fav_png_len);
    window.setIcon(icon.getSize().x, icon.getSize().y,icon.getPixelsPtr());

    sf::Clock clock;
    float delta_time;
    int *stage_number = new int;
    (*stage_number) = 0;
    // 0 - ekran wpisu inputu
    // 1 - ekran symulacji
    // 2 - ekran wyniku symulacji

    // manager scen - powstaly by main nie zostal zapchany XD
    StageManager sm;
    sm.init(&delta_time, &texture_manager, stage_number);
    sm.setup();

    while (window.isOpen())
    {
        delta_time = clock.restart().asSeconds();

        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                sm.handle_key_press();
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left)
                    sm.handle_lpm_press(window);
            }
        }

        window.clear(sf::Color::White);

        /* zmiana sceny ze wzg na indeks sceny (przekazany za pomoca adresu do instancji stage managera) */
        switch (*stage_number) {
            case 0:
                sm.intro(window);
                break;
            case 1:
                sm.main_stage(window);
                break;
            case 2:
                sm.results(window);
                break;
            default:
                break;
        }

        window.display();
    }

    return 0;
}