#include "../inc/incl.h"

/* startRemodel starts off the remodeling of the tree structure.
 */
int executeStartRemodel ()
{
	TreeLeafNode *treeLeafHead = NULL;
	TreeLeafNode *treeLeafTail = NULL;
	TreeLeafNode *treeLeafNode = NULL;
	TreeLeafNode *locatedTreeLeaf = NULL;
	TargNode *targNode = NULL;
	ProdNode *prodNode = NULL;
	char *extFileName = NULL;
	pid_t processID = 0;
	int stat_loc;

	while (gTreeDepListHead != NULL)
	{
		treeBuildLeafList (gTreeDepListHead, &treeLeafHead, &treeLeafTail);
		treeLeafNode = treeLeafHead;
		while (treeLeafNode != NULL)
		{
			if (treeLeafNode->node->dispatchStatus == TRUE)
			{
				treeLeafNode = treeLeafNode->next;
				continue;
			}
			treeLeafNode->processID = fork();
			if (treeLeafNode->processID == 0)
			{
				/* Child */
				extFileName = malloc (strlen (treeLeafNode->node->node->depPath) +
						strlen (".remodel/") + 1);
				utilAppendPathToFileName (treeLeafNode->node->node->depPath, extFileName);
				if ((utilFindIfFileExists (treeLeafNode->node->node->depPath) != TRUE) ||
						(utilFindIfFileExists (extFileName) != TRUE) ||
						(md5DetectFileModif (treeLeafNode->node->node->depPath)) == TRUE)
				{
					/* The leaf node's file does not exist or the MD5 for the file does
						 not exist or the MD5 hash is modified. */
					treeFindTargetNodeFromArg (treeLeafNode->node->node->depPath, &targNode);
					if (targNode == NULL)
					{
						/* Source file */
						if (md5CalcAndStoreMD5 (treeLeafNode->node->node->depPath) == FAILURE)
						{
							printf ("Could not store MD5 for %s\n", 
									treeLeafNode->node->node->depPath);
							exit (EXIT_FAILURE);
						}
						exit (EXIT_SUCCESS);
					}
					treeFindProdForTarget (targNode, &prodNode);
					if (prodNode == FAILURE)
					{
						printf ("Could not locate production node for leaf dependency\n");
						exit (EXIT_FAILURE);
					}
					if (strlen (prodNode->command) == 0)
					{
						printf ("There is no command to build target %s\n",
						        treeLeafNode->node->node->depPath);
						exit (EXIT_FAILURE);
					}

					if (system (prodNode->command) != 0)
					{
						printf ("Could not execute command %s\n", prodNode->command);
						exit (EXIT_FAILURE);
					}
					if (md5CalcAndStoreMD5 (treeLeafNode->node->node->depPath) == FAILURE)
					{
						printf ("Could not store MD5 for %s\n", 
						        treeLeafNode->node->node->depPath);
						exit (EXIT_FAILURE);
					}
				}
				/* If the leaf node's MD5 has not been changed, no need to do anything.*/
				exit (EXIT_SUCCESS);
			}
			else if (treeLeafNode->processID < 0)
			{
				printf ("Failed to spawn child processes \n");
				return FAILURE;
			}
			/* Parent */
			treeLeafNode->node->dispatchStatus = TRUE;
			treeLeafNode = treeLeafNode->next;
		}
		if ((processID = wait (&stat_loc)) != -1)
		{
			/* A child has terminated. */
			if (stat_loc != 0)
			{
				printf ("Child terminated abnormally\n");
				return FAILURE;
			}
			treeLocateTreeLeafFromPID (treeLeafHead, processID, &locatedTreeLeaf);
			if (locatedTreeLeaf == NULL)
			{
				printf ("Leaf node missing\n");
				return FAILURE;
			}

			treeRemoveNodeFromTree (locatedTreeLeaf);
			treeRemoveLeafFromTree (&treeLeafHead, &treeLeafTail, locatedTreeLeaf);
		}
	}
	while ((processID = wait (&stat_loc)) != -1)
	{
		/* A child has terminated. */
		if (stat_loc != 0)
		{
			printf ("Child terminated abnormally\n");
			return FAILURE;
		}
		treeLocateTreeLeafFromPID (treeLeafHead, processID, &locatedTreeLeaf);
		if (locatedTreeLeaf == NULL)
		{
			printf ("Leaf node missing\n");
			return FAILURE;
		}

		treeRemoveNodeFromTree (locatedTreeLeaf);
		treeRemoveLeafFromTree (&treeLeafHead, &treeLeafTail, locatedTreeLeaf);
	}
	return SUCCESS;
}

