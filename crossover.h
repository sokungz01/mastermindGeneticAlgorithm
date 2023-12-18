#ifndef CROSS_H
#define CROSS_H
#include "GA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void gameEngine(char *problem, ANSWER_T *userguess);
void initializePopulation(ANSWER_T *populations);
void generateRandomGuess(char *str);
double calculateNewFitness(int round, ANSWER_T *theGOAT, ANSWER_T guess);
void TournamentSelection(ANSWER_T *populations, char *parentA, char *parentB);
void PropotionalRouletteWheelSelection(ANSWER_T *populations, char *parentA, char *parentB);
void RankingSelection(ANSWER_T *populations, char *parentA, char *parentB);
void mutate(char *guess);
void inversion(char *code);
int addEntryToEHat(ANSWER_T *eHat, int *eHatElemNum, ANSWER_T elem);
int addEntryToChild(ANSWER_T *eHat, int *eHatElemNum, char *child);
void mutateswap(char *code);
void crossingOver(char *parentA, char *parentB);
void singlePointCrossOver(char *parentA, char *parentB);
void twoPointCrossOver(char *parentA, char *parentB);
#endif