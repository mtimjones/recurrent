// rnn.c

#include "nettypes.h"

double inputs[ INPUT_NEURONS+1 ];
double hidden[ HIDDEN_NEURONS+1 ];
double hidden_prior[ HIDDEN_NEURONS+1 ];
double outputs[ OUTPUT_NEURONS ];

double w_h_i[ HIDDEN_NEURONS ][ INPUT_NEURONS+1 ];
double w_o_h[ OUTPUT_NEURONS ][ HIDDEN_NEURONS+1 ];
double w_a_h[ HIDDEN_NEURONS ][ HIDDEN_NEURONS+1 ]; // Feedback Neurons

// One-hot input vector
// input -> hidden -> output
//           ^  |
//           |  v
//          activation
//

// test vocabulary (5)
// hello, help, hope, heel, hole
// poll, plop, peel, polo, pool,
// loop, lope
//
// One hot vectors: 
//
//      [ 1 0 0 0 0 ] = h
//      [ 0 1 0 0 0 ] = e
//      [ 0 0 1 0 0 ] = l
//      [ 0 0 0 1 0 ] = o
//      [ 0 0 0 0 1 ] = p
//      [ 0 0 0 0 0 ] = <END>      ** need to investigate...
//


void clear_hidden_activations( void )
{
   int k;

   for ( k = 0 ; k < HIDDEN_NEURONS+1 ; k++ )
   {
      hidden_prior[ k ] = 0.0;
   }
}


void RNN_feed_forward( void )
{
   int i, j, k;

   // Calculate hidden layer outputs
   for ( i = 0 ; i < HIDDEN_NEURONS ; i++ )
   {
      hidden[ i ] = 0.0;

      // Incorporate the input.
      for ( j = 0 ; j < INPUT_NEURONS+1 ; j++ )
      {
         hidden[ i ] += w_h_i[ i ][ j ] * inputs[ j ];
      }

      // Incorporate the recurrent hidden.
      for ( k = 0 ; k < HIDDEN_NEURONS+1 ; k++ )
      {
         hidden[ i ] += w_a_h[ i ][ k ] * hidden_prior[ k ];
      }

      // apply tanh activation function.

   }

   // Calculate output layer outputs
   for ( i = 0 ; i < OUTPUT_NEURONS ; i++ )
   {
      outputs[ i ] = 0.0;

      for ( j = 0 ; j < HIDDEN_NEURONS+1 ; j++ )
      {
         outputs[ i ] += ( w_o_h[ i ][ j ] * hidden[ j ] );
      }
      // apply tanh activation function.
   }

   // Save the prior hidden value
   for ( k = 0 ; k < HIDDEN_NEURONS+1 ; k++ )
   {
      hidden_prior[ k ] = hidden[ k ];
   }

   return;
}


void RNN_load_network( member *member )
{
   int i, j;

   // Set the bias to one.
   inputs[ INPUT_NEURONS ] = 1.0;
   hidden[ HIDDEN_NEURONS ] = 1.0;
   hidden_prior[ HIDDEN_NEURONS ] = 1.0;

   // @TODO: Convert the member chromosome to a network.

   return;
}


void main( int argc, char *argv[] )
{
   srand( time( NULL ) );

   // Create random population

   // Evaluate the fitness of each member

      // Convert the member to the network weights.

      // Test the network with the available tests.

      // Maintain a fitness value that is stored with the member.

   // Sort the members

   // Selection, Keep the top N, mutate/crossover the rest.

   // Swap population

   // Repeat until MSE < X

}

