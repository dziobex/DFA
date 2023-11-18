//
// Created by jedno on 15.11.2023.
//

#include "UI.h"

Button::Button(bool showText, bool showRect, sf::Text buttonText, sf::RectangleShape buttonRect, sf::Color hoverTextColor, sf::Color hoverButtonColor) {
    this->showText = showText;
    this->showRect = showRect;
    this->buttonRect = buttonRect;
    this->buttonText = buttonText;
    this->hoverTextColor = hoverTextColor;
    this->hoverButtonColor = hoverButtonColor;

    normalButtonColor = buttonRect.getFillColor();
}

// checking whether button is hovered //
bool Button::Hover(sf::RenderWindow& window) {
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    if (showRect)
        buttonRect.setFillColor(buttonRect.getGlobalBounds().contains(mousePos) ? hoverButtonColor : normalButtonColor);
    if (showText)
        buttonText.setFillColor(buttonText.getGlobalBounds().contains(mousePos) ? hoverTextColor : sf::Color::White);
    return (showText && buttonText.getGlobalBounds().contains(mousePos)) || (showRect && buttonRect.getGlobalBounds().contains(mousePos));
}

// drawing button on the screen //
void Button::Render(sf::RenderWindow& window) {
    if (showRect)
        window.draw(buttonRect);
    if (showText)
        window.draw(buttonText);
}

void Button::changeColor(const sf::Color color) {
    normalButtonColor = color;

    this->buttonRect.setFillColor(normalButtonColor);
}

void Button::changeText(const char *string) {
    buttonText.setString(string);
}

/* INPUT */

void Input::init(TextureManager *tm) {
    texture_manager = tm;

    // setup pola na input
    _field = sf::RectangleShape(sf::Vector2f(3 * W_WIDTH / 4, W_HEIGHT / 2));
    _field.setOrigin(3 * W_WIDTH / 8, W_HEIGHT / 4);
    _field.setPosition(W_WIDTH / 2, W_HEIGHT / 1.8);
    _field.setFillColor(sf::Color(242, 239, 220));
    _field.setOutlineThickness(4);
    _field.setOutlineColor(sf::Color::Black);

    // setup tekstow
    _info_text.setString("Podaj ciag znakow (liczba znakow musi nalezec do przedzialu (0; 125>):\n"
                         "!!! Dozwolone sa tylko znaki: { 0; 1 } - inne beda ignorowane");
    _info_text.setFont(*texture_manager->get_font());
    _info_text.setCharacterSize(19);
    _info_text.setPosition(_field.getPosition() - sf::Vector2f(3 * W_WIDTH / 8, W_HEIGHT / 4 + 60));
    _info_text.setFillColor(sf::Color::White);
    _info_text.setOutlineColor(sf::Color::Black);
    _info_text.setOutlineThickness(2);

    _error_text.setFont(*texture_manager->get_font());
    _error_text.setCharacterSize(12);
    _error_text.setPosition(_field.getPosition() - sf::Vector2f(3 * W_WIDTH / 8, (-W_HEIGHT / 4) - 8));

    _text = "";

    _displayed_text.setString(_text);
    _displayed_text.setFont(*texture_manager->get_font());
    _displayed_text.setCharacterSize(25);
    _displayed_text.setPosition(_field.getPosition() - sf::Vector2f(3 * W_WIDTH / 8 - 5, W_HEIGHT / 4 - 5));
    _displayed_text.setFillColor(sf::Color::Black);
    _displayed_text.setLetterSpacing(2);

    ready = false;
    text_count = 0;

    // ustawienia licznika liczb
    _limit_text;
    _limit_text.setFont(*texture_manager->get_font());
    _limit_text.setString("000/125");
    _limit_text.setCharacterSize(N_TEXT_FONT_SIZE / 5);
    _limit_text.setFillColor(sf::Color::Blue);
    _limit_text.setPosition(_displayed_text.getPosition() + sf::Vector2f(_field.getSize().x - 60 ,-30));

    // ustawianie znaku zachety
    _prompt = sf::RectangleShape(sf::Vector2f(20, 5));
    _prompt.setFillColor(sf::Color::Black);
    _prompt.setPosition(_displayed_text.getPosition() + sf::Vector2f(0, _displayed_text.getCharacterSize()));

    _prompt_alpha = 255;
    _prompt_fade = true;

    handle_errors();
}

void Input::handle_text() {

    bool is_num_0 = sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0);
    bool is_num_1 = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1);

    // uzytkownik wpisal 0 (na numpadzie lub tradycyjnie)
    if (is_num_0 && text_count < 125) {
        _text += '0';
        _prompt.setPosition(_prompt.getPosition() + sf::Vector2f(_displayed_text.getCharacterSize() * 3.75/5, 0));
        ++text_count;
    }
    // uzytkownik wpisal 1 (na numpadzie lub tradycyjnie)
    if (is_num_1 && text_count < 125) {
        _text += '1';
        _prompt.setPosition(_prompt.getPosition() + sf::Vector2f(_displayed_text.getCharacterSize() * 3.75/5, 0));
        ++text_count;
    }

    // usun element z inputu
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && text_count > 0) {
        int delete_count = (_text[_text.size() - 1] == '\n' ? 2 : 1);
        _text = _text.substr(0, _text.size() - delete_count);
        _prompt.setPosition(_prompt.getPosition() - sf::Vector2f(_displayed_text.getCharacterSize() * 3.75/5, 0));
        --text_count;
    }

    if (is_num_0 || is_num_1 || sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
        // tu wstawie odpowienio \n
        std::string _text_copy = _text;
        for (int i = 36; i < _text_copy.size(); i += 37) {
            std::string left = _text_copy.substr(0, i);
            std::string right = _text_copy.substr(i);
            _text_copy = left + '\n' + right;
        }

        // pozycja dla prompta
        float _px = (_displayed_text.getCharacterSize() * 3.75/5) * (_text.size() % 36);
        float _py = (_displayed_text.getCharacterSize() * 5/4.5) * (_text.size() / 36);
        sf::Vector2f _based_p_pos = sf::Vector2f(_displayed_text.getPosition() + sf::Vector2f(0, _displayed_text.getCharacterSize()));

        _displayed_text.setString(_text_copy);
        _prompt.setPosition(_based_p_pos + sf::Vector2f (_px, _py));
    }

    // update wyswietlacza ile znakow ma juz input
    std::string inp_count = std::to_string(_text.size());
    _limit_text.setString(std::string (3-inp_count.size(), '0') + inp_count + "/125");
}

/* wyswietlenie elementow inputu */
void Input::render(sf::RenderWindow &window, float dt) {
    window.draw(_field);
    window.draw(_info_text);
    window.draw(_error_text);

    window.draw(_displayed_text);

    fix_propmt(dt);
    window.draw(_prompt);

    window.draw(_limit_text);
}

Input::~Input() {
    delete texture_manager;
}

/* zmieniam tresc tekstu 'bledu' */
void Input::handle_errors() {
    if (text_count == 0) {
        ready = false;
        _error_text.setString("ERROR: Ale... tu nic nie ma!");
        _error_text.setFillColor(sf::Color::Red);
    } else {
        ready = true;
        _error_text.setString("OK: Mozemy zaczynac!\t\t\t\t [kliknij ponizszy przycisk]");
        _error_text.setFillColor(sf::Color::Green);
    }
}

// zwracam czysty input bez bialych znakow
std::string Input::get_input() {
    std::string inpt = "";

    for (auto c : _text)
        if (c != '\n')
            inpt += c;

    return inpt;
}

/* ustawiam kolor znaku zachety */

void Input::fix_propmt(float dt) {
    if (_prompt_alpha >= 255) {
        // rozpoczynanie zaciemniania
        _prompt_fade = true;
        _prompt_alpha = 255;
    }
    if (_prompt_alpha <= 0) {
        // rozpoczynanie rozjasniania
        _prompt_fade = false;
        _prompt_alpha = 0;
    }

    _prompt_alpha += (_prompt_fade ? -1 : 1) * dt * 1000; // <-- wsp. szybkosci rozjas/przyciem -niania znaku

    sf::Color _p_color = _prompt.getFillColor();
    _p_color.a = _prompt_alpha;
    _prompt.setFillColor(_p_color);
}
