#ifndef GA_H
#define GA_H

#define TESTRUN 1000
#define GAME_TYPE 2
// Problem: Mastermind (P=4, N=6) and (P=5, N=8)
// GAME_TYPE 1 = (P=4, N=6) and 2 = (P=5, N=8)

#if GAME_TYPE == 1
#define NUM_COLORS 6
#define NUM_PROBLEM 4
#define INITIAL_GUESS "AABC"
#define MAX_ATTEMPTS 10
#elif GAME_TYPE == 2
#define NUM_COLORS 8
#define NUM_PROBLEM 5
#define INITIAL_GUESS "AABCD"
#define MAX_ATTEMPTS 12
#endif

// Hyperparameter for GA

#define TOURNAMENTSIZE 5
#define CROSSOVER_RATE 50
#define MUTATION_RATE 12
#define PERMUTATION_RATE 3
#define INVERSION_RATE 3

#define EHAT_POPULATION_SIZE 40
#define POPULATION_SIZE 30
#define MAX_GENERATIONS 20
#define ELITSM_POPULATION_RATE 0.5
#define FITNESS_THRESHOLD 0.0
#define FITNESS_BLACK_VALUE 1.0
#define FITNESS_WHITE_VALUE 1.0

typedef struct {
  char ans[NUM_PROBLEM + 1];
  int black;
  int white;
  double fitness;
} ANSWER_T;

#endif
