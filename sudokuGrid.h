//Modul:    Programmieren 2 - Praktikum
//Datum:    2024-06-01 - 18:31:48
//Name:     Lars Ebbeke
//Aufgabe:  sudoku_game
//Todo:
//-

#ifndef SUDOKU_GAME_SUDOKUGRID_H
#define SUDOKU_GAME_SUDOKUGRID_H

#endif //SUDOKU_GAME_SUDOKUGRID_H

#include <set>
#include <SFML/graphics.hpp>
#include "buttons.h"

class SudokuField : public button {
private:
    std::vector<int> possibleNumbers;
    bool fixed = false;

public:
    SudokuField() : button(), possibleNumbers() {}

    void setPossibleNumbers(std::vector<int> n_posNum) {
        possibleNumbers = n_posNum;
    }

    std::vector<int> getPossibleNumbers() {
        return possibleNumbers;
    }

    void setFixed(bool fix) {
        fixed = fix;
    }

    bool isFixed() {
        return fixed;
    }
};

class Sudoku{
private:
    SudokuField fields[9][9];
    SudokuField* activeField = nullptr;

public:
    Sudoku(int numbers[9][9]) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                fields[i][j].setText(std::to_string(numbers[i][j])); // Set the number for each field
                fields[i][j].setColour(sf::Color::Transparent);
                fields[i][j].setTextColour(sf::Color(0xffffffff));
            }
        }
    }

    bool solve(sf::RenderWindow& window);

    void handleInput(sf::Event& event, sf::RenderWindow& window);

    void drawGrid(sf::RenderWindow& window);
    void drawNumbers(sf::RenderWindow& window);

    void saveBoard();
    void loadBoard();

    bool isValid(int row, int col, const std::string& value);

    void reset() {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                fields[i][j].setText("");
                fields[i][j].setFixed(false);
            }
        }
    }
};
