#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXVEC 1000
#define MAXCARCT 30
#define MAXCADENA 30
#define CANTSIMBOLOS 3


struct nodoCodigo{
    int FrecCodigos; //Cant repeticiones en el txt
    char Codigos[MAXCARCT]; //Ej: AAA
    float probabilidades; //prob de aparicion
    float entropia; 
    float cantInfo;
    char cadenaHuffman[MAXCADENA];
};

typedef struct nodoA {
    char dato[MAXCARCT];
    int frec;
    struct nodoA *izq;
    struct nodoA *der;
} NODOA;
typedef NODOA *Tarbol;

typedef struct nodoL{
    Tarbol arbol;
    struct nodoL *sig;
} NODOL;
typedef NODOL *TLista;

/*analisis de los datos otorgados*/
void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter, int * PalabrasTotales, char archivoInicial[]);
int checkRepetido(char lect[MAXCARCT],struct nodoCodigo VCodigos[], int CantPalabras);
void CalculaProbabilidades(struct nodoCodigo VCodigos[],int CantPalabras, int  PalabrasTotales);

/*punto a*/
void CalculaInformacionYEntropia(struct nodoCodigo VCodigos[],int CantPalabras, int LongCaracter, float *entropia, float *infoTotal);
/*punto c*/
int checkCompacto(struct nodoCodigo VCodigos[], int CantPalabras, int LongCaracter);
/*punto e*/
void cracionArbolHuffman(Tarbol *a,struct nodoCodigo VCodigos[], int CantPalabras);
void nuevoNodoArbol(Tarbol *A,int frecuencia, char codigo[MAXCARCT], struct nodoA *izq, struct nodoA *der);
void nuevoNodoLista(Tarbol a, TLista *nuevonodo);
void insertarListaOrdenada(TLista *cabeza, TLista nuevo);
void sacaMinimos(TLista *cabeza, Tarbol *min1, Tarbol *min2);
void GenerarCadenas(struct nodoCodigo VCodigos[], Tarbol a, char cadena []);
int Busqueda(struct nodoCodigo VCodigos[], char palabra []);
/*generales*/
void MostrarVector(struct nodoCodigo VCodigos[MAXVEC],int CantPalabras);

int main(){
    struct nodoCodigo VCodigos[MAXVEC];
    int CantPalabras=0;
    int LongCaracter=0; //variande este parametro se consideran las cadenas de diferentes largos
    int PalabrasTotales=0;
    float EntropiaTotal, cantInfoTotal;
    Tarbol arbolHuffman;
    printf("ingrese la longitud de los caracteres \n");
    scanf("%d",&LongCaracter);
    char archivoInicial[MAXCADENA]="juego-catedra.txt"; //juego-catedra.txt
    LeeArch(VCodigos,&CantPalabras,LongCaracter,&PalabrasTotales,archivoInicial);
    CalculaProbabilidades(VCodigos,CantPalabras,PalabrasTotales);
    CalculaInformacionYEntropia(VCodigos,CantPalabras,LongCaracter,&EntropiaTotal, &cantInfoTotal);
    if (checkCompacto(VCodigos,CantPalabras,LongCaracter))
        printf("es compacto \n");
    else
        printf("no es compacto \n");
    printf("su rendimiento es:  %2.2f %c \n", EntropiaTotal/LongCaracter*100,37);
    printf("su redundancia es: %2.2f %c \n", (1-(EntropiaTotal/LongCaracter))*100,37);
    cracionArbolHuffman(&arbolHuffman,VCodigos,CantPalabras);
    GenerarCadenas(VCodigos,arbolHuffman,"");
    MostrarVector(VCodigos,CantPalabras);
    
    return 0;
}

/*---------------------------------------------------------------analisis de los datos otorgados-------------------------------------------------------------------------------*/

void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter, int * PalabrasTotales, char archivoInicial[]){
    FILE* arch;
    char lect[MAXCARCT];
    int pos;
    arch=fopen(archivoInicial,"rt");
    if(arch==NULL)
        printf("No hay archivo");
    else{
        fread(&lect,sizeof(char),LongCaracter,arch);
        lect[LongCaracter]='\0';
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
            lect[LongCaracter]='\0';
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

void CalculaProbabilidades(struct nodoCodigo VCodigos[],int CantPalabras, int  PalabrasTotales){
    for (int i=0;i<CantPalabras;i++){
        VCodigos[i].probabilidades=(float)VCodigos[i].FrecCodigos/PalabrasTotales;
    }
}

/*-------------------------------------------------------------------------------punto a --------------------------------------------------------------------------------------*/

void CalculaInformacionYEntropia(struct nodoCodigo VCodigos[],int CantPalabras, int LongCaracter, float *entropia, float *infoTotal){
    *entropia=0;
    *infoTotal=0;
    for (int i=0;i<CantPalabras;i++){
            VCodigos[i].cantInfo=(log10(1/VCodigos[i].probabilidades)/log10(CANTSIMBOLOS)); //tambien se calcula la informacion
            VCodigos[i].entropia=VCodigos[i].probabilidades*VCodigos[i].cantInfo;
            *infoTotal+= VCodigos[i].cantInfo;
            *entropia+=VCodigos[i].entropia;
    }
    printf("Informacion Total: %f \n", *infoTotal);
    printf("Entropia Total: %f \n", *entropia);
}

/*-------------------------------------------------------------------------------punto c --------------------------------------------------------------------------------------*/

int checkCompacto(struct nodoCodigo VCodigos[], int CantPalabras, int LongCaracter){
    int i=0;
    while (i<CantPalabras && (i==0)?VCodigos[i].probabilidades==VCodigos[i].probabilidades:VCodigos[i].probabilidades==VCodigos[i-1].probabilidades)
        i++;
    if (i==CantPalabras && pow(CANTSIMBOLOS,LongCaracter))
        return 1;
    else
        return 0;
}

/*-------------------------------------------------------------------------------punto e --------------------------------------------------------------------------------------*/

void cracionArbolHuffman(Tarbol *a,struct nodoCodigo VCodigos[], int CantPalabras){
    TLista cabezaLista=NULL,nuevonodo=NULL;
    Tarbol arbol=NULL,min1,min2;
    int i;
    char codinutil[MAXCARCT]="zzzz";
    for (i=0;i<CantPalabras; i++){ //aca creo la lista ordenada
        arbol=NULL;
        nuevoNodoArbol(&arbol,VCodigos[i].FrecCodigos,VCodigos[i].Codigos,NULL,NULL);
        nuevoNodoLista(arbol,&nuevonodo);
        insertarListaOrdenada(&cabezaLista,nuevonodo);
    }
    while (cabezaLista->sig!=NULL){//aca se crea el arbol
        sacaMinimos(&cabezaLista,&min1,&min2);
        nuevoNodoArbol(&arbol,min1->frec+min2->frec,codinutil,min1,min2);
        nuevoNodoLista(arbol,&nuevonodo);
        insertarListaOrdenada(&cabezaLista,nuevonodo);
    }
    *a=cabezaLista->arbol;
}

void nuevoNodoArbol(Tarbol *A,int frecuencia, char codigo[MAXCARCT], struct nodoA *izq, struct nodoA *der){
    *A=(Tarbol)malloc(sizeof(NODOA));
    strcpy((*A)->dato,codigo);
    (*A)->frec=frecuencia;
    (*A)->der=der;
    (*A)->izq=izq;
}

void nuevoNodoLista(Tarbol a, TLista *nuevonodo){
    *nuevonodo=(TLista)malloc(sizeof(NODOL));
    (*nuevonodo)->arbol=a;
    (*nuevonodo)->sig=NULL;
}

void insertarListaOrdenada(TLista *cabeza, TLista nuevo){
TLista act,ant;
    act=*cabeza;
    if(*cabeza==NULL ||  nuevo->arbol->frec<(*cabeza)->arbol->frec){ // la lista es nula o se ingresa en el primer elemento
        nuevo->sig=act;
        *cabeza=nuevo;
    }
    else{ //si llega aca es porque la cabeza no es null
        while (act != NULL && nuevo->arbol->frec>=act->arbol->frec){
            ant=act;
            act=act->sig;
        }
        ant->sig=nuevo;
        nuevo->sig=act;
    }
}

void sacaMinimos(TLista *cabeza, Tarbol *min1, Tarbol *min2){
    TLista pri,seg;
    pri=*cabeza;
    seg=pri->sig;
    *cabeza=seg->sig;
    *min1=pri->arbol;
    *min2=seg->arbol;
    free(pri);
    free(seg);
}

void GenerarCadenas(struct nodoCodigo VCodigos[], Tarbol a, char cadena []){
    int indice;
    char cadenaAuxiliar1[MAXCADENA];
    char cadenaAuxiliar2[MAXCADENA];
    strcpy(cadenaAuxiliar1,cadena);
    strcpy(cadenaAuxiliar2,cadena);
    if (a!=NULL){
        if(a->izq== NULL && a->der== NULL){
            indice=Busqueda(VCodigos,a->dato);
            strcpy(VCodigos[indice].cadenaHuffman,cadena);
        }
        else{
            GenerarCadenas(VCodigos,a->der,strcat(cadenaAuxiliar1,"0"));
            GenerarCadenas(VCodigos,a->izq,strcat(cadenaAuxiliar2,"1"));
        }
    }
}

int Busqueda(struct nodoCodigo VCodigos[], char palabra []){
    int i=0;
    while(strcmp(VCodigos[i].Codigos,palabra)!=0)
        i++;
    return i;
}

/*------------------------------------------------------------------------------- global --------------------------------------------------------------------------------------*/

void MostrarVector(struct nodoCodigo VCodigos[MAXVEC],int CantPalabras){
int i=0;
    printf("        Codigo           frecuencia            probablilidades            entropia          codigo huffman \n");
    for (i=0;i<CantPalabras;i++){
        printf("%20s ",VCodigos[i].Codigos);
        printf("%20d ",VCodigos[i].FrecCodigos);
        printf("%20f ",VCodigos[i].probabilidades);
        printf("%20f ",VCodigos[i].entropia);
        printf("%20s \n",VCodigos[i].cadenaHuffman);
    }
}