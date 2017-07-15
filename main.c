#include "nettypes.h"
#include "ga.h"


void main( int argc, char *argv[] )
{
   unsigned int cur_pop = 0;

   srand( time( NULL ) );

   GA_init_population( cur_pop );

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

