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
    unsigned int val;
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

    if(getNumberOfTokens()==0)
        return 0;



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
                n++;
            }
            rotulos[j].linha = recuperaToken(i).linha;
            j++;
        }
        else if(i>0 && recuperaToken(i).tipo == Nome && recuperaToken(i-1).tipo == Diretiva && !strcmp(recuperaToken(i-1).palavra,".set")) {
            simbolos[k].palavra = recuperaToken(i).palavra;
            simbolos[k].linha = recuperaToken(i).linha;
            if(recuperaToken(i+1).tipo == Decimal)
                simbolos[k].val = (unsigned int) strtol(recuperaToken(i+1).palavra,NULL,10);
            else if(recuperaToken(i+1).tipo == Hexadecimal)
                simbolos[k].val = (unsigned int) strtol(recuperaToken(i+1).palavra,NULL,16);
            k++;
        }
        else if(recuperaToken(i).tipo == Nome && !(recuperaToken(i-1).tipo == Diretiva && !strcmp(recuperaToken(i-1).palavra,".set"))) {
            for(l=0;l<j;l++) {
                if(!strcmp(recuperaToken(i).palavra,rotulos[l].palavra) && recuperaToken(i).linha>=rotulos[l].linha) {
                    goto fim;
                }
            }
            for(l=0;l<k;l++) {
                if(!strcmp(recuperaToken(i).palavra,simbolos[l].palavra) && recuperaToken(i).linha>=simbolos[l].linha) {
                    goto fim;
                }
            }
            printf("ERRO: Usado mas não definido: %s!", recuperaToken(i).palavra);
            return 1;
        }
        fim: i++;
    }


    //primeiro percurso pelo programa, para achar as posicoes das definicoes de rotulos
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
                    i++;
                    break;
                }
                t++;
            }
            goto loop;
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
        else if(recuperaToken(i).tipo==Diretiva && !strcmp(recuperaToken(i).palavra,".word")) {
            if(lr=='r') {
                printf("IMPOSSIVEL MONTAR CODIGO!");
                return 1;
            }
            lr = 'r';
            i++;
        }
        else if(recuperaToken(i).tipo==Diretiva && !strcmp(recuperaToken(i).palavra,".set")) {
            i=i+3;
            goto loop;
        }
        else if(recuperaToken(i).tipo==Diretiva && !strcmp(recuperaToken(i).palavra,".align")) {
            if (!(linha%strtol(recuperaToken(i+1).palavra,NULL,10)==0 && lr=='l')) {
                lr='l';
                while((linha%strtol(recuperaToken(i+1).palavra,NULL,10)!=0)) {
                    linha++;
                }
            }
            i++;
            goto loop;
        }
        else if(recuperaToken(i).tipo==Diretiva && !strcmp(recuperaToken(i).palavra,".wfill")) {
            linha = linha+(unsigned int)strtol(recuperaToken(i+1).palavra,NULL,10);
            i=i+3;
            goto loop;
        }
        else if(recuperaToken(i).tipo==Instrucao) {
            i++;
        }

        if(lr=='l') {
            lr = 'r';
        }
        else {
            lr='l';
            linha++;
        }
        i++;
    }


    //Segundo percurso, para imprimir o mapa
    linha=0;
    i=0;
    lr='l';
    if(strcmp(recuperaToken(i).palavra,".org")!=0 & !(!strcmp(recuperaToken(i).palavra,".set") && !strcmp(recuperaToken(i+3).palavra,".org"))) {
        printf("000 ");
    }
    loop2:
    while(i<getNumberOfTokens()) {
        if(recuperaToken(i).tipo==DefRotulo) {
            i++;
            goto loop2;
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
            printf("%03X ", linha);
            i = i+2;
            goto loop2;
        }
        else if(recuperaToken(i).tipo==Diretiva && !strcmp(recuperaToken(i).palavra,".word")) {
            if(recuperaToken(i+1).tipo == Decimal)
                printf("%010X\n", (unsigned int)strtol(recuperaToken(i+1).palavra,NULL,10));
            else if(recuperaToken(i+1).tipo == Hexadecimal)
                printf("%010X\n", (unsigned int)strtol(recuperaToken(i+1).palavra,NULL,16));
            else if(recuperaToken(i+1).tipo == Nome) {
                int n;
                for(n=0;n<numRotulos;n++) {
                    if(!strcmp(rotulos[n].palavra,recuperaToken(i+1).palavra)) {
                        printf("%010X\n", rotulos[n].pos);
                        break;
                    }
                }
                for(n=0;n<numSym;n++) {
                    if(!strcmp(simbolos[n].palavra,recuperaToken(i+1).palavra)) {
                        printf("%010X\n", simbolos[n].val);
                        break;
                    }
                }
            }

            lr = 'r';
            i++;
        }
        else if(recuperaToken(i).tipo==Diretiva && !strcmp(recuperaToken(i).palavra,".set")) {
            i=i+3;
            goto loop2;
        }
        else if(recuperaToken(i).tipo==Diretiva && !strcmp(recuperaToken(i).palavra,".align")) {
            unsigned char aux='l';
            if (!(linha%strtol(recuperaToken(i+1).palavra,NULL,10)==0 && lr=='l')) {
                if(lr=='r') {
                    aux='r';
                    printf("00 000\n");
                }
                linha++;
                while((linha%strtol(recuperaToken(i+1).palavra,NULL,10)!=0)) {
                    linha++;
                }
                lr='l';
            }
            if(i+1<getNumberOfTokens() && aux=='r')
                printf("%03X ", linha);
            i++;
            goto loop2;
        }
        else if(recuperaToken(i).tipo==Diretiva && !strcmp(recuperaToken(i).palavra,".wfill")) {
            int t;
            for(t=0;t<strtol(recuperaToken(i+1).palavra,NULL,10);t++) {
                if (i+1<getNumberOfTokens() && t<strtol(recuperaToken(i+1).palavra,NULL,10))
                    printf("%03X ", linha);
                if (recuperaToken(i + 2).tipo == Decimal)
                    printf("%010X\n", (unsigned int) strtol(recuperaToken(i + 2).palavra, NULL, 10));
                else if (recuperaToken(i + 2).tipo == Hexadecimal)
                    printf("%010X\n", (unsigned int) strtol(recuperaToken(i + 2).palavra, NULL, 16));
                else if (recuperaToken(i + 2).tipo == Nome) {
                    int n;
                    for (n = 0; n < numRotulos; n++) {
                        if (!strcmp(rotulos[n].palavra, recuperaToken(i + 2).palavra)) {
                            printf("%010X\n", rotulos[n].pos);
                            break;
                        }
                    }
                    for (n = 0; n < numSym; n++) {
                        if (!strcmp(simbolos[n].palavra, recuperaToken(i + 2).palavra)) {
                            printf("%010X\n", simbolos[n].val);
                            break;
                        }
                    }
                }
                linha++;
            }
            lr='l';
            i=i+3;
            goto loop2;
        }
        else if(recuperaToken(i).tipo==Instrucao) {
            int instrucao = 0;

            if(!strcmp(recuperaToken(i).palavra,"ld"))
                instrucao = 1;
            else if(!strcmp(recuperaToken(i).palavra,"ldinv"))
                instrucao = 2;
            else if(!strcmp(recuperaToken(i).palavra,"ldabs"))
                instrucao = 3;
            else if(!strcmp(recuperaToken(i).palavra,"ldmq"))
                instrucao = 10;
            else if(!strcmp(recuperaToken(i).palavra,"ldmqmx"))
                instrucao = 9;
            else if(!strcmp(recuperaToken(i).palavra,"jump")){
                if(recuperaToken(i+1).tipo != Nome)
                    instrucao = 13;
                else {
                    int n;
                    for(n=0;n<numRotulos;n++) {
                        if(!strcmp(rotulos[n].palavra,recuperaToken(i+1).palavra)) {
                            if(rotulos[n].lr=='l') {
                                instrucao = 13;
                            } else {
                                instrucao = 14;
                            }
                            break;
                        }
                    }
                }
            }
            else if(!strcmp(recuperaToken(i).palavra,"jumpl") || !strcmp(recuperaToken(i).palavra,"jumpr")){
                if(recuperaToken(i+1).tipo != Nome)
                    instrucao = 15;
                else {
                    int n;
                    for(n=0;n<numRotulos;n++) {
                        if(!strcmp(rotulos[n].palavra,recuperaToken(i+1).palavra)) {
                            if(rotulos[n].lr=='l') {
                                instrucao = 15;
                            } else {
                                instrucao = 16;
                            }
                            break;
                        }
                    }
                }
            }
            else if(!strcmp(recuperaToken(i).palavra,"add"))
                instrucao = 5;
            else if(!strcmp(recuperaToken(i).palavra,"addabs"))
                instrucao = 7;
            else if(!strcmp(recuperaToken(i).palavra,"sub"))
                instrucao = 6;
            else if(!strcmp(recuperaToken(i).palavra,"mult"))
                instrucao = 11;
            else if(!strcmp(recuperaToken(i).palavra,"div"))
                instrucao = 12;
            else if(!strcmp(recuperaToken(i).palavra,"lsh"))
                instrucao = 20;
            else if(!strcmp(recuperaToken(i).palavra,"rsh"))
                instrucao = 21;
            else if(!strcmp(recuperaToken(i).palavra,"storal") || !strcmp(recuperaToken(i).palavra,"storar")) {
                if(recuperaToken(i+1).tipo != Nome)
                    instrucao = 18;
                else {
                    int n;
                    for(n=0;n<numRotulos;n++) {
                        if(!strcmp(rotulos[n].palavra,recuperaToken(i+1).palavra)) {
                            if(rotulos[n].lr=='l') {
                                instrucao = 18;
                            } else {
                                instrucao = 19;
                            }
                            break;
                        }
                    }
                }
            }








            if(recuperaToken(i+1).tipo == Decimal) {
                printf("%02X %03X", instrucao, (unsigned int)strtol(recuperaToken(i+1).palavra,NULL,10));
            }
            else if(recuperaToken(i+1).tipo == Hexadecimal) {
                printf("%02X %03X", instrucao, (unsigned int)strtol(recuperaToken(i+1).palavra,NULL,16));
            }
            else if(recuperaToken(i+1).tipo == Nome) {
                int n;
                for(n=0;n<numRotulos;n++) {
                    if(!strcmp(rotulos[n].palavra,recuperaToken(i+1).palavra)) {
                        printf("%02X %03X", instrucao, rotulos[n].pos);
                        break;
                    }
                }
                for(n=0;n<numSym;n++) {
                    if(!strcmp(simbolos[n].palavra,recuperaToken(i+1).palavra)) {
                        printf("%02X %03X", instrucao, simbolos[n].val);
                        break;
                    }
                }
            }
            if(lr=='l')
                printf(" ");
            else if(lr=='r')
                printf("\n");

            i++;
        }

        if(lr=='l') {
            lr = 'r';
        }
        else {
            lr='l';
            linha++;
            if(i+1<getNumberOfTokens() && !(recuperaToken(i+1).tipo==Diretiva))
                printf("%03X ", linha);

        }
        i++;
    }

    if(lr=='r') {
        printf("00 000\n");
    }




    free(rotulos);
    free(simbolos);
    return 0;
}
