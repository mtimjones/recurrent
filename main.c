#include "nettypes.h"

int test_population( unsigned int cur_pop )
{
   unsigned int i;
   double fitness;
   double avgf, maxf;

   maxf = avgf = 0;

   for ( i = 0 ; i < POP_SIZE ; i++ )
   {
      RNN_load_network( cur_pop, i );

      fitness = RNN_test_network( cur_pop, i );

      avgf += fitness;

      if ( fitness > maxf ) maxf = fitness;
   }

   printf( "Avgf = %g, Maxf = %g\n", avgf / (double)POP_SIZE, maxf );

   if ( ( avgf / ( double ) POP_SIZE ) > ( 0.8 * maxf ) ) return 1;

   return 0;
}


void main( int argc, char *argv[] )
{
   unsigned int cur_pop = 0;
   int done = 0;

   srand( time( NULL ) );

   GA_init_population( cur_pop );

   while ( !done ) 
   {
      done = test_population( cur_pop );

      GA_process_population( cur_pop );

      // Switch the population.
      cur_pop = !cur_pop;
   }

   return;
}

