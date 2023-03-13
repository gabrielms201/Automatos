#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 10
#define MAX_STATES 20
#define MAX_INPUT_SIZE 100


// Structs
typedef struct
{
    int CurrentState;
    char Symbol;
    int NextState;
} Transition;

typedef struct
{
    char Alphabet[MAX_SYMBOLS];
    int StateQuantity;
    int FinalStateQuantity;
    int* FinalStates;
    int TransitionQuantity;
    int Transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
    int InputQuantity;
    char** Input;
} InputQuintuple ;

// Functions
InputQuintuple CreateInputQuintupleFromFile(const char* filePath);
void FreeInputQuintuple(InputQuintuple* quintuple);
FILE* OpenFileAndCheck(const char* filePath);
void PrintTransitions(InputQuintuple quintuple);
int ProcessWord(InputQuintuple quintuple, char* word);
void ProcessInput(InputQuintuple quintuple);
void Union(int size, int* arr1, int* arr2, int* result);
void Intersection(int size, int* arr1, int* arr2, int* result);
#endif