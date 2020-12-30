
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char ** argv)
{
	int zSize = atoi(argv[1]), k, zR, zL, zU, zD, zRU, zRD, zLU, zLD;

	FILE *fp;

	fp = fopen("agents.dat", "w");

	if (fp == NULL)
	{
		printf("Error: file creation failed");
		return 1;
	}
    
	fprintf(fp, "#agents\nn=%d\n#adjacency", zSize*zSize);

	for (k = 0; k < zSize*zSize; k++)
	{
		if (k%zSize) zL = k - 1;
		else zL = k + zSize - 1;

		if ((k + 1) % zSize) zR = k + 1;
		else zR = k + 1 - zSize;

		if (k >= zSize) zU = k - zSize;
		else zU = zSize * (zSize - 1) + k;

        if (k < zSize*(zSize-1)) zD=k+zSize;
		else zD=k% (zSize*(zSize-1));

		if (k%zSize && k >= zSize) zLU = k - 1 - zSize;
		else if (!(k%zSize)) zLU = k - 1;
		else zLU = zSize * (zSize - 1) + k - 1;

		if (k%zSize && k < zSize*(zSize - 1)) zLD = k - 1 + zSize;
		else if (!(k%zSize)) zLD = k + 2 * zSize - 1;
		else zLD = k % (zSize*(zSize - 1)) - 1;
		
		if ((k + 1) % zSize && k >= zSize) zRU = k + 1 - zSize;
		else if (!((k + 1) % zSize)) zRU = k + 1 - 2 * zSize;
		else zRU = zSize * (zSize - 1) + k + 1;;

		if ((k + 1) % zSize && k < zSize*(zSize - 1)) zRD = k + 1 + zSize;
		else if (!((k + 1) % zSize)) zRD = k + 1;
		else zRD = k % (zSize*(zSize - 1)) + 1;

		if (!k) zLU = zSize * zSize - 1;
		if (k == zSize * (zSize - 1)) zLD = zSize - 1;
		if (k == zSize - 1) zRU = zSize * (zSize - 1);
		if (k == zSize * zSize - 1) zRD = 0;
		
		fprintf(fp, "\n%d,%d,%d,%d,%d,%d,%d,%d", zR, zU, zRU, zL, zD, zLU, zLD, zRD);
	}
	fclose(fp);

	return 0;
}