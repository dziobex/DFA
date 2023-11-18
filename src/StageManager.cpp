//
// Created by jedno on 15.11.2023.
//

#include <cstdlib>
#include <ctime>

#include "StageManager.h"

/* konstruktor */
StageManager::StageManager() {
    dt = nullptr;
    texture_manager = nullptr;

    current_pass = nullptr;
    current_node = nullptr;
    stage_number = nullptr;

    SIM_SPEED_LOC = SIM_SPEED;
}

/* destruktor i czyszczenie pamieci */
StageManager::~StageManager() {
    delete dt;
    delete texture_manager;

    delete current_node;
    delete current_pass;
    delete stage_number;
}

/* inicjalizacja w petli main */
void StageManager::init(float *deltaTime, TextureManager *tm, int *stage_number) {
    this->dt = deltaTime;
    this->texture_manager = tm;
    this->stage_number = stage_number;
}

//  set up do zmiennych
void StageManager::setup() {
    SIM_SPEED_LOC = SIM_SPEED;

    // setup dla przycisku '''ZACZYNAMY'''

    _btn_text.setFont(*texture_manager->get_font());
    _btn_text.setString("Zaczynamy!");
    _btn_text.setCharacterSize(N_TEXT_FONT_SIZE / 4);
    _btn_text.setFillColor(sf::Color::White);
    _btn_text.setOrigin(_btn_text.getGlobalBounds().getSize() / 2.f + _btn_text.getLocalBounds().getPosition());

    _btn_text.setPosition(W_WIDTH / 2, W_HEIGHT - 70);
    _btn_text.setOutlineThickness(2);
    _btn_text.setOutlineColor(sf::Color::Black);

    _btn_rect = sf::RectangleShape(sf::Vector2f(150, 40));
    _btn_rect.setOrigin(75, 20);
    _btn_rect.setPosition(_btn_text.getPosition());
    _btn_rect.setFillColor(sf::Color::White);
    _btn_rect.setOutlineColor(sf::Color::Black);
    _btn_rect.setOutlineThickness(4);

    _btn = Button(true, true, _btn_text, _btn_rect);

    _input.init(texture_manager);

    /* dla wstepnego okna */
    set_intro();

    /* dla okna symulacji */
    set_main_stage();

    /* dla wynikow */
    set_results();

    (*stage_number) = 0;
}

void StageManager::intro(sf::RenderWindow &window) {
    window.draw(_title);
    window.draw(_subtitle);

    _btn.Hover(window);

    if (_input.ready){
        _btn.changeColor(sf::Color::Green);
        _btn.changeText("Zaczynamy!");
    }
    else {
        _btn.changeColor(sf::Color::Red);
        _btn.changeText("!Zaczynamy");
    }

    _btn.Hover(window);

    _btn.Render(window);
    _input.render(window, *dt);
}

void StageManager::main_stage(sf::RenderWindow &window) {
    // indexChange to zmienna ktora zbiera roznice miedzy czasem rzeczywistym a stanem gry
    // pozwala "znormalizowac" szybkosc dzialania na komputerach na ktorych wlaczany jest ten program
    // indexChange zbiera sume interwalow czasowych, gdy przekroczy pewna ilosc graniczna (SIM_SPEED_LOC)
    // to animacja moze byc kontynuowana
    indexChange += (*dt);

    if (finito && indexChange >= SIM_SPEED_LOC) {

        // q2 wierzcholek stanu koncowego
        if (!current_node->is_ending()) {
            printf("fail: finished in %i node\n", current_node->getPos());
        }
        *stage_number = 2;
        update_results(current_node->getPos() == 2);
    }

    if (indexChange >= SIM_SPEED_LOC && !finito) {
        // ding ding ding - czas minal, pora dzialac :)


        if (input_index == input.size()) {
            finito = true;
            current_pass->toggle_focused();

            if (!current_node->is_focused())
                current_node->toggle_focused();
        }

        else if (current_pass == nullptr) {

            // zupdatuj gdzie jest sprawdzanie - niebieski element ciagu
            if (input_index > 0) {
                std::string checked_str = _checked_input.getString();

                checked_str[input_index_show] = ' ';
                input_index_show++;
                if (checked_str[input_index_show] == '\n')
                    ++input_index_show;
                checked_str[input_index_show] = input[input_index];

                _checked_input.setString(checked_str);
            }

            // sprawdze czy istenieje sciezka z obecnego wierzcholka o wartosci z inputu
            for (int i = 0; (i < 8); ++i) {
                if (pass_s[i].get_from_pos() == current_node->getPos()
                    && pass_s[i].get_value() == (input[input_index] - '0')) {

                    // wylaczam uwage z noda...
                    current_node->toggle_focused();

                    // ... i przekladam ja na sciezke
                    current_pass = &pass_s[i];
                    current_pass->toggle_focused();

                    // dla kolejnego sprawdzenia ustawiam kolejny node
                    current_node = &nodes[pass_s[i].get_to_pos()];

                    break;
                }
            }

            // zupdatuj sprawdzone elementy z ciagu - wlasnie sprawdzany jest niebieski
            ++input_index;

        }

        else if (current_pass != nullptr) {
            current_pass->toggle_focused();

            current_pass = nullptr;
            if (!current_node->is_focused())
            current_node->toggle_focused();

        }

        indexChange = 0;
    }

    // wyswietlam wierzcholki

    for (int i = 0; i < 4; ++i)
        nodes[i].draw(window);

    // wyswietlam sciezki

    for (int i = 0; i < 8; ++i)
        pass_s[i].draw(window);

    // wyswietlam tresci progresowe

    window.draw(_whole_input);
    window.draw(_checked_input);

    _skip_btn.Hover(window);
    _skip_btn.Render(window);
}

void StageManager::set_main_stage() {
    for (int i = 0; i < 4; ++i) {
        // initializacja stanow dla wierzcholkow
        nodes[i].init(i, texture_manager);
        nodes[i].init_text("q" + std::to_string(i));
    }

    /* wartosci wziete z polecenia - schemat przejsc ale inicj. dla graficznej reprezentacji */

    // od q0
    pass_s[0].init(1, nodes[0], nodes[1], texture_manager);
    pass_s[0].init_look(1);
    pass_s[1].init(0, nodes[0], nodes[2], texture_manager);
    pass_s[1].init_look(0);

    // od q1
    pass_s[2].init(0, nodes[1], nodes[0], texture_manager);
    pass_s[2].init_look(1);
    pass_s[3].init(1, nodes[1], nodes[3], texture_manager);
    pass_s[3].init_look(0);

    // od q2
    pass_s[4].init(0, nodes[2], nodes[3], texture_manager);
    pass_s[4].init_look(1);
    pass_s[5].init(1,nodes[2], nodes[0], texture_manager);
    pass_s[5].init_look(0);

    // od q3
    pass_s[6].init(1, nodes[3], nodes[2], texture_manager);
    pass_s[6].init_look(1);
    pass_s[7].init(0, nodes[3], nodes[1], texture_manager);
    pass_s[7].init_look(0);

    // q0=q3 jest wierzcholkiem startujacym
    current_node = &nodes[3];
    current_node->make_starting();
    nodes[3].init(3, texture_manager);
    current_node->toggle_focused();

    // F=q2 jest wierzcholkiem konczacym
    nodes[2].make_ending();
    nodes[2].init(2, texture_manager);

    current_pass = nullptr;

    input_index = 0;
    input_index_show = 0;

    indexChange = 0;
    finito = false;

    // teksty progresowe

    _whole_input;
    _whole_input.setFont(*texture_manager->get_font());
    _whole_input.setPosition(50, W_HEIGHT - 100);
    _whole_input.setCharacterSize(25);
    _whole_input.setFillColor(sf::Color::Black);

    _checked_input;
    _checked_input.setFont(*texture_manager->get_font());
    _checked_input.setPosition(_whole_input.getPosition());
    _checked_input.setCharacterSize(_whole_input.getCharacterSize());
    _checked_input.setFillColor(sf::Color::Blue);


    // przycisk skipu
    _skip_btn_text.setFont(*texture_manager->get_font());
    _skip_btn_text.setString("Skip?");
    _skip_btn_text.setCharacterSize(N_TEXT_FONT_SIZE / 4);
    _skip_btn_text.setFillColor(sf::Color::White);
    _skip_btn_text.setOrigin(_skip_btn_text.getGlobalBounds().getSize() / 2.f + _skip_btn_text.getLocalBounds().getPosition());

    _skip_btn_text.setPosition(W_WIDTH - 90, 40);
    _skip_btn_text.setOutlineThickness(2);
    _skip_btn_text.setOutlineColor(sf::Color::Black);

    _skip_btn_rect = sf::RectangleShape(sf::Vector2f(120, 40));
    _skip_btn_rect.setOrigin(60, 20);
    _skip_btn_rect.setPosition(_skip_btn_text.getPosition());
    _skip_btn_rect.setFillColor(sf::Color::Red);
    _skip_btn_rect.setOutlineColor(sf::Color::Black);
    _skip_btn_rect.setOutlineThickness(4);

    _skip_btn = Button(true, true, _skip_btn_text, _skip_btn_rect);
}

void StageManager::set_intro() {

    /* tytul: napis `automat skonczony` */
    _title.setFont(*texture_manager->get_font());
    _title.setString("Automat Skonczony");

    _title.setCharacterSize(N_TEXT_FONT_SIZE);
    _title.setOrigin(_title.getGlobalBounds().getSize() / 2.f + _title.getLocalBounds().getPosition());
    _title.setPosition(W_WIDTH / 2, 90);

    _title.setFillColor(sf::Color::White);

    _title.setOutlineThickness(4);
    _title.setOutlineColor(sf::Color::Black);

    /* subtytul: napis `(nareszcie)` */
    _subtitle.setFont(*texture_manager->get_font());
    _subtitle.setString("(nareszcie)");

    _subtitle.setCharacterSize(N_TEXT_FONT_SIZE / 3);
    _subtitle.setOrigin(_subtitle.getGlobalBounds().getSize() / 2.f + _subtitle.getLocalBounds().getPosition());
    _subtitle.setPosition(W_WIDTH / 2, 140);

    _subtitle.setFillColor(sf::Color::White);

    _subtitle.setOutlineThickness(2);
    _subtitle.setOutlineColor(sf::Color::Black);

}

void StageManager::handle_press() {
    input = _input.get_input();

    std::string inp_copy = "";
    for (int i = 0; i < input.size(); ++i) {
        if (i == 63)
            inp_copy += "\n";
        inp_copy += input[i];
    }

    // ustawienie przelicznika inputa - calosc ciagu i kolorowy, niebieski element sprawdzany
    _whole_input.setString(inp_copy);

    _whole_input.setOrigin(_whole_input.getGlobalBounds().getSize() / 2.f + _whole_input.getLocalBounds().getPosition());
    _whole_input.setPosition(W_WIDTH / 2, W_HEIGHT - 70);

    for (int i = 0; i < inp_copy.size(); ++i) {
        if (inp_copy[i] != '\n' && i != 0)
            inp_copy[i] = ' ';
    }

    _checked_input.setString(inp_copy);

    _checked_input.setOrigin(_checked_input.getGlobalBounds().getSize() / 2.f + _checked_input.getLocalBounds().getPosition());
    _checked_input.setPosition(W_WIDTH / 2, W_HEIGHT - 70);

    ++(*stage_number);
}

void StageManager::handle_key_press() {
    if (*stage_number != 0)
        return;

    _input.handle_text();
    _input.handle_errors();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && _input.ready) {
        handle_press();
    }
}

// zakladam ze to LPM //
void StageManager::handle_lpm_press(sf::RenderWindow &window) {
    if ((*stage_number) == 0 && _input.ready && _btn.Hover(window)) {
        handle_press();
    }
    else if ((*stage_number) == 1 && _skip_btn.Hover(window)) {
        SIM_SPEED_LOC = 0;
    }
    else if ((*stage_number) == 2 && _restart_btn.Hover(window)) {
        (*stage_number) = 0;
        setup();
    }
}

void StageManager::results(sf::RenderWindow &window) {
    window.draw(_result_text);
    window.draw(_visual_reaction);

    _restart_btn.Hover(window);
    _restart_btn.Render(window);
}

void StageManager::update_results(bool good) { // bool bad - nie znecaj sie!

    int randomed_reaction_pos = 100 * (rand() % 4);

    if (good) {
        _result_text.setString("Hurra, ciag\nzaakceptowany!");
        _result_text.setFillColor(sf::Color::Green);
        _visual_reaction.setTextureRect(sf::IntRect(randomed_reaction_pos, 0, 100, 100));
    } else {
        _result_text.setString("Niestety, ciag\nNIEzaakceptowany!");
        _result_text.setFillColor(sf::Color::Red);
        _visual_reaction.setTextureRect(sf::IntRect(randomed_reaction_pos, 100, 100, 100));
    }

    _result_text.setOrigin(_result_text.getGlobalBounds().getSize() / 2.f + _result_text.getLocalBounds().getPosition());
    _result_text.setPosition(W_WIDTH / 2, 100);
}

void StageManager::set_results() {

    // tekst rezultatowy //
    _result_text;
    _result_text.setFont(*texture_manager->get_font());

    _result_text.setCharacterSize(N_TEXT_FONT_SIZE / 1.2);
    _result_text.setFillColor(sf::Color::Red);
    _result_text.setPosition(30, 30);

    // ustawienie odpowiedniej emotki reakcji //
    _visual_reaction = sf::RectangleShape(sf::Vector2f(200, 200));
    _visual_reaction.setOrigin(_visual_reaction.getGlobalBounds().getSize() / 2.f + _visual_reaction.getLocalBounds().getPosition());
    _visual_reaction.setPosition(W_WIDTH / 2, W_HEIGHT / 2);
    _visual_reaction.setTexture(texture_manager->get_texture(1));

    // setup dla przycisku '''RESTART?'''

    _restart_btn_text.setFont(*texture_manager->get_font());
    _restart_btn_text.setString("Restart?");
    _restart_btn_text.setCharacterSize(N_TEXT_FONT_SIZE / 4);
    _restart_btn_text.setFillColor(sf::Color::White);
    _restart_btn_text.setOrigin(_restart_btn_text.getGlobalBounds().getSize() / 2.f + _restart_btn_text.getLocalBounds().getPosition());

    _restart_btn_text.setPosition(W_WIDTH / 2, W_HEIGHT - 70);
    _restart_btn_text.setOutlineThickness(2);
    _restart_btn_text.setOutlineColor(sf::Color::Black);

    _restart_btn_shape = sf::RectangleShape(sf::Vector2f(150, 40));
    _restart_btn_shape.setOrigin(75, 20);
    _restart_btn_shape.setPosition(_btn_text.getPosition());
    _restart_btn_shape.setFillColor(sf::Color::Red);
    _restart_btn_shape.setOutlineColor(sf::Color::Black);
    _restart_btn_shape.setOutlineThickness(4);

    _restart_btn = Button(true, true, _restart_btn_text, _restart_btn_shape);
}