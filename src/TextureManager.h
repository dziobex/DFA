//
// Created by jedno on 08.11.2023.
//

#ifndef CMAKESFMLPROJECT_TEXTUREMANAGER_H
#define CMAKESFMLPROJECT_TEXTUREMANAGER_H

#include <iostream>
#include <string>
#include <vector>

#include "SFML/Graphics.hpp"

// jedna instancja na caly program

class TextureManager {
    sf::Font *font;
    std::vector <sf::Texture*> textures;

public:
    TextureManager();
    ~TextureManager();

    void load_font(const void *data, std::size_t len);
    void load_texture(const void *data, std::size_t len);

    sf::Font *get_font();
    sf::Texture *get_texture(int key);
};


#endif //CMAKESFMLPROJECT_TEXTUREMANAGER_H
