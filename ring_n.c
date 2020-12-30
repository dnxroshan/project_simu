
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
	int k=0, zTotalAgents=atoi(argv[1]);
	FILE *fp;
	
	fp = fopen("ringn.dat", "w");

	if (fp == NULL)
	{
		printf("Error: file creation failed.");
		return 1;
	}
	 
	fprintf(fp, "#agents\nn=%d\n#adjacency", zTotalAgents);
	while(k < zTotalAgents)
	{
 		fprintf(fp, "\n%d,%d", (k - 1) % zTotalAgents, (k + 1) % zTotalAgents);
		k++;
	}

	fclose(fp);
	return 0;
}
