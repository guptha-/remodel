#include "../inc/incl.h"

/* Globals */

ProdNode *gProdListHead = NULL;
ProdNode *gProdListTail = NULL;
TargNode *gTargListHead = NULL;
TargNode *gTargListTail = NULL;
DepNode *gDepListHead = NULL;
DepNode *gDepListTail = NULL;
TreeDepNode *gTreeDepListHead = NULL;
TreeDepNode *gTreeDepListTail = NULL;

int main (int argc, char **argv)
{
	char *argument;

	/* Create .remodel directory if it hasn't already been created. */
	system ("mkdir -p .remodel");

	/* Check if remodfile modified */
	if (remodfileProcessRemodfileModif () == FAILURE)
	{
		printf ("Error verifying remodfile\n");
	}
	
  if (argc == 2)
	{
		argument = (char *) malloc (sizeof(char) * strlen (argv[1]) + 1);
		if (argument == NULL)
		{
			printf ("Mem allocation failure\n");
			exit (EXIT_FAILURE);
		}
		strncpy (argument, argv[1], sizeof(char) * strlen (argv[1]) + 1);
	}
	else
	{
		argument = (char *) malloc (sizeof(char) * strlen ("DEFAULT") + 1);
		if (argument == NULL)
		{
			printf ("Mem allocation failure\n");
			exit (EXIT_FAILURE);
		}
		strcpy (argument, "DEFAULT");
  }

  /* Populate data structures with production information */
	if (remodfileProcessRemodfile () == FAILURE)
	{
		printf ("Unable to process remodfile \n");
	  free (argument);
		argument = NULL;
		return FAILURE;
	}

	/* Construct dependency tree for the desired argument. */
	if (treeConstructTree (argument) == FAILURE)
	{
	  free (argument);
		argument = NULL;
		return FAILURE;
	}

	free (argument);
	argument = NULL;

	/* Remodel as needed, checking MD5 hashes on the fly and storing if needed.*/
	if (executeStartRemodel () == FAILURE)
	{
		return FAILURE;
	}
	return SUCCESS;
}

