#include "main.h"

#define PRINT_TRANSITIONS 0

FILE* OpenFileAndCheck(const char* filePath)
{
	FILE* file = fopen(filePath, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Erro abrindo arquivo de texto!");
		exit(1);
	}
	return file;
}

InputQuintuple CreateInputQuintupleFromFile(const char* filePath)
{
	// Abre o arquivo e checa se deu tudo OK
	FILE* file = OpenFileAndCheck(filePath);
	int i;
	// Declaracao das variaveis da quintupla e da quintupla
	InputQuintuple quintuple;
	int stateQuantity;
	int finalStateQuantity;
	int* finalStates;
	int transitionQuantity;
	int inputQuantity;
	char** input;

	// Primeira Linha: Alfabeto
	fgets(quintuple.Alphabet, MAX_SYMBOLS, file);
	int n = strcspn(quintuple.Alphabet, "\n");
	// Substitui o '\n' por um '\0'
	quintuple.Alphabet[n] = '\0';

	// Segunda Linha: Quantidade de Estados
	fscanf(file, "%d", &stateQuantity);
	// Terceira Linha: Quantidade de Estados finais
	fscanf(file, "%d", &finalStateQuantity);

	// Quarta Linha: Estados finais
	// Ex: EstadosFinais = {0,1,1} ou seja, q1 e q2
	finalStates = (int*)calloc(stateQuantity, sizeof(int));
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
	input = (char*)malloc(sizeof(char*) * inputQuantity);
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
	quintuple.InitialState = 0;
	return quintuple;
}

void FreeInputQuintuple(InputQuintuple* quintuple)
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
	int i, j, k;
	int alphabetActualSize = 0;
	char* ptr = quintuple.Alphabet;
	while (*ptr != '\0')
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

int ShouldUnion(int size, int* arr)
{
	int i;
	int count = 0;
	for (i = 0; i < size; i++)
	{
		if (arr[i] == 1) count++;
	}
	if (count > 1) return 1;
	return 0;
}
void Intersection(int size, int* arr1, int* arr2, int* result)
{
	int i;
	for (i = 0; i < size; i++)
	{
		result[i] = arr1[i] && arr2[i];
	}
}
void Union(int size, int* arr1, int* arr2, int* result)
{
	int i;
	for (i = 0; i < size; i++)
	{
		result[i] = arr1[i] || arr2[i];
	}
}

int ProcessWord(InputQuintuple quintuple, char* word)
{
	int i;
	int q = quintuple.InitialState;
	int R[MAX_STATES] = { 0 };
	memcpy(R, quintuple.Transitions[q][*word - 'a'], MAX_STATES);
	word++;
	while (*word != '\0')
	{
		int* a = NULL;
		int* b = NULL;
		int captureiA = 0;
		for (i = 0; i < quintuple.StateQuantity; i++)
		{
			if (R[i] == 1)
			{
				if (!captureiA)
				{
					a = quintuple.Transitions[i][*word - 'a'];
					captureiA = 1;
				}
				else
				{
					b = quintuple.Transitions[i][*word - 'a'];
				}
			}
		}

		if (b != NULL)
		{
			Union(quintuple.StateQuantity,
				a, b, R);
		}
		else
		{
			if (a == NULL)
			{
				return 0;
			}
			for (i = 0; i < quintuple.StateQuantity; i++)
			{
				R[i] = a[i];
			}
		}

		word++;
	}

	// Realiza a interseccao com R, e verifica se há um estado final no resultado.
	// havendo um estado final, retorna true
	int output[MAX_STATES] = { 0 };
	Intersection(quintuple.StateQuantity, R, quintuple.FinalStates, output);
	for (i = 0; i < quintuple.StateQuantity; i++)
	{
		if (output[i] == 1)
		{
			return 1;
		}
	}
	return 0;
}

void ProcessInput(InputQuintuple quintuple)
{
	int i;

	for (i = 0; i < quintuple.InputQuantity; i++)
	//for (i = 0; i < 1; i++)
	{

		char* word = quintuple.Input[i];
		//char* word = "ababb";
		int validation = ProcessWord(quintuple, word);
		printf("%d: %s ", i + 1, word);
		if (validation)
		{
			printf("OK\n");
		}
		else
		{
			printf("not OK\n");
		}
	}
}

int main(int argc, char** argv)
{
	// Caso seja fornecido o arquivo de entrada via linha de comando, jogamos esse valor pra variável;
	const char* filePath = "entrada.txt";
	if (argc == 2) filePath = argv[1];
	// Cria a quintupla
	InputQuintuple inputQuintuple = CreateInputQuintupleFromFile(filePath);


	if (PRINT_TRANSITIONS)
	{
		PrintTransitions(inputQuintuple);
	}

	ProcessInput(inputQuintuple);
	FreeInputQuintuple(&inputQuintuple);
	return 0;
}
