#include "../inc/incl.h"

/* processMakefileModif finds out if the makefile has been modified since the
 * last run of remodel. If it has, it discards all the presaved MD5 hashes.
 * It also computes and saves the MD5 of the current makefile.
 */
int processMakefileModif ()
{
	char fileName[] = "makefile";
	char *filePath = NULL;
	char hexDigest[16*2+1];
	char hexDigestNew[16*2+1];

	filePath = malloc (strlen (fileName) + strlen (".remodel/") + 1);
	memset (hexDigest, 0, 16*2+1);
	memset (hexDigestNew, 0, 16*2+1);

	md5AppendPathToFileName (fileName, filePath);

	if ((md5GetMD5FromFile (filePath, hexDigest) == FAILURE) ||
		  (strlen (hexDigest) != 16*2))
	{
		/* Makefile modified/ digest stored is invalid */
		system ("rm -f .remodel/*");
	}

	if (md5CalcFileMD5 (fileName, hexDigestNew) == FAILURE)
	{
		printf ("processMakefileModif: Unable to calc makefile's MD5\n");
		return FAILURE;
	}

	if (md5AreMD5sEqual (hexDigest, hexDigestNew) != TRUE)
	{
		/* Makefile has been modified */
		system ("rm -f .remodel/*");
	}

	if (md5StoreMD5ToFile (filePath, hexDigestNew) == FAILURE)
	{
		printf ("processMakefileModif: Unable to store makefile's MD5\n");
		return FAILURE;
	}

	free (filePath);
	return SUCCESS;
}

int main (int argc, char **argv)
{
	char *argument;
	/* Check if makefile modified */
	if (processMakefileModif () == FAILURE)
	{
		printf ("Error verifying makefile\n");
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

