#ifndef _NALCG_BOARD_H_
#define _NALCG_BOARD_H_

// system includes
#include <iostream>
#include <vector>

// class includes
#include "square.h"

class Board
{
    // Constants

    // Members
    std::size_t width;
    std::size_t height;

    bool initialized;

    // Methods

public:
    Board(std::size_t width = 8, std::size_t height = 8);
    virtual ~Board() { }

    // Methods
    static std::vector<Square> createBoard(std::size_t width = 8, std::size_t height = 8);

    // Getters
    std::size_t getWidth() const { return width; }
    std::size_t getHeight() const { return height; }
    std::size_t getTotalSize() const { return 64; }

    // Setters
};

#endif // _NALCG_BOARD_H_
