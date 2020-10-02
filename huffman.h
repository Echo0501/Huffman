/**
 * @author Duncan McCloud
 */



/**
 * Base structure, ITEM can be anything. Modify this to do useful things with ITEM, and A, or B.
 */
struct _node {
	void * ITEM;
	
	struct _node * A;
	struct _node * B;
};
typedef struct _node Node;

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
 * 
 * @return Node * rtn	Parent node to be created to contain A, and B
 */
Node * reduce_node(Node * A, Node * B, void * (*make_item)(void *, void *)) {
	
	Node * rtn = (Node *) malloc(sizeof(Node));
	
	rtn->ITEM = make_item(A->ITEM, B->ITEM);
	
	rtn->A = A;
	rtn->B = B;
	
	return rtn;
}

/**
 * The Star of the Show.
 * 
 * Could probably use a loop, instead of recursion to be more efficient...
 * 
 * @param Node ** List	This is a list of pointers to nodes to be condensed into a tree.
 * @param count 		Number if nodes inside List
 * @param get_value		Function to return float value of ITEM inside of a Node. ex v = get_value(NODE->ITEM)
 * @param make_item		Function to be used by reduce_node to create a new ITEM inside a parent node
 * 
 * @return Node * rtn	Returns a parent node to a tree containing every value inside of List
 */
Node * huffman(Node ** List, int count, float(*get_value)(void *), void * (*make_item)(void *, void *)) {
	// Recursive end case
	if (count <= 1) {
		return List[0];
	}
	
	// malloc and copy data from list to working_list
	Node ** working_list = (Node **) malloc(sizeof(Node *) * count);
	memcpy(working_list, List, sizeof(Node *) * count);
	
	/* 
	 * The working_base array is used to calculate all possible combinations of every item in working_list
	 * I_index and J_index are used to keep track of what items were used where.
	*/
	int working_base_size = ((float)(count * (count - 1)) * 0.5f);
	Node ** working_base = (Node **) malloc(sizeof(Node *) * working_base_size);
	int * I_index = (int *) malloc(sizeof(int) * working_base_size);
	int * J_index = (int *) malloc(sizeof(int) * working_base_size);
	
	// Fill working_base, I_index, and J_index with values
	int z = 0;
	for (int i = 0; i < count - 1; i++) {
		for (int j = i + 1; j < count; j++) {
			// Build potential node to be used...
			working_base[z] = reduce_node(working_list[i], working_list[j], make_item);
			I_index[z] = i;
			J_index[z] = j;
			z++;
		}
	}
	
	// Find smallest value
	int fav_i = I_index[0];
	int fav_j = J_index[0];
	Node * fav_Node = working_base[0];
	float fav_val = get_value(fav_Node->ITEM);
	
	for(int i = 1; i < working_base_size; i++) {
		
		float tmp_val = get_value(working_base[i]->ITEM);
		
		if (tmp_val < fav_val) {
			fav_val = tmp_val;
			fav_Node = working_base[i];
			fav_i = I_index[i];
			fav_j = J_index[i];
		}
		
	}
	
	// Make new_list to be passed to recursive call
	Node ** new_list = (Node **) malloc(sizeof(Node *) * (count - 1));
	
	// Copy values from working list to new_list, except items I and J
	memcpy(&(new_list[0]),		&(working_list[0]),			sizeof(Node *) * (fav_i));
	memcpy(&(new_list[fav_i]),	&(working_list[fav_i+1]),	sizeof(Node *) * (fav_j - fav_i - 1));
	memcpy(&(new_list[fav_j-1]),&(working_list[fav_j+1]),	sizeof(Node *) * (count - fav_j - 1));
	
	// Add item made from I and J
	new_list[count - 2] = fav_Node;
	
	// recursive return
	Node * rtn = huffman(new_list, count - 1, get_value, make_item);
	
	
	// Cleanup, not a messy boy.
	
	free(new_list);
	for(int i = 0;i < working_base_size; i++) {
		if (working_base[i] != fav_Node) {
			free(working_base[i]->ITEM);
			free(working_base[i]);
		}
	}
	free(J_index);
	free(I_index);
	free(working_base);
	free(working_list);
	
	
	return rtn;
}