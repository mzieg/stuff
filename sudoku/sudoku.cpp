#include <stdio.h>
#include <stdlib.h>
#include "Board.h"
#include "Possibilities.h"

int main( int argc, char **argv )
{
    if( argc != 2 )
    {
        fprintf( stderr, "usage: %s board.txt\n", argv[0] );
        exit( 1 );
    }

    printf( "loading board %s:\n", argv[1] );
    Board b( argv[1] );
    b.dump();

    printf( "\ncomputing initial set of possibilities:\n" );
    Possibilities p( b );
    p.compute();
    p.dump();

    printf( "\nattempting to solve...\n" );
    const Board *solution = b.solve();

    if( solution )
    {
        printf( "\nSolution:\n" );
        solution->dump();
    }
    else
    {
        printf( "ERROR *** board could not be solved?!?\n" );
    }
}
