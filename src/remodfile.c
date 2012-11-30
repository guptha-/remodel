#include "../inc/incl.h"

/* remodfileParseLine takes in a production line from the makefile and updates
 * the basic datastructures from it.
 */
int remodfileParseLine (char *prodLine, long lineNum)
{
	char targName[256][256];
	char targDep[256];
	char command[2560];
	char targString[2560];
	char depName[256][256];
	ProdNode *prodNode = NULL;
	ProdTargNode *prodTargNode = NULL;
	TargNode *targNode = NULL;
	ProdDepNode *prodDepNode = NULL;
	DepNode *depNode = NULL;
	char *tempProdLine = NULL;
	char *tempTargDep = NULL;
	char *tempDep = NULL;
	char *tempCommand = NULL;
	char *tempTargString = NULL;
	int quoteCount = 0;
	int depCount = 0;
	int targCount = 0;
	int tempCount = 0;
	int commandPresent = FALSE;
	int count = 0;

	memset (targDep, 0, 256);
	memset (command, 0, 2560);
	memset (targString, 0, 2560);
	memset (targName, 0, 256 * 256);
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

	/* Parsing out the command. */
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

	strncpy (targString, targDep, strlen (targDep) - strlen (tempTargDep));
	tempTargDep += 2;
	tempDep = tempTargDep;

	for(; *tempTargDep; depCount += (*tempTargDep++ == ','));
	depCount++;

	tempTargString = targString;
	for(; *tempTargString; targCount += (*tempTargString++ == ','));
	targCount++;

	if (targCount >= 256)
	{
		printf ("remodfileParseLine: Too many targets on line %lu\n", lineNum);
		return FAILURE;
	}

	tempTargString = targString;
	for (count = 0; count < targCount; count++)
	{
		tempCount = 0;
		while ((*tempTargString != ',') && (*tempTargString != '\0'))
		{
			targName[count][tempCount++] = *tempTargString++;
		}
		tempTargString++;
	}

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
	prodNode->command = malloc (strlen (command) + 1);

	strcpy (prodNode->command, command);

	count = 0;
	while (count < targCount)
	{
		targNode = (TargNode *) malloc (sizeof (TargNode));
		memset (targNode, 0, sizeof (TargNode));
		prodTargNode = (ProdTargNode *) malloc (sizeof (ProdTargNode));
		memset (prodTargNode, 0, sizeof (ProdTargNode));
		prodTargNode->node = targNode;
		prodTargNode->prod = prodNode;
		if (prodNode->targListHead == NULL)
		{
			prodNode->targListHead = prodTargNode;
			prodNode->targListTail = prodTargNode;
		}
		else
		{
			prodNode->targListTail->next = prodTargNode;
			prodNode->targListTail = prodTargNode;
		}
		targNode->targPath = malloc (strlen (targName[count]) + 1);
		memset (targNode->targPath, 0, strlen (targName[count]) + 1);
		strcpy (targNode->targPath, targName[count]);
		if (gTargListHead == NULL)
		{
			gTargListHead = targNode;
			gTargListTail = targNode;
		}
		else
		{
			gTargListTail->next = targNode;
			gTargListTail = targNode;
		}
		count++;
	}

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
			fclose (fp);
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
		system ("rm -rf .remodel/*");
	}

	if (md5CalcFileMD5 (fileName, hexDigestNew) == FAILURE)
	{
		printf ("remodfileProcessRemodfileModif: Unable to calc remodfile's MD5\n");
		return FAILURE;
	}

	if (md5AreMD5sEqual (hexDigest, hexDigestNew) != TRUE)
	{
		/* Remodfile has been modified */
		system ("rm -rf .remodel/*");
	}

	if (md5StoreMD5ToFile (filePath, hexDigestNew) == FAILURE)
	{
		printf ("remodfileProcessRemodfileModif: Unable to store remodfile's MD5\n");
		return FAILURE;
	}

	free (filePath);
	return SUCCESS;
}


