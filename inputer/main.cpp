#include <stdio.h>

int avfiles=0;
char avnames[16][64];

bool ave_read(char* FNAME)
{
	FILE *set_file;
	set_file = fopen(FNAME, "r"); 
	fscanf(set_file, "%i", &avfiles);
	if (avfiles==16) avfiles--;
	if (avfiles != 0) 
	{
		for (int i=0; i<avfiles; i++)
		{
			fscanf(set_file,"%[^\n]\n",avnames[i+1]);
		}
	}
	fclose(set_file);
	return true;
}

bool ave_write(char* FNAME)
{
	FILE *set_file;
	set_file = fopen(FNAME, "w"); 
	fprintf(set_file, "%i", avfiles);
	for (int i=0; i<avfiles; i++)
	{
		fprintf(set_file,"%s\n",avnames[i]);
	}
	fclose(set_file);
	return true;
}

int main(int argc, char** argv)
{
	printf("%s\n", argv[1]);
	ave_read(argv[1]);
	printf("Enter new filename\n");
	scanf("%s",avnames[0]);
	avfiles++;
	ave_write(argv[1]);
	return 0;
}