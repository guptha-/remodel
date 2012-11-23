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
	memset (*content, 0, sizeof(char) * *fileSize + 1);
	result = fread(*content, 1, *fileSize, fp);
	if (result <= 0)
	{
		printf ("utilFileToString: File is empty\n");
		free (*content);
		return FAILURE;
	}
	return SUCCESS;
}
