#include "main.h"


// InputQuintuple

// Alphabet - Columns
int ***CreateRaw(int alphabet, int transitionQuantity)
{

    int i, j, k;
    int ***matrix = (int ***) malloc(transitionQuantity * sizeof(int **)); // alocação da matriz
    int depth = 4;
    for (i = 0; i < transitionQuantity; i++)
    {
        matrix[i] = (int **) malloc(alphabet * sizeof(int *)); // alocação de cada linha da matriz
        for (j = 0; j < alphabet; j++)
        {
            matrix[i][j] = (int *) malloc(depth * sizeof(int));
            for (k = 0; k < depth; k++)
            {
                matrix[i][j][k] = i + j + k; // atribuição de valores para cada elemento da matriz
            }
        }
    }
    return matrix;
}


FILE *OpenFileAndCheck(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Erro abrindo arquivo de texto!");
        exit(1);
    }
    return file;
}

InputQuintuple CreateInputQuintupleFromFile(const char *filePath)
{
    // Abre o arquivo e checa se deu tudo OK
    FILE *file = OpenFileAndCheck(filePath);
    int i;
    // Declaracao das variaveis da quintupla
    char alphabet[10];
    int stateQuantity;
    int finalStateQuantity;
    int *finalStates;
    int transitionQuantity;
    int ***rawTransitions;
    int inputQuantity;
    char **input;

    // Primeira Linha: Alfabeto

    fgets(&alphabet, 10, file);
    // Segunda Linha: Quantidade de Estados

    fscanf(file, "%d", &stateQuantity);
    // Terceira Linha: Quantidade de Estados finais
    fscanf(file, "%d", &finalStateQuantity);

    // Quarta Linha: Estados finais
    // Ex: EstadosFinais = {0,1,1} ou seja, q1 e q2
    finalStates = (int *) calloc(stateQuantity, sizeof(int));
    for (i = 0; i < finalStateQuantity; i++)
    {
        int finalState;
        fscanf(file, "%d", &finalState);
        finalStates[finalState] = 1;
    }
    // QuintaLinha: Numero de Transicoes
    fscanf(file, "%d", &transitionQuantity);

    // SextaLinha: Transicoes:
    // TODO: Matrix
    Transition* transitions = (Transition*) malloc(sizeof(Transition) * transitionQuantity);
    for (int i = 0; i < transitionQuantity; i++)
    {
        int CurrentState;
        char Symbol;
        int NextState;
        fscanf(file, "%d", &CurrentState);
        fgetc(file);
        fscanf(file, "%c", &Symbol);
        fgetc(file);
        fscanf(file, "%d", &NextState);
        Transition transition = {CurrentState, Symbol, NextState};
        transitions[i] = transition;
    }

    // SetimaLinha: Numero de Entradas
    fscanf(file, "%d", &inputQuantity);
    fgetc(file);
    input = (char*) malloc(sizeof (char*) * inputQuantity);
    for (i = 0; i < inputQuantity; i++)
    {
        input[i] = malloc(sizeof(char) * 100);
        char* symbols = input[i];
        fgets(symbols, 100, file);
        char* c = strchr(symbols, '\n');
        if (c) *c = 0;
        input[i] = symbols;
    }

    // Fecha o arquivo
    fclose(file);
    InputQuintuple quintuple = {
            alphabet,
            stateQuantity,
            finalStateQuantity,
            finalStates,
            transitionQuantity,
            transitions,
            inputQuantity,
            input
    };
    return quintuple;
}

void FreeInputQuintuple(InputQuintuple *quintuple)
{
    int i;
    free(quintuple->FinalStates);
    for (i = 0; i < quintuple->InputQuantity; i++)
    {
        free(quintuple->Input[i]);
    }
    free(quintuple->Input);
}

int main(int argc, char **argv)
{
    const char *filePath = "entrada.txt";
    if (argc == 2) filePath = argv[1];

    InputQuintuple inputQuintuple = CreateInputQuintupleFromFile(filePath);
    printf(filePath);

    FreeInputQuintuple(&inputQuintuple);
    return 0;
}
