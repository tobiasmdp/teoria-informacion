#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXVEC 10000
#define MAXCARCT 10
#define MAXCADENA 30

struct nodoCodigo{
    int FrecCodigos; //Cant repeticiones en el txt
    char Codigos[MAXCARCT]; //Ej: AAA
    float probabilidades; //prob de aparicion
    float entropia; 
    float cantInfo;
    char cadenaHuffman[MAXCADENA];
};

struct nodoCodigoDecodificador{
        char Codigos[MAXCADENA]; //Ej: AAA
        char largoCadenaHuffman; //Ej: Si la cadena de huffman es 0101 el largo es un int =3
        int cadenaHuffmanBinario;// Ej:  01010
        char cadenaHuffmanString [MAXCADENA]; //EJ: 01010/0 en string
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

void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter, int * , char []);
int checkRepetido(char [MAXCARCT],struct nodoCodigo [], int );
void quickSort(struct nodoCodigo VCodigos[], int low, int high);
int partition(struct nodoCodigo VCodigos[], int low, int high);
void swap(struct nodoCodigo *a, struct nodoCodigo *b);
void MostrarVector(struct nodoCodigo VCodigos[MAXVEC],int CantPalabras);
void CalculaProbabilidades(struct nodoCodigo [],int , int  );
void CalculaInformacionYEntropia(struct nodoCodigo VCodigos[],int CantPalabras, int, float* entropia,  float* cantInfo);
int checkCompacto(struct nodoCodigo VCodigos[], int CantPalabras, int LongCaracter);
void creacionHuffman (struct nodoCodigo VCodigos[], int CantPalabras);
void BuscoMinimos(Tarbol VArbol[],int cantarboles,int arbtemp,float *min1,float *min2,int *posmin1, int *posmin2);
void CreoArbol (Tarbol *A, char dato1[], float frec1, char dato2[], float frec2);
void UnoArboles(Tarbol VArbol[],int posmin1,int posmin2);
void postorden(Tarbol a);
void GenerarCadenas(struct nodoCodigo [], Tarbol , char  []);
int Busqueda(struct nodoCodigo [], char []);
void EscribirArchivoConHuffman(struct nodoCodigo [], int , int, char [], char []);
void sumadorBinario(int* , int* , char  [],int );
int traductorBinario(char cadena[]);
void escribirEncabezado(struct nodoCodigo VCodigos [], int cantPalabras, int longCaracter,char archivoFinal[]);
void DecodificarArchivoConHuffman(char  [], char []);
void  DecodificarBodyConHuffman(unsigned char ,short int ,char [],char  [],struct nodoCodigoDecodificador [],int , int );
int BusquedaHuffman(struct nodoCodigoDecodificador [], char  [],int );
void TraductorAString( char cadena[], char largo, int cadenaBinaria);


int main(){
    struct nodoCodigo VCodigos[MAXVEC];
    int CantPalabras=0;
    int LongCaracter=3; //variande este parametro se consideran las cadenas de diferentes largos
    int PalabrasTotales=0;
    float EntropiaTotal, cantInfoTotal;
    /*printf("ingrese la longitud de los caracteres");
    scanf("%d",&LongCaracter);*/
    char archivoInicial[MAXCADENA]="prueba1.txt"; //juego-catedra.txt
    char archivoFinal[MAXCADENA]="prueba2.dat";
    char archivoResultado[MAXCADENA]="resultado.txt";
    LeeArch(VCodigos,&CantPalabras,LongCaracter,&PalabrasTotales,archivoInicial);
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
    escribirEncabezado(VCodigos,CantPalabras,LongCaracter,archivoFinal);
    EscribirArchivoConHuffman(VCodigos,CantPalabras,LongCaracter,archivoInicial,archivoFinal);

    DecodificarArchivoConHuffman(archivoFinal,archivoResultado);

    return 0;
}

void DecodificarArchivoConHuffman(char archivoFinal [], char archivoResultado []){

    int tamanio,postamanio, LongCaracter, CantPalabras;
    
    FILE * archHuffman;
    struct nodoCodigoDecodificador VCodigos[MAXVEC];
    archHuffman = fopen(archivoFinal,"rb");

    fread(&LongCaracter,sizeof(int),1,archHuffman); 
    fread(&CantPalabras,sizeof(int),1,archHuffman);
    for (int i=0;i< CantPalabras;i++){
        fread(&(VCodigos[i].Codigos),LongCaracter,1,archHuffman); //escribe la palabra, ej ABA o ABAAB
        fread(&(VCodigos[i].largoCadenaHuffman),sizeof(int),1,archHuffman);
        fread(&(VCodigos[i].cadenaHuffmanBinario),sizeof(int),1,archHuffman);
        TraductorAString(VCodigos[i].cadenaHuffmanString, VCodigos[i].largoCadenaHuffman, VCodigos[i].cadenaHuffmanBinario);
    }
    fread(&tamanio,sizeof(int),1,archHuffman);
    fread(&tamanio,sizeof(int),1,archHuffman);
    
    postamanio=ftell(archHuffman);
    fclose(archHuffman);
    DecodificarBodyConHuffman(LongCaracter,CantPalabras,archivoFinal,archivoResultado,VCodigos,tamanio, postamanio);
}

void TraductorAString( char cadena[], char largo, int cadenaBinaria){
    char auxCadena[MAXCADENA] = "";
    cadenaBinaria <<= (32-largo);
    for(int i=0 ; i < largo ; i++){
        auxCadena[i] = cadenaBinaria & 0x80000000;
        cadenaBinaria <<= 1;
    }
    strcpy(cadena,auxCadena);
}

void  DecodificarBodyConHuffman(unsigned char LongCaracter,short int CantPalabras,char archivoFinal[],char archivoResultado [],struct nodoCodigoDecodificador VCodigos[],int tamanioKB, int postamanio){
    FILE * archHuffman;
    FILE * archRes;
    archHuffman = fopen(archivoFinal,"rb");
    archRes=fopen(archivoResultado,"w+");
    int bytesleidos,auxiliar,pos,bitsCompletados=0, ultimaLinea;
    char cadenaHuffman[MAXCADENA];
    int tamanio= tamanioKB*1024; //Tamaño en bytes
    //redondearlo;
    fseek(archHuffman,postamanio,SEEK_SET);
    for(int i=0; i<(int) tamanio/4; i++){//Analiza todas las lineas excepto la ultima que es probable que este incompleta
        fread(&bytesleidos,sizeof(int),1,archHuffman);
        while(bitsCompletados<=32){
            auxiliar=bytesleidos & 0x80000000; 
            if(auxiliar==1){ //ES bit UNO
                strcat(cadenaHuffman,"1");
            }
            else{ //ES bit CERO
                strcat(cadenaHuffman,"0");
            }
            pos=BusquedaHuffman(VCodigos,cadenaHuffman,CantPalabras);
            if(pos!=-1){//Lo encuentra
                fwrite(VCodigos[pos].Codigos,strlen(VCodigos[pos].Codigos),1,archRes);
                strcpy(cadenaHuffman,"");
            }
            bitsCompletados++;
            auxiliar<<=1;
        }   
        bitsCompletados=0;
    }
    ultimaLinea = tamanio % 4;
    if (ultimaLinea){ //los ultimos 4 bytes del archivo, que pueden no ser en su totalidad bits para Huffman
        fread(&bytesleidos,sizeof(int),1,archHuffman);
        while(bitsCompletados<=ultimaLinea){ // lee solo los bit para huffman
            auxiliar=bytesleidos & 0x80000000; 
            if(auxiliar==1)
                strcat(cadenaHuffman,"1");
            else
                strcat(cadenaHuffman,"0");
            pos=BusquedaHuffman(VCodigos,cadenaHuffman,CantPalabras);
            if(pos!=-1){//Lo encuentra
                fwrite(VCodigos[pos].Codigos,strlen(VCodigos[pos].Codigos),1,archRes);
                strcpy(cadenaHuffman,"");
            }
            bitsCompletados++;
            auxiliar<<=1;
        }
    }
    fclose(archHuffman);
    fclose(archRes);
}

void LeeArch(struct nodoCodigo VCodigos[],int *CantPalabras,int LongCaracter, int * PalabrasTotales, char archivoInicial[]){
    FILE* arch;
    char lect[MAXCARCT];
    int pos;
    arch=fopen(archivoInicial,"rt");
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
            BuscoMinimos(VArbol,cantarboles,arbtemp,&min1,&min2,&posmin1,&posmin2);
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
            ant++;
        } 
        else if(sig==-1){ //ultimo elemento del vector
            CreoArbol(&VArbol[posmin1],VCodigos[ant].Codigos,VCodigos[ant].FrecCodigos,"zzz",0);
            arbtemp++;
            cantarboles++;
        }else{
            CreoArbol(&VArbol[posmin1],VCodigos[ant].Codigos,VCodigos[ant].probabilidades,VCodigos[ant].Codigos,VCodigos[ant].probabilidades); //vuelvo a pasar en la segunda parte lo mismo porque tengo que rellenar los parametros, sino no serian necesarios
        }
        
        ant++;
        BuscoMinimos(VArbol,cantarboles,arbtemp,&min1,&min2,&posmin1,&posmin2);
    }
    while (arbtemp>1){
        UnoArboles(VArbol,posmin1,posmin2);
        BuscoMinimos(VArbol,cantarboles,arbtemp,&min1,&min2,&posmin1,&posmin2);
        arbtemp--;
    }//teoricamente en la pos 0 del vector tiene que quedar el arbol
    GenerarCadenas(VCodigos,VArbol[0],"");
    postorden(VArbol[0]); //aca tendria que recorrese el arbol para encontrar los codigos
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
int BusquedaHuffman(struct nodoCodigoDecodificador VCodigos[], char cadenaHuffman [],int CantPalabras){
    int i=0;
    while( i<CantPalabras && strcmp(VCodigos[i].cadenaHuffmanString,cadenaHuffman)!=0)
        i++;
    if(i==CantPalabras )
        return -1;
    else
        return i;
}

void EscribirArchivoConHuffman(struct nodoCodigo VCodigos[], int CantPalabras, int LongCaracter, char archivoInicial[], char archivoFinal[]){
    FILE* archIni, *archFin;
    int indice,i,bitsCompletados=0,auxiliar,libre,bitsTotales=0,posTamanio;
    char lect[MAXCARCT];
    char auxString[MAXCARCT];
    float kbytesTotales;
    archIni=fopen(archivoInicial,"rt");
    archFin=fopen(archivoFinal,"rb+"); // no funciona con append tampoco
    if(archIni==NULL)
        printf("No hay archivo");
    else{
        fread(&lect,sizeof(char),LongCaracter,archIni);
        fseek(archFin,0,SEEK_END);
        posTamanio = ftell(archFin)-4; //guarda 4 bytes atras, donde arranca el espacio para escribir el tamanio
        while(!feof(archIni)){

            indice=Busqueda(VCodigos,lect);
            strcpy(auxString,VCodigos[indice].cadenaHuffman);

            if(bitsCompletados+strlen(auxString)<=32){//Que puedo insertarlo tranquilo
                for(i=0;i<strlen(auxString);i++){
                    sumadorBinario(&auxiliar,&bitsCompletados,auxString,i);   
                }
                if(bitsCompletados==32){
                    bitsCompletados=0;
                    fwrite(&auxiliar,sizeof(int),1,archFin);
                    bitsTotales+=32;
            }
            }
            else{// Inserto una particion
                libre=bitsCompletados+strlen(auxString)-32;
                for(i=0;i<32-bitsCompletados;i++){
                    sumadorBinario(&auxiliar,&bitsCompletados,auxString,i);
                }
                fwrite(&auxiliar,sizeof(int),1,archFin);
                bitsTotales+=32;
                bitsCompletados=libre;
                for(i=0;i<libre;i++){
                    sumadorBinario(&auxiliar,&bitsCompletados,auxString,i);
                }
            }
            fread(&lect,sizeof(char),LongCaracter,archIni);
            
        }
        if(bitsCompletados!=32){
            bitsTotales+=bitsCompletados;
            auxiliar<<=32-bitsCompletados;
            fwrite(&auxiliar,sizeof(int),1,archFin);
        }
        kbytesTotales = bitsTotales / 8. / 1024.;
        kbytesTotales = bitsTotales;
        fseek(archFin, posTamanio, SEEK_SET);
        fwrite(&kbytesTotales,4,1,archFin); 
    }
    fclose(archIni);
    fclose(archFin);
}

void sumadorBinario(int* auxiliar, int* bitsCompletados, char lect [], int i){
    (*auxiliar)<<=1;
    *bitsCompletados+=1;
    if(lect[i]=='1')
        *auxiliar+=1; 
}



void BuscoMinimos(Tarbol VArbol[],int cantarboles,int arbtemp,float *min1,float *min2,int *posmin1, int *posmin2){
float auxmin1=99999, auxmin2=99999;
int  auxpos1,auxpos2,i=0,auxcontarb=0;
    while (auxcontarb<arbtemp){
        if(VArbol[i]!=NULL){
            if(arbtemp==1){
                auxmin1=VArbol[i]->prob;
                auxmin2=VArbol[i]->prob;
                auxpos1=i;
                auxpos2=auxpos1;
            }
            else if (auxmin1<VArbol[i]->prob && auxmin2>VArbol[i]->prob){
                auxmin2=VArbol[i]->prob;
                auxpos2=i;
            }else if (auxmin1>VArbol[i]->prob && auxmin2>VArbol[i]->prob){
                auxmin2=auxmin1;
                
                auxmin1=VArbol[i]->prob;
                auxpos1=i;
            }
        auxcontarb++;
        }   
        i++;
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
        hoja2->prob = frec2;
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

void postorden(Tarbol a) {
    if (a != NULL) {
        postorden(a->izq);
        postorden(a->der);
        printf("%2.4f,", a->prob);
    }
}

//void EscribirArchivoConHuffman(struct nodoCodigo VCodigos[], int CantPalabras, int LongCaracter, char archivoInicial[], char archivoFinal[]){
void escribirEncabezado(struct nodoCodigo VCodigos [], int cantPalabras, int longCaracter,char archivoFinal[]){
    FILE* archHuffman;
    int auxHuffman,byteLongCaracter,byteCantPalabras;
    char palHuffman[MAXCADENA];
    archHuffman = fopen(archivoFinal,"wb");
    if (!archHuffman){
        printf("Error al abrir el archivo para escribir los headers");
    } 
    else {
        fwrite(&longCaracter,sizeof(int),1,archHuffman); 
        fwrite(&cantPalabras,sizeof(int),1,archHuffman);
        for (int i=0;i< cantPalabras;i++){
            fwrite(&(VCodigos[i].Codigos),longCaracter,1,archHuffman); //escribe la palabra, ej ABA o ABAAB
            //auxHuffman = strlen(VCodigos[i].cadenaHuffman)<<24; 
            fwrite(&auxHuffman,4,1,archHuffman); //longitud codigo huffman
            auxHuffman = traductorBinario(VCodigos[i].cadenaHuffman);
            fwrite(&auxHuffman,4,1,archHuffman); //codigo de huffman en un entero
        }
        fwrite(NULL,4,1,archHuffman); // dejo 4 bytes para poner el tamanio del archivo
        fclose(archHuffman);
    }
}

int traductorBinario(char cadena[]){
    int largo = strlen(cadena), resultado=0;
    for (int i = 0; i<largo; i++){
        resultado<<=1;
        if(cadena[i] == '1'){
            resultado++;
        }
    }
    return resultado;
}