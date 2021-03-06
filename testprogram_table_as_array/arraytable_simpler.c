/*
 * 
 * Key "REMOVED" is reserved for elements which has been removed
 * 
 * 
 */


#include <stdio.h>
#include <string.h>
#include "array.h"
#include "arraytable.h"

#define NUM_ELEMENTS 100

typedef struct ArrayTable{
	array *values;
	CompareFunction *cf;
	KeyFreeFunc *keyFree;
	ValueFreeFunc *valueFree;
	int nrOccupied;
} ArrayTable;

/* Creates a table.
 *  compare_function - Pointer to a function that is called for comparing
 *                     two keys. The function should return <0 if the left
 *                     parameter is smaller than the right parameter, 0 if
 *                     the parameters are equal, and >0 if the left
 *                     parameter is larger than the right item.
 * Returns: A pointer to the table. NULL if creation of the table failed. */
Table *table_create(CompareFunction *compare_function){
	ArrayTable *a = calloc(sizeof (ArrayTable),1); //varför argument i den ordningen? vad säger vi här? calloc->0 så 0 == empty, n+1==returned?
//	if(!a);	//kollar så att a fick allokerat minne, annars skapas inte a??
//		return NULL;
	a->values = array_create(2,0,0,NUM_ELEMENTS-1,NUM_ELEMENTS-1); //col 1=index, col2=key, col3=value
	a->cf = compare_function;
	if(array_hasValue(a->values,0,0) == 0){
		a->nrOccupied = 0;
	}
	array_setMemHandler(a->values, free);
	return a;
}

/* Install a memory handling function responsible for removing a key when removed from the table
 *  table - Pointer to the table.
 *  freeFunc- Pointer to a function that is called for  freeing all
 *                     the memory used by keys inserted into the table*/
void table_setKeyMemHandler(Table *table,KeyFreeFunc *freeFunc){
	ArrayTable *a = (ArrayTable*)table;
	a->keyFree = freeFunc;
}

/* Install a memory handling function responsible for removing a value when removed from the table
 *  table - Pointer to the table.
 *  freeFunc- Pointer to a function that is called for  freeing all
 *                     the memory used by keys inserted into the table*/
void table_setValueMemHandler(Table *table,ValueFreeFunc *freeFunc){
	ArrayTable *a = (ArrayTable*)table;
	a->valueFree = freeFunc;
}

/* Determines if the table is empty.
 *  table - Pointer to the table.
 * Returns: false if the table is not empty, true if it is. */
bool table_isEmpty(Table *table){
	ArrayTable *a = (ArrayTable*)table;
//	return array_hasValue(a->values,0,1) == 0;
	return a->nrOccupied == 0;
}

/* Inserts a key and value pair into the table. The table type takes
 * ownership of the key and value pointers and is responsible for 
 * deallocating them when they are removed.
 *  table - Pointer to the table.
 *  key   - Pointer to the key.
 *  value - Pointer to the value.
 */
//void table_insert(Table *table, KEY key, VALUE value){
//	ArrayTable *a = (ArrayTable*)table;
//	KEY key2;
//	int key_set = 0;
//	int i = 0;
//	while(array_hasValue(a->values,i,0)){ // kolla om det finns dubletter
//		key2 = array_inspectValue(a->values,i,0);
//		if(a->cf(key,key2) == 0){
//			array_setValue(a->values,value,i,1);
//			key_set = 1;
//			break;
//		}
//		i++;
//		if((i>NUM_ELEMENTS-1) || (array_inspectValue(a->values,i,0) == NULL)){
//			break;
//		}
//	}
//	if(key_set == 0){ // fylla på längst bak eller avbryta
//		if(i>NUM_ELEMENTS-1){
//			printf("Table full, cannot insert new value.");
//		}
//		else{
//			array_setValue(a->values,key,i,0);
//			array_setValue(a->values,value,i,1);
//			a->nrOccupied++;
//		}
//	}
//}

void table_insert(Table *table, KEY key, VALUE value){
	ArrayTable *a = (ArrayTable*)table;
	KEY key2;
	int i = 0;
	while(i<NUM_ELEMENTS-1){ // kolla om det finns dubletter
		if(array_hasValue(a->values,i,0)){ 
			key2 = array_inspectValue(a->values,i,0);
			if(a->cf(key,key2) == 0){
				array_setValue(a->values,value,i,1);
				break;
			}
		}
		else if((array_inspectValue(a->values,i,0) == NULL) || (array_inspectValue(a->values,i,0) == "FREE")){
			array_setValue(a->values,key,i,0);
			array_setValue(a->values,value,i,1);
			a->nrOccupied++;
		}
		i++;
	}
}


/* Finds a value given its key.
 *  table - Pointer to the table.
 *  key   - Pointer to the item's key.
 * Returns: Pointer to the item's value if the lookup succeded. NULL if the
 *          lookup failed. The pointer is owned by the table type, and the
 *          user should not attempt to deallocate it. It will remain valid
 *          until the item is removed from the table, or the table is
 *          destroyed. */
//VALUE table_lookup(Table *table, KEY key){
//	ArrayTable *a = (ArrayTable*)table;
//	KEY key2;
//	int i = 0;
//	while(array_hasValue(a->values,i,0)){ // söker nyckel
//		key2 = array_inspectValue(a->values,i,0);
//		if(a->cf(key,key2) == 0){
//			break;
//		}
//		i++;
//		if(i>NUM_ELEMENTS-1){
//			printf("No such key to look up.");
//			break;
//		}
//	}
//	return array_inspectValue(a->values,i,1);
//}
VALUE table_lookup(Table *table, KEY key){
	ArrayTable *a = (ArrayTable*)table;
	KEY key2;
	int i = 0;
	while(i<NUM_ELEMENTS-1){ // söker nyckel
		key2 = array_inspectValue(a->values,i,0);
		if(a->cf(key,key2) == 0){
			break;
		}
		i++;
	}
	return array_inspectValue(a->values,i,1);
}


/* Removes an item from the table given its key.
 *  table - Pointer to the table.
 *  key   - Pointer to the item's key.
 */
//void table_remove(Table *table, KEY key){
//	ArrayTable *a = (ArrayTable*)table;
//	KEY key2;
//	KEY key_move;
//	VALUE value_move;
//	int i = 0;
//	int j = 0;
//	while(array_hasValue(a->values,i,1)){ // söker nyckel
//		key2 = array_inspectValue(a->values,i,0);
//		if(a->cf(key,key2) == 0){
//			while(array_hasValue(a->values,j+1,0)){
//				
//				key_move = array_inspectValue(a->values,j+1,0);
//				value_move = array_inspectValue(a->values,j+1,1);
//				
//				array_setValue(a->values,key_move,j,0);
//				array_setValue(a->values,value_move,j,1);
//				
//				key_move = NULL;
//				value_move = NULL;
//				j++;
//			}
//			
//			array_setValue(a->values,NULL,j,0);
//			array_setValue(a->values,NULL,j,1);
//			break;
//		}
//		j++;
//		i++;
//		if(i>NUM_ELEMENTS-1){
//			printf("No such element to remove.");
//			break;
//		}
//	}
//}

//void table_remove(Table *table, KEY key){
//	ArrayTable *a = (ArrayTable*)table;
//	KEY key2;
//	int i = 0;
//	while(array_hasValue(a->values,i,1)){ // söker nyckel
//		key2 = array_inspectValue(a->values,i,0);
//		if(a->cf(key,key2) == 0){
//				
//			array_setValue(a->values,NULL,i,0);
//			array_setValue(a->values,NULL,i,1);
//			a->nrOccupied--;
//			break;
//		}
//		
//		if((i>NUM_ELEMENTS-1) || (array_inspectValue(a->values,i,0) == NULL)){
//			break;
//		}
//	}
//}

void table_remove(Table *table, KEY key){
	ArrayTable *a = (ArrayTable*)table;
	KEY key2;
	int i = 0;
	while(i<NUM_ELEMENTS-1){ // söker nyckel
		key2 = array_inspectValue(a->values,i,0);
		if(a->cf(key,key2) == 0){
				
			array_setValue(a->values,"FREE",i,0);
			array_setValue(a->values,"FREE",i,1);
			a->nrOccupied--;
			break;
		}
		
	}
}


void table_free(Table *table){
	ArrayTable *a = (ArrayTable*)table;
	array_free(a->values);
	free(table);
}

