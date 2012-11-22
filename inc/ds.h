#ifndef ds_INCL
#define ds_INCL
/* Chain node */
typedef struct strChainNode ChainNode;
typedef struct strChainProdNode ChainProdNode;
typedef struct strProdNode ProdNode;
typedef struct strDepNode DepNode;
typedef struct strProdDepNode ProdDepNode;
struct strChainNode
{
	ChainProdNode *prodListHead;
	ChainProdNode *prodListTail;
	ChainNode *next;
};

/* List node for a chain's productions */
struct strChainProdNode
{
	ChainNode *chain; /* Backpointer to parent chain */
	ProdNode *prod; /* Actual node */ 
	ChainProdNode *prev;
	ChainProdNode *next;
};

/* Production node */
struct strProdNode
{
	char targetPath[256];
	char command[256];
	ProdDepNode *depListHead;
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
	char depPath[256];
	DepNode *next;
};

#endif
