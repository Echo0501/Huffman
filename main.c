/**
 * Example program to use RTree lib
 * 
 * Makes a Node tree of rect, based on size of rects.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "RTree.h"

typedef struct {
	float x1;
	float y1;
	float x2;
	float y2;
} rect;



// make_item, used to make new ITEMs for parent objects
void * make_rect(void * A, void * B) {
	rect * cA = (rect *)A;
	rect * cB = (rect *)B;
	
	rect * rtn = (rect *) malloc(sizeof(rect));
	
	rtn->x1 = (cA->x1 < cB->x1 ? cA->x1 : cB->x1);
	rtn->y1 = (cA->y1 < cB->y1 ? cA->y1 : cB->y1);
	rtn->x2 = (cA->x2 > cB->x2 ? cA->x2 : cB->x2);
	rtn->y2 = (cA->y2 > cB->y2 ? cA->y2 : cB->y2);
	
	return rtn;
}

float get_value_rect(void * A) {
	rect * cA = (rect *)A;
	
	float dx = cA->x2 - cA->x1;
	float dy = cA->y2 - cA->y1;
	
	return dx*dx+dy*dy;
}


// Basic output for debug?
void print_rect(void * o) {
	rect * cO = (rect *)o;
	printf("[%.1f,%.1f]->[%.1f,%.1f]\n", cO->x1, cO->y1, cO->x2, cO->y2);
}




int main(void) {
	
	int n = 32;
	int nn = n*n;
	
	rect ** rect_list = (rect **) malloc(sizeof(rect *) * nn);
	Node ** node_list = (Node **) malloc(sizeof(Node *) * nn);
	
	// Build rectangles and Nodes
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			
			int z = j + i * n;
			
			// Make rect
			rect * r = (rect *) malloc(sizeof(rect));
			r->x1 = i;
			r->y1 = j;
			r->x2 = i+1;
			r->y2 = j+1;
			
			rect_list[z] = r;
			
			// Make Node from rect
			Node * s = (Node *) malloc(sizeof(Node));
			s->ITEM = (void *) r;
			s->value = get_value_rect(s->ITEM);
			
			// No children
			s->A = NULL;
			s->B = NULL;
			
			node_list[z] = s;
		}
	}
	
	// Make tree form list, with functions.
	Node * Top = BuildTree(node_list, nn, &make_rect, &get_value_rect);
	
	// Print tree.
	print_node(Top, 0, &print_rect);
	
	
	// Cleanup
	
	free_node(Top);
	
	free(node_list);
	free(rect_list);
	
	
}