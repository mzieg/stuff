#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <string>

typedef unsigned char uchar;

class Board
{
public:
    Board( const std::string &filename );
    Board( const Board& b );

    const Board* solve() const;
    void  dump() const;

    uchar get( uchar x, uchar y ) const;

private:
    void set( uchar x, uchar y, uchar val );
    bool is_valid()  const;
    bool is_solved() const;

private: 
    static unsigned long iterations;
    uchar _board[9*9];
};

#endif
