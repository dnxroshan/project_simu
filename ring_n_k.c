#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv)
{

	if(argc < 3)
	{
		printf("Number of inputs is less than required number of inputs.\n");
		return 1;
	}


	int n = atoi(argv[1]),
            k = atoi(argv[2])/2,
            m=0;

	char adj[100];

		

	FILE *fp;
	
	fp = fopen("Output/20/ring_20_0.dat", "w");

	if (fp == NULL)
	{
		printf("Error: file creation failed.");
		return 1;
	}

	fprintf(fp, "#agents\nn=%d\n#adjacency", n);

	while(m < n)
	{
		*adj = 0;
                for(int l = 1; l <= k; l++)
		{
		    if((m-l) % n >= 0) sprintf(adj, "%s,%d", adj, (m - l) % n);
		    else  sprintf(adj, "%s,%d", adj, (m - l) % n + n);
		    sprintf(adj, "%s,%d", adj, (m + l) % n);

		}
		fprintf(fp, "\n%s", &adj[1]);
		m++;
	}

	fclose(fp);
	return 0;
}
