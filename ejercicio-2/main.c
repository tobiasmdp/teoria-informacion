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
    char cadenaHuffman[MAXCADENA]; //10101011
};

struct nodoCodigoDecodificador{
        char Codigos[MAXCADENA]; //Ej: AAA
        char largoCadenaHuffman; //Ej: Si la cadena de huffman es 0101 el largo es un int =3
        int cadenaHuffmanBinario;// Ej:  01010
        char cadenaHuffmanString [MAXCADENA]; //EJ: 01010/0 en string
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
int escribirEncabezado(struct nodoCodigo VCodigos [], int cantPalabras, int LongCaracter,char archivoFinal[]);
int traductorBinario(char cadena[]);
void EscribirArchivoConHuffman(struct nodoCodigo [], int , int, char [], char []);
void sumadorBinario(int* , int* , char  [],int ); 
/*generales*/
void MostrarVector(struct nodoCodigo VCodigos[MAXVEC],int CantPalabras);
// testing functions
void printBits(size_t const size, void const * const ptr);
//Decodificacion
void DecodificarArchivoConHuffman(char  [], char []);
void  DecodificarBodyConHuffman(unsigned char ,short int ,char [],char  [],struct nodoCodigoDecodificador [] , int );
int BusquedaHuffman(struct nodoCodigoDecodificador [], char  [],int );

void TraductorAString( char [], char, int );
int main(){
    struct nodoCodigo VCodigos[MAXVEC];
    int exito; //usado para verificar si se escribio correctamente el encabezado del archivo binario
    int CantPalabras=0;
    int LongCaracter=0; //variande este parametro se consideran las cadenas de diferentes largos
    int PalabrasTotales=0;
    float EntropiaTotal, cantInfoTotal;
    Tarbol arbolHuffman;
    char archivoInicial[MAXVEC]="D://Facultad//teoria-informacion//ejercicio-1//juego-catedra.txt"; //juego-catedra.txt
    char archivoFinal[MAXCADENA];
    char archivoResultado[MAXCADENA]="resultado.txt";
    printf("Ingrese la longitud de las palabras \n");
    scanf("%d",&LongCaracter);
    snprintf(archivoFinal,MAXCADENA,"huffman-%d.dat",LongCaracter);
    
    LeeArch(VCodigos,&CantPalabras,LongCaracter,&PalabrasTotales,archivoInicial);
    CalculaProbabilidades(VCodigos,CantPalabras,PalabrasTotales);
    CalculaInformacionYEntropia(VCodigos,CantPalabras,LongCaracter,&EntropiaTotal, &cantInfoTotal);
    if (checkCompacto(VCodigos,CantPalabras,LongCaracter))
        printf("Es compacto \n");
    else
        printf("No es compacto \n");
    printf("Su rendimiento es:  %2.2f %c \n", EntropiaTotal/LongCaracter*100,37);
    printf("Su redundancia es: %2.2f %c \n", (1-(EntropiaTotal/LongCaracter))*100,37);
    cracionArbolHuffman(&arbolHuffman,VCodigos,CantPalabras);
    GenerarCadenas(VCodigos,arbolHuffman,"");
    MostrarVector(VCodigos,CantPalabras);
    exito = escribirEncabezado(VCodigos,CantPalabras,LongCaracter,archivoFinal);
    if (exito != -1)
        EscribirArchivoConHuffman(VCodigos,CantPalabras,LongCaracter,archivoInicial,archivoFinal);
    //Decodificacion
    DecodificarArchivoConHuffman(archivoFinal,archivoResultado);

    
    return exito;
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


int escribirEncabezado(struct nodoCodigo VCodigos [], int cantPalabras, int LongCaracter,char archivoFinal[]){
    FILE* archBin;
    int auxHuffman, exito = -1;
    char palHuffman[MAXCADENA];
    archBin = fopen(archivoFinal,"wb");
    if (!archBin){
        printf("Error: No se pudo abrir el archivo binario");
    } 
    else {
        fwrite(&LongCaracter,sizeof(int),1,archBin); 
        fwrite(&cantPalabras,sizeof(int),1,archBin);
        for (int i=0;i< cantPalabras;i++){
            fwrite(&(VCodigos[i].Codigos),LongCaracter,1,archBin); //escribe la palabra, ej ABA o ABAAB
            auxHuffman = strlen(VCodigos[i].cadenaHuffman); 
            fwrite(&auxHuffman,sizeof(int),1,archBin); //longitud codigo huffman
            auxHuffman = traductorBinario(VCodigos[i].cadenaHuffman);
            fwrite(&auxHuffman,sizeof(int),1,archBin); //codigo de huffman en un entero
        }
        fwrite(&auxHuffman,sizeof(int),1,archBin); // dejo 4 bytes para poner el tamanio del archivo mas tarde
        fclose(archBin);
        exito = 1;
    }
    return exito;
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

void EscribirArchivoConHuffman(struct nodoCodigo VCodigos[], int CantPalabras, int LongCaracter, char archivoInicial[], char archivoFinal[]){
    FILE* archIni, *archFin;
    int indice,i,bitsCompletados=0,auxiliar,libre,bitsTotales=0,posTamanio;
    char lect[MAXCARCT];
    char auxString[MAXCADENA];
    int repeticiones;
    archIni=fopen(archivoInicial,"rt");
    archFin=fopen(archivoFinal,"rb+"); 
    if(archIni==NULL)
        printf("Error: No existe el archivo de texto");
    else{
        fread(&lect,sizeof(char),LongCaracter,archIni);
        lect[LongCaracter] = '\0'; //se marca el fin de cadena
        fseek(archFin,0,SEEK_END);
        posTamanio = ftell(archFin)- sizeof(int); //guarda posicion 4 bytes atras (= sizeof(int)) , donde arranca el espacio para escribir el tamanio
        while(!feof(archIni)){
            indice=Busqueda(VCodigos,lect);
            strcpy(auxString,VCodigos[indice].cadenaHuffman);
            // printf("%s",auxString); //printea los strings concatenados -------------------------borrar al entregar, solo para testing functions
            for(i=0;i<strlen(auxString);i++){
                sumadorBinario(&auxiliar,&bitsCompletados,auxString,i);   
                if(bitsCompletados==32){
                    bitsCompletados=0;
                    // printBits(sizeof(int), &auxiliar); //printea la traduccion a binario de los enteros que se van a escribir en el binario ------------borrar al entregar, solo para testing functions
                    fwrite(&auxiliar,sizeof(int),1,archFin);
                    bitsTotales+=32;
                }
            }
            fread(&lect,sizeof(char),LongCaracter,archIni);
            lect[LongCaracter] = '\0';//se marca el fin de cadena
            
        }
        if(bitsCompletados!=0){
            bitsTotales+=bitsCompletados;
            auxiliar<<=32-bitsCompletados;
            fwrite(&auxiliar,sizeof(int),1,archFin);
            // printBits(sizeof(int), &auxiliar); //printea la traduccion a binario de los enteros que se van a escribir en el binario ------------borrar al entregar, solo para testing functions
        }
        fseek(archFin, posTamanio, SEEK_SET);
        fwrite(&bitsTotales,sizeof(int),1,archFin); 
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

/*------------------------------------------------------------------------------- testing functions --------------------------------------------------------------------------------------*/

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    //puts("");
}


/*------------------------------------------------------------------------------- Decodificacion --------------------------------------------------------------------------------------*/
/*Posible borrado*/
void DecodificarArchivoConHuffman(char archivoFinal [], char archivoResultado []){

    int postamanio, LongCaracter, CantPalabras;
    
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
    postamanio=ftell(archHuffman);
    fclose(archHuffman);
    DecodificarBodyConHuffman(LongCaracter,CantPalabras,archivoFinal,archivoResultado,VCodigos, postamanio);
}
/*Posible borrado*/
void TraductorAString( char cadena[], char largo, int cadenaBinaria){
    char auxCadena[MAXCADENA] = "";
    cadenaBinaria <<= (32-largo);
    for(int i=0 ; i < largo ; i++){
        if(cadenaBinaria & 0x80000000)
            auxCadena[i] = '1';
        else
            auxCadena[i] = '0';
        cadenaBinaria <<= 1;
    }
    strcpy(cadena,auxCadena);
}

/*Posible borrado*/
int BusquedaHuffman(struct nodoCodigoDecodificador VCodigos[], char cadenaHuffman [],int CantPalabras){
    int i=0;
    while( i<CantPalabras && strcmp(VCodigos[i].cadenaHuffmanString,cadenaHuffman)!=0)
        i++;
    if(i==CantPalabras )
        return -1;
    else
        return i;
}

/*Posible borrado*/

void  DecodificarBodyConHuffman(unsigned char LongCaracter,short int CantPalabras,char archivoFinal[],char archivoResultado [],struct nodoCodigoDecodificador VCodigos[], int postamanio){
    FILE * archHuffman;
    FILE * archRes;
    archHuffman = fopen(archivoFinal,"rb");
    archRes=fopen(archivoResultado,"w+");
    int tamanio, bytesleidos,auxiliar,pos,bitsCompletados=0, ultimaLinea;
    char cadenaHuffman[MAXCADENA]={};
    //redondearlo;
    fseek(archHuffman,postamanio,SEEK_SET);
    fread(&tamanio,sizeof(int),1,archHuffman);
    for(int i=0; i<(int) tamanio/32; i++){//Analiza todas las lineas excepto la ultima que es probable que este incompleta
        fread(&bytesleidos,sizeof(int),1,archHuffman);
        while(bitsCompletados<32){
            auxiliar=bytesleidos & 0x80000000; 
            if(auxiliar!=0){ //ES bit UNO
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
            bytesleidos<<=1;
        }   
        bitsCompletados=0;
    }
    ultimaLinea = tamanio % 32;
    if (ultimaLinea){ //los ultimos 4 bytes del archivo, que pueden no ser en su totalidad bits para Huffman
        fread(&bytesleidos,sizeof(int),1,archHuffman);
        while(bitsCompletados<=ultimaLinea){ // lee solo los bit para huffman
            auxiliar=bytesleidos & 0x80000000; 
            if(auxiliar!=0)
                strcat(cadenaHuffman,"1");
            else
                strcat(cadenaHuffman,"0");
            pos=BusquedaHuffman(VCodigos,cadenaHuffman,CantPalabras);
            if(pos!=-1){//Lo encuentra
                fwrite(VCodigos[pos].Codigos,strlen(VCodigos[pos].Codigos),1,archRes);
                strcpy(cadenaHuffman,"");
            }
            bitsCompletados++;
            bytesleidos<<=1;
        }
    }
    fclose(archHuffman);
    fclose(archRes);
} 
