//Modul:    Programmieren 2 - Praktikum
//Datum:    2024-06-08 - 21:22:12
//Name:     Lars Ebbeke
//Aufgabe:  sudoku_game
//Todo:
//-

#ifndef SUDOKU_GAME_BUTTONS_H
#define SUDOKU_GAME_BUTTONS_H

#endif //SUDOKU_GAME_BUTTONS_H

#include <SFML/Graphics.hpp>

class button {
private:
    sf::Vector2f bPos;
    sf::Vector2f bSize;
    sf::Color bColour;
    sf::Color textColour = sf::Color::White;
    std::string dispText;
public:
    button() : bPos(0, 0), bSize(0, 0), bColour(sf::Color::White), dispText("") {}

    button(sf::Vector2f size, sf::Vector2f position = sf::Vector2f(0, 0), sf::Color n_colour = sf::Color(0xFFFFFFFF), std::string n_dispText = "")
            :bSize(size), bPos(position), bColour(n_colour), dispText(n_dispText){};

    void setPos(sf::Vector2f n_pos) {
        bPos = n_pos;
    }

    void setSize(sf::Vector2f n_size) {
        bSize = n_size;
    }

    void setColour(sf::Color n_colour) {
        bColour = n_colour;
    }

    void setText(std::string n_dispText) {
        dispText = n_dispText;
    }

    void setTextColour(sf::Color nCol) {
        textColour = nCol;
    }

    std::string getText() {
        return dispText;
    }

    bool isMe(sf::Vector2i position) {
        return ((position.x > bPos.x && position.x < bPos.x + bSize.x) &&
                (position.y > bPos.y && position.y < bPos.y + bSize.y));
    }

    void paint(sf::RenderWindow& window) {

        sf::Font font;
        #ifdef _WIN32
        if (!font.loadFromFile("C:/Windows/Fonts/Arial.ttf")) {
        #else
            if (!font.loadFromFile("LiberationSans-Regular.ttf")) {
        #endif
            // Handle font loading error
            return;
        }

        sf::RectangleShape rect(bSize);
        rect.setPosition(bPos);
        rect.setFillColor(bColour);
        window.draw(rect);

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(24); // Adjust the font size as needed
        text.setFillColor(textColour);
        text.setString(dispText);

        // Calculate the position to center the text within the button
        sf::FloatRect textBounds = text.getLocalBounds();
        float xOffset = (bSize.x - textBounds.width) / 2;
        float yOffset = (bSize.y - textBounds.height) / 2;
        sf::Vector2f textPosition = bPos + sf::Vector2f(xOffset, yOffset);
        text.setPosition(textPosition);

        window.draw(text);
    }
};