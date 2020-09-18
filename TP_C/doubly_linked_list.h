#ifndef DOUBLY_LINKED_LIST_H

#define DOUBLY_LINKED_LIST_H

typedef int TElement;

typedef struct Cell {
	TElement data;
	struct Cell *next;
	struct Cell *prev;
}* Case;


typedef struct {
	Case head;
	Case tail;
	int size;
}List;

List new_List();

bool isEmpty(List list);

int pushHead(TElement elem, List* list);

int pushTail(TElement elem, List* list);

int pushAfterElement(TElement elem, List* list);

void dump_List(List list);

int deleteHead(List* list);

int deleteElement(TElement elem, List* list);

void delete_List(List* list);

#endif