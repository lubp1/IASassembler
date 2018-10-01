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
    novoToken.palavra = malloc(tamanho*sizeof(char));


    for(i=0;i<tamanho;i++) { //transformando toda a string em lowercase
      if(entrada[i]>='A' && entrada[i]<='Z') {
        entrada[i] = entrada[i] + 32;
      }
    }

    i=0;
    //o int i eh o contador para percorrer a entrada e o int j o contador para cada palavra
    while(entrada[i]!='\0') {
loop: j=0;

      if(entrada[i]=='#') { //eh uma linha de comentario, que deve ser ignorada
        while(entrada[i]!='/n')
          i++;
        linha++;
        i++;
        goto loop;
      }

      while(i<tamanho && entrada[i]!=' ' && entrada[i]!='\n') { //le uma palavra
          palavra[j]=entrada[i];
          i++;
          j++;
      }
      while(entrada[i]=' ' && i<tamanho) { //remove os espacos
          i++;
      }

      palavra[j]='\0';

      if(palavra[j-1]=='\n') {
        linha++;
        palavra[j-1] = '\0';
      }

      if(palavra[0]=='.') { //possivel diretiva
          novoToken.tipo = "Diretiva";
      }
      else if (palavra[j-1]==':') { //possivel rotulo
          novoToken.tipo = "DefRotulo";
          palavra[j-1] = '\0';
      }
      else if (palavra[0]=='0' && palavra[1]=='x') { //possivel hexadecimal
          novoToken.tipo = "Hexadecimal";
      }
      else if (ehDecimal(palavra)) { //numero decimal
          novoToken.tipo = "Decimal";
      }
      else {
          novoToken.tipo = "Nome";
      }
      strcpy(novoToken.palavra,palavra);
      novoToken.linha = linha;
      adicionarToken(novoToken);

  }


    free(palavra);
    free(novoToken.palavra);
    free(novoToken);
    return 0;
}


int ehDecimal(char* dec) {

  while(char[i]!='\0') {
    if(char[i]<0 || char[i]>9)
      return 0;
  }
  return 1;
}
