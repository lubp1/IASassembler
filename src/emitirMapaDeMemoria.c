#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct PosRot {
    char* palavra;
    unsigned int linha;
    unsigned int pos;
    unsigned char lr;
} PosRot;

typedef struct Sym {
    char* palavra;
    unsigned int linha;
    char* val;
} Sym;






/*
    ---- Voce deve implementar essa função para a Parte 2! ----
    Utilize os tokens da estrutura de tokens para montar seu código!
    Retorna:
        *  1 caso haja erro na montagem; (imprima o erro em stderr)
        *  0 caso não haja erro.
 */
int emitirMapaDeMemoria()
{
    unsigned int i, numRotulos=0, numSym=0, j=0, k=0, l, linha=0;
    unsigned char lr;



    //contando o numero de declaracoes de rotulos e simbolos
    for(i=0;i<getNumberOfTokens();i++) {
        if(recuperaToken(i).tipo == DefRotulo)
            numRotulos++;
        if(i>0 && recuperaToken(i).tipo == Nome && recuperaToken(i-1).tipo == Diretiva && !strcmp(recuperaToken(i-1).palavra,".set"))
            numSym++;
    }

    PosRot* rotulos = malloc(numRotulos* sizeof(PosRot));
    Sym* simbolos = malloc(numSym* sizeof(Sym));

    //salvando nos vetores todas os rotulos e simbolos definidos, e checando se cada nome que nao eh declaracao ja foi declarado antes
    i=0;
    while(i<getNumberOfTokens()) {
        if(recuperaToken(i).tipo == DefRotulo) {
            rotulos[j].palavra = recuperaToken(i).palavra;

            //tirando o : da definicao do rotulo
            int n=0;
            while(1) {
                if(rotulos[j].palavra[n] == ':') {
                    rotulos[j].palavra[n] = '\0';
                    break;
                }
            }
            rotulos[j].linha = recuperaToken(i).linha;
            j++;
        }
        else if(i>0 && recuperaToken(i).tipo == Nome && recuperaToken(i-1).tipo == Diretiva && !strcmp(recuperaToken(i-1).palavra,".set")) {
            simbolos[k].palavra = recuperaToken(i).palavra;
            simbolos[k].linha = recuperaToken(i).linha;
            simbolos[k].val = recuperaToken(i+1).palavra;
            k++;
        }
        else if(recuperaToken(i).tipo == Nome && !(recuperaToken(i-1).tipo == Diretiva && !strcmp(recuperaToken(i-1).palavra,".set"))) {
            for(l=0;l<numRotulos;l++) {
                if(!strcmp(recuperaToken(i).palavra,rotulos[l].palavra) && recuperaToken(i).linha>=rotulos[l].linha) {
                    goto fim;
                }
            }
            for(l=0;l<numSym;l++) {
                if(!strcmp(recuperaToken(i).palavra,simbolos[l].palavra) && recuperaToken(i).linha>=simbolos[l].linha) {
                    goto fim;
                }
            }
            printf("ERRO: Usado mas não definido: %s!", recuperaToken(i).palavra);
            return 1;
        }
        fim: i++;
    }

    i=0;
    lr='l';
    loop:
    while(i<getNumberOfTokens()) {
        if(recuperaToken(i).tipo==DefRotulo) {
            int t=0;
            while(t<numRotulos) {
                if(!strcmp(recuperaToken(i).palavra,rotulos[t].palavra)) {
                    rotulos[t].pos=linha;
                    rotulos[t].lr=lr;
                }
                t++;
            }
        }
        else if(recuperaToken(i).tipo==Diretiva && !strcmp(recuperaToken(i).palavra,".org")) {
            if (recuperaToken(i+1).tipo == Decimal) {
                linha = (unsigned int) strtol(recuperaToken(i + 1).palavra, NULL, 10);
                lr='l';
            }
            else if (recuperaToken(i+1).tipo == Hexadecimal) {
                linha = (unsigned int) strtol(recuperaToken(i + 1).palavra, NULL, 16);
                lr='l';
            }
            i = i+2;
            goto loop;
        }

        if(lr=='l') {
            lr = 'r';
        }
        else {
            linha++;
        }
        i++;
    }






    free(rotulos);
    free(simbolos);
    return 0;
}
