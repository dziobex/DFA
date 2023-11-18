//
// Created by jedno on 08.11.2023.
//

#ifndef CMAKESFMLPROJECT_CONSTANTS_H
#define CMAKESFMLPROJECT_CONSTANTS_H

#include <SFML/Graphics.hpp>

/* STAŁE DLA PROGRAMU
* - powtarzają się często w kodzie
* - na ich podstawie określan np. pozycje jakiegoś elementu
* - oszczędność pisania
* - ochrona przed błędami (ręczne wpisywanie liczb to łatwa droga do pomyłki)
*/

// szybkosc symulacji
const float SIM_SPEED = 0.5;

// wymiary okna aplikacji
const int W_WIDTH = 950;
const int W_HEIGHT = 800;

// wymiary roznych ksztaltow
const int N_RADIUS = 80;

// rozmiary czcionki na wierzcholkach
const int N_TEXT_FONT_SIZE = 70;

// odleglosc wierzcholka od krawedzi okna (szerokosc, wysokosc)
const int N_W_GAP = N_RADIUS * 2;
const int N_H_GAP = N_RADIUS;

const int MOVE_OBJECT_Y = -50;

// kolory
const sf::Color START_COLOR = sf::Color(0, 180, 0);
const sf::Color START_COLOR_FOCUSED = sf::Color(0, 110, 0);

const sf::Color END_COLOR = sf::Color(180, 0, 0);
const sf::Color END_COLOR_FOCUSED = sf::Color(110, 0, 0);

const sf::Color N_COLOR = sf::Color(180, 180, 180);
const sf::Color N_COLOR_FOCUSED = sf::Color(110, 110, 110);

const sf::Color A_COLOR = sf::Color(180, 180, 180);
const sf::Color A_COLOR_FOCUSED = sf::Color(110, 110, 110);


#endif //CMAKESFMLPROJECT_CONSTANTS_H
