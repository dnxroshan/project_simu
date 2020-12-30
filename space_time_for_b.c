
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include "mt19937-64.h"


#define ITERATIONS 10000

#define MAX_CHAR 100

#define FLAG_SEC_AGENTS 	0
#define FLAG_SEC_ADJACENCY 	1

#define END_STR '\0'
#define NEW_LINE '\n'
#define COMMENT_LINE '$'
#define TOKEN_SEP ","

#define R 1.0
#define S 0.0
#define P 0.0

#define ERROR_NO_ERROR 0
#define ERROR_MEMORY_ALLOCATION 1
#define ERROR_READING_FILE 2
#define ERROR_WRITING_FILE 3
#define ERROR_WRONG_FORMAT 4

typedef struct
{
	int zAction,
	    zNumAdj,
	    *zpAdjList;
	float flPayoff;
}agent;

int zTotalAgents;
agent *agpAgents;

char *trim_white_spaces(char *);
int  count_tokens(char *);

void LoadAgents(char *);

void TakeRandomActions();
void ComputePayoff(double);
void UpdateActions(double);

int WriteOutput(FILE *f, int);

int Play(double, double , FILE *);

main()
{

   double b;
   char ofile[50];
   init_genrand64(time(0));
   FILE *f, *pipe;

   LoadAgents("ring/ring_128_2_0.00.dat");

   for(b = 0.8;b<=1.2;b+=0.001)
   {
	sprintf(ofile,"Output/ST/ST_128_2_0_for_b_%4.3f.jpg",b);
        pipe = popen("gnuplot -persist","w");
   	fprintf(pipe, "set xlabel 'agents'\n");
   	fprintf(pipe, "unset key\n");
   	fprintf(pipe, "set xrange[0:127]\n");
   	fprintf(pipe, "set yrange[0:]\n");
   	fprintf(pipe, "set ylabel 'time'\n");
   	fprintf(pipe,"set terminal jpeg\n");
	fprintf(pipe,"unset colorbox\n");
	fprintf(pipe,"set title 'K = 0.3 b = `%4.3f'\n",b);
	f = fopen("temp.dat","w");
	Play(0.3,b,f);
   	fclose(f);
	fprintf(pipe,"set output '%s'\n", ofile);
	fprintf(pipe,"plot 'temp.dat' matrix with image");
	close(pipe);
   }
   free(agpAgents);

}
int Play(double K, double b, FILE *f)
{
   int zIterations = 0;

   TakeRandomActions();
   ComputePayoff(b);
   WriteOutput(f, 0);

   while (zIterations++ < ITERATIONS-1)
   {
	   UpdateActions(K);
	   ComputePayoff(b);
	   if(WriteOutput(f, zIterations)) return 0;
   }
   return 0;
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
void LoadAgents(char *strpFname)
{
	FILE *fp;
	char strLine[MAX_CHAR], strFlag[MAX_CHAR], *chpToken,*strpLine;
	int zSecFlag, zTemp,*zpTemp;
	agent *agpAgent;

	fp = fopen(strpFname, "r");

	if (fp == NULL) exit(ERROR_READING_FILE);

	while (fgets(strLine, MAX_CHAR, fp) != NULL)
	{
		strpLine = trim_white_spaces(strLine);
		if (*strpLine == NEW_LINE || *strpLine == COMMENT_LINE) continue;
		*strFlag = END_STR;
		if (sscanf(strpLine, "# %s", strFlag))
		{
			if (!strcmp(strFlag, "agents"))
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
			if (sscanf(strpLine, "n= %d", &zTotalAgents))
			{
				agpAgent = (agent *)malloc(zTotalAgents * sizeof(agent));
				if (agpAgent == NULL) exit(ERROR_MEMORY_ALLOCATION);
				agpAgents = agpAgent;
			}
			continue;
		case FLAG_SEC_ADJACENCY:
			zTemp = count_tokens(strpLine);
			zpTemp = (int *)malloc(zTemp * sizeof(int));
			if (zpTemp == NULL) exit(ERROR_MEMORY_ALLOCATION);
			agpAgent->zpAdjList = zpTemp;
			agpAgent->zNumAdj = zTemp;
			agpAgent++;
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

void TakeRandomActions()
{
	agent *agpAgent,*agpEnd;

	agpAgent = agpAgents;
	agpEnd = agpAgent + zTotalAgents;

	while (agpAgent != agpEnd) agpAgent++->zAction = genrand64_real1() * 2;
}

void ComputePayoff(double b)
{
	agent *agpAgent, *agpEnd;
	int *zpAdjList, *zpAdjListEnd;

	agpAgent = agpAgents;
	agpEnd = agpAgent + zTotalAgents;

	while (agpAgent != agpEnd)
	{
		int zNumCorp=0;
        zpAdjList=agpAgent->zpAdjList;
		zpAdjListEnd=zpAdjList+agpAgent->zNumAdj;
		while(zpAdjList != zpAdjListEnd) zNumCorp+=agpAgents[*zpAdjList++].zAction;
	    if(agpAgent->zAction) agpAgent->flPayoff= zNumCorp*R;
		else agpAgent->flPayoff= zNumCorp*b;
		agpAgent++;
	}
}

void UpdateActions(double K)
{
	int *zpTmpArrayEnd,*zpTmpArrayStart,zRandomIndex;
	agent *agpAgent, *agpEnd;
	double dbProbability;
 
	agpAgent = agpAgents;
	agpEnd = agpAgent + zTotalAgents;

	zpTmpArrayEnd = (int *)malloc(zTotalAgents * sizeof(int));

	if (zpTmpArrayEnd == NULL) exit(ERROR_MEMORY_ALLOCATION);

	zpTmpArrayStart = zpTmpArrayEnd;
	while (agpAgent != agpEnd) *zpTmpArrayEnd++ = agpAgent++->zAction;

	agpAgent = agpAgents;

	while (agpAgent != agpEnd)
	{
		zRandomIndex = genrand64_real1() * agpAgent->zNumAdj;
		dbProbability = (double)1/(1.0 + exp((double)(agpAgent->flPayoff - agpAgents[agpAgent->zpAdjList[zRandomIndex]].flPayoff) /(double)K));
		if (genrand64_real1() < dbProbability) agpAgent->zAction = zpTmpArrayStart[agpAgent->zpAdjList[zRandomIndex]];
		agpAgent++;
	}
	free(zpTmpArrayStart);
}

int WriteOutput(FILE *f, int zTime)
{
	agent *agpAgent, *agpEnd;
	int zCount=0;



	agpAgent = agpAgents;
	agpEnd = agpAgent + zTotalAgents;

	if(ftell(f)) fprintf(f,"\n");
	while (agpAgent != agpEnd)
	{
		fprintf(f,"%d ",agpAgent->zAction);
		zCount+=agpAgent->zAction;
		agpAgent++;
	}
	
	if (zCount ==0 || zCount == zTotalAgents) return 1;
	return 0;

}

