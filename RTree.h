/**
 * @author Duncan McCloud
 */



/**
 * Base structure, ITEM can be anything. Modify this to do useful things with ITEM, and A, or B.
 */
struct _node {
	void * ITEM;
	float value;
	
	struct _node * A;
	struct _node * B;
};
typedef struct _node Node;



/**
 * Simple output, not needed.
 */
void print_node(Node * N, int depth, void (*print_item)(void *));



/**
 * Simple garbage man.
 */
void free_node(Node * A);



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
				 void*(*make_item)(void *, void *),
				 float(*get_value)(void *)
				);

