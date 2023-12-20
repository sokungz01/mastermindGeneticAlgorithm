# Mastermind Game with Genetic Algorithm🕹️
This project aims to apply a Genetic Algorithm to solve the Mastermind Game. The Mastermind Game in our study has 2 game modes: P (6,4) and P(8,5) in the limit of guesses of 10 and 12 turns, respectively.

## Objective🎯
- To decrease the average round of guesses as much as possible
- To decrease execution time usage

## GA Method🌿
### Parent Selection 👨🏻‍👩🏻‍👧🏻‍👦🏻
- Tournament Selection
### Crossing-over Methods🧬
- Single-Point Crossover
- Two-Point Crossover
### Mutation Methods☣️
- Random Resetting
- Inversion
- Swap (Permutation)

### Hyperparameter🎚️
#define CROSSOVER_RATE 10  
#define MUTATION_RATE 10  
#define PERMUTATION_RATE 2  
#define INVERSION_RATE 3  
#define EHAT_POPULATION_SIZE 20  
#define POPULATION_SIZE 30  
#define MAX_GENERATIONS 20  
#define ELITSM_POPULATION_RATE 0.3000  
#define FITNESS_THRESHOLD 0.0  
#define FITNESS_BLACK_VALUE 1.0  
#define FITNESS_WHITE_VALUE 1.0  
