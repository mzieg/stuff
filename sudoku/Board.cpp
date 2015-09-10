#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <set>
#include "Board.h"
#include "Possibilities.h"

using std::string;
using std::list;

unsigned long Board::iterations = 0;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Ctors & Dtors                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Board::Board( const string &filename ) 
{
    memset( _board, 0, sizeof( _board ) );
    FILE *f;
    if( ( f = fopen( filename.c_str(), "r" ) ) != NULL )
    {
        uchar count = 0;
        while( count < 9 * 9 && !feof( f ) )
        {
            char c;
            fscanf( f, "%c", &c );
            if( !isspace( c ) )
            {
                uchar x = count % 9;
                uchar y = count / 9;
                uchar n = 0;
                if( '1' <= c && c <= '9' )
                    n = c - '0';
                set( x, y, n );
                count++;
            }
        }
    }
    else
    {
        fprintf( stderr, "fatal: can't read %s\n", filename.c_str() );
        exit( 1 );
    }
}

Board::Board( const Board& b ) 
{
    memcpy( _board, b._board, sizeof( _board ) );
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               Accessors                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Board::set( uchar x, uchar y, uchar val )
{
    _board[y*9 + x] = val;
}

uchar Board::get( uchar x, uchar y ) const
{
    return _board[y*9 + x];
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                Gameplay                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/*! @brief  recursively attempt to solve the board via brute-force permutations
    @return pointer to solved board on success; NULL on failure
*/
const Board* Board::solve() const
{
    const Board* retval = NULL;

    // is the board solved?
    if( is_solved() )
    {
        // return a pointer to this board (the final solution)
        retval = new Board(*this);
    }
    else if( is_valid() )
    {
        // take initial set of possibilities
        Possibilities board_possibilities( *this );

        // for one unsolved square, get the list of all possibilities
        list<Possibility> square_possibilities = board_possibilities.get_one_square();
        for( list<Possibility>::const_iterator i = square_possibilities.begin();
             i != square_possibilities.end(); i++ )
        {
            const Possibility& p = *i;
            printf( "\rIteration: %5lu", ++iterations );

            // make a deep copy of the current board
            Board b1( *this );
            
            // apply the possibility to the new board
            b1.set( p.x, p.y, p.val );

            // solve the new board
            if( retval = b1.solve() )
                break;
        }
    }
    return retval;
}

bool Board::is_solved() const
{
    for( uchar x = 0; x < 9; x++ )
        for( uchar y = 0; y < 9; y++ )
            if( !get( x, y ) )
                return false;
    return true;
}

bool Board::is_valid() const
{
    std::set<uchar> seen;

    // check rows
    for( uchar y = 0; y < 9; seen.clear(), y++ )
        for( uchar x = 0; x < 9; x++ )
            if( uchar val = get( x, y ) )
                if( seen.count( val ) )
                    return false;
                else
                    seen.insert( val );

    // check cols
    for( uchar x = 0; x < 9; seen.clear(), x++ )
        for( uchar y = 0; y < 9; y++ )
            if( uchar val = get( x, y ) )
                if( seen.count( val ) )
                    return false;
                else
                    seen.insert( val );

    // check quadrants
    for( uchar qx = 0; qx < 3; qx++ )
        for( uchar qy = 0; qy < 3; seen.clear(), qy++ )
            for( uchar xi = 0; xi < 3; xi++ )
                for( uchar yi = 0; yi < 3; yi++ )
                    if( uchar val = get( qx*3 + xi, qy*3 + yi ) )
                        if( seen.count( val ) )
                            return false;
                        else
                            seen.insert( val );
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                Utility                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Board::dump() const
{
    string row( "+-----------+-----------+-----------+\n" );
    printf( row.c_str() );
    for( uchar y = 0; y < 9; y++ )
    {
        printf( "| " );
        for( uchar x = 0; x < 9; x++ )
        {
            uchar n = get( x, y );
            char c = '0';
            c += n;
            printf( "%c ", n > 0 ? c : '.' );
            printf( "%c ", ( x + 1 ) % 3 ? ':' : '|' );
        }
        printf( "\n" );
        if( !( ( y + 1 ) % 3 ) )
            printf( row.c_str() );
    }
}
