#include <stdio.h>
#include <stdlib.h>
#define  MAX 30

/*Prototipos*/

void IngresoAbecedario(char [],int*);
void LeeArch(char Vsimbolos[],int ,int  [][MAX],int  []);
void MostrarVector(int cantidad, int VContsimbolos [] );
void MostrarMatriz(int cantidad, int MContSimbolos [] [MAX]);
void MostrarMatrizFloat(int cantidad, float Matriz[] [MAX]);
void Resultados(int MContSimbolos [] [MAX],int cantidad, int VContsimbolos [] );
void VectorEstacionario(int MContSimbolos [] [MAX],int cantidad,int VContsimbolos[]);
int checkTolerancia(float MatrizNueva[][MAX],float MatrizActual[][MAX],int, float );

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
    VectorEstacionario(MContSimbolos,cantidad,VContsimbolos);
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

void MostrarMatrizFloat(int cantidad, float Matriz [] [MAX]){
    int i,j;

    for(i=0;i<cantidad;i++){
        for(j=0;j<cantidad;j++){
        printf("%f  ",Matriz[i][j]);
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
    printf("matriz resultado: \n");
    for (int i=0; i<cantidad;i++){
        for (int j=0;j<cantidad;j++)
            printf("%2.4f ",(float)MContSimbolos[i][j]/VContsimbolos[j]);
        printf("\n");
    }
}

void VectorEstacionario(int MContSimbolos [] [MAX],int cantidad,int VContsimbolos[]){
float suma,MatrizAnterior[MAX][MAX],MatrizActual[MAX][MAX],MatrizNueva[MAX][MAX];
int a=0,c,d,k,i,j;
int vecresultado[MAX];
float tolerancia=0.00001;

    for (i=0;i<cantidad;i++)
        for (j=0;j<cantidad;j++){
            MatrizActual[i][j]=(float)MContSimbolos[i][j]/VContsimbolos[j];
            MatrizNueva[i][j]=(float)MContSimbolos[i][j]/VContsimbolos[j];
        }
    do{
        for (i=0;i<cantidad;i++)
            for (j=0;j<cantidad;j++){
                MatrizAnterior[i][j]=MatrizActual[i][j];
                MatrizActual[i][j]=MatrizNueva[i][j];
            }
        for (c = 0; c < cantidad; c++) 
            for (d = 0; d < cantidad; d++){
                for (k = 0; k < cantidad; k++) 
                    suma += MatrizAnterior[c][k] * MatrizActual[k][d];
                MatrizNueva[c][d] = suma;
                suma = 0;
            }
        a++;
        printf("Iteracion numero = %d \n",a);
        printf("Matriz de la iteracion: \n");
        MostrarMatrizFloat(cantidad,MatrizNueva);
    }
    while (checkTolerancia(MatrizNueva,MatrizActual,cantidad,tolerancia));
}

int checkTolerancia(float MatrizNueva[][MAX],float MatrizActual[][MAX],int cantidad, float tolerancia){
int i=0,j=0;
    for (i=0;i<cantidad;i++)
        for (j=0;j<cantidad;j++){
           if (tolerancia<MatrizActual[i][j]-MatrizNueva[i][j])
            return 1;
    }
    return 0;
}