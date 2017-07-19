// rnn.c

#include "nettypes.h"
#include "string.h"

double inputs[ INPUT_NEURONS+1 ];
double hidden[ HIDDEN_NEURONS+1 ];
double context1[ HIDDEN_NEURONS+1 ];
double context2[ HIDDEN_NEURONS+1 ];
double outputs[ OUTPUT_NEURONS ];

double w_h_i[ HIDDEN_NEURONS ][ INPUT_NEURONS+1 ];
double w_o_h[ OUTPUT_NEURONS ][ HIDDEN_NEURONS+1 ];
double w_h_c1[ HIDDEN_NEURONS+1 ]; // Feedback (context) Neurons
double w_h_c2[ HIDDEN_NEURONS+1 ]; // Feedback (context) Neurons

#define sigmoid( x )    ( 1.0 / ( 1.0 + exp( -x ) ) )

// One-hot input vector
// input -> hidden -> output
//           ^  |
//           |  v
//          context
//

// One hot vectors: 
//
//      [ 0 0 0 0 0 1 ] = b
//      [ 0 0 0 0 1 0 ] = a
//      [ 0 0 0 1 0 0 ] = n
//      [ 0 0 1 0 0 0 ] = s
//      [ 0 1 0 0 0 0 ] = e
//      [ 1 0 0 0 0 0 ] = d
//


// Test data set for last letter prediction.
const char *test_strings[ MAX_TESTS ] = {
    "based", "baned", "sedan", "nabes", "snead",
    "bend", "bead", "sand", "band", "dean", "send", "bands", "bade", "sean",
    "end", "dab", "ben", "and", "sed", "den", "abs", "sad"
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
      case 'b': inputs[ 0 ] = 1.0; break;
      case 'a': inputs[ 1 ] = 1.0; break;
      case 'n': inputs[ 2 ] = 1.0; break;
      case 's': inputs[ 3 ] = 1.0; break;
      case 'e': inputs[ 4 ] = 1.0; break;
      case 'd': inputs[ 5 ] = 1.0; break;
      default: assert( 0 ); break;
   }

   return;
}

char get_parsed_output_vector( void )
{
   // Classify and then return the output char (winner-takes-all).

   int i, best;
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
      case 0: return('b'); break;
      case 1: return('a'); break;
      case 2: return('n'); break;
      case 3: return('s'); break;
      case 4: return('e'); break;
      case 5: return('d'); break;
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
      context1[ k ] = 0.0;
      context2[ k ] = 0.0;
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
      hidden[ i ] += w_h_c1[ i ] * context1[ i ];
      hidden[ i ] += w_h_c2[ i ] * context2[ i ];

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
      context2[ k ] = context1[ k ];
      context1[ k ] = hidden[ k ];
   }

   return;
}


void RNN_load_network( unsigned int cur_pop, unsigned int member )
{
   int i, j, weight = 0;

   // Set the biases.
   inputs[ INPUT_NEURONS ] = 1.0;
   hidden[ HIDDEN_NEURONS ] = 1.0;
   context1[ HIDDEN_NEURONS ] = 1.0;
   context2[ HIDDEN_NEURONS ] = 1.0;

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
   for ( j = 0 ; j < ( HIDDEN_NEURONS + 1 ) ; j++ )
   {
      w_h_c1[ j ] = GA_get_member_weight( cur_pop, member, weight++ );
   }

   for ( j = 0 ; j < ( HIDDEN_NEURONS + 1 ) ; j++ )
   {
      w_h_c2[ j ] = GA_get_member_weight( cur_pop, member, weight++ );
   }

   return;
}


int test_word( unsigned int index, int debug )
{
   int i, len;
   char test_char;

   // Clear the context nodes.
   clear_hidden_context( );

   len = strlen( test_strings[ index ] );

   if ( debug ) printf("Testing %s\n", test_strings[ index ] );

   for ( i = 0 ; i < len-1 ; i++ )
   {
      test_char = (char)test_strings[ index ][ i ];
      set_input_vector( test_char );
      RNN_feed_forward( );
      if ( debug ) printf("\tFed %c, got %c\n", test_char, get_parsed_output_vector( ) );
   }

   if ( debug ) printf("\n");

   if ( get_parsed_output_vector( ) == (char)test_strings[ index ][ i ] )
   {
      return 1;
   }
   
   return 0;
}


int RNN_test_network( unsigned int cur_pop, unsigned int member, int debug )
{
   int i;
   int score = 0;

   for ( i = 0 ; i < MAX_TESTS ; i++ )
   {
      score += test_word( i, debug );
   }

   GA_set_fitness( cur_pop, member, ( double )( ( score * score ) + 1.0 ) );

   return score;
}

