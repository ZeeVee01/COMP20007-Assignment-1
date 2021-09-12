/*
problem3.c

Driver function for Problem 3.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

/* Constants */
#define OLDCHIP 0
#define NEWCHIP 1
#define MAXNUMERATOR 100
#define MAXDENOMINATOR 100

/* Used to store all the statistics for a single chip. */
struct statistics;

/* Used to store all the statistics for both chips for each problem. */
struct chipStatistics;

struct statistics {
  int operations;
  int instances;
  int minOperations;
  double avgOperations;
  int maxOperations;
};

struct chipStatistics {
  struct statistics oldChipEuclid;
  struct statistics newChipEuclid;
  struct statistics oldChipSieve;
  struct statistics newChipSieve;
};

/* Set all statistics to 0s */
void initialiseStatistics(struct statistics *stats);

/* Collects the minimum, average and maximum operations from running all
combinations of numerators from 1 to the given maxNumerator and 1 to the given
maxDenominator. */
void collectStatistics(struct chipStatistics *chipStats, int maxNumerator,
  int maxDenominator);

/* Divides the number of operations by the number of instances. */
void calculateAverage(struct statistics *stats);

/* Prints out the minimum, average and maximum operations from given
statistics. */
void printStatistics(struct statistics *stats);

/* Calculates the number of operations required for Euclid's algorithm given the
numerator and denominator when running on the given chip type (one of OLDCHIP
and NEWCHIP) by moving through the steps of the algorithm and counting each
pseudocode operation. */
void euclid(int numerator, int denominator, int chip, struct statistics *s);

/* Calculates the number of operations required for the sieve of Eratosthenes
given the numerator and denominator when running on the given chip type (one of
OLDCHIP and NEWCHIP) by moving through the steps of the algorithm and counting
each pseudocode operation. */
void eratosthenes(int numerator, int denominator, int chip,
  struct statistics *s);

int main(int argc, char **argv){
  struct chipStatistics summaryStatistics;

  collectStatistics(&summaryStatistics, MAXNUMERATOR, MAXDENOMINATOR);

  printf("Old chip (Euclid):\n");
  printStatistics(&(summaryStatistics.oldChipEuclid));
  printf("\n");
  printf("New chip (Euclid)\n");
  printStatistics(&(summaryStatistics.newChipEuclid));
  printf("\n");
  printf("Old chip (Sieve)\n");
  printStatistics(&(summaryStatistics.oldChipSieve));
  printf("\n");
  printf("New chip (Sieve)\n");
  printStatistics(&(summaryStatistics.newChipSieve));
  printf("\n");

  return 0;
}

void collectStatistics(struct chipStatistics *chipStats, int maxNumerator,
  int maxDenominator){
  int numerator, denominator;
  /* Initialise all statistics */
  initialiseStatistics(&(chipStats->oldChipEuclid));
  initialiseStatistics(&(chipStats->newChipEuclid));
  initialiseStatistics(&(chipStats->oldChipSieve));
  initialiseStatistics(&(chipStats->newChipSieve));

  for(numerator = 1; numerator <= maxNumerator; numerator++){
    for(denominator = 1; denominator <= maxDenominator; denominator++){
      /* Run algorithms for all combinations of numerator and denominator. */
      euclid(numerator, denominator, OLDCHIP,
        &(chipStats->oldChipEuclid));
      euclid(numerator, denominator, NEWCHIP,
        &(chipStats->newChipEuclid));
      eratosthenes(numerator, denominator, OLDCHIP,
        &(chipStats->oldChipSieve));
      eratosthenes(numerator, denominator, NEWCHIP,
        &(chipStats->newChipSieve));
    }
  }
  calculateAverage(&(chipStats->oldChipEuclid));
  calculateAverage(&(chipStats->newChipEuclid));
  calculateAverage(&(chipStats->oldChipSieve));
  calculateAverage(&(chipStats->newChipSieve));
}

void calculateAverage(struct statistics *stats){
  stats->avgOperations = (double) stats->operations / stats->instances;
}

void initialiseStatistics(struct statistics *stats){
  stats->operations = 0;
  stats->instances = 0;
  stats->minOperations = INT_MAX;
  stats->avgOperations = 0;
  stats->maxOperations = 0;
}

void euclid(int numerator, int denominator, int chip, struct statistics *s){
  
  int operations = 0;
  int t;

  int b = numerator;
  int a = denominator;

  operations += 2; // for assignment operatations

  while (b != 0){
    operations += 1; // for while branch check

    t = b;
    b = a % b;
    a = t;

    operations += 8; // for 3 assignments and one modulus

    //printf("%d/%d\n", (numerator / a), (denominator / a));
  }

  operations += 1; // to cover for last while branch check, that doesnt enter loop


  if (operations < s->minOperations) {
    s->minOperations = operations;
  }
  if (operations > s->maxOperations) {
    s->maxOperations = operations;
  }
  s->instances = s->instances + 1;
  s->operations = s->operations + operations;

}

void eratosthenes(int numerator, int denominator, int chip,
  struct statistics *s){
  
  int operations = 0;
  
  int num = numerator;
  int den = denominator;
  int numCandidates = den;
  int i = 1;

  // Not in Psuedocode
  if (num < den) {
    numCandidates = num;
  } 


  int primes[MAXNUMERATOR] = {0};
  for(int k=0; k<MAXNUMERATOR; k++) {
    primes[k] = 1;
  }

  operations += 5; // for initial 5 assignments
  

  while(i < numCandidates) {
    i = i + 1;
    operations += 2; // for while check and reassignment of i


    operations += 1; // for if check
    if (primes[i]) {

      // Entire statement below costs 1 on newchip
      if (chip == NEWCHIP) {operations += 1;}
      
      
      int j = i + i;
      if (chip == OLDCHIP) {operations += 1;} // assignment


      while (j <= numCandidates){
        if (chip == OLDCHIP) {operations += 1;} // While loop
  
        if (chip == OLDCHIP) {operations += 11;} // If statment below
        if ((j/i > 1) && (j%i == 0 )){

          primes[j] = 0;
          if (chip == OLDCHIP) {operations += 1;} // assignment

        }

        j += i;
        if (chip == OLDCHIP) {operations += 1;} // assignment

      }

      if (chip == OLDCHIP) {operations += 1;} // for last while check that doesnt pass guard


      while (((num % i) == 0) && ((den % i) == 0)) {
        num = num / i;
        den = den / i;
        operations += 23; // for while, 2x mod and 2x assignments
      }

      operations += 11; // for last while check that doesnt pass guard



    }
  }

  operations += 1; // for last while check that doesnt pass guard

  //printf("%d/%d", num, den);

  if (operations < s->minOperations) {
    s->minOperations = operations;
  }
  if (operations > s->maxOperations) {
    s->maxOperations = operations;
  }
  s->instances = s->instances + 1;
  s->operations = s->operations + operations;
  
}

void printStatistics(struct statistics *stats){
  printf("Minimum operations: %d\n", stats->minOperations);
  printf("Average operations: %f\n", stats->avgOperations);
  printf("Maximum operations: %d\n", stats->maxOperations);
}

