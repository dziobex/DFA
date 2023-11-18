//
// Created by jedno on 08.11.2023.
//

#include "Node.h"
#include "Constants.h"

Node::Node() {
    focused = false;

    // domyslne tytuly wierzcholka: nie jest ani startujacy ani konczacy
    starting = false;
    ending = false;
}

// destruktor
Node::~Node() {
    // czyszczenie pamieci
    delete texture_manager;
}

void Node::init(int pos, TextureManager *tm) {
    focused = false;

    // na podstawie liczby pos (nr indeksu) bede ustawiac lokalizacje kolka na ekranie
    this->pos = pos;

    // ustawiam wskaznik do glownego texture managera programu
    this->texture_manager = tm;


    // WYGLAD WIERZOCHOLKA

    //// rozmiar/kolor ogolny wierzcholka
    _nodeBody = sf::CircleShape(N_RADIUS);
    if (starting) {
        _nodeBody.setFillColor(START_COLOR);
    } else if (ending) {
        _nodeBody.setFillColor(END_COLOR);
    } else {
        _nodeBody.setFillColor(N_COLOR);
    }

    //// obramowanie wierzcholka
    _nodeBody.setOutlineColor(sf::Color::Black);
    _nodeBody.setOutlineThickness(5);

    // ustawiam pozycje wierzcholka
    // idzie to tak:
    // 0 1
    // 2 3
    // nieparzyste na prawo, parzyste lewo

    int _x = (pos % 2 == 0 ? N_W_GAP : W_WIDTH - N_RADIUS * 2 - N_W_GAP);
    int _y = (pos >= 2 ? W_HEIGHT - N_RADIUS * 2 - N_H_GAP: N_H_GAP);

    // przesuwam obiekt na osi Y wg stalej
    _y += MOVE_OBJECT_Y;

    _nodeBody.setPosition(_x, _y);
}

/* rysowanie wierzcholka na ekranie */
void Node::draw(sf::RenderWindow &app) {
    app.draw(_nodeBody);
    app.draw(_text);
}

void Node::init_text(std::string text) {
    _text.setFont(*texture_manager->get_font());
    _text.setString(text);

    /// rozmiar, kolor, pozycja tekstu
    _text.setCharacterSize(N_TEXT_FONT_SIZE);
    _text.setFillColor(sf::Color::White);
    _text.setPosition(_nodeBody.getPosition() + sf::Vector2f(N_RADIUS / 2, N_RADIUS / 3));

    /// obramowanie tekstu
    _text.setOutlineColor(sf::Color::Black);
    _text.setOutlineThickness(5);
}

int Node::getPos() {
    return this->pos;
}

void Node::toggle_focused() {
    // przelaczam stan focusu wierzcholka qx
    // byl ustawiony na focus -> juz nie i w druga strone analogicznie;
    focused = !focused;

    // zaciemnienie powierzchni, zeby uzytkownik wiedzial skad sprawdzanie ciagu sie zaczyna

    if (starting) {
        _nodeBody.setFillColor(focused ? START_COLOR_FOCUSED : START_COLOR);
    } else if (ending) {
        _nodeBody.setFillColor(focused ? END_COLOR_FOCUSED : END_COLOR);
    } else {
        _nodeBody.setFillColor(focused ? N_COLOR_FOCUSED : N_COLOR);
    }
}

/* w 2 kolejnych funkcjach nastepuje nadanie wierzcholkowi tytulu: STARTUJACEGO (q0)/KONCZACEGO (F) */

void Node::make_starting() {
    starting = true;
}

void Node::make_ending() {
    ending = true;
}

/* zwraca informacje czy dany obiekt (wierzcholek) jest konczacy (F) */
bool Node::is_ending() {
    return ending;
}

/* zwracam informacje czy obecnie wierzcholek jest sprawdzany (zaciemniony) */
bool Node::is_focused() {
    return focused;
}
