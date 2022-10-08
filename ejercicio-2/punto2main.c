#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXVEC 10000
#define MAXCARCT 10

struct nodoCodigo{
    int FrecCodigos;
    char Codigos[MAXCARCT];
    float probabilidades;
};

void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter, int * );
int checkRepetido(char [MAXCARCT],struct nodoCodigo [], int );
void MostrarVector(struct nodoCodigo VCodigos[MAXVEC],int CantPalabras);
void CalculaProbabilidades(struct nodoCodigo [],int , int  );
void CalculaEntropia(struct nodoCodigo VCodigos[],int CantPalabras);
int main(){
    struct nodoCodigo VCodigos[MAXVEC];
    int CantPalabras=0;
    int LongCaracter=3;
    int PalabrasTotales=0;
    LeeArch(VCodigos,&CantPalabras,LongCaracter,&PalabrasTotales);
    MostrarVector(VCodigos,CantPalabras);
    CalculaProbabilidades(VCodigos,CantPalabras,PalabrasTotales);
    CalculaEntropia(VCodigos,CantPalabras);
    return 0;
}

void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter, int * PalabrasTotales){
    FILE* arch;
    char lect[MAXCARCT];
    int pos;
    arch=fopen("juego-catedra.txt","rt");
    if(arch==NULL)
        printf("No hay archivo");
    else{
        fread(&lect,sizeof(char),LongCaracter,arch);
        while(!feof(arch)){
            *PalabrasTotales+=1;
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
        printf("frecuencia: %d ",VCodigos[i].FrecCodigos);
        printf("probablilidades: %f \n",VCodigos[i].probabilidades);
    }
}


void CalculaProbabilidades(struct nodoCodigo VCodigos[],int CantPalabras, int  PalabrasTotales){
    for (int i=0;i<CantPalabras;i++){
        VCodigos[i].probabilidades=(float)VCodigos[i].FrecCodigos/PalabrasTotales;
    }
}


void CalculaEntropia(struct nodoCodigo VCodigos[],int CantPalabras){
    float acum=0;
    for (int i=0;i<CantPalabras;i++){
            acum+=VCodigos[i].probabilidades*(log10(1/VCodigos[i].probabilidades)/log10(2));
    }
    printf("Entropia: %f", acum);
}