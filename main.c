#include "nettypes.h"
#include "ga.h"


void test_population( unsigned int cur_pop )
{
   unsigned int i;

   for ( i = 0 ; i < POP_SIZE ; i++ )
   {
      RNN_load_network( i );

      RNN_test_network( i );
   }

   return;
}


void main( int argc, char *argv[] )
{
   unsigned int cur_pop = 0;

   srand( time( NULL ) );

   GA_init_population( cur_pop );

   test_population( cur_pop );

   // Evaluate the fitness of each member

      // Convert the member to the network weights.

      // Test the network with the available tests.

      // Maintain a fitness value that is stored with the member.

   // Sort the members

   // Selection, Keep the top N, mutate/crossover the rest.

   // Swap population

   // Repeat until MSE < X

   return;
}

