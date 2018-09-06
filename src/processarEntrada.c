#include "montador.h"
#include <stdio.h>

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

    while(i<tamanho && entrada[i]!=0 && entrada[i]!='\n' && entrada[i]!='\0') {
        palavra[j]=entrada[i];
        j++;
    }
    palavra[j]='\0';
    if(palavra[0]=='.') { //o token pode ser uma diretiva
        novoToken.tipo = "Diretiva";
        //copia a palavra
    }



    free(palavra);
    return 0;
}
