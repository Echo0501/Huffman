/*

Huffman.c

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "huffman.h"




/**
 * Simple output, not needed.
 */
void print_node(Node * N, int depth, void (*print_item)(void *)) {
	for (int i = 0; i < depth; i++) printf("\t");
	print_item(N->ITEM);
	
	if (N->A) print_node(N->A, depth+1, print_item);
	if (N->B) print_node(N->B, depth+1, print_item);
}

/**
 * Simple garbage man.
 */
void free_node(Node * A) {
	if (A->A) free_node(A->A);
	if (A->B) free_node(A->B);
	if (A->ITEM) free(A->ITEM);
	free(A);
}


/**
 * Helper helper.
 * 
 * @param Node * A		Node to be child of rtn
 * @param Node * B		Node to be child of rtn
 * @param make_item		Function to be used to create parent ITEM from children ITEMs
 * @param get_value		Function to calculate the value of ITEM
 * 
 * @return Node * rtn	Parent node to be created to contain A, and B
 */
Node * reduce_node(Node * A, Node * B, void*(*make_item)(void *, void *), float(*get_value)(void*)) {
	
	Node * rtn = (Node *) malloc(sizeof(Node));
	
	rtn->ITEM = make_item(A->ITEM, B->ITEM);
	rtn->value = get_value(rtn->ITEM);
	
	rtn->A = A;
	rtn->B = B;
	
	return rtn;
}

/**
 * Used to compare two nodes with each other.
 */
int compare_nodes(const void* A, const void* B) {
	
	return (int)((*(*((Node **)A))).value - (*(*((Node **)B))).value);
}


/**
 * The Star of the Show.
 * 
 * Could probably use a loop, instead of recursion to be more efficient...
 * 
 * @param working_list			A list of pointers to nodes to be condensed into a tree.
 * @param count 				Number if nodes inside List
 * @param working_base			A list of every potential combination to be made
 * @param working_base_size		size of working_base
 * @param make_item				Function to be used by reduce_node to create a new ITEM inside a parent node
 * @param get_value				Function to calculate value of ITEM inside of a Node
 * 
 * @return Node * rtn			Returns a parent node to a tree containing every value inside of List
 */
Node * huffman(Node ** working_list, 
			   int count, 
			   Node ** working_base, 
			   int working_base_size, 
			   void*(*make_item)(void *, void *),
			   float(*get_value)(void *)
			  ) {
	
	// Recursive end case
	if (count <= 1) {
		return working_list[0];
	}
	
	// DEBUG OUTPUT
	printf("%d, %d\n", count, working_base_size);
	
	// Find smallest value
	Node * fav_Node = working_base[0];
	
	
	// Clean working_base of fav_Node
	int i = 0;
	while (i < working_base_size) {
		
		if (fav_Node == working_base[i]) {
			
			memmove(&(working_base[i]), &(working_base[i+1]), sizeof(Node *) * (working_base_size - 1 - i));
			working_base[--working_base_size] = NULL;
		
		} else if (fav_Node->A == working_base[i]->A || 
					fav_Node->A == working_base[i]->B || 
					fav_Node->B == working_base[i]->A || 
					fav_Node->B == working_base[i]->B) {
			
			free(working_base[i]->ITEM);
			free(working_base[i]);
			memmove(&(working_base[i]), &(working_base[i+1]), sizeof(Node *) * (working_base_size - 1 - i));
			working_base[--working_base_size] = NULL;
		
		} else {
			i++;
		}
	}
	
	
	
	int fav_i = 0;
	int fav_j = 1;
	
	for(int i = 0; i < count; i++) {
		if (working_list[i] == fav_Node->A) {
			fav_i = i;
		} else if (working_list[i] == fav_Node->B) {
			fav_j = i;
			break;
		}
	}
	
	// remove items I and J
	memmove(&(working_list[fav_i]),	&(working_list[fav_i+1]),	sizeof(Node *) * (fav_j - fav_i - 1));
	memmove(&(working_list[fav_j-1]),&(working_list[fav_j+1]),	sizeof(Node *) * (count - fav_j - 1));
	
	working_list[--count] = NULL;
	working_list[count - 1] = fav_Node;
	
	
	
	
	
	// Add new items to working base
	for(int i = 0; i < count - 1; i++) {
		Node * new_node = reduce_node(working_list[i], fav_Node, make_item, get_value);
		working_base[working_base_size++] = new_node;
	}
	qsort(working_base, working_base_size, sizeof(Node *), compare_nodes);
	
	
	
	// recursive return
	return huffman(working_list, count, working_base, working_base_size, make_item, get_value);
}



/**
 * Wrapper for the recursive function.
 * 
 * @param List					A list of pointers to nodes to be condensed into a tree, not modified.
 * @param count 				Number if nodes inside List
 * @param make_item				Function to be used by reduce_node to create a new ITEM inside a parent node
 * @param get_value				Function to calculate value of ITEM inside of a NODE
 * 
 * @return Node * rtn			Returns a parent node to a tree containing every value inside of List
 */
Node * BuildTree(Node ** List, 
				 int count, 
				 void * (*make_item)(void *, void *),
				 float (*get_value)(void *)
				) {
	
	
	// malloc and copy data from list to working_list
	Node ** working_list = (Node **) malloc(sizeof(Node *) * count);
	memcpy(working_list, List, sizeof(Node *) * count);
	
	// The working_base array is used to calculate all possible combinations of every item in working_list
	int working_base_size = ((float)(count * (count - 1)) * 0.5f);
	Node ** working_base = (Node **) malloc(sizeof(Node *) * working_base_size);
	
	// Fill working_base, I_index, and J_index with values
	int z = 0;
	for (int i = 0; i < count - 1; i++) {
		for (int j = i + 1; j < count; j++) {
			// Build potential node to be used...
			working_base[z++] = reduce_node(working_list[i], working_list[j], make_item, get_value);
		}
	}
	qsort(working_base, working_base_size, sizeof(Node *), compare_nodes);
	
	
	Node * rtn = huffman(working_list, count, working_base, working_base_size, make_item, get_value);
	
	// cleanup
	free(working_list);
	free(working_base);
	
	return rtn;
	
}

