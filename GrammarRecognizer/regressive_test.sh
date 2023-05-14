#!/bin/bash

# Define a pasta onde estão os arquivos a serem processados
PASTA_ARQUIVOS="./tests"

# Define o caminho completo para o executável "reconhecedor.exe"
EXECUTAVEL="./reconhecedor"

# Variável para armazenar o estado de erro (0 = sem erro, 1 = erro)
ERRO=0

VERDE='\033[0;32m'
VERMELHO='\033[0;31m'
RESET='\033[0m'
AZUL='\033[0;34m'



# Itera sobre cada arquivo da pasta
for ARQUIVO in $PASTA_ARQUIVOS/*
do
  # Verifica se o item iterado é um arquivo (e não uma pasta ou outro tipo de item)
  if [ -f "$ARQUIVO" ]; then
    # Verifica se o nome do arquivo começa com "FAIL"
    if [[ "$ARQUIVO" == *"FAIL"* ]]; then
      echo -e "${AZUL}~ [OK] O arquivo $ARQUIVO falhou como esperado. ${RESET}"
    else
      # Executa o programa "reconhecedor.exe" com o argumento do caminho desse arquivo
      OUTPUT=$("$EXECUTAVEL" "$ARQUIVO" 2>/dev/null)
      
      # Verifica se a string "Palavra reconhecida" foi printada no console
      if echo "$OUTPUT" | grep -q "Palavra reconhecida"; then
        echo -e "${VERDE}~ [OK] O arquivo $ARQUIVO passou como esperado. ${RESET}"
      else
        # Se houver um erro, define a variável "ERRO" para 1 e imprime o caminho do arquivo
        ERRO=1
        echo "${VERMELHO}~ [ERROR] Erro no arquivo: $ARQUIVO ${RESET}"
      fi
    fi
  fi
done

# Verifica se houve um erro e imprime uma mensagem correspondente
if [ $ERRO -eq 1 ]; then
  echo ""
  echo "Houve um erro ao processar os arquivos."
else
  echo ""
  echo "-> Todos os arquivos foram processados com sucesso como esperado"
fi