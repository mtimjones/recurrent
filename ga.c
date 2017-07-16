// ga.c

#include "nettypes.h"

static Member population[ 2 ][ POP_SIZE ];

#define getRandWeight( )  ( getRand( 65536 ) )


void GA_init_population( unsigned int pop )
{
   int i, j;

   for ( i = 0 ; i < POP_SIZE ; i++ )
   {
      for ( j = 0 ; j < NUM_WEIGHTS ; j++ )
      {
         population[ pop ][ i ].chromosome[ j ] = getRandWeight( );
      }
      population[ pop ][ i ].fitness = 0.0;
   }

   return;
}


double GA_get_member_weight( unsigned int pop, unsigned int member, unsigned int index )
{
   // Weights are 0..65535.
   // Translated as ( Weight - 32767 ) * 0.001

   if ( index >= NUM_WEIGHTS ) assert( 0 );

   return ( ( double )population[ pop ][ member ].chromosome[ index ] - 32767.0 ) * 0.001;
}


void GA_set_fitness( unsigned int pop, unsigned int member, double fitness )
{
   population[ pop ][ member ].fitness = fitness;
}


