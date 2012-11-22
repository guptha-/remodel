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

/* md5DetectFileModif finds out if the given file has been modified since
 * last run of remodel and returns TRUE if modified. 
 */
int md5DetectFileModif (char *fileName)
{
	char *filePath = NULL;
	char hexDigest[16*2+1];
	char hexDigestNew[16*2+1];

	filePath = malloc (strlen (fileName) + strlen (".remodel/") + 1);
	memset (hexDigest, 0, 16*2+1);
	memset (hexDigestNew, 0, 16*2+1);

	md5AppendPathToFileName (fileName, filePath);

	if (md5CalcFileMD5 (fileName, hexDigestNew) == FAILURE)
	{
		printf ("detectFileModif: Unable to calc file's MD5\n");
		return TRUE;
	}

	if ((md5GetMD5FromFile (filePath, hexDigest) == FAILURE) ||
		  (strlen (hexDigest) != 16*2) ||
			(md5AreMD5sEqual (hexDigest, hexDigestNew) != TRUE))
	{
		/* It is a new/modified file */
		return TRUE;
	}

	free (filePath);
	return FALSE;
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
		printf ("md5FileToString: File is empty\n");
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
	md5_append(&state, (const md5_byte_t *)content, fileSize);
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

