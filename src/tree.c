#include "../inc/incl.h"

/* treeRemoveLeafFromTree removes the given tree leaf from the list.
 */
void treeRemoveLeafFromTree (TreeLeafNode **Head, TreeLeafNode **Tail,
														TreeLeafNode *treeLeafNode)
{
	TreeLeafNode *prevTreeListNode = NULL;
	TreeLeafNode *tempTreeList = NULL;

	tempTreeList = *Head;
	while (tempTreeList != NULL)
	{
		if (tempTreeList == treeLeafNode)
		{
			if (prevTreeListNode == NULL)
			{
				if (*Head == *Tail)
				{
					free (*Head);
					*Head = NULL;
					*Tail = NULL;
					tempTreeList = NULL;
				}
				else
				{
					*Head = (*Head)->next;
					free (tempTreeList);
					tempTreeList = *Head;
				}
			}
			else
			{
				if (tempTreeList->next == NULL)
				{
					*Tail = prevTreeListNode;
					prevTreeListNode->next = NULL;
					free (tempTreeList);
					tempTreeList = NULL;
				}
				else
				{
					prevTreeListNode->next = tempTreeList->next;
					free (tempTreeList);
					tempTreeList = prevTreeListNode->next;
				}
			}
		}
		else
		{
			prevTreeListNode = tempTreeList;
			tempTreeList = tempTreeList->next;
		}
	}
}

/* treeRemoveNodeFromTree removes the given tree node from the tree and also deletes
 * the relevant MD5 file for the parent.
 */
void treeRemoveNodeFromTree (TreeLeafNode *treeLeafNode)
{
	TreeDepNode *treeDepNode = treeLeafNode->node;
	TreeDepNode *treeDepListNode = NULL;
	TreeDepNode *prevTreeDepListNode = NULL;
	TreePredNode *treePredNode = treeDepNode->treePredHead;
	TreeSuccNode *treeSuccNode = NULL;
	TreeSuccNode *prevTreeSuccNode = NULL;
	char *command = NULL;

	while (treePredNode != NULL)
	{
		/* Removing MD5 information of all the predecessors of this node and removing
		   this node from their lists. */
		command = malloc (strlen (treePredNode->node->node->depPath) + 
							strlen (".remodel/") + strlen ("rm -f") + 1);
		memset (command, 0, strlen (treePredNode->node->node->depPath) + 
							strlen (".remodel/") + strlen ("rm -f ") + 1);
		strcat (command, "rm -f ");
		strcat (command, ".remodel/");
		strcat (command, treePredNode->node->node->depPath);
		system (command);

		treeSuccNode = treePredNode->node->treeSuccHead;
		while (treeSuccNode != NULL)
		{
			if (strcmp (treeSuccNode->node->node->depPath, treeDepNode->node->depPath) 
				  == 0)
			{
				if (prevTreeSuccNode == NULL)
				{
					if (treePredNode->node->treeSuccHead == treePredNode->node->treeSuccTail)
					{
						free (treePredNode->node->treeSuccHead);
						treePredNode->node->treeSuccHead = NULL;
						treePredNode->node->treeSuccTail = NULL;
						treeSuccNode = NULL;
					}
					else
					{
						treePredNode->node->treeSuccHead = 
														treePredNode->node->treeSuccHead->next;
						free (treeSuccNode);
						treeSuccNode = treePredNode->node->treeSuccHead;
					}
				}
				else
				{
					if (treeSuccNode->next == NULL)
					{
						treePredNode->node->treeSuccTail = prevTreeSuccNode;
						prevTreeSuccNode->next = NULL;
						free (treeSuccNode);
						treeSuccNode = NULL;
					}
					else
					{
						prevTreeSuccNode->next = treeSuccNode->next;
						free (treeSuccNode);
						treeSuccNode = prevTreeSuccNode->next;
					}
				}
			}
			else
			{
				prevTreeSuccNode = treeSuccNode;
				treeSuccNode = treeSuccNode->next;
			}
		}

		treePredNode = treePredNode->next;
	}

	treeDepListNode = gTreeDepListHead;
	while (treeDepListNode != NULL)
	{
		if (strcmp (treeDepListNode->node->depPath, treeDepNode->node->depPath) 
				== 0)
		{
			if (prevTreeDepListNode == NULL)
			{
				if (gTreeDepListHead == gTreeDepListTail)
				{
					free (gTreeDepListHead);
					gTreeDepListHead = NULL;
					gTreeDepListTail = NULL;
					treeDepListNode = NULL;
				}
				else
				{
					gTreeDepListHead = gTreeDepListHead->next;
					free (treeDepListNode);
					treeDepListNode = gTreeDepListHead;
				}
			}
			else
			{
				if (treeDepListNode->next == NULL)
				{
					gTreeDepListTail = prevTreeDepListNode;
					prevTreeDepListNode->next = NULL;
					free (treeDepListNode);
					treeDepListNode = NULL;
				}
				else
				{
					prevTreeDepListNode->next = treeDepListNode->next;
					free (treeDepListNode);
					treeDepListNode = prevTreeDepListNode->next;
				}
			}
		}
		else
		{
			prevTreeDepListNode = treeDepListNode;
			treeDepListNode = treeDepListNode->next;
		}
	}
}

	
/* treeLocateTreeLeafFromPID locates the tree leaf node from the embedded process
 * ID.
 */
void treeLocateTreeLeafFromPID (TreeLeafNode *treeLeafHead, pid_t processID,
                                TreeLeafNode **locatedTreeLeaf)
{
	*locatedTreeLeaf = NULL;
	while (treeLeafHead != NULL)
	{
		if (treeLeafHead->processID == processID)
		{
			*locatedTreeLeaf = treeLeafHead;
			break;
		}
	}
}

/* treeBuildLeafList builds a list of all the leaf nodes of the tree.
 */
void treeBuildLeafList (TreeDepNode *treeDepNode, TreeLeafNode **treeLeafHead,
                        TreeLeafNode **treeLeafTail)
{
	TreeLeafNode *treeLeafNode = NULL;
	TreeSuccNode *treeSuccNode = treeDepNode->treeSuccHead;

	if ((treeSuccNode == NULL) && (treeDepNode->dispatchStatus == FALSE))
	{
		/* This is a leaf node. Add it to the list. */
		treeLeafNode = (TreeLeafNode *) malloc (sizeof (TreeLeafNode));
		memset (treeLeafNode, 0, sizeof (TreeLeafNode));
		treeLeafNode->node = treeDepNode;
		if (*treeLeafHead == NULL)
		{
			(*treeLeafHead) = treeLeafNode;
			(*treeLeafTail) = treeLeafNode;
		}
		else
		{
			(*treeLeafTail)->next = treeLeafNode;
			(*treeLeafTail) = treeLeafNode;
		}
	}
	while (treeSuccNode != NULL)
	{
		treeBuildLeafList (treeSuccNode->node, treeLeafHead, treeLeafTail);
		treeSuccNode = treeSuccNode->next;
	}
}

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
		if (strcmp ((*treeDepNode)->node->depPath, depNode->depPath) == 0)
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
	int nodeAlreadyExists = FALSE;

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
			tempTreeDepNode->dispatchStatus = FALSE;
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
			nodeAlreadyExists = TRUE;
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

		if (nodeAlreadyExists == TRUE)
		{
			prodDepNode = prodDepNode->next;
			nodeAlreadyExists = FALSE;
			continue;
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
	treeDepNode->dispatchStatus = FALSE;

	gTreeDepListHead = treeDepNode;
	gTreeDepListTail = treeDepNode;

	if (treeAddSuccessors (treeDepNode) == FAILURE)
	{
		return FAILURE;
	}
	return SUCCESS;
}
