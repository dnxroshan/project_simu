

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

main()
{
	FILE *infile, *outfile;
	int n = 100,k,m,size = 101;
	char fname[100];
	float *buffer1,*buffer2,data1,data2,data3;
	
	buffer1 = calloc(sizeof(float), size);
	buffer2 = calloc(sizeof(float), size);

	if(buffer1 == NULL || buffer2 == NULL)
	{
		printf("Memory allocation failed");
		exit(0);
	}

	outfile = fopen("/home/dnroshan/Project/output/p-t/MR/ring_1024_2/mean_time.dat","w");
	
	for(k = 0; k < n; k++)
	{
		printf("%d\n",k);
		sprintf(fname,"/home/dnroshan/Project/output/p-t/MR/ring_1024_2/time/ring_1024_time(%d).dat",k);
		infile = fopen(fname,"r");
		if(infile == NULL)
		{
			printf("No such file"); 
			exit(0);
		}
		m = 0;
		while(!feof(infile))
		{
			fscanf(infile,"%f %f",&data1,&data2);
			buffer1[m] = data1;
			buffer2[m] = buffer2[m] + (data2/n);
			m++;
		}
		fclose(infile);
	}

	for(k = 0; k<size; k++)
		fprintf(outfile,"%f %f\n",buffer1[k],buffer2[k]);

	fclose(outfile);
	free(buffer1);
	free(buffer2);
}
