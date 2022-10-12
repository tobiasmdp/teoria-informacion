#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXVEC 10000
#define MAXCARCT 10

struct nodoCodigo{
    int FrecCodigos; //Cant repeticiones en el txt
    char Codigos[MAXCARCT]; //Ej: AAA
    float probabilidades; //prob de aparicion
    float entropia; 
    float cantInfo;
};

void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter, int * );
int checkRepetido(char [MAXCARCT],struct nodoCodigo [], int );
void MostrarVector(struct nodoCodigo VCodigos[MAXVEC],int CantPalabras);
void CalculaProbabilidades(struct nodoCodigo [],int , int  );
void CalculaInformacionYEntropia(struct nodoCodigo VCodigos[],int CantPalabras, int, float* entropia,  float* cantInfo);
int checkCompacto(struct nodoCodigo VCodigos[], int CantPalabras, int LongCaracter);

int main(){
    struct nodoCodigo VCodigos[MAXVEC];
    int CantPalabras=0;
    int LongCaracter=3; //variande este parametro se consideran las cadenas de diferentes largos
    int PalabrasTotales=0;
    float EntropiaTotal, cantInfoTotal;
    /*printf("ingrese la longitud de los caracteres");
    scanf("%d",&LongCaracter);*/
    LeeArch(VCodigos,&CantPalabras,LongCaracter,&PalabrasTotales);
    CalculaProbabilidades(VCodigos,CantPalabras,PalabrasTotales);
    CalculaInformacionYEntropia(VCodigos,CantPalabras,LongCaracter,&EntropiaTotal, &cantInfoTotal);
    MostrarVector(VCodigos,CantPalabras);
    if (checkCompacto(VCodigos,CantPalabras,LongCaracter))
        printf("es compacto \n");
    else 
        printf("no es compacto \n");
    printf("su rendimiento es:  %2.2f %c \n", EntropiaTotal/LongCaracter*100,37);
    printf("su redundancia es: %2.2f %c \n", (1-(EntropiaTotal/LongCaracter))*100,37);
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
        printf("probablilidades: %f ",VCodigos[i].probabilidades);
        printf("entropia: %f \n",VCodigos[i].entropia);
    }
}


void CalculaProbabilidades(struct nodoCodigo VCodigos[],int CantPalabras, int  PalabrasTotales){
    for (int i=0;i<CantPalabras;i++){
        VCodigos[i].probabilidades=(float)VCodigos[i].FrecCodigos/PalabrasTotales;
    }
}

void CalculaInformacionYEntropia(struct nodoCodigo VCodigos[],int CantPalabras, int LongCaracter, float *entropia, float *infoTotal){
    *entropia=0;
    *infoTotal=0;
    for (int i=0;i<CantPalabras;i++){
            VCodigos[i].cantInfo=(log10(1/VCodigos[i].probabilidades)/log10(LongCaracter)); //tambien se calcula la informacion
            VCodigos[i].entropia=VCodigos[i].probabilidades*VCodigos[i].cantInfo;
            *infoTotal+= VCodigos[i].cantInfo;
            *entropia+=VCodigos[i].entropia;
    }
    printf("Informacion Total: %f \n", *infoTotal);
    printf("Entropia Total: %f \n", *entropia);
}

int checkCompacto(struct nodoCodigo VCodigos[], int CantPalabras, int LongCaracter){
    int i=0;
    while (i<CantPalabras && VCodigos[i].entropia/VCodigos[i].probabilidades<=LongCaracter)
        i++;
    if (i==CantPalabras)
        return 1;
    else 
        return 0;
}
