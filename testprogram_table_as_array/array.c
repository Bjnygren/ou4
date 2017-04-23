/*
 * Copyright 2012 Johan Eliasson (johane@cs.umu.se). Tillåtelse ges för användning på kurserna 
 * i Datastrukturer och algoritmer vid Umeå Universitet. All annan användning kräver författarens
 * tillstånd.
 */

#include "array.h"

/*
Datatypen Array som så långt som möjligt följer definitionen på sidan
91 i Lars-Erik Janlert, Torbjörn Wiberg Datatyper och algoritmer 2.,
[rev.] uppl.,Lund, Studentlitteratur, 2000, x, 387 s. ISBN 91-44-01364-7

Fält av godtycklig dimension och storlek kan skapas.
Datavärden lagras i form av void pekare.
*/

/*
Syfte: Skapa en ny array (Fält)
Parametrar: numDimensions - int som anger antalet dimensioner på arrayen.
            low - en int per dimension. Anger lägsta möjliga index för den dimensionen
            high - en int per dimension. Anger högsta möjliga index för den dimensionen
            Ex för 3-Dimensionell:
            array *arr=array_create(3,0,0,0,10,10,10);
Returvärde: den nyskapade arrayen.
Kommentarer: Om low och high innehåller felaktigt antal värden eller low > high
             är beteendet ospecificerat
*/
array *array_create(int numDimensions, ... /*low followed by high*/) {
    va_list high_lo;
    int arraySize=1;
    va_start(high_lo,numDimensions);
    array *arr=calloc(1,sizeof(array));
    arr->low=malloc(numDimensions*sizeof(int));
    arr->high=malloc(numDimensions*sizeof(int));
    arr->numDimensions=numDimensions;

    for(int i=0;i<numDimensions*2;i++) {
       int value=va_arg(high_lo,int);
       if(i<numDimensions) {
           arr->low[i]=value;
       }
       else {
           arr->high[i-numDimensions]=value;
           arraySize*=arr->high[i-numDimensions]-arr->low[i-numDimensions]+1;
       }
    }
    va_end(high_lo);
    arr->internal_array=calloc(arraySize,sizeof(void *));
    arr->arraySize=arraySize;
    return arr;
}

/*
Syfte: Installera en minneshanterare för arrayen (fältet) så att den kan ta över
       ansvaret för att avallokera minnet för värdena då de ej finns kvar
       i fältet mer.
Parametrar: arr - arrayen
            f - en funktionspekare till en funktion som tar in ett värde
                av typen data (void pekare) och avallokerar minnet för värdet
                den pekar på (ett av den typ av värden som lagras i arrayen).
Kommentarer: Bör anropas direkt efter att arrayen skapats om funktionaliteten
             ska användas. Arrayen funkar även utan att denna funktion anropas,
             men det är då upp till användaren av datatypen att avallokra allt
             minne för datat som lagras i arrayen.
*/
void array_setMemHandler(array *arr, memFreeFunc *f) {
   arr->freeFunc=f;
}

/*
Endast hjälpfunktion. Ej för publik användning
*/
int getInternalArrayIndex(array *arr,va_list index) {
    int internalIndex=0;
    int multiplier=1;
    for(int i=1;i<arr->numDimensions;i++) {
        multiplier*=(arr->high[i]-arr->low[i]+1);
    }
    for(int i=0;i<arr->numDimensions-1;i++) {
       internalIndex+=(va_arg(index,int)-arr->low[i])*multiplier;
       multiplier/=(arr->high[i+1]-arr->low[i+1]+1);
    }
    internalIndex+=(va_arg(index,int)-arr->low[arr->numDimensions-1]);
    return internalIndex;  
}

/*
Syfte: Sätta in ett värde på en viss plats i arrayen.
Parametrar: arr - arrayen
            value - värdet som ska sättas in.
            index - index för arrayen. en int per dimension. low<= index <=high
Kommentarer: Beteendet är ej specificerat för ogiltiga index.
*/
void array_setValue(array *arr,data value, .../*index*/) {
    va_list index;
    va_start(index, value);
    int internalIndex=getInternalArrayIndex(arr,index);
    va_end(index);
    if(arr->freeFunc!=NULL && arr->internal_array[internalIndex] != NULL)
        arr->freeFunc(arr->internal_array[internalIndex]);
    arr->internal_array[internalIndex]=value;
}

/*
Syfte: Hämta ett givet värde från arrayen
Parametrar: arr - arrayen
            index - index för arrayen. en int per dimension. low<= index <=high
Returvärde: Det efterfrågade värdet.
Kommentarer: Beteendet är ej specificerat för ogiltiga index
             och om hasValue(index) är false
*/
data array_inspectValue(array *arr,... /*index*/) {
    va_list index;
    va_start(index, arr);
    int internalIndex=getInternalArrayIndex(arr,index);
    va_end(index);
    return arr->internal_array[internalIndex];
}

/*
Syfte: Kolla om ett värde satts för ett givet index i arrayen.
Parametrar: arr - arrayen
            index - index för arrayen. en int per dimension. low<= index <=high
Returvärde: bool som indikerar om ett värde har satts för index i arrayen.
Kommentarer: Beteendet är ej specificerat för ogiltiga index.
*/
bool array_hasValue(array *arr,... /*index*/) {
    va_list index;
    va_start(index, arr);
    int internalIndex=getInternalArrayIndex(arr,index);
    va_end(index);
    return arr->internal_array[internalIndex]!=NULL;
}

/*
Syfte: Hämta de högsta möjliga index som är giltiga för arrayen.
Parametrar: arr - arrayen.
Returvärde: Endimensionell array med ett värde per dimension i arrayen som man
            vill kontrolera de giltiga indexen för. Arrayen innehåller int-pekare.
            low för den returnerade arrayen är 0 och high antalet dimensioner-1.
Kommentarer: Användaren av funktionen är ansvarig för att avallokera minnet
             för den returnerade arrayen.
             Minne för värdena i den returnerade arrayen får ej avallokeras.
             Datavärdena i den returnerade arrayen är endast tillgängliga så
             länge som man ej avallokerat minnet för den ursprungliga arrayen.
*/
array * array_high(array * arr) {
    array *highIndex=array_create(1,0,arr->numDimensions-1);
    for(int i=0;i<arr->numDimensions;i++) {
        array_setValue(highIndex,&(arr->high[i]),i);
    }
    return highIndex;
}

/*
Syfte: Hämta de lägsta möjliga index som är giltiga för arrayen.
Parametrar: arr - arrayen.
Returvärde: Endimensionell array med ett värde per dimension i arrayen som man
            vill kontrolera de giltiga indexen för. Arrayen innehåller int-pekare.
            low för den returnerade arrayen är 0 och high antalet dimensioner-1.
Kommentarer: Användaren av funktionen är ansvarig för att avallokera minnet
             för den returnerade arrayen.
             Minne för värdena i den returnerade arrayen får ej avallokeras.
             Datavärdena i den returnerade arrayen är endast tillgängliga så
             länge som man ej avallokerat minnet för den ursprungliga arrayen.
*/
array * array_low(array * arr) {
    array *lowIndex=array_create(1,0,arr->numDimensions-1);
    for(int i=0;i<arr->numDimensions;i++) {
        array_setValue(lowIndex,&(arr->low[i]),i);
    }
    return lowIndex;
}

/*
Syfte: Avallokerar allt minne som används av arrayen.
Parametrar: arr - Arrayen vars minne ska avallokeras.
Kommentarer: Efter anropet är det ej möjligt att använda arrayen.
             Minne för datat i arrayen måste innan anropet avallokeras av användaren av datatypen
             för att förhindra minnesläckor om minne allokerats dynamiskt för dessa.
*/
void array_free(array * arr) {

    if(arr->freeFunc!=NULL) {
        for(int i=0;i<arr->arraySize;i++) {
            if(arr->internal_array[i]!=NULL) {
                arr->freeFunc(arr->internal_array[i]);
            }
        }
    }
    free(arr->internal_array);
    free(arr->high);
    free(arr->low);
    free(arr);
}