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
    token tokensLinha = malloc(20*sizeof(token));


    for(i=0;i<tamanho;i++) { //transformando toda a string em lowercase
      if(entrada[i]>='A' && entrada[i]<='Z') {
        entrada[i] = entrada[i] + 32;
      }
    }

    i=0;
    //o int i eh o contador para percorrer a entrada e o int j o contador para cada palavra
loop:
    while(entrada[i]!='\0') {
      j=0;

      while((entrada[i]==' ' || entrada[i]==9) && i<tamanho) { //remove tabulacoes e espacos
        i++;
      }

      if(entrada[i]=='#') { //eh uma linha de comentario, que deve ser ignorada
        while(entrada[i]!='/n' && i<tamanho)
          i++;
        linha++;
        i++;

        //olhando a linha que acabou de ser lida para possiveis erros gramaticais
        unsigned a, b=0;
        for(a=0;a<getNumberOfTokens();a++) {
          if(recuperaToken(a).linha == linha-1) {
            tokensLinha[b++] = recuperaToken(a);
          }
          if(recuperaToken(a).linha == linha) {
            break;
          }
        }
        goto loop;
      }

      while(i<tamanho && entrada[i]!=' ' && entrada[i]!=9 && entrada[i]!='\n') { //le uma palavra
          palavra[j]=entrada[i];
          i++;
          j++;
      }

      palavra[j]='\0';

      if(entrada[i]=='\n') { //se chegou no fim da linha
        linha++;
        i++;


        //olhando a linha que acabou de ser lida para possiveis erros gramaticais
        unsigned a, b=0;
        for(a=0;a<getNumberOfTokens();a++) {
          if(recuperaToken(a).linha == linha-1) {
            tokensLinha[b++] = recuperaToken(a);
          }
          if(recuperaToken(a).linha == linha) {
            break;
          }
        }
      }

      if(palavra[0]=='.') { //possivel diretiva
        if(!ehDiretiva(palavra)) {
          fprintf(stderr, "ERRO LEXICO: palavra invalida na linha %d\n", &linha);
          return 1;
        }
        novoToken.tipo = "Diretiva";
      }
      else if (palavra[j-1]==':') { //possivel rotulo
        if(!ehRotulo(palavra)) {
          fprintf(stderr, "ERRO LEXICO: palavra invalida na linha %d\n", &linha);
          return 1;
        }
        novoToken.tipo = "DefRotulo";
        palavra[j-1] = '\0';
      }
      else if (palavra[0]=='0' && palavra[1]=='x') { //possivel hexadecimal
        if(!ehHexa(palavra)) {
          fprintf(stderr, "ERRO LEXICO: palavra invalida na linha %d\n", &linha);
          return 1;
        }
        novoToken.tipo = "Hexadecimal";
      }
      else if (ehDecimal(palavra)) { //numero decimal
        novoToken.tipo = "Decimal";
      }
      else if (ehInstrucao(palavra)) { //instrucao
        novoToken.tipo = "Instrucao";
      }
      else {
        if(!ehNome(palavra)) {
          fprintf(stderr, "ERRO LEXICO: palavra invalida na linha %d\n", &linha);
          return 1;
        }
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


int ehHexa(char* hex) {
  int i=2;
  while(hex[i]!='\0') {
    if(!(hex[i]>='0' && hex[i]<='9') || !(hex[i]>='a' || hex[i]<='f'))
      return 0;
    i++;
  }
  if(i>2)
    return 1;
}



int ehDecimal(char* dec) {
  int i=0;
  while(dec[i]!='\0') {
    if(dec[i]<'0' || dec[i]>'9')
      return 0;
    i++;
  }
  return 1;
}

int ehRotulo(char* rot) {
  int a=0;

  if(rot[0]>='0' && rot[0]<='9') {
    return 0;
  }
  while(rot[a+1]!='\0') {
    if((!(rot[a]>='a' && rot[a]<='z') || !(rot[a]>='0' && rot[a]<='9')) && rot[a]!='_') {
      return 0;
    }
    a++;
  }

  return 1;
}

int ehDiretiva(char* dir) {
  if(strcmp(dir,".set") || strcmp(dir,".org") || strcmp(dir,".align") || strcmp(dir,"wfill") || strcmp(dir,"word")) {
    return 1;
  }
  return 0;
}


int ehInstrucao(char* ins) {
  if(strcmp(ins,"ld") || strcmp(ins,"ldinv") || strcmp(ins,"ldabs") || strcmp(ins,"ldmq") || strcmp(ins,"ldmqmx") || strcmp(ins,"store") || strcmp(ins,"jump") ||
    strcmp(ins,"jumpl") || strcmp(ins,"jumpr") || strcmp(ins,"add") || strcmp(ins,"addabs") || strcmp(ins,"sub") || strcmp(ins,"subabs") || strcmp(ins,"mult") ||
    strcmp(ins,"div") || strcmp(ins,"lsh") || strcmp(ins,"rsh") || strcmp(ins,"storal") || strcmp(ins,"storar")) {
      return 1;
  }
  return 0;
}


int ehNome(char* nome) {

  if(nome[0]>='0' && nome[0]<='9') {
    return 0;
  }

  int a=0;
  while(nome[a]!='\0') {
    if((!(nome[a]>='a' && nome[a]<='z') || !(nome[a]>='0' && nome[a]<='9')) && nome[a]!='_') {
      return 0;
    }
    a++;
  }

  return 1;
}
