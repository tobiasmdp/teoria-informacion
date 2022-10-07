#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXVEC 10000
#define MAXCARCT 10

struct nodoCodigo{
    int FrecCodigos;
    char Codigos[MAXCARCT];
};

void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter);
int checkRepetido(char [MAXCARCT],struct nodoCodigo [], int );
void MostrarVector(struct nodoCodigo VCodigos[MAXVEC],int CantPalabras);

int main(){
    struct nodoCodigo VCodigos[MAXVEC];
    int CantPalabras=0;
    int LongCaracter=3;
    LeeArch(VCodigos,&CantPalabras,LongCaracter);
    MostrarVector(VCodigos,CantPalabras);
    return 0;
}

void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter){
    FILE* arch;
    char lect[MAXCARCT];
    int pos;
    arch=fopen("juego-catedra.txt","rt");
    if(arch==NULL)
        printf("No hay archivo");
    else{
        fread(&lect,sizeof(char),LongCaracter,arch);
        while(!feof(arch)){
            pos=checkRepetido(lect,VCodigos,*CantPalabras);
            if (pos!=-1) 
                VCodigos[pos].FrecCodigos+=1;
            else{
                strcpy(VCodigos[*CantPalabras].Codigos,lect);
                VCodigos[*CantPalabras].FrecCodigos++;
                *CantPalabras+=1;
            }
            fread(&lect,sizeof(char),LongCaracter,arch);
        }
        fclose(arch);
    }
}

int checkRepetido(char lect[MAXCARCT],struct nodoCodigo VCodigos[], int CantPalabras){ // check repetido devuelve la posicion del vector repetida, si es -1 es que no esta repetida y lo tengo que agregar
int i=0;
    while (i<CantPalabras && strcmp(VCodigos[i].Codigos,lect)!=0)
        i++;
    if (i>=CantPalabras)
        return -1;
    else 
        return i;
}

void MostrarVector(struct nodoCodigo VCodigos[MAXVEC],int CantPalabras){
int i=0;
    for (i=0;i<CantPalabras;i++){
        printf("Codigo: %s ",VCodigos[i].Codigos);
        printf("frecuencia: %d \n",VCodigos[i].FrecCodigos);
    }
}