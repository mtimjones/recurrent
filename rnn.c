// rnn.c

#include "nettypes.h"
#include "string.h"

double inputs[ INPUT_NEURONS+1 ];
double hidden[ HIDDEN_NEURONS+1 ];
double context[ HIDDEN_NEURONS+1 ];
double outputs[ OUTPUT_NEURONS ];

double w_h_i[ HIDDEN_NEURONS ][ INPUT_NEURONS+1 ];
double w_o_h[ OUTPUT_NEURONS ][ HIDDEN_NEURONS+1 ];
double w_h_c[ HIDDEN_NEURONS ][ HIDDEN_NEURONS+1 ]; // Feedback (context) Neurons

#define sigmoid( x )    ( 1.0 / ( 1.0 + exp( -x ) ) )

// One-hot input vector
// input -> hidden -> output
//           ^  |
//           |  v
//          context
//

// test vocabulary (6)
// hello, help, hope, heel, hole
// poll, plop, peel, polo, pool,
// loop, lope
//
// One hot vectors: 
//
//      [ 0 0 0 0 0 1 ] = h
//      [ 0 0 0 0 1 0 ] = e
//      [ 0 0 0 1 0 0 ] = l
//      [ 0 0 1 0 0 0 ] = o
//      [ 0 1 0 0 0 0 ] = p
//      [ 1 0 0 0 0 0 ] = . <END>
//

#define MAX_TESTS    11

const char *test_strings[ MAX_TESTS ] = {
       "hello", "help", "hope", "heel", "hole",
       "poll",  "plop", "peel", "pool",
       "loop", "lope"
};


void set_input_vector( char c )
{
   int i;

   // Clear the inputs.
   for ( i = 0 ; i < INPUT_NEURONS ; i++ )
   {
      inputs[ i ] = 0.0;
   }

   // Set the one-hot.
   switch( c )
   {
      case 'h': inputs[ 0 ] = 1.0; break;
      case 'e': inputs[ 1 ] = 1.0; break;
      case 'l': inputs[ 2 ] = 1.0; break;
      case 'o': inputs[ 3 ] = 1.0; break;
      case 'p': inputs[ 4 ] = 1.0; break;
      case '.': inputs[ 5 ] = 1.0; break;
      default: assert( 0 ); break;
   }

   return;
}

char get_parsed_output_vector( void )
{
   // Classify and then return the output char (winner-takes-all).

   int i, best = -1;
   double max;

   best = 0;
   max = outputs[ best ];

   for ( i = 1 ; i < OUTPUT_NEURONS ; i++ )
   {
      if ( outputs[ i ] > max )
      {
         max = outputs[ i ];
         best = i;
      }
   }

   switch( best )
   {
      case 0: return('h'); break;
      case 1: return('e'); break;
      case 2: return('l'); break;
      case 3: return('o'); break;
      case 4: return('p'); break;
      case 5: return('.'); break;
      default: assert( 0 ) ; break;
   }

   assert( 0 );
   return ' ';
}


void clear_hidden_context( void )
{
   int k;

   for ( k = 0 ; k < HIDDEN_NEURONS+1 ; k++ )
   {
      context[ k ] = 0.0;
   }

   return;
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
         hidden[ i ] += w_h_c[ i ][ k ] * context[ k ];
      }

      // apply tanh activation function.
      hidden[ i ] = tanh( hidden[ i ] );

   }

   // Calculate output layer outputs
   for ( i = 0 ; i < OUTPUT_NEURONS ; i++ )
   {
      outputs[ i ] = 0.0;

      for ( j = 0 ; j < HIDDEN_NEURONS+1 ; j++ )
      {
         outputs[ i ] += ( w_o_h[ i ][ j ] * hidden[ j ] );
      }

      // apply sigmoid activation function.
      outputs[ i ] = sigmoid( outputs[ i ] );
   }

   // Save the context hidden value
   for ( k = 0 ; k < HIDDEN_NEURONS+1 ; k++ )
   {
      context[ k ] = hidden[ k ];
   }

   return;
}


void RNN_load_network( unsigned int cur_pop, unsigned int member )
{
   int i, j, weight = 0;

   // Set the biases.
   inputs[ INPUT_NEURONS ] = 1.0;
   hidden[ HIDDEN_NEURONS ] = 1.0;
   context[ HIDDEN_NEURONS ] = 1.0;

   // Load the input->hidden weights into the network.
   for ( i = 0 ; i < HIDDEN_NEURONS ; i++ )
   {
      for ( j = 0 ; j < ( INPUT_NEURONS + 1 ) ; j++ )
      {
         w_h_i[ i ][ j ] = GA_get_member_weight( cur_pop, member, weight++ );
      }
   }

   // Load the hidden->output weights into the network.
   for ( i = 0 ; i < OUTPUT_NEURONS ; i++ )
   {
      for ( j = 0 ; j < ( HIDDEN_NEURONS + 1 ) ; j++ )
      {
         w_o_h[ i ][ j ] = GA_get_member_weight( cur_pop, member, weight++ );
      }
   }
   
   // Load the context->hidden weights into the network.
   for ( i = 0 ; i < HIDDEN_NEURONS ; i++ )
   {
      for ( j = 0 ; j < ( HIDDEN_NEURONS + 1 ) ; j++ )
      {
         w_h_c[ i ][ j ] = GA_get_member_weight( cur_pop, member, weight++ );
      }
   }

   return;
}


int test_word( unsigned int index )
{
   int i, len;

   // Clear the context nodes.
   clear_hidden_context( );

   len = strlen( test_strings[ index ] );

printf("Testing %s\n", test_strings[ index ] );

   for ( i = 0 ; i < len-1 ; i++ )
   {
      set_input_vector( (char)test_strings[ index ][ i ] );
      RNN_feed_forward( );
      printf("     %c\n", get_parsed_output_vector( ) );
   }

   // Advance to push the predicted character
   set_input_vector( '.' );
   RNN_feed_forward( );
   printf("     %c\n", get_parsed_output_vector( ) );

   printf("Testing %c == %c\n", 
      get_parsed_output_vector( ), test_strings[ index ][ i ] );

   if ( get_parsed_output_vector( ) == test_strings[ index ][ i ] )
   {
      return 1;
   }
   
   return 0;
}


void RNN_test_network( unsigned int cur_pop, unsigned int member )
{
   int i;
   int score = 0;

   for ( i = 0 ; i < MAX_TESTS ; i++ )
   {
      score += test_word( i );
   }

   printf("Score = %d\n", score );

   GA_set_fitness( cur_pop, member, ( double )( 2 ^ score ) );

   return;
}

