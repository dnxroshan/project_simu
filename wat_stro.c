
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "mt19937-64.h"


#define MAX_CHAR 100
/////
#define FLAG_SEC_AGENTS 	0
#define FLAG_SEC_ADJACENCY 	1

#define END_STR '\0'
#define NEW_LINE '\n'
#define COMMENT_LINE '$'
#define TOKEN_SEP ","

#define ERROR_NO_ERROR 0
#define ERROR_MEMORY_ALLOCATION 1
#define ERROR_READING_FILE 2
#define ERROR_WRITING_FILE 3
#define ERROR_WRONG_FORMAT 4

#define NOWHERE -1

#define D_RAND ((double) fast_rand() / (double) RAND_MAX)
#define N_RAND(x) (fast_rand() % x)

typedef struct
{
	int zNumAdj,
	    *zpAdjList;
} node;

typedef struct
{
	int zSource,
	    zDestination;
} edge;

int zTotalNodes;
node *ndpNodes;

int zTotalEdges;
edge *edpEdges;

static unsigned int g_seed;  

static inline int fast_rand(void);

char *trim_white_spaces(char *);
int  count_tokens(char *);

int  IsExist(int zSource, int zTarget);
void LoadNetwork(char *);
void WattsStrogatz(double);
void SaveNetwork(char *);
void CreateEdgeList();

int main()
{
    g_seed = time(0);

	LoadNetwork("agents.dat");
	//printf("%d", IsExist(0,2));
    WattsStrogatz(0.5);
	SaveNetwork("ring10_sw.dat");

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

static inline int fast_rand(void) 
{
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}
void LoadNetwork(char *strpFname)
{
	FILE *fp;
	char strLine[MAX_CHAR], strFlag[MAX_CHAR], *chpToken,*strpLine;
	int zSecFlag, zTemp,*zpTemp;
	node *ndpNode;

	fp = fopen(strpFname, "r");

	if (fp == NULL) exit(ERROR_READING_FILE);

	while (fgets(strLine, MAX_CHAR, fp) != NULL)
	{
		strpLine = trim_white_spaces(strLine);
		if (*strpLine == NEW_LINE || *strpLine == COMMENT_LINE) continue;
		*strFlag = END_STR;
		if (sscanf(strpLine, "# %s", strFlag))
		{
			if (!strcmp(strFlag, "nodes"))
			{
				zSecFlag = FLAG_SEC_AGENTS;
				continue;
			}
			else if (!strcmp(strFlag, "adjacency"))
			{
				zSecFlag = FLAG_SEC_ADJACENCY;
				continue;
			}
		}
		switch (zSecFlag)
		{
		case FLAG_SEC_AGENTS:
			if (sscanf(strpLine, "n= %d", &zTotalNodes))
			{
				ndpNode = (node *)malloc(zTotalNodes * sizeof(node));
				if (ndpNode == NULL) exit(ERROR_MEMORY_ALLOCATION);
				ndpNodes = ndpNode;
			}
			continue;
		case FLAG_SEC_ADJACENCY:
			zTemp = count_tokens(strpLine);
			zpTemp = (int *)malloc(zTemp * sizeof(int));
			if (zpTemp == NULL) exit(ERROR_MEMORY_ALLOCATION);
			ndpNode->zpAdjList = zpTemp;
			ndpNode->zNumAdj = zTemp;
			ndpNode++;
			chpToken = strtok(strpLine, TOKEN_SEP);
			while (chpToken != NULL)
			{
				*zpTemp++ = atoi(chpToken);
				chpToken = strtok(NULL, TOKEN_SEP);
			}
			continue;
		}
		exit(ERROR_WRONG_FORMAT);
	}
	fclose(fp);
}

void WattsStrogatz(double dbp)
{
	node *ndpNode, *ndpEnd;
    int zTarget, zIndex = 0,k,i;

	ndpNode = ndpNodes;
	ndpEnd = ndpNode + zTotalNodes;

	while (ndpNode != ndpEnd)
    {
		for(k=0; k<3; k++)
		{
        	if(D_RAND < dbp)
        	{
            	do
            		zTarget = N_RAND(zTotalNodes);
            	while(zTarget == zIndex || IsExist(zIndex,zTarget));
				for(i=0;i<8;i++)
			   	if(ndpNodes[ndpNode->zpAdjList[k]].zpAdjList[i] == zIndex)
			    	ndpNodes[ndpNode->zpAdjList[k]].zpAdjList[i] = NOWHERE;
            	ndpNode->zpAdjList[k] = zTarget;
        	}
		}
		zIndex++;
        ndpNode++;
    }
}

int IsExist(int zSource, int zTarget)
{
    int k;

    for(k=0; k < ndpNodes[zSource].zNumAdj; k++)
        if(ndpNodes[zSource].zpAdjList[k] == zTarget) return 1;

    return 0;
}

void SaveNetwork(char *strpFname)
{
	node *ndpNode, *ndpEnd;
	FILE *fp;
	int k,c,p=0;

	ndpNode = ndpNodes;
	ndpEnd = ndpNode + zTotalNodes;

	fp = fopen(strpFname, "w");

	if (fp == NULL)
	{
		printf("Error: file creation failed");
		exit(ERROR_WRITING_FILE);
	}
    
	fprintf(fp, "#agents\nn=%d\n#adjacency", zTotalNodes);
	
	while (ndpNode != ndpEnd)
    {
		c=0;
		fprintf(fp,"\n");
		for(k = 0; k < ndpNodes->zNumAdj; k++)
		{
			if(ndpNode->zpAdjList[k] != NOWHERE)
			{
				fprintf(fp,"%d",ndpNode->zpAdjList[k]);
				p=1;
				c++;
			}
			if(k != ndpNodes->zNumAdj-1 && c !=0 && p)
			{
			   fprintf(fp,",");
			   p=0;
			}
		}
        ndpNode++;
    }

    fclose(fp);
}
