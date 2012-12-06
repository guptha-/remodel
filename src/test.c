#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define FAILURE 0
#define SUCCESS 1

void getLineFromFile (char *fileName, char *string)
{
	FILE *fp = NULL;

	fp = fopen (fileName, "r");
	if (fp == NULL)
	{
		printf ("Comparison file %s not present\n", fileName);
		fclose (fp);
		exit (EXIT_FAILURE);
	}
	fgets (string, 256, fp);
	fclose (fp);
}

int isSubstringPresent (char *infileName)
{
	char string[256];
	char bigString[256];
	char fileName[256];
	memset (string, 0, 256);
	memset (bigString, 0, 256);
	getLineFromFile (infileName, string);
	strcat (bigString, "grep '");
	strncat (bigString, string, strlen(string) - 1);
	strcat (bigString, "' tempfile > tempfile1");
	system (bigString);
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	strcpy (fileName, "tempfile1");
	getLineFromFile (fileName, string);
	if (string[0] == '\0')
	{
		return FALSE;
	}
	return TRUE;
}

int executeTest10 ()
{
	/* See if everything compiles when the remodfile is modified. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("mkdir -p obj; mkdir -p bin; mv remodfile remodfiletemp");
	system ("cp remodfilecomplex remodfile; ./remodel |tee tempfile");
	system ("mv remodfiletemp remodfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}
	return SUCCESS;
}


int executeTest9 ()
{
	/* See if it works with caz removed. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("mkdir -p obj; mkdir -p bin; rm -rf bin/caz");
	system ("./remodel |tee tempfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	return SUCCESS;
}

int executeTest8 ()
{
	/* See if it works with d.o's md5 modified. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("mkdir -p obj; mkdir -p bin; cp ../test/test_d.o.md5mod.out .remodel/obj/d.o");
	system ("./remodel |tee tempfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	return SUCCESS;
}

int executeTest7 ()
{
	/* See if it works with d.o's md5 removed. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("mkdir -p obj; mkdir -p bin; rm -f .remodel/obj/d.o");
	system ("./remodel |tee tempfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	return SUCCESS;
}

int executeTest6 ()
{
	/* See if it works with d.o removed. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("mkdir -p obj; mkdir -p bin; rm -f obj/d.o");
	system ("./remodel |tee tempfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	return SUCCESS;
}


int executeTest5 ()
{
	/* See if it works with b.o removed. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("mkdir -p obj; mkdir -p bin; rm -f obj/b.o");
	system ("./remodel |tee tempfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	return SUCCESS;
}


int executeTest4 ()
{
	/* See if it works with a.o removed. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("mkdir -p obj; mkdir -p bin; rm -f obj/a.o");
	system ("./remodel |tee tempfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == TRUE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	return SUCCESS;
}


int executeTest3 ()
{
	/* See if all works without any bin or obj files. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("mkdir -p obj; mkdir -p bin; rm -rf bin/* obj/*");
	system ("./remodel |tee tempfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	return SUCCESS;
}

int executeTest2 ()
{
	/* See if all works without any .remodel files. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("rm -rf .remodel/*; mkdir -p obj; mkdir -p bin");
	system ("./remodel |tee tempfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	return SUCCESS;
}


int executeTest1 ()
{
	/* See if all works for the first time. */
	char fileName[256], string[256];
	memset (fileName, 0, 256);
	memset (string, 0, 256);
	system ("rm -rf .remodel/*; mkdir -p obj; mkdir -p bin");
	system ("rm -rf obj/*; rm -rf bin/*; ./remodel |tee tempfile");

	strcpy (fileName, "../test/test_a.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_b.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_c.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_d.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_e.o.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_baz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}

	strcpy (fileName, "../test/test_caz.out");
	if (isSubstringPresent (fileName) == FALSE)
	{
		return FAILURE;
	}
	return SUCCESS;
}

int main ()
{
	if (executeTest1 () == FAILURE)
	{
		printf ("TestCase 1 failed\n");
	}
	else
	{
		printf ("TestCase 1 passed\n");
	}

	if (executeTest2 () == FAILURE)
	{
		printf ("TestCase 2 failed\n");
	}
	else
	{
		printf ("TestCase 2 passed\n");
	}

	if (executeTest3 () == FAILURE)
	{
		printf ("TestCase 3 failed\n");
	}
	else
	{
		printf ("TestCase 3 passed\n");
	}

	if (executeTest4 () == FAILURE)
	{
		printf ("TestCase 4 failed\n");
	}
	else
	{
		printf ("TestCase 4 passed\n");
	}

	if (executeTest5 () == FAILURE)
	{
		printf ("TestCase 5 failed\n");
	}
	else
	{
		printf ("TestCase 5 passed\n");
	}

	if (executeTest6 () == FAILURE)
	{
		printf ("TestCase 6 failed\n");
	}
	else
	{
		printf ("TestCase 6 passed\n");
	}

	if (executeTest7 () == FAILURE)
	{
		printf ("TestCase 7 failed\n");
	}
	else
	{
		printf ("TestCase 7 passed\n");
	}

	if (executeTest8 () == FAILURE)
	{
		printf ("TestCase 8 failed\n");
	}
	else
	{
		printf ("TestCase 8 passed\n");
	}

	if (executeTest9 () == FAILURE)
	{
		printf ("TestCase 9 failed\n");
	}
	else
	{
		printf ("TestCase 9 passed\n");
	}

	if (executeTest10 () == FAILURE)
	{
		printf ("TestCase 10 failed\n");
	}
	else
	{
		printf ("TestCase 10 passed\n");
	}

	system ("rm -f tempfile tempfile1");
	return SUCCESS;
}
