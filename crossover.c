#include "crossover.h"
#include "GA.h"

// int cmpFitness(const void *a, const void *b) {
//   return (((ANSWER_T *)a)->fitness - ((ANSWER_T *)b)->fitness);
// }

void gameEngine(char *problem, ANSWER_T *userguess) {
  userguess->black = 0;
  userguess->white = 0;

  int problemUsed[NUM_PROBLEM];
  int guessUsed[NUM_PROBLEM];

  memset(problemUsed, 0, sizeof(problemUsed));
  memset(guessUsed, 0, sizeof(guessUsed));

  // Check for correct positions
  for (int i = 0; i < NUM_PROBLEM; i++) {
    if (userguess->ans[i] == problem[i]) {
      userguess->black++;
      problemUsed[i] = 1;
      guessUsed[i] = 1;
    }
  }

  for (int i = 0; i < NUM_PROBLEM; i++) {
    if (guessUsed[i] == 0) {
      for (int j = 0; j < NUM_PROBLEM; j++) {
        if (problemUsed[j] == 0 && userguess->ans[i] == problem[j]) {
          userguess->white++;
          problemUsed[j] = 1;
          break;
        }
      }
    }
  }
}

void initializePopulation(ANSWER_T *populations) {
  for (int i = 0; i < POPULATION_SIZE; i++) {
    generateRandomGuess(populations[i].ans);
    populations[i].black = 0;
    populations[i].white = 0;
    populations[i].fitness = 0;
  }
}

void generateRandomGuess(char *str) {
  for (int i = 0; i < NUM_PROBLEM; i++) {
    str[i] = 'A' + (rand() % NUM_COLORS);
  }
  str[NUM_PROBLEM] = '\0';
}

// รับค่าปูชณียบุคคลที่เจ๋งที่สุดตลอดกาลเข้ามา
// Developer note : theGOAT = theGreatestOfAllTime is the member that has the
// most satisfy fitness value in the "hallOfFame" array
double calculateNewFitness(int round, ANSWER_T *theGOAT, ANSWER_T guess) {
  double retVal = 0;
  int difBlack = 0;
  int difWhite = 0;
  for (int i = 0; i < round; i++) {
    int b, w;
    b = theGOAT[i].black;
    w = theGOAT[i].white;
    gameEngine(theGOAT[i].ans, &guess);
    difBlack += abs(guess.black - b);
    difWhite += abs(guess.white - w);
  }
  // printf("\n%d %d\n", difBlack, difWhite);
  retVal = (difBlack * FITNESS_BLACK_VALUE) +
           (difWhite * FITNESS_WHITE_VALUE); // + NUM_PROBLEM * (round - 1);
  return retVal;
}

int addEntryToEHat(ANSWER_T *eHat, int *eHatElemNum, ANSWER_T elem) {
  if (*eHatElemNum == 0) {
    eHat[*eHatElemNum] = elem;
    *eHatElemNum += 1;
    return 0;
  } else {
    for (int i = 0; i < *eHatElemNum; i++) {
      if (strcmp((eHat[i]).ans, elem.ans) == 0) {
        return -1;
      }
    }
    eHat[*eHatElemNum] = elem;
    *eHatElemNum += 1;
  }
  return 0;
}

int addEntryToChild(ANSWER_T *eHat, int *eHatElemNum, char *child) {
  if (*eHatElemNum == 0) {
    for (int i = 0; i <= NUM_PROBLEM; i++) {
      eHat[*eHatElemNum].ans[i] = child[i];
    }
    *eHatElemNum += 1;
    return 0;
  } else {
    for (int i = 0; i < *eHatElemNum; i++) {
      if (strcmp((eHat[i]).ans, child) == 0) {
        return -1;
      }
    }
    for (int i = 0; i <= NUM_PROBLEM; i++) {
      eHat[*eHatElemNum].ans[i] = child[i];
    }
    *eHatElemNum += 1;
  }
  return 0;
}

void TournamentSelection(ANSWER_T *populations, char *parentA, char *parentB) {
  int tournamentSize = TOURNAMENTSIZE; // Adjust as needed
  int randIndexA, randIndexB;

  // Perform tournament selection for parent A
  randIndexA = rand() % POPULATION_SIZE;
  strcpy(parentA, populations[randIndexA].ans);

  for (int i = 1; i < tournamentSize; i++) {
    int currentIndex = rand() % POPULATION_SIZE;
    if (populations[currentIndex].fitness <= populations[randIndexA].fitness) {
      strcpy(parentA, populations[currentIndex].ans);
      randIndexA = currentIndex;
    }
  }

  // Perform tournament selection for parent B (ensuring diversity)
  randIndexB = (randIndexA + tournamentSize / 2) % POPULATION_SIZE;
  while (strcmp(parentA, populations[randIndexB].ans) == 0) {
    randIndexB = (randIndexB + 1) % POPULATION_SIZE;
  }
  strcpy(parentB, populations[randIndexB].ans);
}

void PropotionalRouletteWheelSelection(ANSWER_T *populations, char *parentA,
                                       char *parentB) {
  double totalFitness = 0.0;

  // Calculate the total fitness of the population
  for (int i = 0; i < POPULATION_SIZE; ++i) {
    totalFitness += populations[i].fitness;
  }

  // Generate random values for selection
  double randomValue1 = (double)rand() / RAND_MAX * totalFitness;
  double randomValue2;
  do {
    randomValue2 = (double)rand() / RAND_MAX * totalFitness;
  } while (randomValue2 == randomValue1);

  // Perform the first selection
  double partialSum = 0.0;
  int parentIndex1 = 0;
  for (int i = 0; i < POPULATION_SIZE; ++i) {
    partialSum += populations[i].fitness;
    if (partialSum >= randomValue1) {
      parentIndex1 = i;
      strcpy(parentA, populations[parentIndex1].ans);
      break;
    }
  }

  // Perform the second selection
  partialSum = 0.0;
  int parentIndex2 = 0;
  for (int i = 0; i < POPULATION_SIZE; ++i) {
    partialSum += populations[i].fitness;
    if (partialSum >= randomValue2) {
      parentIndex2 = i;
      strcpy(parentB, populations[parentIndex2].ans);
      break;
    }
  }
}

// void RankingSelection(ANSWER_T *populations, char *parentA, char *parentB) {
//   // Sort the population based on fitness (ascending order)
//   qsort(populations, POPULATION_SIZE, sizeof(ANSWER_T), cmpFitness);

//   // Calculate the selection probabilities based on rank
//   double totalProbability = 0.0;
//   for (int i = 0; i < POPULATION_SIZE; ++i) {
//     totalProbability += (double)(i + 1) / POPULATION_SIZE;
//   }

//   // Generate random values for selection
//   double randomValue1 = (double)rand() / RAND_MAX * totalProbability;
//   double randomValue2 = (double)rand() / RAND_MAX * totalProbability;

//   // Perform the first selection
//   double partialSum = 0.0;
//   int parentIndex1 = 0;
//   for (int i = 0; i < POPULATION_SIZE; ++i) {
//     partialSum += (double)(i + 1) / POPULATION_SIZE;
//     if (partialSum >= randomValue1) {
//       parentIndex1 = i;
//       break;
//     }
//   }

//   // Perform the second selection
//   partialSum = 0.0;
//   int parentIndex2 = 0;
//   for (int i = 0; i < POPULATION_SIZE; ++i) {
//     partialSum += (double)(i + 1) / POPULATION_SIZE;
//     if (partialSum >= randomValue2) {
//       parentIndex2 = i;
//       break;
//     }
//   }
// }

void crossingOver(char *parentA, char *parentB) {
  int rate = rand() % 100;
  if (rate < CROSSOVER_RATE) {
    singlePointCrossOver(parentA, parentB);
  } else {
    twoPointCrossOver(parentA, parentB);
  }
}

void singlePointCrossOver(char *parentA, char *parentB) {
  int p1 = rand() % NUM_PROBLEM;
  for (int i = p1; i < NUM_PROBLEM; i++) {
    char temp = parentA[i];
    parentA[i] = parentB[i];
    parentB[i] = temp;
  }
  parentA[NUM_PROBLEM] = '\0';
  parentB[NUM_PROBLEM] = '\0';
}

void twoPointCrossOver(char *parentA, char *parentB) {
  int p1 = rand() % NUM_PROBLEM;
  int p2 = rand() % NUM_PROBLEM;
  if (p1 > p2) {
    int temp1 = p1;
    p1 = p2;
    p2 = temp1;
  }
  for (int i = p1; i <= p2; i++) {
    char temp2 = parentA[i];
    parentA[i] = parentB[i];
    parentB[i] = temp2;
  }
  parentA[NUM_PROBLEM] = '\0';
  parentB[NUM_PROBLEM] = '\0';
}

void mutate(char *guess) {
  int rate = rand() % 100;
  if (rate < MUTATION_RATE) {
    guess[rand() % NUM_PROBLEM] = 'A' + (rand() % NUM_COLORS);
    guess[NUM_PROBLEM] = '\0';
  }
}

void inversion(char *code) {
  int rate = rand() % 100;
  if (rate < INVERSION_RATE) {
    int start = rand() % NUM_PROBLEM;
    int end = rand() % NUM_PROBLEM;

    if (start > end) {
      int temp = start;
      start = end;
      end = temp;
    }

    while (start == end) {
      end = rand() % NUM_PROBLEM;
    }

    while (start < end) {
      char temp = code[start];
      code[start] = code[end];
      code[end] = temp;
      start++;
      end--;
    }
    code[NUM_PROBLEM] = '\0';
  }
}

void mutateswap(char *code) {
  int rate = rand() % 100;
  if (rate < PERMUTATION_RATE) {
    int point1 = rand() % NUM_PROBLEM;
    int point2 = rand() % NUM_PROBLEM;

    while (point1 == point2)
      point2 = rand() % NUM_PROBLEM;

    char temp = code[point1];
    code[point1] = code[point2];
    code[point2] = temp;
    code[NUM_PROBLEM] = '\0';
  }
}