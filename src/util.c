#include "../inc/incl.h"

/* utilAppendPathToFileName prepends ".remodel/" to the filename.
 * !! Make sure to reserve enough memory to the return variable before calling.
 */
void utilAppendPathToFileName (char *fileName, char *retFileName)
{
	memset (retFileName, 0, strlen (fileName) + strlen (".remodel/") + 1);
	strcat (retFileName, ".remodel/");
	strcat (retFileName, fileName);
}

/* utilFindIfFileExists takes in a filename and checks if it exists.
 */
int utilFindIfFileExists (char *fileName)
{
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		return FALSE;
	}
	fclose (fp);
	return TRUE;
}
	
/* utilFileToString takes in a filename and loads and returns the contents of the
 * file as a string.
 */
int utilFileToString (char *fileName, char **content, long *fileSize)
{
	long result = 0;
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		return FAILURE;
	}
	fseek(fp, 0, SEEK_END);
	*fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*content = malloc(sizeof(char) * *fileSize + 1);
	if (*content == NULL)
	{
		printf ("Mem allocation failure\n");
		exit (EXIT_FAILURE);
	}
	memset (*content, 0, sizeof(char) * *fileSize + 1);
	result = fread(*content, 1, *fileSize, fp);
	if (result <= 0)
	{
		printf ("utilFileToString: File is empty\n");
		free (*content);
		*content = NULL;
		fclose (fp);
		return FAILURE;
	}
	fclose (fp);
	return SUCCESS;
}
