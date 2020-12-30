
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
	int k=0, zTotalAgents=atoi(argv[1]);
	FILE *fp;
	
	fp = fopen("ringc.dat", "w");

	if (fp == NULL)
	{
		printf("Error: file creation failed.");
		return 1;
	}
	 
	fprintf(fp, "#agents\nn=%d\n#adjacency\n%d,1", zTotalAgents, zTotalAgents -1);
	while(k++ < zTotalAgents-2) fprintf(fp, "\n%d,%d", k - 1, k + 1);
	fprintf(fp, "\n%d,%d", k - 1, 0);

	fclose(fp);
	return 0;
}
