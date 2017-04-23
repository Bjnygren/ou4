/*
 * Copyright 2012 Johan Eliasson (johane@cs.umu.se). Tillåtelse ges för användning på kurserna 
 * i Datastrukturer och algoritmer vid Umeå Universitet. All annan användning kräver författarens
 * tillstånd.
 */

/*
Datatypen Array som så långt som möjligt följer definitionen på sidan
91 i Lars-Erik Janlert, Torbjörn Wiberg Datatyper och algoritmer 2.,
[rev.] uppl.,Lund, Studentlitteratur, 2000, x, 387 s. ISBN 91-44-01364-7

Fält av godtycklig dimension och storlek kan skapas.
Datavärden lagras som standard i form av void pekare.
Det går att ändra via typedef-deklarationen för data nedan (OBS high och
low funkar endast om data är void * eller int *).
*/

#ifndef __ARRAY_H
#define __ARRAY_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef __DATA
#define __DATA
typedef void *data;
#endif

#ifndef __MEMFREEDATAFUNC
#define __MEMFREEDATAFUNC
typedef void memFreeFunc(data);
#endif

typedef struct {
    int *low;
    int *high;
    int numDimensions;
    int arraySize;
    memFreeFunc *freeFunc;
    void **internal_array;
} array;

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
array *array_create(int numDimensions,... /*low followed by high*/);

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
void array_setMemHandler(array *arr, memFreeFunc *f);

/*
Syfte: Sätta in ett värde på en viss plats i arrayen.
Parametrar: arr - arrayen
            value - värdet som ska sättas in.
            index - index för arrayen. en int per dimension. low<= index <=high
Kommentarer: Beteendet är ej specificerat för ogiltiga index.
*/
void array_setValue(array *arr,data value,... /*index*/);

/*
Syfte: Hämta ett givet värde från arrayen
Parametrar: arr - arrayen
            index - index för arrayen. en int per dimension. low<= index <=high
Returvärde: Det efterfrågade värdet.
Kommentarer: Beteendet är ej specificerat för ogiltiga index
             och om hasValue(index) är false
*/
data array_inspectValue(array *arr,... /*index*/);

/*
Syfte: Kolla om ett värde satts för ett givet index i arrayen.
Parametrar: arr - arrayen
            index - index för arrayen. en int per dimension. low<= index <=high
Returvärde: bool som indikerar om ett värde har satts för index i arrayen.
Kommentarer: Beteendet är ej specificerat för ogiltiga index.
*/
bool array_hasValue(array *arr,... /*index*/);

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
array *array_high(array *arr);

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
array *array_low(array *arr);

/*
Syfte: Avallokerar allt minne som används av arrayen.
Parametrar: arr - Arrayen vars minne ska avallokeras.
Kommentarer: Efter anropet är det ej möjligt att använda arrayen.
             Minne för datat i arrayen måste innan anropet avallokeras av användaren av datatypen
             för att förhindra minnesläckor om minne allokerats dynamiskt för dessa.
*/
void array_free(array *arr);

#endif