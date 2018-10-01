#include "montador.h"
#include <stdio.h>

/* Retorna:
 *  1 caso haja erro na montagem;
 *  0 caso não haja erro.
 *
 * Esse é o arquivo principal utilizado para chamar as rotinas de processamento de entrada e emissão de código
 * Para testar a parte 1, não escreva nada na função emitirMapaDeMemoria();
 * Para testar a parte 2, comente a linha 40 imprimeListaTokens()
 * Esta função será utilizada como está, portanto, não altere-a, nem adicione coisas neste arquivo
 */
int main(int args, char** argv)
{
  int ret = 0;

  if (args < 2) {
    printf("Você deve passar o nome do arquivo como parâmetro!\n");
    return 1;
  }

  char entrada[4096 + 1];
  FILE *fp = fopen(argv[1], "r");
  size_t novoTam = 0;
  if (fp != NULL) {
    novoTam = fread(entrada, sizeof(char), 4096, fp);
    if ( ferror( fp ) != 0 )
      fputs("Error ao abrir arquivo!\n", stderr);
    else
      entrada[novoTam++] = '\0';
    fclose(fp);
  }

  if ( (ret=processarEntrada(entrada, novoTam)) ) {
    // Caso haja erro no processamento de entrada, retorne.
    return ret;
  }

  //Senão imprime os tokens coletados
  imprimeListaTokens();

  return emitirMapaDeMemoria();
}
