#include "../inc/incl.h"

int md5FileToString (char *fileName, char **content, long *fileSize)
{
	long result = 0;
	FILE* fp = fopen(fileName, "r");
	fseek(fp, 0, SEEK_END);
	*fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*content = malloc(sizeof(char) * *fileSize);
	result = fread(*content, 1, *fileSize, fp);
	if (result <= 0)
	{
		return FAILURE;
	}
	return SUCCESS;
}


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

	for (i = 0; i < 16; ++i)
	{
		sprintf(hexDigest + i * 2, "%02x", digest[i]);
	}

	return SUCCESS;
}


int md5CompareMD5 (char *MD5One, char *MD5Two)
{
	if (strncmp (MD5One, MD5Two, 16*2+1) == 0)
	{
		return TRUE;
	}
	return FALSE;
}

