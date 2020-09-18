#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "doubly_linked_list.h"

List new_List() {
	List list;

	list.head=NULL;
	list.tail=NULL;
	list.size=0;

	return list;
}

bool isEmpty(List list) {
	return list.size==0;
}

void pushEmply_List(TElement elem, List* list){
	Case c;
	
	c=(Case) malloc (sizeof(struct Cell));
	c->data=elem;
	
	c->next=NULL;
	c->prev=NULL;
	list->head=c;
	list->tail=c;
	list->size=1;
}

int pushHead(TElement elem, List* list) {
	Case c;
	
	if(list==NULL) {
		return 0;
	}
	else {
		if(isEmpty(*list)) {
			pushEmply_List(elem,list);
		}
		else {	
			c=(Case) malloc (sizeof(struct Cell));
			c->data=elem;

			c->next=list->head;
			c->prev=NULL;
			(list->head)->prev=c;
			list->head=c;
			list->size=(list->size)+1;
		}

		return 1;
	}
}

int pushTail(TElement elem, List* list) {
	Case c;
	
	if(list==NULL) {
		return 0;
	}
	else {
		if(isEmpty(*list)) {
			pushEmply_List(elem,list);
		}
		else {	
			c=(Case) malloc (sizeof(struct Cell));
			c->data=elem;

			c->next=NULL;
			c->prev=list->tail;
			(list->tail)->next=c;
			list->tail=c;
			list->size=(list->size)+1;
		}

		return 1;
	}
}

void dump_List(List list) {
	for(int i=0; i<list.size; i++) {
		if(i<list.size-1)
			printf("[%d]-->",list.head->data);
		else
			printf("[%d]",list.head->data);

		list.head=(list.head)->next;
	}
	printf("\n");	
}

int deleteHead(List* list) {
	Case c;

	c=list->head;
	if(list->size>1)
		list->head=(list->head)->next;
	(list->head)->prev=NULL;

	free(c);
	list->size=(list->size)-1;
}

Case getCell(TElement elem, List list) {
	Case c=NULL;

	for(int i=0; i<list.size && c==NULL; i++) {
		if((list.head)->data==elem){
			c=list.head;
		}

		list.head=(list.head)->next;
	}	

	return c;
}

int deleteElement(TElement elem, List* list) {
	Case c = getCell(elem, *list);

	if(c != NULL) {
		if(c->prev==NULL) {
			deleteHead(list);
		}
		else if(c->next==NULL) {
			(c->prev)->next=NULL;
			list->tail=c->prev;
			list->size=(list->size)-1;
			free(c);
		}
		else {
			(c->prev)->next=c->next;
			(c->next)->prev=c->prev;
			list->size=(list->size)-1;
			free(c);
		}
		
		return 1;
	}

	return 0;
}

void delete_List(List* list) {
	int size=list->size;
	for(int i=0; i<size; i++) {
		deleteHead(list);
	}
}

int main() {

	List list=new_List();

	pushHead(-7, &list); pushHead(4, &list);
	pushHead(2, &list); pushHead(9, &list);
	pushTail(20, &list); pushHead(-15, &list);
	pushTail(-25, &list);

	dump_List(list);

	deleteElement(9,&list);
	deleteElement(-15,&list);
	deleteElement(-25,&list);

	dump_List(list);

	delete_List(&list);

	return 0;
}