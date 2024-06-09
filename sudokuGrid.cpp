//Modul:    Programmieren 2 - Praktikum
//Datum:    2024-06-01 - 18:31:48
//Name:     Lars Ebbeke
//Aufgabe:  sudoku_game
//Todo:
//-

#include "sudokuGrid.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void Sudoku::drawGrid(sf::RenderWindow &window) {
    int spacing = window.getSize().y / 9;


    for (int i = 1; i < 9; ++i) {
        sf::VertexArray line(sf::LineStrip, 2);
        line[0].position = sf::Vector2f(spacing * i, 0);
        line[1].position = sf::Vector2f(spacing * i, window.getSize().y);

        // Print the coordinates of the line
        //std::cout << "Line " << i << ": (" << line[0].position.x << ", " << line[0].position.y << ") to (" << line[1].position.x << ", " << line[1].position.y << ")" << std::endl;

        if (i%3 == 0) {
            line[0].color = sf::Color::Red;
            line[1].color = sf::Color::Red;
        } else {
            line[0].color = sf::Color::White;
            line[1].color = sf::Color::White;
        }


        window.draw(line);
    }

    //std::cout << "Second set" << std::endl;
    for (int i = 1; i < 9; ++i) {
        sf::VertexArray line(sf::LineStrip, 2);
        line[0].position = sf::Vector2f(0, spacing * i);
        line[1].position = sf::Vector2f(window.getSize().y, spacing * i);

        // Print the coordinates of the line
        //std::cout << "Line " << i << ": (" << line[0].position.x << ", " << line[0].position.y << ") to (" << line[1].position.x << ", " << line[1].position.y << ")" << std::endl;

        if (i%3 == 0) {
            line[0].color = sf::Color::Red;
            line[1].color = sf::Color::Red;
        } else {
            line[0].color = sf::Color::White;
            line[1].color = sf::Color::White;
        }

        window.draw(line);
    }
}

void Sudoku::drawNumbers(sf::RenderWindow &window) {
    int spacing = window.getSize().y / 9;

    //drawing all the numbers
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            sf::Vector2f fieldPos   (spacing * i, spacing * j);
            sf::Vector2f fieldSize  (spacing, spacing);
            fields[i][j].setSize(fieldSize);
            fields[i][j].setPos(fieldPos);

            // Check if the current field is the active field
            if (&fields[i][j] == activeField) {
                // Set its color to green
                fields[i][j].setColour(sf::Color::Green);
            } else {
                    // Set its color to transparent
                    fields[i][j].setColour(sf::Color::Transparent);
            }

            if (fields[i][j].isFixed()) {
                fields[i][j].setColour(sf::Color(0x325aafff));
            } else {
                fields[i][j].setColour(sf::Color(0x000000ff));
            }

            if (fields[i][j].getText() == "0" || fields[i][j].getText() == "") {
                fields[i][j].setText("");
                fields[i][j].setFixed(false);
            }

            fields[i][j].paint(window);
        }
    }
}

void Sudoku::handleInput(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        // Check if mouse click occurred within the boundaries of any Sudoku field button
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (fields[i][j].isMe(mousePos)) {
                    // Activate the field for input
                    activeField = &fields[i][j];
                    return;
                }
            }
        }
    } else if (event.type == sf::Event::TextEntered && activeField != nullptr) {
        if (event.text.unicode >= '0' && event.text.unicode <= '9') {
            // Update the text of the active field with the typed number
            activeField->setText(std::to_string(event.text.unicode - '0'));
            activeField->setFixed(true);
        } else if (event.text.unicode == 13) { // Enter key pressed
            // Finish input, deactivate the field
            activeField = nullptr;
        }
    }
}


bool Sudoku::isValid(int row, int col, const std::string& value) {
    // Check the row
    for (int j = 0; j < 9; ++j) {
        if (fields[row][j].getText() == value) return false;
    }
    // Check the column
    for (int i = 0; i < 9; ++i) {
        if (fields[i][col].getText() == value) return false;
    }
    // Check the 3x3 grid
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = startRow; i < startRow + 3; ++i) {
        for (int j = startCol; j < startCol + 3; ++j) {
            if (fields[i][j].getText() == value) return false;
        }
    }
    return true;
}

bool Sudoku::solve(sf::RenderWindow& window) {
    // Step 1: Find the first empty cell
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (fields[i][j].getText() == "") { // Empty cell found
                // Step 2: Try each number from 1 to 9
                for (int num = 1; num <= 9; ++num) {
                    std::string numStr = std::to_string(num);
                    // Step 3: Check validity of the number
                    if (isValid(i, j, numStr)) {
                        // Valid number, place it in the cell
                        fields[i][j].setText(numStr);

                        //fields[i][j].paint(window);
                        //window.display();

                        // Step 4: Recursively attempt to solve the rest of the grid
                        if (solve(window)) {
                            return true; // Solved
                        }
                        // Step 5: If not solved, backtrack by resetting the cell
                        fields[i][j].setText("");

                        //fields[i][j].setColour(sf::Color::Black);
                        //fields[i][j].paint(window);
                        //drawGrid(window);
                        //window.display();

                    }
                }


                sf::Event event;

                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }

                // If no valid number found, return false to trigger backtracking
                return false;
            }
        }
    }
    // Step 6: If no empty cell is found, the puzzle is solved
    return true;
}

std::string openFileDialog() {
    OPENFILENAME ofn;
    TCHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) == TRUE) {
        return ofn.lpstrFile;
    } else {
        return "";
    }
}

std::string saveFileDialog() {
    OPENFILENAME ofn;
    TCHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn) == TRUE) {
        return ofn.lpstrFile;
    } else {
        return "";
    }
}

void Sudoku::saveBoard() {
    // Open file dialog to select file path for saving
    std::string filename = saveFileDialog();

    // Open the file for writing
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    json data;

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            data["numbers"][i][j] = fields[i][j].getText();
            data["fixed"][i][j] = fields[i][j].isFixed();
        }
    }

    file << std::setw(4) << data << std::endl;

    std::cout << "Board saved successfully." << std::endl;
}

void Sudoku::loadBoard() {
    // Open file dialog to select file path for loading
    std::string filename = openFileDialog();

    // Open the file for reading
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file for reading." << std::endl;
        return;
    }

    json data = json::parse(file);

    // Update the Sudoku board with the loaded data
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::string fieldValue = data["numbers"][i][j].get<std::string>(); // Get the field value from JSON
            fields[i][j].setText(fieldValue); // Update the corresponding SudokuField object

            fields[i][j].setFixed(data["fixed"][i][j].get<bool>());
        }
    }

    std::cout << "Board loaded successfully." << std::endl;
}