#include <stdio.h>
#include <stdlib.h>
#define  MAX 30

/*Prototipos*/

void IngresoAbecedario(char [],int*);
void LeeArch(char Vsimbolos[],int ,int  [][MAX],int  []);
void MostrarVector(int cantidad, int VContsimbolos [] );
void MostrarMatriz(int cantidad, int MContSimbolos [] [MAX]);
void Resultados(int MContSimbolos [] [MAX],int cantidad, int VContsimbolos [] );

int main()
{
    char Vsimbolos[MAX]={};
    int VContsimbolos[MAX]={0};
    int cantidad=0;
    int MContSimbolos [MAX][MAX]={};
    IngresoAbecedario(Vsimbolos,&cantidad);
    /*for(int i=0;i<=cantidad;i++){
        printf("%c",Vsimbolos[i]);
    }*/
    LeeArch(Vsimbolos,cantidad,MContSimbolos,VContsimbolos);
    Resultados(MContSimbolos,cantidad,VContsimbolos);
    return 0;
}


void LeeArch(char Vsimbolos[],int cantidad,int MContSimbolos [][MAX],int VContsimbolos []){
    int j,pos,posant=-1;
    FILE* arch;
    char lect;
    arch=fopen("juego-catedra.txt","rt");
    if(arch==NULL)
        printf("No hay archivo");
    else{
        fread(&lect,sizeof(char),1,arch);
        while(!feof(arch)){
            pos=0;
            while(pos<cantidad && lect!=Vsimbolos[pos] ){//BUSCA EL CARACTER EN EL ARREGLO
                pos++;
            }
            if(posant!=-1){
                VContsimbolos[pos]++;                  //!= -1 PARA EVITAR EL PRIMER CARACTER
                MContSimbolos[pos][posant]++;     //INCREMENTA EL CONTADOR EN LA MATRIZ SIMBOLOS
            }
            posant=pos;                               //FILA = Actual COLUMNA = Anterior
            fread(&lect,sizeof(char),1,arch);
        }
        fclose(arch);
        MostrarMatriz(cantidad,MContSimbolos);
        MostrarVector(cantidad,VContsimbolos);
    }
}

void MostrarVector(int cantidad, int VContsimbolos [] ){
    for(int i=0;i<cantidad;i++){
        printf("%d ",VContsimbolos [i]);
    }
    printf("\n");
}

void MostrarMatriz(int cantidad, int MContSimbolos [] [MAX]){
    int i,j;

    for(i=0;i<cantidad;i++){
        for(j=0;j<cantidad;j++){
        printf("%d  ",MContSimbolos[i][j]);
        }
        printf("\n");
    }

}

void IngresoAbecedario(char Vsimbolos[],int *cantidad){

    int i;
    printf("Ingrese la cantidad de simbolos del alfabeto:\n ");
    scanf("%d",cantidad);
    for(i=0;i<*cantidad;i++){
        fflush(stdin);
        printf("\n Ingrese simbolo numero %d:",i+1);
        scanf("%c",&Vsimbolos[i]);
    }

}

void Resultados(int MContSimbolos [] [MAX],int cantidad, int VContsimbolos [] ){
    for (int i=0; i<cantidad;i++){
        for (int j=0;j<cantidad;j++)
            printf("%2.4f ",(float)MContSimbolos[i][j]/VContsimbolos[j]);
        printf("\n");
    }
}
