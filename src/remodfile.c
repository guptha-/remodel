#include "../inc/incl.h"

/* remodfileParseLine takes in a production line from the makefile and updates
 * the basic datastructures from it.
 */
int remodfileParseLine (char *prodLine, long lineNum)
{
	char targDep[256];
	char command[2560];
	char targetName[256];
	char depName[256][256];
	ProdNode *prodNode = NULL;
	ProdDepNode *prodDepNode = NULL;
	DepNode *depNode = NULL;
	char *tempProdLine = NULL;
	char *tempTargDep = NULL;
	char *tempDep = NULL;
	char *tempCommand = NULL;
	int quoteCount = 0;
	int depCount = 0;
	int tempCount = 0;
	int commandPresent = FALSE;
	int count = 0;

	memset (targDep, 0, 256);
	memset (command, 0, 2560);
	memset (targetName, 0, 256);
	memset (depName, 0, 256 * 256);

	tempProdLine = prodLine;
	tempTargDep = targDep;

	/* Stripping newline character from the end */
	while (*tempProdLine != '\0')
	{
		if ((*tempProdLine == '\n') && (*(tempProdLine + 1) == '\0'))
		{
			*tempProdLine = '\0';
		}
		tempProdLine++;
	}

	tempProdLine = prodLine;

	for(; *tempProdLine; quoteCount += (*tempProdLine++ == '\"'));
	if ((quoteCount != 0) && (quoteCount != 2))
	{
	  /* More than one command - Return failure */
		printf("remodfileParseLine: Wrong number of quotes found on %lu\n", lineNum);
		return FAILURE;
	}

	tempProdLine = prodLine;

	if (quoteCount == 2)
	{
		commandPresent = TRUE;
	}

	if (commandPresent == TRUE)
	{
		while (*tempProdLine != '\0')
		{
			if (*tempProdLine != '\"')
			{
				tempProdLine++;
				continue;
			}
			tempProdLine++;
			strncpy (command, tempProdLine, strlen (tempProdLine) - 1);
			tempProdLine--;
			memset (tempProdLine, 0, strlen (tempProdLine));
			tempCommand = command;
			quoteCount = 0;
			for(; *tempCommand; quoteCount += (*tempCommand++ == '\"'));
			if (quoteCount > 0)
			{
				printf("remodfileParseLine: Chars detect. after command on %lu\n", lineNum);
				return FAILURE;
			}
			break;
		}
	}

	tempProdLine = prodLine;

	while (*tempProdLine != '\0')
	{
		if ((*tempProdLine != ' ') && (*tempProdLine != ':'))
		{
			*tempTargDep++ = *tempProdLine;
		}
		tempProdLine++;
	}

	tempTargDep = targDep;

	while ((*tempTargDep != '<') && (*(tempTargDep + 1) != '-') && 
		     (*tempTargDep != '\0'))
	{
		tempTargDep++;
	}
	if (*tempTargDep == '\0')
	{
		printf ("remodfileParseLine: Missing -> on line %lu\n", lineNum);
		return FAILURE;
	}

	strncpy (targetName, targDep, strlen (targDep) - strlen (tempTargDep));
	tempTargDep += 2;
	tempDep = tempTargDep;

	for(; *tempTargDep; depCount += (*tempTargDep++ == ','));
	depCount++;

	if (depCount >= 256)
	{
		printf ("remodfileParseLine: Too many dependencies on line %lu\n", lineNum);
		return FAILURE;
	}

	tempTargDep = tempDep;
	for (count = 0; count < depCount; count++)
	{
		tempCount = 0;
		while ((*tempTargDep != ',') && (*tempTargDep != '\0'))
		{
			depName[count][tempCount++] = *tempTargDep++;
		}
		tempTargDep++;
	}

	prodNode = (ProdNode *) malloc (sizeof (ProdNode));
	memset (prodNode, 0, sizeof (ProdNode));
	prodNode->targetPath = malloc (strlen (targetName) + 1);
	prodNode->command = malloc (strlen (command) + 1);

	strcpy (prodNode->targetPath, targetName);
	strcpy (prodNode->command, command);

	count = 0;
	while (count < depCount)
	{
		depNode = (DepNode *) malloc (sizeof (DepNode));
		memset (depNode, 0, sizeof (DepNode));
		prodDepNode = (ProdDepNode *) malloc (sizeof (ProdDepNode));
		memset (prodDepNode, 0, sizeof (ProdDepNode));
		prodDepNode->node = depNode;
		prodDepNode->prod = prodNode;
		if (prodNode->depListHead == NULL)
		{
			prodNode->depListHead = prodDepNode;
			prodNode->depListTail = prodDepNode;
		}
		else
		{
			prodNode->depListTail->next = prodDepNode;
			prodNode->depListTail = prodDepNode;
		}
		depNode->depPath = malloc (strlen (depName[count]) + 1);
		memset (depNode->depPath, 0, strlen (depName[count]) + 1);
		strcpy (depNode->depPath, depName[count]);
		if (gDepListHead == NULL)
		{
			gDepListHead = depNode;
			gDepListTail = depNode;
		}
		else
		{
			gDepListTail->next = depNode;
			gDepListTail = depNode;
		}
		count++;
	}
	if (gProdListHead == NULL)
	{
		gProdListHead = prodNode;
		gProdListTail = prodNode;
	}
	else
	{
		gProdListTail->next = prodNode;
		gProdListTail = prodNode;
	}

	printf("%s\n%s\n%s\n%s\n%s\n%s\n", prodLine, command, targDep, targetName, depName[0], depName[1]);
	return SUCCESS;
}

/* remodfileProcessRemodfile processes the entire remod file and loads all its
 * contents into internal data structures.
 */
int remodfileProcessRemodfile ()
{
	char content[2560];
	FILE *fp = NULL;
	long lineNum = 0;

	memset (content, 0, 2560);
	fp = fopen ("remodfile", "r");
	if (fp == NULL)
	{
		printf ("remodfileProcessRemodfile: Could not open remodfile for input\n");
		return FAILURE;
	}
	while (fgets (content, 2560, fp) != NULL)
	{
		lineNum++;
		if (remodfileParseLine (content, lineNum) == FAILURE)
		{
			return FAILURE;
		}
	}

	fclose (fp);
	return SUCCESS;
}	

/* remodfileProcessRemodfileModif finds out if the remodfile has been modified 
 * since the last run of remodel. If it has, it discards all the presaved MD5 
 * hashes.  It also computes and saves the MD5 of the current remodfile.
 */
int remodfileProcessRemodfileModif ()
{
	char fileName[] = "remodfile";
	char *filePath = NULL;
	char hexDigest[16*2+1];
	char hexDigestNew[16*2+1];

	filePath = malloc (strlen (fileName) + strlen (".remodel/") + 1);
	memset (hexDigest, 0, 16*2+1);
	memset (hexDigestNew, 0, 16*2+1);

	utilAppendPathToFileName (fileName, filePath);

	if ((md5GetMD5FromFile (filePath, hexDigest) == FAILURE) ||
		  (strlen (hexDigest) != 16*2))
	{
		/* Remodfile modified/ digest stored is invalid */
		system ("rm -f .remodel/*");
	}

	if (md5CalcFileMD5 (fileName, hexDigestNew) == FAILURE)
	{
		printf ("remodfileProcessRemodfileModif: Unable to calc remodfile's MD5\n");
		return FAILURE;
	}

	if (md5AreMD5sEqual (hexDigest, hexDigestNew) != TRUE)
	{
		/* Remodfile has been modified */
		system ("rm -f .remodel/*");
	}

	if (md5StoreMD5ToFile (filePath, hexDigestNew) == FAILURE)
	{
		printf ("remodfileProcessRemodfileModif: Unable to store remodfile's MD5\n");
		return FAILURE;
	}

	free (filePath);
	return SUCCESS;
}


