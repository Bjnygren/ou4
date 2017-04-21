//Written by Johan Eliasson <johane@cs.umu.se>.
//May be used in the course Datastrukturer och Algoritmer (C) at Umeå University.
//Usage exept those listed above requires permission by the author.

#include <stdio.h>
#include "mtftable.h"
#include "dlist.h"

typedef struct MyTable {
	dlist *values;
	CompareFunction *cf;
	KeyFreeFunc *keyFree;
	ValueFreeFunc *valueFree;
} MyTable;

typedef struct TableElement{
	KEY key;
	VALUE value;
} TableElement;

/* Function which moves a key-value pair to the front of a table. This reduces 
 *  		the time to find inputs which are often looked up. 
 *	table - a table created as a directional list.
 *  pos	  - the position which is to be moved to the front of the table*/
void table_MTF(Table *table, dlist_position pos){

	MyTable *t = (MyTable*)table;
	if(pos != t->values->head){ //no need to swich if pos is first

		dlist_position temp=malloc(sizeof(dlist_position));
		temp->next = pos->next;
		if(dlist_isEnd(t->values,pos->next) == 0){
			TableElement *i;
			TableElement *k;
			k=dlist_inspect(t->values,pos->next);
			i=dlist_inspect(t->values,pos);
			
			
			if (t->cf(i->key,k->key)==0){
				temp->next = pos->next;
				int key_equal = 1;
				while ((key_equal == 1) && (!dlist_isEnd(t->values,temp))){
					temp=dlist_next(t->values,temp);
					k=dlist_inspect(t->values,temp);
					
					if (t->cf(i->key,k->key)!=0){
						key_equal = 0;
					}
					
				}
				
			}
		}
	
	
	
//		if(!dlist_isEnd(t->values,temp)){
//			temp = dlist_next(t->values,temp);
			temp->next = temp->next->next;
			dlist_position first = malloc(sizeof(dlist_position));
			first->next = dlist_first(t->values);
			
//			temp->next=pos->next->next;
			pos->next->next = first->next->next;
			first->next->next=pos->next;
			pos->next = temp->next;
//			free(temp->next);
			free(temp);
//			free(first->next);
			free(first);
//		}
	}
}
	
	

/* Creates a table.
 *  compare_function - Pointer to a function that is called for comparing
 *                     two keys. The function should return <0 if the left
 *                     parameter is smaller than the right parameter, 0 if
 *                     the parameters are equal, and >0 if the left
 *                     parameter is larger than the right item.
 * Returns: A pointer to the table. NULL if creation of the table failed. */
Table *table_create(CompareFunction *compare_function)
{
	MyTable *t = calloc(sizeof (MyTable),1);
	if (!t)
		return NULL;
	t->values=dlist_empty();
	dlist_setMemHandler(t->values, free);
	
	t->cf = compare_function;
	return t;
}

/*
 *  freeFunc- Pointer to a function that is called for  freeing all
 *                     the memory used by keys inserted into the table*/
void table_setKeyMemHandler(Table *table,KeyFreeFunc *freeFunc) {
	MyTable *t = (MyTable*)table;
	t->keyFree=freeFunc;
	
}
/*
 *  freeFunc- Pointer to a function that is called for  freeing all
 *                     the memory used by values inserted into the table*/
void table_setValueMemHandler(Table *table,ValueFreeFunc *freeFunc) {
	MyTable *t = (MyTable*)table;
	t->valueFree=freeFunc;
}

/* Determines if the table is empty.
 *  table - Pointer to the table.
 * Returns: false if the table is not empty, true if it is. */
bool table_isEmpty(Table *table) {
	MyTable *t = (MyTable*)table;
	return dlist_isEmpty(t->values);
}


/* Inserts a key and value pair into the table. If memhandlers are set the table takes
 * ownership of the key and value pointers and is responsible for calling
 * the destroy function to free them when they are removed.
 *  table - Pointer to the table.
 *  key   - Pointer to the key.
 *  value - Pointer to the value.
 */
void table_insert(Table *table, KEY key,VALUE value) {
	MyTable *t = (MyTable*)table;
	TableElement *e=malloc(sizeof(TableElement));
	e->key = key;
	e->value = value;
	dlist_insert(t->values,dlist_first(t->values),e);
  
}

VALUE table_lookup(Table *table, KEY key) {
	MyTable *t = (MyTable*)table;
	TableElement *i;
	dlist_position p=dlist_first(t->values);
	while (!dlist_isEnd(t->values,p)) {
		i=dlist_inspect(t->values,p);
		if (t->cf(i->key,key)==0){
			table_MTF(table,p);
			i=dlist_inspect(t->values,dlist_first(t->values));
			return i->value;		
		}
		p=dlist_next(t->values,p);
	}
	return NULL;
}

/* This function removes the element Corresponding to the given key*/
void table_remove(Table *table, KEY key) {
	MyTable *t = (MyTable*)table;
	TableElement *i;
	dlist_position p=dlist_first(t->values);
	
	while (!dlist_isEnd(t->values,p)) {
		i=dlist_inspect(t->values,p);
		if (t->cf(i->key,key)==0) {
			if(t->keyFree!=NULL)
				t->keyFree(i->key);
			if(t->valueFree!=NULL)
				t->valueFree(i->value);
			p=dlist_remove(t->values,p);
		}
		else
			p=dlist_next(t->values,p);
	}
	
}

/*This function removes the table */
void table_free(Table *table) {
	MyTable *t = (MyTable*)table;
	TableElement *i;
	dlist_position p=dlist_first(t->values);
	
	while (!dlist_isEnd(t->values,p)) {
		i=dlist_inspect(t->values,p);
		if(t->keyFree!=NULL)
			t->keyFree(i->key);
		if(t->valueFree!=NULL)
			t->valueFree(i->value);
		p=dlist_remove(t->values,p);
	}
	dlist_free(t->values);
	free(t);
}

