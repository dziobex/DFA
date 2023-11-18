//
// Created by jedno on 15.11.2023.
//

#ifndef AUTOMAT_UI_H
#define AUTOMAT_UI_H

#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "TextureManager.h"

/* KOD UI DLA PROGRAMU */

struct Button {
    sf::Text buttonText;
    sf::Color normalTextColor, hoverTextColor;
    sf::RectangleShape buttonRect;
    sf::Color normalButtonColor, hoverButtonColor;
    bool showText, showRect;

    Button() = default;
    Button(bool showText, bool showRect, sf::Text buttonText, sf::RectangleShape buttonRect, sf::Color hoverTextColor = sf::Color::White, sf::Color hoverButtonColor = sf::Color::Black);

    bool Hover(sf::RenderWindow& window);

    void Render(sf::RenderWindow& window);

    void changeColor(const sf::Color color);
    void changeText(const char *string);
};

struct Input {
    bool ready;

    int text_count;
    std::string _text;

    sf::RectangleShape _field;

    sf::Text _info_text;
    sf::Text _error_text;
    sf::Text _displayed_text;
    sf::Text _limit_text;

    sf::RectangleShape _prompt;
    float _prompt_alpha;
    bool _prompt_fade;

    TextureManager *texture_manager;

    Input() = default;
    ~Input();

    void init(TextureManager *tm);

    void handle_errors();
    void handle_text();

    void fix_propmt(float dt);
    void render(sf::RenderWindow &window, float dt);

    std::string get_input();

};

#endif //AUTOMAT_UI_H
