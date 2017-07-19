// ga.h

#define NUM_WEIGHTS    ( ( ( INPUT_NEURONS  + 1 ) * ( HIDDEN_NEURONS ) ) + \
                         ( HIDDEN_NEURONS + 1 ) +                          \
                         ( HIDDEN_NEURONS + 1 ) +                          \
                         ( ( HIDDEN_NEURONS + 1 ) * OUTPUT_NEURONS   ) )

#define POP_SIZE       2000

typedef unsigned short Chromosome[ NUM_WEIGHTS ];

typedef struct Member
{
   Chromosome chromosome;
   double     fitness;
} Member;

#define MUTATE_PROB    0.006
#define CROSS_PROB     0.003

// Prototypes

void GA_init_population( unsigned int );
double GA_get_member_weight( unsigned int, unsigned int, unsigned int );
void GA_set_fitness( unsigned int, unsigned int, double );
void GA_process_population( unsigned int );

