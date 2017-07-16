#include "nettypes.h"

void test_population( unsigned int cur_pop )
{
   unsigned int i;

   for ( i = 0 ; i < POP_SIZE ; i++ )
   {
      RNN_load_network( cur_pop, i );

      RNN_test_network( cur_pop, i );
   }

   return;
}


void main( int argc, char *argv[] )
{
   unsigned int cur_pop = 0;

   srand( time( NULL ) );

   GA_init_population( cur_pop );

   test_population( cur_pop );

   GA_process_population( cur_pop );

   // Switch the population.
   cur_pop = !cur_pop;

   // Repeat until MSE < X

   return;
}

