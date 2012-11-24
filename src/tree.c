#include "../inc/incl.h"

/* treeFindProdForTarget takes in the target and gives the first production that 
 * has the target node as one of its targets.
 */
void treeFindProdForTarget (TargNode *targNode, ProdNode **prodNode)
{
	*prodNode = gProdListHead;
	ProdTargNode *tempProdTargNode = NULL;

	while (*prodNode != NULL)
	{
		tempProdTargNode = (*prodNode)->targListHead;
		while (tempProdTargNode != NULL)
		{
		  if (tempProdTargNode->node == targNode)
		  {
			  break;
		  }
			tempProdTargNode = tempProdTargNode->next;
		}
		if (tempProdTargNode != NULL)
		{
			break;
		}
		*prodNode = (*prodNode)->next;
	}
}
	
/* treeFindTargetNodeFromArg takes in the argument and gives the corresponding 
 * target node.
 */
void treeFindTargetNodeFromArg (char *argument, TargNode **targNode)
{
	*targNode = gTargListHead;

	while (*targNode != NULL)
	{
		if (strcmp ((*targNode)->targPath, argument) == 0)
		{
			break;
		}
		*targNode = (*targNode)->next;
	}
}

/* treeLocateTreeDepNode finds the tree dep node of a dep node if it exists. 
 */
void treeLocateTreeDepNode (DepNode *depNode, TreeDepNode **treeDepNode)
{
	*treeDepNode = gTreeDepListHead;

	while (*treeDepNode != NULL)
	{
		if (depNode == (*treeDepNode)->node)
		{
			break;
		}
		*treeDepNode = (*treeDepNode)->next;
	}
}
		
/* treeAddSuccessors takes in a treeDepNode and adds successor dependencies.
 */
int treeAddSuccessors (TreeDepNode *treeDepNode)
{
	TargNode *targNode = NULL;
	ProdNode *prodNode = NULL;
	ProdDepNode *prodDepNode = NULL;
	TreeDepNode *tempTreeDepNode = NULL;
	TreePredNode *treePredNode = NULL;
	TreeSuccNode *treeSuccNode = NULL;
	FILE *fp = NULL;

	treeFindTargetNodeFromArg (treeDepNode->node->depPath, &targNode);

	if (targNode != NULL)
	{
		/* Found a target node. If there is no matching production node, there is 
		 * something wrong. */
		treeFindProdForTarget (targNode, &prodNode);
		if (prodNode == NULL)
		{
			printf ("Could not find production to remodel %s\n", 
			        treeDepNode->node->depPath);
			return FAILURE;
		}
	}
	else
	{
		/* No target node found. This means that there is no rule to build this 
		 * particular case in the remodfile. There are 2 possibilities:
		 * 1. The file is a source file
		 * 2. The remodfile is not proper
		 * Other than determining what the source files are for each lang., there is no
		 * real way of distinguishing between the cases. So, we are going to assume 
		 * that it is a source file. This means that any stale object files will be 
		 * used as their MD5's will reflect no changes in their contents.
	   */
		fp = fopen (treeDepNode->node->depPath, "r");
		if (fp == NULL)
		{
			printf ("Could not find target to remodel %s and file does "
			        "not exist in the path\n", treeDepNode->node->depPath);
			return FAILURE;
		}
		fclose (fp);
		/* This is a src file. There are no further dependencies. */
		return SUCCESS;
	}

	/* At this point, there are dependencies. */
	prodDepNode = prodNode->depListHead;
	while (prodDepNode != NULL)
	{
		/* Ensuring that there is only one tree dep node for each dep */
		treeLocateTreeDepNode (prodDepNode->node, &tempTreeDepNode);
		if (tempTreeDepNode == NULL)
		{
			tempTreeDepNode = (TreeDepNode *) malloc (sizeof(TreeDepNode));
			memset (tempTreeDepNode, 0, sizeof (TreeDepNode));
			tempTreeDepNode->node = prodDepNode->node;
			if (gTreeDepListHead == NULL)
			{
				gTreeDepListHead = tempTreeDepNode;
				gTreeDepListTail = tempTreeDepNode;
			}
			else
			{
				gTreeDepListTail->next = tempTreeDepNode;
				gTreeDepListTail = tempTreeDepNode;
			}
		}
		else
		{
			printf ("Located existing tree dep node\n");
		}
		treePredNode = (TreePredNode *) malloc (sizeof(TreePredNode));
		memset (treePredNode, 0, sizeof (TreePredNode));
		treePredNode->node = treeDepNode;
		if (tempTreeDepNode->treePredHead == NULL)
		{
			tempTreeDepNode->treePredHead = treePredNode;
			tempTreeDepNode->treePredTail = treePredNode;
		}
		else
		{
			tempTreeDepNode->treePredTail->next = treePredNode;
			tempTreeDepNode->treePredTail = treePredNode;
		}

		treeSuccNode = (TreeSuccNode *) malloc (sizeof(TreeSuccNode));
		memset (treeSuccNode, 0, sizeof (TreeSuccNode));
		treeSuccNode->node = tempTreeDepNode;
		if (treeDepNode->treeSuccHead == NULL)
		{
			treeDepNode->treeSuccHead = treeSuccNode;
			treeDepNode->treeSuccTail = treeSuccNode;
		}
		else
		{
			treeDepNode->treeSuccTail->next = treeSuccNode;
			treeDepNode->treeSuccTail = treeSuccNode;
		}
		if (treeAddSuccessors (tempTreeDepNode) == FAILURE)
		{
			return FAILURE;
		}
		prodDepNode = prodDepNode->next;
	}

	return SUCCESS;
}
	
/* treeConstructTree takes in the "root" and creates the tree structure of 
 * dependencies.
 */
int treeConstructTree (char *argument)
{
	DepNode *depNode = NULL;
	TreeDepNode *treeDepNode = NULL;

	/* Creating dummy depNode for the root: this doesn't have a depNode in reality
	 * as it is not anyone's dependency, but for elegance of code, adding the node
	 * here. */
	depNode = (DepNode *) malloc (sizeof (DepNode));
	memset (depNode, 0, sizeof (DepNode));
	treeDepNode = (TreeDepNode *) malloc (sizeof (TreeDepNode));
	memset (treeDepNode, 0, sizeof (TreeDepNode));

	depNode->depPath = malloc (strlen (argument) + 1);
	memset (depNode->depPath, 0, strlen (argument) + 1);
	strcpy (depNode->depPath, argument);
	treeDepNode->node = depNode;

	gTreeDepListHead = treeDepNode;
	gTreeDepListTail = treeDepNode;

	if (treeAddSuccessors (treeDepNode) == FAILURE)
	{
		return FAILURE;
	}
	return SUCCESS;
}
