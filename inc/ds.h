#ifndef ds_INCL
#define ds_INCL
typedef struct strChainNode ChainNode;
typedef struct strChainProdNode ChainProdNode;
typedef struct strProdNode ProdNode;
typedef struct strDepNode DepNode;
typedef struct strProdDepNode ProdDepNode;
typedef struct strTargNode TargNode;
typedef struct strProdTargNode ProdTargNode;
typedef struct strTreeDepNode TreeDepNode;
typedef struct strTreePredNode TreePredNode;
typedef struct strTreeSuccNode TreeSuccNode;
typedef struct strTreeLeafNode TreeLeafNode;

/* Production node */
struct strProdNode
{
	char *command;
	ProdTargNode *targListHead;
	ProdTargNode *targListTail;
	ProdDepNode *depListHead;
	ProdDepNode *depListTail;
	ProdNode *next; /* Link to next production */
};

/* List node for a production's dependencies */
struct strProdDepNode
{
	DepNode *node; /* Link to the actual node */
	ProdNode *prod; /* Backpointer to parent production */
	ProdDepNode *next; /* The next production for this program */
};

/* Dependency node */
struct strDepNode
{
	char *depPath;
	DepNode *next;
};

/* Tree successor node */
struct strTreeSuccNode
{
	TreeDepNode *node;
	TreeSuccNode *next;
};

/* Tree predecessor node */
struct strTreePredNode
{
	TreeDepNode *node;
	TreePredNode *next;
};

/* Tree dependency node */
struct strTreeDepNode
{
	DepNode *node; /*Actual dependency node */
	TreeSuccNode *treeSuccHead; /* List of successors*/
	TreeSuccNode *treeSuccTail;
	TreePredNode *treePredHead; /* List of predecessors */
	TreePredNode *treePredTail;
	TreeDepNode *next;
	int dispatchStatus;
};

/* Tree leaf node */
struct strTreeLeafNode
{
	TreeDepNode *node;
	TreeLeafNode *next;
	pid_t processID;
};

/* List node for a production's targets */
struct strProdTargNode
{
	TargNode *node; /* Link to the actual node */
	ProdNode *prod; /* Backpointer to parent production */
	ProdTargNode *next; /* The next production for this program */
};

/* Target node */
struct strTargNode
{
	char *targPath;
	TargNode *next;
};

#endif
