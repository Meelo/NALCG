#ifndef _NALCG_BOARD_H_
#define _NALCG_BOARD_H_

// system includes
#include <iostream>

// class includes

class Board
{
    // Constants

    // Members
    std::size_t width;
    std::size_t height;
    
    bool initialized;

    // Methods

public:
    Board(std::size_t width, std::size_t height);
    virtual ~Board() { }

    // Methods

    // Getters
    virtual std::size_t getWidth() const { return width; }
    virtual std::size_t getHeight() const { return height; }
    virtual std::size_t getTotalSize() const { return 64; }
    
    // Setters
};

#endif // _NALCG_BOARD_H_
