#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "mt19937-64.h"


#define MAX_CHAR 100

#define FLAG_SEC_AGENTS 	1
#define FLAG_SEC_ADJACENCY 	0

#define END_STR '\0'
#define NEW_LINE '\n'
#define COMMENT_LINE '$'
#define TOKEN_SEP ","

#define ERROR_NO_ERROR 0
#define ERROR_MEMORY_ALLOCATION 1
#define ERROR_READING_FILE 2
#define ERROR_WRITING_FILE 3
#define ERROR_WRONG_FORMAT 4

typedef struct struct_link
{
   int zSrc,
       zDest;
    struct struct_link *next;
}Link;

Link *head;
int zTotalLinks=0;
int zTotalAgents;

char *trim_white_spaces(char *);
int  count_tokens(char *);

int exist(int src, int dest);

void ReadLinks(char *);
void WattsStrogatz(double);
void WriteOutput(char *);

void print();

int main()
{
   init_genrand64(time(0));
    ReadLinks("o2/ring.dat");
    WattsStrogatz(1.0);
    WriteOutput("o2/ring_sw_1.0.dat");
}

char *trim_white_spaces(char * strLine)
{
	char *chpEnd;

	while (*strLine==' ') strLine++;
	if (!*strLine) return strLine;
	chpEnd = strLine + strlen(strLine) - 1;
	while (chpEnd > strLine && *chpEnd==' ') chpEnd--;
	*++chpEnd = END_STR;
	return strLine;
}

int count_tokens(char *strLine)
{
	int zCount = 0;
	while (*strLine) if (*strLine++ == ',')++zCount;
	return zCount + 1;
}

void ReadLinks(char *strFname)
{
    FILE *fp;
    char strLine[MAX_CHAR], strFlag[MAX_CHAR], *chpToken,*strpLine;
    Link *node, *new;
    int zTemp, zSrc=0, zDest, zFlag=0;


    fp = fopen(strFname, "r");
    
    if(fp == NULL) exit(ERROR_READING_FILE);

	while(fgets(strLine, MAX_CHAR, fp) != NULL)
    {
	   strpLine = trim_white_spaces(strLine);
	   if (*strpLine == NEW_LINE || *strpLine == COMMENT_LINE) continue;
	   *strFlag = END_STR;
       if(sscanf(strpLine, "# %s", strFlag))
	{
		if (!strcmp(strFlag, "agents")) zFlag = FLAG_SEC_AGENTS;
       		if(!strcmp(strFlag, "adjacency")) goto READLINKS;
	}
	if(zFlag == FLAG_SEC_AGENTS) sscanf(strpLine, "n= %d", &zTotalAgents);
    }

    exit(ERROR_WRONG_FORMAT);
    
READLINKS:
    
    head = (Link *)malloc(sizeof(Link));
    head->next = NULL;
    
    node = head;

    while(fgets(strLine, MAX_CHAR, fp) != NULL)
    {
	   strpLine = trim_white_spaces(strLine);
	   if (*strpLine == NEW_LINE || *strpLine == COMMENT_LINE) continue;
	   chpToken = strtok(strpLine, TOKEN_SEP);
	   while (chpToken != NULL)
	   {
		zDest = atoi(chpToken);
		if(!exist(zSrc, zDest))
		{
			    new = (Link *)malloc(sizeof(Link));
			    new->zSrc=zSrc;
			    new->zDest=zDest;
			    new->next = NULL;
			    node->next = new;
			    node = node->next;
			    zTotalLinks++;
			
		}
		chpToken = strtok(NULL, TOKEN_SEP);
	   }
	zSrc++;
    }
    fclose(fp);
}

void WattsStrogatz(double dbP)
{
	Link *node = head->next;
        int zTarget;

        while(node != NULL)
	{
		if(genrand64_real2() < dbP)
		{
			do
				zTarget = genrand64_real2() * zTotalAgents;
                        while(zTarget == node->zSrc || exist(node->zSrc, zTarget));
                        node->zDest = zTarget;
		}
                node = node->next;
	}

}

void WriteOutput(char *strFname)
{
	int zLink = 0;
        Link *node;
	FILE *fp;
	char strLine[MAX_CHAR], *strpLine;

	fp = fopen(strFname, "w");

	if(fp == NULL) exit(ERROR_WRITING_FILE);
	fprintf(fp, "#agents\nn=%d\n#adjacency\n", zTotalAgents);
	while(zLink < zTotalAgents)
	{
		node =head->next;
                *strLine = 0;
		while(node != NULL)
		{
                        if(node->zSrc == zLink)
                            sprintf(strLine, "%s,%d",strLine, node->zDest);
                        else if(node->zDest == zLink)
                            sprintf(strLine, "%s,%d",strLine, node->zSrc);
			node = node->next;
		}
                strpLine = strLine+1;
		fprintf(fp, "%s\n",strpLine);
                zLink++;
	}
        fclose(fp);

}
int exist(int src, int dest)
{
	Link *node = head->next;

	while(node != NULL)
	{
                if((node->zSrc == src && node->zDest == dest) || (node->zSrc == dest && node->zDest == src)) return 1;
		node = node->next;
	}
	return 0;
}

void print()
{
	Link *node = head->next;
	
        while(node != NULL)
	{
		printf("%d %d\n", node->zSrc, node->zDest);
                node= node->next;
	}
}

