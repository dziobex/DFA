//
// Created by jedno on 08.11.2023.
//

#ifndef CMAKESFMLPROJECT_NODE_H
#define CMAKESFMLPROJECT_NODE_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "TextureManager.h"

class Node {
    bool focused;
    bool starting, ending;
    int pos;

    sf::CircleShape _nodeBody;
    sf::Text _text;

    TextureManager *texture_manager;

public:

    Node();
    ~Node();

    void init (int pos, TextureManager *tm);
    void init_text(std::string text);

    void toggle_focused();

    void draw(sf::RenderWindow &app);

    void make_starting();
    void make_ending();

    int getPos();
    bool is_ending();
    bool is_focused();
};


#endif //CMAKESFMLPROJECT_NODE_H
