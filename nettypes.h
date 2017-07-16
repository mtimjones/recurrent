// nettypes.h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define INPUT_NEURONS    6
#define HIDDEN_NEURONS  10
#define OUTPUT_NEURONS   ( INPUT_NEURONS )

#define getSRand( )       ( ( double ) rand( ) / ( double ) RAND_MAX )
#define getRand( x )      ( int ) ( ( x ) * getSRand( ) )

#include "rnn.h"
#include "ga.h"

