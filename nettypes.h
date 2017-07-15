// nettypes.h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INPUT_NEURONS    5
#define HIDDEN_NEURONS  10
#define OUTPUT_NEURONS   5

#define getSRand( )       ( ( double ) rand( ) / ( double ) RAND_MAX )
#define getRand( x )      ( int ) ( ( x ) * getSRand( ) )

