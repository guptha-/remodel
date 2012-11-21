#include "../inc/incl.h"

int main (int argc, char **argv)
{
	/* Check if makefile modified */
	
	/* Load in the presaved MD5 hashes */
	
  if (argc == 2)
	{
		/* Find out parameter and load only the necessary production */
	}
	else
	{
		/* Load first production alone */
  }
  /* Populate data structures with production information */

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

