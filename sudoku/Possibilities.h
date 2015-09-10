#ifndef SUDOKU_POSSIBILITIES_H
#define SUDOKU_POSSIBILITIES_H

#include <set>
#include <list>
#include "Board.h"

struct Possibility
{
    uchar x, y, val;
};

class Possibilities
{
public:
    Possibilities( const Board& b );
    Possibilities( const Possibilities& p );

    std::list<Possibility> get_one_square();
    void compute();
    void dump() const;

private:
    void compute( uchar x, uchar y );

    const std::set<uchar>& get( uchar x, uchar y ) const;
          std::set<uchar>& get( uchar x, uchar y );

    void add( uchar x, uchar y, uchar val );
    void del( uchar x, uchar y, uchar val );

    void fill();
    void fill( uchar x, uchar y );

    void clear();
    void clear( uchar x, uchar y );

private:
    std::set<uchar> choices[9*9];
    const Board *m_board;
};

#endif
