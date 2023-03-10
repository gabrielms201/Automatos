#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>


// Structs
typedef struct
{
    int CurrentState;
    char Symbol;
    int NextState;
} Transition;

typedef struct
{
    char* Alphabet;
    int StateQuantity;
    int FinalStateQuantity;
    int* FinalStates;
    int TransitionQuantity;
    Transition* Transitions;
    //int*** RawTransitions;
    int InputQuantity;
    char** Input;
} InputQuintuple ;

// Functions
InputQuintuple CreateInputQuintupleFromFile(const char* filePath);
void FreeInputQuintuple(InputQuintuple* quintuple);
FILE* OpenFileAndCheck(const char* filePath);


#endif