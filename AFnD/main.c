#include "main.h"

#define PRINT_TRANSITIONS 1

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
    // Declaracao das variaveis da quintupla e da quintupla
    InputQuintuple quintuple;
    int stateQuantity;
    int finalStateQuantity;
    int *finalStates;
    int transitionQuantity;
    int ***rawTransitions;
    int inputQuantity;
    char **input;

    // Primeira Linha: Alfabeto

    fgets(&quintuple.Alphabet, MAX_SYMBOLS, file);
    // TODO: Colocar em ordem alfabetica

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
    memset(quintuple.Transitions, 0, sizeof(quintuple.Transitions));
    for (int i = 0; i < transitionQuantity; i++)
    {
        int CurrentState;
        char Symbol;
        int NextState;
        fscanf(file, "%d", &CurrentState);
        fgetc(file); // Pulando os caracteres de quebra de linha
        fscanf(file, "%c", &Symbol);
        fgetc(file); // Pulando os caracteres de quebra de linha
        fscanf(file, "%d", &NextState);
        quintuple.Transitions[CurrentState][Symbol - 'a'][NextState] = 1;
    }
    // SetimaLinha: Numero de Entradas
    fscanf(file, "%d", &inputQuantity);
    fgetc(file);
    input = (char*) malloc(sizeof (char*) * inputQuantity);
    for (i = 0; i < inputQuantity; i++)
    {
        input[i] = malloc(sizeof(char) * MAX_INPUT_SIZE);
        char* symbols = input[i];
        fgets(symbols, MAX_INPUT_SIZE, file);
        char* c = strchr(symbols, '\n');
        if (c) *c = 0;
        input[i] = symbols;
    }

    // Fecha o arquivo
    fclose(file);
    // Atribui os valores a quintuple
    quintuple.StateQuantity = stateQuantity;
    quintuple.FinalStateQuantity = finalStateQuantity;
    quintuple.FinalStates = finalStates;
    quintuple.TransitionQuantity = transitionQuantity;
    quintuple.InputQuantity = inputQuantity;
    quintuple.Input = input;
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

void PrintTransitions(InputQuintuple quintuple)
{
    int i,j,k;
    int alphabetActualSize = 0;
    char* ptr = quintuple.Alphabet;
    while (*ptr != '\n')
    {
        alphabetActualSize++;
        ptr++;
    }

    for (i = 0; i < quintuple.StateQuantity; i++)
    {
        printf("Linha q%d: ", i);
        for (j = 0; j < alphabetActualSize; j++)
        {
            printf("Coluna %c[ ", j + 'a');
            for (k = 0; k < quintuple.StateQuantity; k++)
            {
                printf("%d ", quintuple.Transitions[i][j][k]);
            }
            printf("] \t ");
        }
        printf("\n");
    }
}


int ProcessWord(InputQuintuple quintuple, char* word)
{
    return 0;
}

void ProcessInput(InputQuintuple quintuple)
{
    
}

int main(int argc, char **argv)
{
    const char *filePath = "entrada.txt";
    if (argc == 2) filePath = argv[1];

    InputQuintuple inputQuintuple = CreateInputQuintupleFromFile(filePath);


    if (PRINT_TRANSITIONS)
    {
        PrintTransitions(inputQuintuple);
    }


    FreeInputQuintuple(&inputQuintuple);
    return 0;
}
