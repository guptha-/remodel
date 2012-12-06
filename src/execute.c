#include "../inc/incl.h"
#include <errno.h>
/* startRemodel starts off the remodeling of the tree structure.
 */
int executeStartRemodel ()
{
	TreeLeafNode *treeLeafHead = NULL;
	TreeLeafNode *treeLeafTail = NULL;
	TreeLeafNode *treeLeafNode = NULL;
	TreeLeafNode *locatedTreeLeaf = NULL;
	TreePredNode *treePredNode = NULL;
	TargNode *targNode = NULL;
	ProdNode *prodNode = NULL;
	char *extFileName = NULL;
	char *command = NULL;
	pid_t processID = 0;
	int stat_loc;
	int doesFileExist = TRUE;
	int doesMD5Exist = TRUE;
	int isFileModif = TRUE;

	while (gTreeDepListHead != NULL)
	{
		treeBuildLeafList (gTreeDepListHead, &treeLeafHead, &treeLeafTail);
		treeLeafNode = treeLeafHead;
		while (treeLeafNode != NULL)
		{
			if (treeLeafNode->node->dispatchStatus == TRUE)
			{
				if (processID == -1)
				{
					printf ("executeStartRemodel: No child process, but something dispatched\n");
					exit (EXIT_FAILURE);
				}
				treeLeafNode = treeLeafNode->next;
				continue;
			}
			treeLeafNode->processID = fork();
			if (treeLeafNode->processID == 0)
			{
				/* Child */
				extFileName = malloc (strlen (treeLeafNode->node->node->depPath) +
						strlen (".remodel/") + 1);
				if (extFileName == NULL)
				{
					printf ("Mem allocation failure\n");
					exit (EXIT_FAILURE);
				}
				utilAppendPathToFileName (treeLeafNode->node->node->depPath, extFileName);
				doesFileExist = utilFindIfFileExists (treeLeafNode->node->node->depPath);
				if (doesFileExist == TRUE)
				{
					doesMD5Exist = utilFindIfFileExists (extFileName);
					if (doesMD5Exist == TRUE)
					{
						isFileModif = md5DetectFileModif (treeLeafNode->node->node->depPath);
					}
				}
				if ((doesFileExist != TRUE) ||
						(doesMD5Exist != TRUE) ||
						(isFileModif == TRUE))
				{
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
						treePredNode = treeLeafNode->node->treePredHead;
						while (treePredNode != NULL)
						{ 
							/* Removing MD5 information of all the predecessors of this node and removing
								 this node from their lists. */
							command = malloc (strlen (treePredNode->node->node->depPath) +
									strlen (".remodel/") + strlen ("rm -f ") + 1);
							if (command == NULL)
							{
								printf ("Mem allocation failure\n");
								exit (EXIT_FAILURE);
							}

							memset (command, 0, strlen (treePredNode->node->node->depPath) +
									strlen (".remodel/") + strlen ("rm -f ") + 1);
							strcat (command, "rm -f ");
							strcat (command, ".remodel/");
							strcat (command, treePredNode->node->node->depPath);
							system (command);
							free (command);
							command = NULL;
							treePredNode = treePredNode->next;
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
						/* There is no command for this. Could be a problem with the 
						 * remodfile, but could also be OK.
						 */
						exit (EXIT_SUCCESS);
					}

					if (system (prodNode->command) != 0)
					{
						printf ("Could not execute command %s\n", prodNode->command);
						exit (EXIT_FAILURE);
					}
					printf ("Executed command %s\n", prodNode->command);
					if (md5CalcAndStoreMD5 (treeLeafNode->node->node->depPath) == FAILURE)
					{
						printf ("Could not store MD5 for %s\n", 
						        treeLeafNode->node->node->depPath);
						exit (EXIT_FAILURE);
					}
					treePredNode = treeLeafNode->node->treePredHead;
					while (treePredNode != NULL)
					{ 
						/* Removing MD5 information of all the predecessors of this node and removing
							 this node from their lists. */
						command = malloc (strlen (treePredNode->node->node->depPath) +
								strlen (".remodel/") + strlen ("rm -f ") + 1);
						if (command == NULL)
						{
							printf ("Mem allocation failure\n");
							exit (EXIT_FAILURE);
						}
						memset (command, 0, strlen (treePredNode->node->node->depPath) +
								strlen (".remodel/") + strlen ("rm -f ") + 1);
						strcat (command, "rm -f ");
						strcat (command, ".remodel/");
						strcat (command, treePredNode->node->node->depPath);
						system (command);
						free (command);
						command = NULL;
						treePredNode = treePredNode->next;
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

