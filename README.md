# Mastermind Game with Genetic Algorithm🕹️
This project aims to apply a Genetic Algorithm to solve the Mastermind Game. The Mastermind Game in our study has 2 game modes: P(6,4) and P(8,5) in the limit of guesses of 10 and 12 turns, respectively.

## Objective🎯
- To decrease the average round of guesses as much as possible
- To decrease execution time usage

## Best Result⭐
### P(6,4): Pick 4 colors from 6 colors📍
- TEST RUN: 1000 games
- Min Round: 1
- Max Round: 7
- Average Round: 4.542
- Average time: 10.243 ms per game
- Winrate: 100%
### P(8,5): Pick 5 colors from 8 colors📍
- TEST RUN: 1000 games
- Min Round: 2
- Max Round: 9
- Average Round: 5.811
- Average time: 560.853 ms per game
- Winrate: 100%

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
#define TOURNAMENT_SIZE 2  
#define CROSSOVER_RATE 10  
#define MUTATION_RATE 10  
#define PERMUTATION_RATE 2  
#define INVERSION_RATE 3  
#define EHAT_POPULATION_SIZE 20  
#define POPULATION_SIZE 30  
#define MAX_GENERATIONS 20  
#define ELITSM_POPULATION_RATE 0.3  
#define FITNESS_THRESHOLD 0.0  
#define FITNESS_BLACK_VALUE 1.0  
#define FITNESS_WHITE_VALUE 1.0  

## Note📝
1. **CROSSOVER_RATE** is the ratio between single-point and two-point crossover. Therefore MUTATION_RATE 10 means a 10% chance of using a single-point crossover and a 90% chance of using a two-point crossover (10/90)
2. **PERMUTATION_RATE** is the chance of being mutated by the **"swap mutation"** method
3. **ELITSM_POPULATION_RATE** is the rate of elitism from the eligible set
4. The eligible set noted by **"EHAT"**
5. The fitness value converges to 0 is better

## Project Report📃
[Mastermind_with_GA_report.pdf](Mastermind_with_GA_report.pdf)

## Reference📜
[Berghman et. al. (2009) Efficient solutions for Mastermind using genetic algorithms](https://www.researchgate.net/publication/265728382_Efficient_solutions_for_Mastermind_using_genetic_algorithms)

## Background🏫
- This project is a part of the CPE231 Algorithm subject in Semester 1/2023
- Department of Computer Engineering, King Mongkut's University of Technology Thonburi

Members 
| Student ID  | Name                     | Github |
|-------------|--------------------------|--------|
| 65070501039 | Pongpon Butseemart       |   [Pongpon00](https://github.com/Pongpon00)     |
| 65070501055 | Sorrawit Udomwittayakrai |   [SorrawitUdom](https://github.com/SorrawitUdom)    |
| 65070501069 | Kanitsorn Darunaitorn    |   [Dr804t](https://github.com/Dr804t)     |
| 65070501083 | Panurut Sirinapaisan     |   [sokungz01](https://github.com/sokungz01)    |
| 65070501092 | Park Saranunt            |   [Saranunt](https://github.com/Saranunt)     |
