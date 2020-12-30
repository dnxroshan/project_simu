

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

main()
{
	FILE *infile, *outfile;
	int n = 100,k,m,size = 15472;
	char fname[100];
	float *buffer1,*buffer2, *buffer3,data1,data2,data3;
	
	buffer1 = calloc(sizeof(float), size);
	buffer2 = calloc(sizeof(float), size);
	buffer3 = calloc(sizeof(float), size);

	if(buffer1 == NULL || buffer2 == NULL)
	{
		printf("Memory allocation failed");
		exit(0);
	}

	outfile = fopen("/home/dnroshan/Project/output/async/phase/mean_1.0.dat","w");
	
	for(k = 0; k < n; k++)
	{
		printf("%d\n",k);
		sprintf(fname,"/home/dnroshan/Project/output/async/phase/1.0/async_ring_128_2_1.0(%d).dat",k);
		infile = fopen(fname,"r");
		if(infile == NULL)
		{
			printf("No such file"); 
			exit(0);
		}
		m = 0;
		while(!feof(infile))
		{
			fscanf(infile,"%f %f %f",&data1,&data2,&data3);
			buffer1[m] = data1;
			buffer2[m] = data2;
			buffer3[m] = buffer3[m] + (data3/n);
			m++;
		}
		fclose(infile);
	}

	for(k = 0; k<size; k++)
		fprintf(outfile,"%f %f %f\n",buffer1[k],buffer2[k],buffer3[k]);

	fclose(outfile);
	free(buffer1);
	free(buffer2);
}
