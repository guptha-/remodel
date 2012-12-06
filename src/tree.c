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
					tempTreeList = NULL;
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
					tempTreeList = NULL;
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

/* treeRemoveNodeFromTree removes the given tree node from the tree.
 */
void treeRemoveNodeFromTree (TreeLeafNode *treeLeafNode)
{
	TreeDepNode *treeDepNode = NULL;
	TreeDepNode *prevTreeDepNode = NULL;
	TreeDepNode *tempTreeDepNode = NULL;
	TreePredNode *treePredNode = NULL;
	TreePredNode *tempTreePredNode = NULL;
	TreeSuccNode *treeSuccNode = NULL;
	TreeSuccNode *prevTreeSuccNode = NULL;

	if (treeLeafNode == NULL)
	{
		printf ("treeRemoveNodeFromTree: Cannot remove non-existant node\n");
		return;
	}

	treeDepNode = (TreeDepNode *) malloc (sizeof (TreeDepNode));
	
	if (treeDepNode == NULL)
	{
		printf ("treeRemoveNodeFromTree: Cannot allocate memory\n");
		exit (EXIT_FAILURE);
	}

	if (treeLeafNode->node == NULL)
	{
		printf ("treeRemoveNodeFromTree: Cannot remove non-existant node\n");
		free (treeDepNode);
		return;
	}

	memcpy (treeDepNode, treeLeafNode->node, sizeof (TreeDepNode));

	/* If this is not the leaf node, deletion is not permitted. */
	if (treeDepNode->treeSuccHead != NULL)
	{
		printf ("treeRemoveNodeFromTree: This is a leaf node. Cannot delete. "
						"Exiting because something went irrevocably wrong.\n");
		free (treeDepNode);
		exit (EXIT_FAILURE);
	}

	treePredNode = treeDepNode->treePredHead;
	while (treePredNode != NULL)
	{
		/* Making sure that references to the current node are removed from the 
		 * predecessors.
		 */
		prevTreeSuccNode = NULL;
		treeSuccNode = treePredNode->node->treeSuccHead;
		while (treeSuccNode != NULL)
		{
			if (strcmp (treeSuccNode->node->node->depPath, treeDepNode->node->depPath)
					== 0)
			{
				/* From the list of successors of the predecessor node, we have found
				 * the required node.
				 */
				if ((treeSuccNode == treePredNode->node->treeSuccHead) &&
						(treeSuccNode == treePredNode->node->treeSuccTail))
				{
					/* This is the only successor node in the predecessor's successor
					 * list.
					 */
					treePredNode->node->treeSuccHead = NULL;
					treePredNode->node->treeSuccTail = NULL;
					free (treeSuccNode);
					treeSuccNode = NULL; /* Exit condition. */
				}
				else if (treeSuccNode == treePredNode->node->treeSuccHead)
				{
					/* This node is at the head of the list. */
					treePredNode->node->treeSuccHead = treeSuccNode->next;
					free (treeSuccNode);
					treeSuccNode = treePredNode->node->treeSuccHead;
				}
				else if (treeSuccNode == treePredNode->node->treeSuccTail)
				{
					/* This node is at the tail of the list. */
					treePredNode->node->treeSuccTail = prevTreeSuccNode;
					treePredNode->node->treeSuccTail->next = NULL;
					free (treeSuccNode);
					treeSuccNode = NULL;
				}
				else
				{
					/* This is an intermediate node. */
					prevTreeSuccNode->next = treeSuccNode->next;
					free (treeSuccNode);
					treeSuccNode = prevTreeSuccNode->next;
				}
			}
			else
			{
				/* This is not the one. Move on. */
				prevTreeSuccNode = treeSuccNode;
				treeSuccNode = treeSuccNode->next;
			}
		} /* End scan of one predecessor's successor list. */

		treePredNode = treePredNode->next;
	}

	tempTreePredNode = NULL;
	treePredNode = treeDepNode->treePredHead;
	while (treePredNode != NULL)
	{
		/* Removing all the pred nodes from the current node. */
		tempTreePredNode = treePredNode->next;
		free (treePredNode);
		treePredNode = tempTreePredNode;
	}

	treeDepNode->treePredHead = NULL;
	treeDepNode->treePredTail = NULL;

	prevTreeDepNode = NULL;
	tempTreeDepNode = gTreeDepListHead;

	while (tempTreeDepNode != NULL)
	{
		if (strcmp (tempTreeDepNode->node->depPath, treeDepNode->node->depPath)
				== 0)
		{
			/* We have found the desired treeDepNode from the list. */
			if ((tempTreeDepNode == gTreeDepListHead) &&
					(tempTreeDepNode == gTreeDepListTail))
			{
				 /* This is the only node in the TreeDepList*/
				gTreeDepListHead = NULL;
				gTreeDepListTail = NULL;
				free (tempTreeDepNode);
				tempTreeDepNode = NULL; /* Exit condition. */
			}
			else if (tempTreeDepNode == gTreeDepListHead)
			{
				/* This node is at the head of the list. */
				gTreeDepListHead = tempTreeDepNode->next;
				free (tempTreeDepNode);
				tempTreeDepNode = gTreeDepListHead;
			}
			else if (tempTreeDepNode == gTreeDepListTail)
			{
				/* This node is at the tail of the list. */
				gTreeDepListTail = prevTreeDepNode;
				gTreeDepListTail->next = NULL;
				free (tempTreeDepNode);
				tempTreeDepNode = NULL;
			}
			else
			{
				/* This is an intermediate node. */
				prevTreeDepNode->next = tempTreeDepNode->next;
				free (tempTreeDepNode);
				tempTreeDepNode = prevTreeDepNode->next;
			}
		}
		else
		{
			/* This is not the one. Move on. */
			prevTreeDepNode = tempTreeDepNode;
			tempTreeDepNode = tempTreeDepNode->next;
		}
	} /* End scan global TreeDepList */
	
	/* We do not remove depNode, prodNode or targNodes from their resp. lists. */
	free (treeDepNode);
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
		treeLeafHead = treeLeafHead->next;
	}
}

/* treeBuildLeafList builds a list of all the leaf nodes of the tree.
 */
void treeBuildLeafList (TreeDepNode *treeDepNode, TreeLeafNode **treeLeafHead,
                        TreeLeafNode **treeLeafTail)
{
	TreeLeafNode *treeLeafNode = NULL;
	TreeSuccNode *treeSuccNode = NULL;
	TreeLeafNode *treeTempLeafNode = *treeLeafHead;
	
	while (treeTempLeafNode != NULL)
	{
		if (treeTempLeafNode->node == treeDepNode)
		{
			/* Node already exists in leaf list. */
			return;
		}
		treeTempLeafNode = treeTempLeafNode->next;
	}
	if ((treeDepNode == NULL) || (treeDepNode->node == NULL))
	{
		return;
	}
	treeSuccNode = treeDepNode->treeSuccHead;

	if ((treeSuccNode == NULL) && (treeDepNode->dispatchStatus == FALSE))
	{
		/* This is a leaf node. Add it to the list. */
		treeLeafNode = (TreeLeafNode *) malloc (sizeof (TreeLeafNode));
		if (treeLeafNode == NULL)
		{
			printf ("Mem allocation failure\n");
			exit (EXIT_FAILURE);
		}
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
	while ((prodDepNode != NULL) && (prodDepNode->node != NULL))
	{
		/* Ensuring that there is only one tree dep node for each dep */
		treeLocateTreeDepNode (prodDepNode->node, &tempTreeDepNode);
		if (tempTreeDepNode == NULL)
		{
			tempTreeDepNode = (TreeDepNode *) malloc (sizeof(TreeDepNode));
			if (tempTreeDepNode == NULL)
			{
				printf ("Mem allocation failure\n");
				exit (EXIT_FAILURE);
			}
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
		treePredNode = (TreePredNode *) malloc (sizeof(TreePredNode));
		if (treePredNode == NULL)
		{
			printf ("Mem allocation failure\n");
			exit (EXIT_FAILURE);
		}
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
		if (treeSuccNode == NULL)
		{
			printf ("Mem allocation failure\n");
			exit (EXIT_FAILURE);
		}
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
	if (depNode == NULL)
	{
		printf ("Mem allocation failure\n");
		exit (EXIT_FAILURE);
	}
	memset (depNode, 0, sizeof (DepNode));
	treeDepNode = (TreeDepNode *) malloc (sizeof (TreeDepNode));
	if (treeDepNode == NULL)
	{
		printf ("Mem allocation failure\n");
		exit (EXIT_FAILURE);
	}
	memset (treeDepNode, 0, sizeof (TreeDepNode));

	depNode->depPath = malloc (strlen (argument) + 1);
	if (depNode->depPath == NULL)
	{
		printf ("Mem allocation failure\n");
		exit (EXIT_FAILURE);
	}
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
