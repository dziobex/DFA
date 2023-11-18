//
// Created by jedno on 14.11.2023.
//

#include <iostream>

#include "TextureManager.h"
#include "Node.h"
#include "Constants.h"

#ifndef AUTOMAT_PASS_H
#define AUTOMAT_PASS_H


class Pass {
    sf::RectangleShape _passBody;

    bool focused;
    bool can_pass;
    int value;
    sf::Text _text;

    TextureManager *texture_manager;

    Node *from;
    Node *to;

public:
    Pass();
    ~Pass();

    void init(bool can_pass, Node &from, Node &to, TextureManager *tm);
    void init_look(int value);

    void draw(sf::RenderWindow &app);

    void toggle_focused();

    int get_value();
    int get_from_pos();
    int get_to_pos();

    Node *get_to() {
        return to;
    }

    bool get_can_pass() {
        return can_pass;
    }

    bool is_focused() {
        return focused;
    }
};


#endif //AUTOMAT_PASS_H
