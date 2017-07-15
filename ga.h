// ga.h

#include "nettypes.h"

#define NUM_WEIGHTS    ( ( INPUT_NEURONS        * ( HIDDEN_NEURONS + 1 ) ) + \
                       ( ( HIDDEN_NEURONS + 1 ) * ( HIDDEN_NEURONS + 1 ) ) + \
                       ( ( HIDDEN_NEURONS + 1 ) * OUTPUT_NEURONS ) )

#define POP_SIZE       200

typedef unsigned short Chromosome[ NUM_WEIGHTS ];

typedef struct Member
{
   Chromosome chromosome;
   double     fitness;
} Member;


// Prototypes

void GA_init_population( unsigned int );
double GA_get_member_weight( unsigned int, unsigned int, unsigned int );

