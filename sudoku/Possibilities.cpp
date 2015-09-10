#include <stdio.h>
#include <string>
#include <set>
#include "Possibilities.h"

using std::set;
using std::list;
using std::string;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             Ctors & Dtors                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Possibilities::Possibilities( const Board& b ) : m_board( &b )
{
    clear();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               Algorithm                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Possibilities::compute()
{
    clear();

    // build the list of possibilities for each square
    for( uchar y = 0; y < 9; y++ )
        for( uchar x = 0; x < 9; x++ )
            compute( x, y );
}

void Possibilities::compute( uchar x, uchar y )
{
    // skip solved squares
    if( !m_board->get( x, y ) )
    {
        // assume the square can accept any value, then prune from there
        fill( x, y );

        // delete possibilities occuring elsewhere in the row
        for( uchar i = 0; i < 9; i++ )
            if( i != x )
                if( uchar n = m_board->get( i, y ) )
                    del( x, y, n );

        // delete possibilities occuring elsewhere in the column
        for( uchar i = 0; i < 9; i++ )
            if( i != y )
                if( uchar n = m_board->get( x, i ) )
                    del( x, y, n );

        // delete possibilities occuring elsewhere in the "quadrant"
        uchar qx = x / 3;
        uchar qy = y / 3;
        for( uchar xi = 0; xi < 3; xi++ )
            for( uchar yi = 0; yi < 3; yi++ )
            {
                uchar xn = qx * 3 + xi;
                uchar yn = qy * 3 + yi;
                if( xn != x || yn != y )
                    if( uchar n = m_board->get( xn, yn ) )
                        del( x, y, n );
            }

    }
}

list<Possibility> Possibilities::get_one_square() 
{
    list<Possibility> l;
    for( uchar y = 0; y < 9; y++ )
        for( uchar x = 0; x < 9; x++ )
            if( !m_board->get( x, y ) )
            {
                compute( x, y );
                const set<uchar>& s = get( x, y );
                for( set<uchar>::const_iterator i = s.begin(); i != s.end(); i++ )
                {
                    Possibility p;
                    p.x = x;
                    p.y = y;
                    p.val = *i;
                    l.push_back( p );
                }
                return l;
            }
    return l;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                               Accessors                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Possibilities::clear()
{
    for( uchar y = 0; y < 9; y++ )
        for( uchar x = 0; x < 9; x++ )
            clear( x, y );
}

void Possibilities::fill()
{
    for( uchar y = 0; y < 9; y++ )
        for( uchar x = 0; x < 9; x++ )
            fill( x, y );
}

void Possibilities::clear( uchar x, uchar y )
{
    set<uchar>& choices = get( x, y );
    choices.clear();
}

void Possibilities::fill( uchar x, uchar y )
{
    for( uchar val = 1; val <= 9; val++ )
        add( x, y, val );
}

const set<uchar>& Possibilities::get( uchar x, uchar y ) const 
{
    return choices[ y * 9 + x ];
}

set<uchar>& Possibilities::get( uchar x, uchar y ) 
{
    return choices[ y * 9 + x ];
}

void Possibilities::add( uchar x, uchar y, uchar val )
{
    choices[ y * 9 + x ].insert( val );
}

void Possibilities::del( uchar x, uchar y, uchar val )
{
    set<uchar>& s = get( x, y );
    s.erase( val );
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                Utility                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Possibilities::dump() const
{
    string row( "+---+---+---+  +---+---+---+  +---+---+---+\n" );
    printf( row.c_str() );
    for( uchar y = 0; y < 9; y++ )
    {
        for( uchar yi = 0; yi < 3; yi++ )
        {
            printf( "|" );
            for( uchar x = 0; x < 9; x++ )
            {
                if( uchar n = m_board->get( x, y ) )
                {
                    switch( yi )
                    {
                        case 0: printf( "   |" );     break;
                        case 1: printf( "<%d>|", n ); break;
                        case 2: printf( "   |" );     break;
                    }
                }
                else
                {
                    const set<uchar>& choices = get( x, y );
                    for( uchar xi = 0; xi < 3; xi++ )
                    { 
                        uchar val = yi * 3 + xi + 1;
                        printf( "%c", choices.count( val ) ? '0' + val : ' ' );
                    }
                    printf( "|" );
                }
                if( !( ( x + 1 ) % 3 ) )
                {
                    printf( "  " );
                    if( x + 1 < 9 )
                        printf( "|" );
                }
            }
            printf( "\n" );
        }
        printf( row.c_str() );

        if( !( ( y + 1 ) % 3 ) )
        {
            printf( "\n" );
            if( y + 1 < 9 )
                printf( row.c_str() );
        }
    }
}
