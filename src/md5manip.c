#include "../inc/incl.h"

/* Function name : md5FileToText
 * Description : Takes a file path as input and returns the contents of the file
 *               as a string.
 *               !! Free contents after using it!!
 */

int md5FileToText (char *fileName, char **contents, long *fileSize)
{
	long result;
	FILE* fp = NULL;

	fp = fopen(fileName, "r");
	fseek(fp, 0, SEEK_END);
	*fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*contents = malloc (sizeof(char) * *fileSize);
	result = fread(*contents, 1, *fileSize, fp);

	if (result == 0)
	{
		return FAILURE;
	}
	return SUCCESS;
}

/* Function name : md5CalcFileMD5
 * Description : Takes a file path as input and returns the MD5 hash of its
 * contents.
 */

int md5CalcFileMD5 (char *fileName, char *hexDigest)
{
	md5_state_t state;
	md5_byte_t digest[16];
	long fileSize = 0;
	int i = 0;
	char *contents = NULL;

	if (md5FileToText (fileName, &contents, &fileSize) == FAILURE)
	{
		return FAILURE;
	}

	/* Calling APIs to calculate the MD5 */
	md5_init (&state);
	md5_append (&state, (const md5_byte_t *) contents, fileSize);
	md5_finish (&state, digest);

	free (contents);

	for (i = 0; i < 16; ++i)
	{
	  sprintf(hexDigest + i * 2, "%02x", digest[i]);
	}

	return SUCCESS;
}

/* Function name : md5CompareMD5
 * Description : Compares 2 given MD5 hashes and returns TRUE if they match
 */

int md5CompareMD5 (char *MD5One, char *MD5Two)
{
  if (strncmp (MD5One, MD5Two, 16*2) == 0)
	{
		return TRUE;
	}
	return FALSE;
}
