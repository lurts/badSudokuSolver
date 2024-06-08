//Modul:    Programmieren 2 - Praktikum
//Datum:    2024-06-01 - 18:31:25
//Name:     Lars Ebbeke
//Aufgabe:  sudoku-game
//Todo:
//-

#include <iostream>
#include <SFML/graphics.hpp>
#include "sudokuGrid.h"

const int windowSize = 750;

//int numbers[9][9] = { {0, 4, 0, 9, 0, 0, 3, 0, 8},
//                      {2, 0, 8, 0, 0, 4, 0, 6, 1},
//                      {6, 0, 8, 0, 0, 0, 0, 2, 0},
//                      {0, 0, 1, 0, 4, 0, 0, 0, 7},
//                      {7, 6, 0, 0, 3, 1, 9, 8, 0},
//                      {0, 0, 9, 8, 2, 0, 0, 1, 0},
//                      {0, 2, 0, 0, 0, 3, 0, 0, 0},
//                      {9, 3, 6, 2, 1, 0, 7, 4, 0},
//                      {0, 0, 7, 0, 9, 6, 8, 3, 0},
//                      };

int numbers[9][9] = { {0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0},
                      };

int main() {
    Sudoku sd = Sudoku(numbers);

    int framerateLimit          = 60;
    sf::RenderWindow window(sf::VideoMode(windowSize+250, windowSize), "SUDOKU", sf::Style::Close);
    window.setFramerateLimit(framerateLimit);

    sf::Vector2f bSize(window.getSize().x - window.getSize().y , 100);

    //Drawing the buttons
    button solveButton(bSize,
                       sf::Vector2f(window.getSize().y, 0),
                       sf::Color::Blue,
                       "Solve");

    button saveButton(bSize,
                       sf::Vector2f(window.getSize().y, window.getSize().y - bSize.y * 2),
                       sf::Color::Red,
                       "Save");

    button loadButton(bSize,
                      sf::Vector2f(window.getSize().y, window.getSize().y - bSize.y),
                      sf::Color::Green,
                      "Load");

    button resetButton(bSize,
                       sf::Vector2f(window.getSize().y, bSize.y),
                       sf::Color::Red,
                       "RESET");



    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (solveButton.isMe(mousePos)) {
                    if (sd.solve())
                        std::cout << "Solved" << std::endl;
                    else
                        std::cout << "Not Solved" << std::endl;
                }

                if (saveButton.isMe(mousePos)) {
                    sd.saveBoard();
                }

                if (loadButton.isMe(mousePos)) {
                    sd.loadBoard();
                }

                if (resetButton.isMe(mousePos)) {
                    sd.reset();
                }
            }

            sd.handleInput(event, window);

        }

        solveButton.paint(window);
        saveButton.paint(window);
        loadButton.paint(window);
        resetButton.paint(window);

        sd.drawGrid(window);

        window.display();
    }
    return 0;
}
