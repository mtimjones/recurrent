// ga.c

#include "nettypes.h"

static Member population[ 2 ][ POP_SIZE ];

#define getRandWeight( )      ( getRand( 65536 ) )

#define PROB( probability )   ( getSRand( ) < probability )

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


void recombine( unsigned int pop, 
                unsigned int parent1, 
                unsigned int parent2, 
                unsigned int child,
                double       mutate_p,
                double       cross_p
                )
{
   unsigned int sel = getRand(2);
   unsigned int w_index;
   unsigned short weight;

   for ( w_index = 0 ; w_index < NUM_WEIGHTS ; w_index++ )
   {
      // Get the parent chromsome's weight
      if ( sel == 0 )
      {
         // Select from parent1
         weight = population[ pop ][ parent1 ].chromosome[ w_index ];
      }
      else
      {
         // Select from parent2
         weight = population[ pop ][ parent2 ].chromosome[ w_index ];
      }

      // Mutate with the given probability.
      if ( PROB( mutate_p ) )
      {
         weight = getRandWeight( );
      }

      // Crossover with the given probability.
      if ( PROB( cross_p ) )
      {
         sel = !sel;
      }

      // Load the weight into the child's chromosome.
      population[ !pop ][ child ].chromosome[ w_index ] = weight;
   }
}

unsigned int select_parent( unsigned int pop, double sumf )
{
   int index;
   double sum = 0.0;
   double limit = ( ( ( double ) rand( ) / ( double ) RAND_MAX ) * sumf );

   for ( index = 0 ; index < POP_SIZE ; index++ )
   {
      sum += population[ pop ][ index ].fitness;
      if ( limit < sum )
      {
         break;
      }
   }
   return index;
}

void GA_process_population( unsigned int pop )
{
   double sum = 0.0;
   double max = 0.0;
   int best;
   int i, child;

   best = 0;
   sum = max = population[ pop ][ best ].fitness;

   // Calculate the total population fitness
   for ( i = 1 ; i < POP_SIZE ; i++ )
   {
      sum += population[ pop ][ i ].fitness;
      if ( population[ pop ][ i ].fitness > max )
      {
         best = i;
         max = population[ pop ][ i ].fitness;
      }
   }

   // Elitist -- keep the best performing chromosome.
   recombine( pop, best, best, 0, 0.0, 0.0 );

   // Generate the next generation.
   for ( child = 1 ; child < POP_SIZE ; child++ )
   {
      unsigned int parent1 = select_parent( pop, sum );
      unsigned int parent2 = select_parent( pop, sum );

      recombine( pop, parent1, parent2, child, MUTATE_PROB, CROSS_PROB );
   }

   return;
}
