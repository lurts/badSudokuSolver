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
                // Set its color to black
                fields[i][j].setColour(sf::Color::Black);
            }

            if (fields[i][j].getText() == "0") {
                fields[i][j].setText("");
            }

            fields[i][j].paint(window);
        }
    }

    for (int i = 1; i < 9; ++i) {
        sf::VertexArray line(sf::LineStrip, 2);
        line[0].position = sf::Vector2f(spacing * i, 0);
        line[1].position = sf::Vector2f(spacing * i, window.getSize().y);

        // Print the coordinates of the line
        //std::cout << "Line " << i << ": (" << line[0].position.x << ", " << line[0].position.y << ") to (" << line[1].position.x << ", " << line[1].position.y << ")" << std::endl;

        if (i%3 == 0) {
            line[0].color = sf::Color::Red;
            line[1].color = sf::Color::Red;
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
        }

        window.draw(line);
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
                    updatePossibleNumbers(i, j);
                    return;
                }
            }
        }
    } else if (event.type == sf::Event::TextEntered && activeField != nullptr) {
        if (event.text.unicode >= '0' && event.text.unicode <= '9') {
            // Update the text of the active field with the typed number
            activeField->setText(std::to_string(event.text.unicode - '0'));
        } else if (event.text.unicode == 13) { // Enter key pressed
            // Finish input, deactivate the field
            activeField = nullptr;
        }
    }
}


void Sudoku::updatePossibleNumbers(int x, int y) {
    if (fields[x][y].getText() != "") {
        fields[x][y].setPossibleNumbers(std::vector<int>({std::stoi(fields[x][y].getText())}));
        return;
    }


    // Reset possible numbers to 1-9
    std::vector<int> possibleNumbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Cross off numbers from the same 3x3 chunk
    int chunk_x = x / 3;
    int chunk_y = y / 3;
    int startX = chunk_x * 3;
    int startY = chunk_y * 3;

    for (int i = startX; i < startX + 3; ++i) {
        for (int j = startY; j < startY + 3; ++j) {
            if (fields[i][j].getText() != "") {
                int number = std::stoi(fields[i][j].getText());
                possibleNumbers.erase(std::remove(possibleNumbers.begin(), possibleNumbers.end(), number),possibleNumbers.end());
            }
        }
    }

    // Cross off numbers from the same row
    for (int j = 0; j < 9; ++j) {
        if (fields[x][j].getText() != "") {
            int number = std::stoi(fields[x][j].getText());
            possibleNumbers.erase(std::remove(possibleNumbers.begin(), possibleNumbers.end(), number),possibleNumbers.end());
        }
    }

    // Cross off numbers from the same column
    for (int i = 0; i < 9; ++i) {
        if (fields[i][y].getText() != "") {
            int number = std::stoi(fields[i][y].getText());
            possibleNumbers.erase(std::remove(possibleNumbers.begin(), possibleNumbers.end(), number),possibleNumbers.end());
        }
    }

    fields[x][y].setPossibleNumbers(possibleNumbers);


    std::cout << "[";
    for (size_t i = 0; i < possibleNumbers.size(); ++i) {
        std::cout << possibleNumbers[i];
        if (i < possibleNumbers.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

bool Sudoku::solve() {
    while (true) {
        bool progress = false;

        // Iterate through each cell
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                // If the cell is empty
                if (fields[i][j].getText().empty()) {
                    // Update possible numbers for the cell
                    updatePossibleNumbers(i, j);

                    // If only one possible number, fill in the cell
                    if (fields[i][j].getPossibleNumbers().size() == 1) {
                        fields[i][j].setText(std::to_string(fields[i][j].getPossibleNumbers()[0]));
                        progress = true;
                    }
                }
            }
        }

        // If no progress made in this iteration, break the loop
        if (!progress) {
            break;
        }
    }

    // Check if the puzzle is solved
    // If any cell is empty, the puzzle is not solved
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (fields[i][j].getText().empty()) {
                return false;
            }
        }
    }

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
            data[i][j] = fields[i][j].getText();
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
            std::string fieldValue = data[i][j].get<std::string>(); // Get the field value from JSON
            fields[i][j].setText(fieldValue); // Update the corresponding SudokuField object
        }
    }

    std::cout << "Board loaded successfully." << std::endl;
}