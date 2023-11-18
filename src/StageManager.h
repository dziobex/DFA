//
// Created by jedno on 15.11.2023.
//

#ifndef AUTOMAT_STAGEMANAGER_H
#define AUTOMAT_STAGEMANAGER_H

#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "TextureManager.h"
#include "Node.h"
#include "Pass.h"
#include "UI.h"

class StageManager {
    TextureManager *texture_manager;
    float *dt;
    int *stage_number;

    float SIM_SPEED_LOC;

    /* zmienne dla intro */
    sf::Text _title, _subtitle;
    std::string input;

    int input_index;
    int input_index_show;

    Button _btn;
    sf::Text _btn_text;
    sf::RectangleShape _btn_rect;

    Input _input;

    /* zmienne dla main_stage */
    float indexChange = 0;
    bool finito = false;

    sf::Text _checked_input;
    sf::Text _whole_input;

    Node nodes[4];
    Pass pass_s[8];

    Node *current_node;
    Pass *current_pass;

    Button _skip_btn;
    sf::Text _skip_btn_text;
    sf::RectangleShape _skip_btn_rect;

    /* ZMIENNE DLA FAILOW/AKCEPTOW */
    sf::Text _result_text;
    sf::RectangleShape _visual_reaction;

    Button _restart_btn;
    sf::Text _restart_btn_text;
    sf::RectangleShape _restart_btn_shape;

    void set_intro();
    void set_main_stage();

    void set_results();

    void update_results(bool good);

    void handle_press();

public:
    StageManager();
    ~StageManager();

    void init(float *deltaTime, TextureManager *tm, int *stage_number);

    void setup();

    /* wyswietlane okna */
    void intro(sf::RenderWindow &window);
    void main_stage(sf::RenderWindow &window);

    void results(sf::RenderWindow &window);

    void handle_key_press();
    void handle_lpm_press(sf::RenderWindow &window);

};


#endif //AUTOMAT_STAGEMANAGER_H
