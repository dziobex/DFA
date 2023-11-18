//
// Created by jedno on 08.11.2023.
//

#include "TextureManager.h"

TextureManager::TextureManager() {
    font = nullptr;

    printf("LOG: Stworzono obiekt texture manager\n");
}

TextureManager::~TextureManager() {
    printf("LOG: Zabito obiekt texture manager\n");

    for (auto t : textures)
        delete t;

    delete font;
}

void TextureManager::load_font(const void *data, std::size_t len) {
    font = new sf::Font;

    if (!(*font).loadFromMemory(data, len)) {
        std::cout << "Nie znaleziono czcionki.\n";
        return;
    }
}

sf::Font *TextureManager::get_font() {
    if (font == NULL) {
        std::cout << "Nie udalo sie zaladowac czcionki.";
        return nullptr;
    }
    return font;
}

void TextureManager::load_texture(const void *data, std::size_t len) {

    sf::Texture *newTxtr = new sf::Texture;

    if (!(*newTxtr).loadFromMemory(data, len)) {
        std::cout << "Nie znaleziono textury.\n";
        return;
    }

    textures.push_back(newTxtr);
}


sf::Texture *TextureManager::get_texture(int key) {

    if (key < 0 || key >= textures.size() ) {
        std::cout << "Nie udalo sie zaladowac texturys.";
        return nullptr;
    }

    return textures[key];
}
