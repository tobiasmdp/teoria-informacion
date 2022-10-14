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

/*typedef struct nodo{
  Tarbol arbol;
  struct nodo *sig;} nodo;
typedef nodo * TLista;*/

typedef struct nodo {
    char dato[MAXCARCT];
    float prob;
    struct nodo *izq;
    struct nodo *der;
} NODO;
typedef NODO * Tarbol;

void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter, int * );
int checkRepetido(char [MAXCARCT],struct nodoCodigo [], int );
void quickSort(struct nodoCodigo VCodigos[], int low, int high);
int partition(struct nodoCodigo VCodigos[], int low, int high);
void swap(struct nodoCodigo *a, struct nodoCodigo *b);
void MostrarVector(struct nodoCodigo VCodigos[MAXVEC],int CantPalabras);
void CalculaProbabilidades(struct nodoCodigo [],int , int  );
void CalculaInformacionYEntropia(struct nodoCodigo VCodigos[],int CantPalabras, int, float* entropia,  float* cantInfo);
int checkCompacto(struct nodoCodigo VCodigos[], int CantPalabras, int LongCaracter);
void creacionHuffman (struct nodoCodigo VCodigos[], int CantPalabras);
void BuscoMinimos(Tarbol VArbol[],int cantarboles,float *min1,float *min2,int *posmin1, int *posmin2);
void CreoArbol (Tarbol *A, char dato1[], float frec1, char dato2[], float frec2);
void UnoArboles(Tarbol VArbol[],int posmin1,int posmin2);

int main(){
    struct nodoCodigo VCodigos[MAXVEC];
    int CantPalabras=0;
    int LongCaracter=3; //variande este parametro se consideran las cadenas de diferentes largos
    int PalabrasTotales=0;
    float EntropiaTotal, cantInfoTotal;
    /*printf("ingrese la longitud de los caracteres");
    scanf("%d",&LongCaracter);*/
    LeeArch(VCodigos,&CantPalabras,LongCaracter,&PalabrasTotales);
    quickSort(VCodigos, 0, CantPalabras - 1);
    CalculaProbabilidades(VCodigos,CantPalabras,PalabrasTotales);
    CalculaInformacionYEntropia(VCodigos,CantPalabras,LongCaracter,&EntropiaTotal, &cantInfoTotal);
    MostrarVector(VCodigos,CantPalabras);
    if (checkCompacto(VCodigos,CantPalabras,LongCaracter))
        printf("es compacto \n");
    else 
        printf("no es compacto \n");
    printf("su rendimiento es:  %2.2f %c \n", EntropiaTotal/LongCaracter*100,37);
    printf("su redundancia es: %2.2f %c \n", (1-(EntropiaTotal/LongCaracter))*100,37);
    creacionHuffman (VCodigos,CantPalabras);
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

void quickSort(struct nodoCodigo VCodigos[], int low, int high) {
  if (low < high) {
    int pi = partition(VCodigos, low, high);
    quickSort(VCodigos, low, pi - 1);
    quickSort(VCodigos, pi + 1, high);
  }
}

int partition(struct nodoCodigo VCodigos[], int low, int high) {
  int pivot = VCodigos[high].FrecCodigos;
  int i = (low - 1);
  for (int j = low; j < high; j++) {
    if (VCodigos[j].FrecCodigos <= pivot) {
      i++;
      swap(&VCodigos[i], &VCodigos[j]);
    }
  }
  swap(&VCodigos[i + 1], &VCodigos[high]);
  return (i + 1);
}

void swap(struct nodoCodigo *a, struct nodoCodigo *b) {
  struct nodoCodigo t = *a;
  *a = *b;
  *b = t;
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

void creacionHuffman (struct nodoCodigo VCodigos[], int CantPalabras){
    float min1=-1,min2=-1;  // el min1 siempre va a ser el mas minimo de los 2
    int posmin1,posmin2, sumcantarb;
    Tarbol VArbol[MAXVEC]={NULL};
    int cantarboles=0,arbtemp=0;
    int ant=0,sig;
    while (ant<CantPalabras){ //esto es para agregar todas las hojas
        if(sig==CantPalabras)
            sig=-1;
        else
            sig=ant+1;
        if (min1>-1 && min2> -1 && min1< VCodigos[ant].probabilidades && min2< VCodigos[ant].probabilidades) {
            UnoArboles(VArbol,posmin1,posmin2);
            arbtemp--;
            BuscoMinimos(VArbol,cantarboles,&min1,&min2,&posmin1,&posmin2);
        }
        if (ant==0 || (min1<VCodigos[ant].probabilidades && min2>VCodigos[ant].probabilidades && min1==VCodigos[sig].probabilidades) ) { //primer elemento del vector o esta en el medio
            if (!VArbol[cantarboles]) // esto es para saber si despues le tenvo que sumar o no a cantarboles
                sumcantarb=1;
            else
                sumcantarb=0;
            CreoArbol(&VArbol[cantarboles],VCodigos[ant].Codigos,VCodigos[ant].probabilidades,VCodigos[sig].Codigos,VCodigos[sig].probabilidades);
            arbtemp++;
            if (sumcantarb)
                cantarboles++;
            if (ant==0){
                min1=VArbol[ant]->prob;
                posmin1=ant;
            }
            ant++;
        } 
        else if(sig==-1){ //ultimo elemento del vector
            CreoArbol(&VArbol[posmin1],VCodigos[ant].Codigos,VCodigos[ant].FrecCodigos,"zzz",0);
            arbtemp++;
            cantarboles++;
        }
        BuscoMinimos(VArbol,cantarboles,&min1,&min2,&posmin1,&posmin2);
        ant++;
    }
    while (arbtemp>1){
        UnoArboles(VArbol,posmin1,posmin2);
        BuscoMinimos(VArbol,cantarboles,&min1,&min2,&posmin1,&posmin2);
        arbtemp--;
    }//teoricamente en la pos 0 del vector tiene que quedar el arbol
    //aca tendria que recorrese el arbol para encontrar los codigos
}

/*void AgregoNodoListaOrdenada(TLista *L){
    TLista ant,recorre,nuevo;
    int i;
    nuevo= (TLista)malloc (sizeof(nodo));
    nuevo->arbol=a;
    if(*L==NULL){
        nuevo->sig=*L;
        *L=nuevo;
    }
    else{
        while ( recorre!=NULL && recorre->arbol->frec<nuevo->arbol->frec){
            ant=recorre;
            recorre=recorre->sig;
        }
        if (recorre==NULL){
            nuevo->sig=ant->sig; //asigno null
            ant->sig=nuevo;
       }
       else{
        nuevo->sig=recorre;
        ant->sig=nuevo;
       }
    }
}*/

void BuscoMinimos(Tarbol VArbol[],int cantarboles,float *min1,float *min2,int *posmin1, int *posmin2){
float auxmin1=99999, auxmin2=99999;
int  auxpos1,auxpos2;
    for(int i=0;i<cantarboles;i++){
        if (auxmin1<VArbol[i]->prob && auxmin2>VArbol[i]->prob){
            auxmin2=VArbol[i]->prob;
            auxpos2=i;
        }
        else if (auxmin1>VArbol[i]->prob && auxmin2>VArbol[i]->prob){
            auxmin2=auxmin1;
            auxpos2=auxpos1;
            auxmin1=VArbol[i]->prob;
            auxpos1=i;
        }
    }
    *min1=auxmin1;
    *min2=auxmin2;
    *posmin1=auxpos1;
    *posmin2=auxpos2;
}

void CreoArbol (Tarbol *A, char dato1[], float frec1, char dato2[], float frec2) { //frec 1 siempre es menor a frec2, entonces el menor va a la izquierda 
    Tarbol hoja1=NULL,hoja2=NULL,nuevaraiz;
    char nohoja[4]="zzz";
    if (*A == NULL) {//no hay arbol
        hoja1 = (Tarbol)malloc(sizeof(NODO));
        strcpy(hoja1->dato,dato1);
        hoja1->prob = frec1;
        hoja1->der = NULL;
        hoja1->izq = NULL;
        hoja2 = (Tarbol)malloc(sizeof(NODO));
        strcpy(hoja2->dato,dato2);
        hoja2->prob = frec1;
        hoja2->der = NULL;
        hoja2->izq = NULL;
        *A = (Tarbol)malloc(sizeof(NODO));
        strcpy((*A)->dato,nohoja);
        (*A)->prob = frec1+frec2;
        (*A)->izq = hoja1;
        (*A)->der = hoja2;
    } 
    else{ //si hay arbol
        hoja1 = (Tarbol)malloc(sizeof(NODO));
        strcpy(hoja1->dato,dato1);
        hoja1->prob = frec1;
        hoja1->der = NULL;
        hoja1->izq = NULL;
        nuevaraiz = (Tarbol)malloc(sizeof(NODO));
        strcpy(nuevaraiz->dato,nohoja);
        nuevaraiz->prob = (*A)->prob + hoja1->prob;
        if ((*A)->prob>hoja1->prob){
            nuevaraiz->der = *A;
            nuevaraiz->izq = hoja1;
        }
        else{
            nuevaraiz->der =hoja1 ;
            nuevaraiz->izq =*A ;
        }
        *A=nuevaraiz;
    }
}

void UnoArboles(Tarbol VArbol[],int posmin1,int posmin2){
Tarbol a;
 char nohoja[4]="zzz";
    a = (Tarbol)malloc(sizeof(NODO));
    strcpy((a)->dato,nohoja);
    (a)->prob = VArbol[posmin1]->prob + VArbol[posmin2]->prob;
    (a)->izq = VArbol[posmin1];
    (a)->der = VArbol[posmin2];
    VArbol[posmin1]=a;
    VArbol[posmin2]=NULL;
}