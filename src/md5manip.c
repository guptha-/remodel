#include "../inc/incl.h"

/* md5AppendPathToFileName prepends ".remodel/" to the filename.
 * !! Make sure to reserve enough memory to the return variable before calling.
 */
void md5AppendPathToFileName (char *fileName, char *retFileName)
{
	memset (retFileName, 0, strlen (fileName) + strlen (".remodel/") + 1);
	strcat (retFileName, ".remodel/");
	strcat (retFileName, fileName);
}

/* md5GetMD5FromFile gets the hexDigest from the given fileName. 
 * !! Make sure hexDigest has 16*2+1 bytes allocated */
int md5GetMD5FromFile (char *fileName, char *hexDigest)
{
	FILE *fp = NULL;

	fp = fopen (fileName, "r");

	if (fp == NULL)
	{
		return FAILURE;
	}
	if (fgets (hexDigest, 16*2+1, fp) == NULL)
	{
		return FAILURE;
	}

	fclose (fp);
	return SUCCESS;
}


/* md5StoreMD5ToFile stores the hexDigest to the given fileName. */
int md5StoreMD5ToFile (char *fileName, char *hexDigest)
{
	FILE *fp = NULL;

	fp = fopen (fileName, "w");

	if (fp == NULL)
	{
		printf ("md5StoreMD5ToFile: Error writing to file\n");
		return FAILURE;
	}
	fputs (hexDigest, fp);
	fclose (fp);

	return SUCCESS;
}

/* md5FileToString takes in a filename and loads and returns the contents of the
 * file as a string.
 */
int md5FileToString (char *fileName, char **content, long *fileSize)
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
	*content = malloc(sizeof(char) * *fileSize);
	result = fread(*content, 1, *fileSize, fp);
	if (result <= 0)
	{
		printf ("File is empty\n");
		return FAILURE;
	}
	return SUCCESS;
}


/* md5CalcFileMD5 calculates and returns the MD5 hash of the given file.
 */
int md5CalcFileMD5 (char *fileName, char *hexDigest)
{
  md5_state_t state;
	md5_byte_t digest[16];
	char *content = NULL;
	long fileSize = 0;
	int i = 0;

	if (md5FileToString (fileName, &content, &fileSize) == FAILURE)
	{
		return FAILURE;
	}

	md5_init(&state);
	md5_append(&state, content, fileSize);
	md5_finish(&state, digest);

	free (content);

	for (i = 0; i < 16; ++i)
	{
		sprintf(hexDigest + i * 2, "%02x", digest[i]);
	}

	return SUCCESS;
}

/* md5AreMD5sEqual compares 2 given MD5 hashes and returns TRUE if they match.
*/
int md5AreMD5sEqual (char *MD5One, char *MD5Two)
{
	if (strncmp (MD5One, MD5Two, 16*2+1) == 0)
	{
		return TRUE;
	}
	return FALSE;
}

