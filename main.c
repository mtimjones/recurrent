#include "nettypes.h"

int test_population( unsigned int cur_pop, FILE *evolution )
{
   unsigned int i;
   int fittest;
   int fitness, maxf;
   double avgf;
   static int x = 1;

   maxf = 0;
   avgf = 0.0;

   for ( i = 0 ; i < POP_SIZE ; i++ )
   {
      RNN_load_network( cur_pop, i );

      fitness = RNN_test_network( cur_pop, i, 0 );

      avgf += (double) fitness;

      if ( fitness > maxf ) 
      {
         fittest = i;
         maxf = fitness;
      }
   }

   fprintf( evolution, "%d\t%g\t%d\n", x++, avgf / (double)POP_SIZE, maxf );

   if ( ( avgf / ( double ) POP_SIZE ) > ( 0.8 * maxf ) ) 
   {
      printf( "Population prematurely converged.  No solution found.\n" );
      return 1;
   }

   if ( maxf == MAX_TESTS )
   {
      printf("Solution found.\n\n");
      RNN_load_network( cur_pop, fittest );
      ( void )RNN_test_network( cur_pop, fittest, 1 );
      return 1;
   }

   return 0;
}


int main( int argc, char *argv[] )
{
   unsigned int cur_pop = 0;
   int done = 0;
   FILE *evolution;

   evolution = fopen( "data.csv", "w" );

   srand( time( NULL ) );

   GA_init_population( cur_pop );

   while ( !done ) 
   {
      done = test_population( cur_pop, evolution );

      GA_process_population( cur_pop );

      // Switch the population.
      cur_pop = !cur_pop;
   }

   fclose( evolution );

   return 0;
}

