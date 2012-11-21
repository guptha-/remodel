#include "../inc/incl.h"

int main (int argc, char **argv)
{
	char hexDigest [16*2+1];
	(void) md5CalcFileMD5 ("test/md5calc.txt", &hexDigest);
	printf("%s\n", hexDigest);
	return SUCCESS;
}

