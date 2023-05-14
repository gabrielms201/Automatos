#pragma once
// main.h
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h> 
#include <string.h>


// Utils
void Error();

// File && String Operations
long GetFileSize(FILE* fp);
char* PutFileContentIntoString(FILE* fp, long charCount);
FILE* OpenFileAndCheck(const char* fileName);


// Conditions to Skip ( \n - \t - \r - \\ - " " )
int GetNumberOfCharactersToSkip();
void NextTerminal();

// Derivacoes
void Variavel();
void Declaracao();
void Declaracoes();
void Atribuicao();
void Comandos();
void Programa();
void StartRecognizer();
// ~main.h