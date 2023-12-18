#include "GA.h"
#include "crossover.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int cmpBlack(const void *a, const void *b) {
  return (((ANSWER_T *)b)->black - ((ANSWER_T *)a)->black);
}
int cmpFitness(const void *a, const void *b) {
  return (((ANSWER_T *)a)->fitness - ((ANSWER_T *)b)->fitness);
}
void dataLogger(const char *fileName, int minRound, int maxRound, int winCount,
                int *frequency, double avgRun, time_t t);

int main(void) {
  time_t t = time(NULL);
  srand((unsigned)time(&t));
  int minRound = 99;
  int maxRound = 0;
  int frequency[MAX_ATTEMPTS];
  memset(frequency, 0, sizeof(frequency));
  float avgRun = 0;
  int winCount = 0;
  clock_t rawTime;
  for (int i = 0; i < TESTRUN; i++) {
    rawTime = clock();
    ANSWER_T guess;
    char problem[NUM_PROBLEM + 1];
    char initalGuess[NUM_PROBLEM + 1] = INITIAL_GUESS;
    char tempParentA[NUM_PROBLEM + 1];
    char tempParentB[NUM_PROBLEM + 1];
    ANSWER_T hallOfFame[MAX_ATTEMPTS];
    ANSWER_T population[POPULATION_SIZE];
    ANSWER_T eHat[EHAT_POPULATION_SIZE];
    ANSWER_T Child[POPULATION_SIZE];
    ANSWER_T Elite[POPULATION_SIZE];
    strcpy(guess.ans, initalGuess);
    problem[NUM_PROBLEM] = '\0';
    guess.black = 0;
    guess.white = 0;
    guess.fitness = 0;
    int round = 1;
    int EliteCount = 0;
    generateRandomGuess(problem);
    gameEngine(problem, &guess);
    for (int j = 0; j < NUM_PROBLEM + 1; j++) {
      hallOfFame[0].ans[j] = guess.ans[j];
    }
    hallOfFame[0].black = guess.black;
    hallOfFame[0].white = guess.white;
    hallOfFame[0].fitness = guess.fitness;
    printf("TRY: %d -  PROBLEM IS : %s \n", i + 1, problem);
    printf("Round %-4d | Guess : %-6s | B: %-2d | W:%-2d \n", round, guess.ans,
           guess.black, guess.white);
    while (guess.black != NUM_PROBLEM && round < MAX_ATTEMPTS) {

      initializePopulation(population);
      for (int j = 0; j < EliteCount; j++) {
        strcpy(population[j].ans, Elite[j].ans);
      }
      EliteCount = 0;
      int generation = 1;
      int eHatElemNum = 0, ChildSize = 0;
      memset(eHat, 0, sizeof eHat);
      memset(Child, 0, sizeof Child);
      memset(Elite, 0, sizeof Elite);
      while ((generation <= MAX_GENERATIONS &&
              eHatElemNum <= EHAT_POPULATION_SIZE) ||
             (eHatElemNum == 0)) {

        for (int pairing = 0; pairing < POPULATION_SIZE / 2; pairing++) {
          if (generation > 5 && eHatElemNum == 0) {
            for (int genPop = POPULATION_SIZE / 2; genPop < POPULATION_SIZE;
                 genPop++) {
              generateRandomGuess(population[genPop].ans);
              population[genPop].black = 0;
              population[genPop].white = 0;
              population[genPop].fitness = 0;
            }
          }
          for (int runner = 0; runner < POPULATION_SIZE; runner++) {
            population[runner].fitness =
                calculateNewFitness(round, hallOfFame, population[runner]);
          }
          TournamentSelection(population, tempParentA, tempParentB);
          // PropotionalRouletteWheelSelection(population, tempParentA,
          //                                   tempParentB);
          int isAlreadyExistsA = 1;
          int isAlreadyExistsB = 1;
          while (isAlreadyExistsA == 1 || isAlreadyExistsB == 1) {
            crossingOver(tempParentA, tempParentB);
            mutate(tempParentA);
            mutate(tempParentB);
            inversion(tempParentA);
            inversion(tempParentB);
            mutateswap(tempParentA);
            mutateswap(tempParentB);

            for (int findHistory = 0; findHistory < round; findHistory++) {
              if (strcmp(tempParentA, hallOfFame[findHistory].ans) == 0) {
                isAlreadyExistsA = 1;
                break;
              } else {
                isAlreadyExistsA = 0;
              }
            }
            for (int findHistory = 0; findHistory < round; findHistory++) {
              if (strcmp(tempParentB, hallOfFame[findHistory].ans) == 0) {
                isAlreadyExistsB = 1;
                break;
              } else {
                isAlreadyExistsB = 0;
              }
            }
          }

          if (isAlreadyExistsA == 0) {
            addEntryToChild(Child, &ChildSize, tempParentA);
          }
          if (isAlreadyExistsB == 0) {
            addEntryToChild(Child, &ChildSize, tempParentB);
          }

          for (int i = 0; i < ChildSize; i++) {
            Child[i].fitness = calculateNewFitness(round, hallOfFame, Child[i]);
            if (Child[i].fitness <= FITNESS_THRESHOLD &&
                eHatElemNum < EHAT_POPULATION_SIZE) {
              addEntryToEHat(eHat, &eHatElemNum, Child[i]);
            }
          }
        }

        generation++;
        ChildSize = 0;
      }
      round++;
      EliteCount = eHatElemNum * ELITSM_POPULATION_RATE;

      int runner = 0;
      for (int j = 0; j < eHatElemNum && runner < EliteCount; j++) {
        if (eHat[j].ans[0] != '\0') {
          strcpy(Elite[runner].ans, eHat[j].ans);
          runner++;
        }
      }

      for (int i = 0; i < NUM_PROBLEM; i++) {
        guess.ans[i] = eHat[0].ans[i];
        hallOfFame[round - 1].ans[i] = eHat[0].ans[i];
      }
      gameEngine(problem, &hallOfFame[round - 1]);
      gameEngine(problem, &guess);
      printf("Round %-4d | Guess : %-6s | B: %-2d | W:%-2d | eHat : %d\n",
             round, guess.ans, guess.black, guess.white, eHatElemNum);
    }
    rawTime = clock() - rawTime;
    printf("TimeTaken : %.3lf ms\n", (((double)rawTime) / CLOCKS_PER_SEC)*1000);
    printf("---------------------------------------------\n");
    if (round <= MAX_ATTEMPTS && guess.black == NUM_PROBLEM) {
      winCount++;
    }
    avgRun += round;
    if (round < minRound) {
      minRound = round;
    }
    if (round > maxRound) {
      maxRound = round;
    }
    frequency[round - 1]++;
  }
  dataLogger("result.txt", minRound, maxRound, winCount, frequency, avgRun, t);
  return 0;
}

void dataLogger(const char *fileName, int minRound, int maxRound, int winCount,
                int *frequency, double avgRun, time_t t) {
  struct tm tm = *localtime(&t);
  printf("TEST DATE : %d-%02d-%02d | TIME : %02d:%02d:%02d\n",
         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, (tm.tm_hour + 7) % 24,
         tm.tm_min, tm.tm_sec);
  if (GAME_TYPE == 1) {
    printf("GAMETYPE : 1 (P = 4, N = 6)\n");
  } else {
    printf("GAMETYPE : 2 (P = 5, N = 8)\n");
  }
  printf("TEST RUN : %d times\n", TESTRUN);
  printf("Min Round : %d\n", minRound);
  printf("Max Round : %d\n", maxRound);
  printf("Average Round : %g\n", avgRun / TESTRUN);
  printf("Winrate : %.2f%%\n", (((double)winCount) / TESTRUN) * 100);

  printf("\n|Round : Frequency|\n");
  for (int i = 0; i < MAX_ATTEMPTS; i++) {
    if (frequency[i] != 0) {
      printf("| %-4d : %-4d |\n", i + 1, frequency[i]);
    }
  }

  printf("\n|GA PARAMETER|\n");
  printf("#define CROSSOVER_RATE %d\n", CROSSOVER_RATE);
  printf("#define MUTATION_RATE %d\n", MUTATION_RATE);
  printf("#define PERMUTATION_RATE %d\n", PERMUTATION_RATE);
  printf("#define INVERSION_RATE %d\n", INVERSION_RATE);
  printf("#define EHAT_POPULATION_SIZE %d\n", EHAT_POPULATION_SIZE);
  printf("#define POPULATION_SIZE %d\n", POPULATION_SIZE);
  printf("#define MAX_GENERATIONS %d\n", MAX_GENERATIONS);
  printf("#define ELITSM_POPULATION_RATE %.4lf\n", ELITSM_POPULATION_RATE);
  printf("#define FITNESS_THRESHOLD %.4lf\n", FITNESS_THRESHOLD);
  printf("#define FITNESS_BLACK_VALUE %.4lf\n", FITNESS_BLACK_VALUE);
  printf("#define FITNESS_WHITE_VALUE %.4lf\n", FITNESS_WHITE_VALUE);
  printf("---------------------------------------------\n");

  FILE *fp = fopen(fileName, "a");
  fprintf(fp, "TEST DATE : %d-%02d-%02d | TIME : %02d:%02d:%02d\n",
          tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, (tm.tm_hour + 7) % 24,
          tm.tm_min, tm.tm_sec);
  if (GAME_TYPE == 1) {
    fprintf(fp, "GAMETYPE : 1 (P = 4, N = 6)\n");
  } else {
    fprintf(fp, "GAMETYPE : 2 (P = 5, N = 8)\n");
  }
  fprintf(fp, "TEST RUN : %d times\n", TESTRUN);
  fprintf(fp, "Min Round : %d\n", minRound);
  fprintf(fp, "Max Round : %d\n", maxRound);
  fprintf(fp, "Average Round : %g\n", avgRun / TESTRUN);
  fprintf(fp, "Winrate : %.2f%%\n", (((double)winCount) / TESTRUN) * 100);

  fprintf(fp, "\n|Round : Frequency|\n");
  for (int i = 0; i < MAX_ATTEMPTS; i++) {
    if (frequency[i] != 0) {
      fprintf(fp, "| %-4d : %-6d |\n", i + 1, frequency[i]);
    }
  }

  fprintf(fp, "\n|GA PARAMETER|\n");
  fprintf(fp, "#define TOURNAMENTSIZE %d\n", TOURNAMENTSIZE);
  fprintf(fp, "#define CROSSOVER_RATE %d\n", CROSSOVER_RATE);
  fprintf(fp, "#define MUTATION_RATE %d\n", MUTATION_RATE);
  fprintf(fp, "#define PERMUTATION_RATE %d\n", PERMUTATION_RATE);
  fprintf(fp, "#define INVERSION_RATE %d\n", INVERSION_RATE);
  fprintf(fp, "#define EHAT_POPULATION_SIZE %d\n", EHAT_POPULATION_SIZE);
  fprintf(fp, "#define POPULATION_SIZE %d\n", POPULATION_SIZE);
  fprintf(fp, "#define MAX_GENERATIONS %d\n", MAX_GENERATIONS);
  fprintf(fp, "#define ELITSM_POPULATION_RATE %.4f\n", ELITSM_POPULATION_RATE);
  fprintf(fp, "#define FITNESS_THRESHOLD %.4f\n", FITNESS_THRESHOLD);
  fprintf(fp, "#define FITNESS_BLACK_VALUE %.4f\n", FITNESS_BLACK_VALUE);
  fprintf(fp, "#define FITNESS_WHITE_VALUE %.4f\n", FITNESS_WHITE_VALUE);
  fprintf(fp, "---------------------------------------------\n\n");
  fclose(fp);
}