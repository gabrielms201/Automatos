#include <stdlib.h>
#include <stdio.h>
#include <ctype.h> 
#include <string.h>

//#define LOG_DEBUG
char* g_input = NULL;
long g_charCount = 0;
long g_currentLine = 1;


void Error()
{
    fprintf(stderr, "Falha na derivacao.\n Erro na linha %ld", g_currentLine);
    exit(1);
}

// File && String Operations
long GetFileSize(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp) + 1; // numberOfBytes + '\0'
    fseek(fp, 0, SEEK_SET);
    return size;
}

char* PutFileContentIntoString(FILE* fp, long charCount)
{
    char* input = (char*) calloc(charCount, sizeof(char));
    
    int i = 0;
    while(!feof(fp))
    {   
        int result = fscanf(fp, "%c", &input[i]);
        if (!result)
        {
            fprintf(stderr, "ERRO AO LER ARQUIVO!\n");
            exit(1);
        }
        i++;
    }
    return input;
}

FILE* OpenFileAndCheck(const char* fileName)
{
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "ERRO AO ABRIR ARQUIVO!\n");
        exit(1);
    }
    return fp;
}

// Grammar Functions

/// <summary>
/// Returns the number of characters to skip
/// </summary>
/// <returns>Number of characters we should skip</returns>
int GetNumberOfCharactersToSkip()
{
    int isLineBreak = *g_input == '\n';
    int isEscape = *g_input == '\r';
    int isTab = *g_input == '\t';
    int isSpace = *g_input == ' ';
    int isComment = *g_input == '/' && *(g_input + 1) != '\0' && *(g_input + 1) == '/';
    
    if (isLineBreak) g_currentLine++;
    if (isComment) return 2;
    return isLineBreak || isEscape || isTab || isSpace || isComment;

}

/// <summary>
/// Goes to the next terminal given certain conditions
/// </summary>
void NextTerminal()
{
    int numberOfCharactersToSkip = GetNumberOfCharactersToSkip();
    while (numberOfCharactersToSkip)
    {
        #ifdef LOG_DEBUG
            printf("~> [DEBUG] JUMPING CHARACTER: [%c]\n", *g_input);
            printf("\t\t Current Value: {%s}\n", g_input);
        #endif
        g_input += numberOfCharactersToSkip;
        if (numberOfCharactersToSkip == 2)
        {
            while (*g_input != '\n' && *g_input != '\0')
            {
                g_input++;
            }
        }
        numberOfCharactersToSkip = GetNumberOfCharactersToSkip();
    }
}

void Letra()
{
    if (isalpha(*g_input))
    {
        g_input++;
        NextTerminal();
    }
    else
    {
        Error();
    }
}
void Variavel()
{
    //while ()
    //Letra();

    while (isalpha(*g_input))
    {
        g_input++;
        NextTerminal();
        Variavel();
    }
}
// Não terminais
void Declaracao()
{
    switch (*g_input)
    {
    case 'i':
        g_input++;
        NextTerminal();
        if (*g_input == 'n')
        {
            g_input++;
            NextTerminal();
            if (*g_input == 't')
            {
                g_input++;
                NextTerminal();
                Variavel();
                if (*g_input == ';')
                {
                    g_input++;
                    NextTerminal();
                    break;
                }
                else Error();
            }
            else Error();
        }
        else Error();
    default:
        Error();
    }
}
void Declaracoes()
{
    Declaracao();
    //todo

}
void Comandos()
{
    //todo
    return;
}
void Programa()
{
    Declaracoes();
    Comandos();
}


//
void StartRecognizer()
{
    if (g_input == NULL)
    {
        fprintf(stderr, "ERRO AO ABRIR ARQUIVO!\n");
        exit(1);
    }
    // Start Analyzing
    NextTerminal();
    Declaracao();
    // Finishes
    if (g_input != NULL && *g_input == '\0')
    {
        printf("Palavra reconhecida.\n ~> %ld Linha(s) Analizadas. \n", g_currentLine);
    }
    else
    {
        fprintf(stderr, "Falha na derivacao.\n Erro na linha %ld", g_currentLine);
    }

    //return 0;
    //switch (*g_input)
    //{
    //default:
    //    break;
    //}
}



int main(int argc, char** argv)
{
    const char* fileName = argc == 2 ? argv[1] : "input.txt";
    FILE* fp = OpenFileAndCheck(fileName);
    
    g_charCount = GetFileSize(fp);
    g_input = PutFileContentIntoString(fp, g_charCount);
    printf("Numero de Bytes: %ld\n", g_charCount);
    printf("INPUT: \n```\n%s\n```\n", g_input);
    // Since we're going to move the input pointer to check if the grammar is valid, we MUST
    // storage a reference to the beggining of the input, otherwise, we can't free this memory block
    char* originalInputPointer = g_input;

    // Playground for a while
    NextTerminal();

    // EndPlayground

    StartRecognizer();

    
    free(originalInputPointer);
    fclose(fp);
}