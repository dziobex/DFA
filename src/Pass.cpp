//
// Created by jedno on 14.11.2023.
//

#include "Pass.h"

Pass::Pass() {
    // wyzerowanie wskaznikow
    texture_manager = NULL;
    from = NULL;
    to = NULL;

    focused = false;
    can_pass = true;
}

Pass::~Pass() {
    // czyszczenie pamieci
    delete from;
    delete to;
    delete texture_manager;
}

void Pass::init(bool can_pass, Node &from, Node &to, TextureManager *tm) {
    this->can_pass = can_pass;

    this->from = &from;
    this->to = &to;
    this->texture_manager = tm;

    _passBody = sf::RectangleShape(sf::Vector2f(300, 100));
    _passBody.setFillColor(A_COLOR);

    // ustawiam punkt obrotu, polozenia na grzbiecie strzalki (mniej wiecej w polowie tekstury)
    _passBody.setOrigin(150, 40);
    _passBody.setTexture(texture_manager->get_texture(0));
}

void Pass::init_look(int value) {
    this->value = value;

    // ustawianie pozycji strzalki:

    //  -rotacji
    float a = 0;
    a += (from->getPos() % 2 == to->getPos() % 2) ? 90 : 0;
    a += (from->getPos() > to->getPos()) ? 180 : 0;

    // warunek: from->getPos() % 2 == to->getPos() % 2
    // sprawdza czy oba wierz. sa w pionie

    //  -pozycji
    int x = (from->getPos() % 2 == to->getPos() % 2) ? W_HEIGHT / 4 : W_WIDTH / 2;
    int y = (from->getPos() % 2 == to->getPos() % 2) ? W_HEIGHT / 2 : W_WIDTH / 5;

    // male dopracowanie bazowych pozycji
    if (from->getPos() % 2 == to->getPos() % 2 && a < 180)
        x += 90;
    if (from->getPos() % 2 != to->getPos() % 2 && a < 90)
        y -= 90;

    // teraz rozlozenie pozycji

    if (from->getPos() % 2 == to->getPos() % 2 && from->getPos() >= 1 && to->getPos() >= 1)
        x += W_WIDTH / 2;

    if (from->getPos() % 2 != to->getPos() % 2 && from->getPos() >= 2 && to->getPos() >= 2)
        y += W_HEIGHT / 1.7;

    y += MOVE_OBJECT_Y;

    _passBody.setPosition(x, y);
    _passBody.rotate(a);

    // ustawianie wygladu tekstu

    _text.setFont(*texture_manager->get_font());
    _text.setString(std::to_string(value));

    _text.setCharacterSize(N_TEXT_FONT_SIZE / 3);
    _text.setFillColor(sf::Color::White);
    _text.setPosition(_passBody.getPosition());
    _text.setOrigin(_text.getGlobalBounds().getSize() / 2.f + _text.getLocalBounds().getPosition());

    _text.setOutlineColor(sf::Color::Black);
    _text.setOutlineThickness(5 / 2);
}

void Pass::draw(sf::RenderWindow &app) {
    app.draw(_passBody);
    app.draw(_text);
}

void Pass::toggle_focused() {
    // przelaczam stan focusu sciezki px
    // byla ustawiona na focus -> juz nie i w druga strone analogicznie;
    focused = !focused;

    // zaciemnienie powierzchni, zeby uzytkownik wiedzial skad sprawdzanie ciagu sie zaczyna
    _passBody.setFillColor(focused ? A_COLOR_FOCUSED : A_COLOR);
}

int Pass::get_from_pos() {
    return from->getPos();
}

int Pass::get_to_pos() {
    return to->getPos();
}

int Pass::get_value() {
    return this->value;
}

