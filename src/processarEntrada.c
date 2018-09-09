#include "montador.h"
#include <stdio.h>
#include <string.h>

/*
Exemplo de erros:
const char* get_error_string (enum errors code) {
    switch (code) {
        case ERR_HEXADECIMAL_NAO_VALIDO:
            return "LEXICO: Numero Hexadecimal Inválido!";
        case ERRO_ROTULO_JA_DEFINIDO:
            return "GRAMATICAL: ROTULO JA FOI DEFINIDO!";
        case ERR_DIRETIVA_NAO_VALIDA:
            return "LEXICO: Diretiva não válida";
*/

/*
    ---- Você Deve implementar esta função para a parte 1.  ----
    Entrada da função: arquivo de texto lido e seu tamanho
    Retorna:
        * 1 caso haja erro na montagem;
        * 0 caso não haja erro.
*/
int processarEntrada(char* entrada, unsigned tamanho)
{
    int i=0, j=0, linha=0;
    char* palavra = malloc(tamanho*sizeof(char));
    token novoToken = malloc(sizeof(token));


    while (entrada[i]!='\0') {
      j=0;
      while(i<tamanho && entrada[i]!=' ' && entrada[i]!='\n') {
          palavra[j]=entrada[i];
          i++;
          j++;
      }

      palavra[j]='\0';
      i++;
      if(palavra[j-1]=='\n') {
        linha++;
      }

      if(palavra[0]=='.') { //possivel diretiva
          novoToken.tipo = "Diretiva";
      }
      else if (palavra[j-1]==':') { //possivel rotulo
          novoToken.tipo = "DefRotulo";
      }
      else if (palavra[0]=='0' && palavra[1]=='x') { //possivel hexadecimal
          novoToken.tipo = "Hexadecimal";
      }
      strcpy(novoToken.palavra,palavra);
      novoToken.linha = linha;
      adicionarToken(novoToken);

  }


    free(palavra);
    free(novoToken);
    return 0;
}
