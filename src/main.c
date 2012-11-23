#include "../inc/incl.h"

/* Globals */

ProdNode *prodListHead = NULL;
DepNode *depListHead = NULL;
ProdNode *prodListTail = NULL;
DepNode *depListTail = NULL;

int main (int argc, char **argv)
{
	char *argument;

	/* Create .remodel directory. */
	system ("mkdir -p .remodel");

	/* Check if remodfile modified */
	if (remodfileProcessRemodfileModif () == FAILURE)
	{
		printf ("Error verifying remodfile\n");
	}
	
	/* Load in the presaved MD5 hashes */
	
  if (argc == 2)
	{
		argument = (char *) malloc (sizeof(char) * strlen (argv[1]) + 1);
		strncpy (argument, argv[1], sizeof(char) * strlen (argv[1]) + 1);
	}
	else
	{
		argument = (char *) malloc (sizeof(char) * strlen ("DEFAULT") + 1);
		strcpy (argument, "DEFAULT");
  }
  /* Populate data structures with production information */
	if (remodfileProcessRemodfile () == FAILURE)
	{
		printf ("Unable to process remodfile \n");
	  free (argument);
		return FAILURE;
	}

	free (argument);

	/* Compute and compare MD5s for all the productions' dependencies */
	
	/* Generate list of productions whose dependencies have been modified/ MD5s
	don't preexist. Iteration is required: from the root up - something clever 
	needed here. */

	/* Iterate through list of productions, finding out those that have only src
	dependencies or object dependencies that have just been done, and spawning
	threads to execute them */

	/* Store new MD5 hashes for everything that's been changed, either by the
	programmer or by the program */

	return SUCCESS;
}

